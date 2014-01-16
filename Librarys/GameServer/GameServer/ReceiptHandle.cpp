//////////////////////////////////////
//			������������		   //
/////////////////////////////////////
#include "stdafx.h"
#include "Base.h"
#include "ReceiptHandle.h"
#include "NetBase.h"
#include "ClientContext.h"
#include "SQLDataBase.h"
#include "Utility.h"
#include "CrackURL.h"
#include "HttpBase.h"
#include "ServerConfig.h"
#include "tinyxml.h"

#include <sstream>
#include <string>
#include <algorithm>
#include <vector>


//#define AppleverifyURL L"https://sandbox.itunes.apple.com/verifyReceipt"
//#define AppleverifyURL L"https://buy.itunes.apple.com/verifyReceipt"

static int s_Item1 = 0;
static int s_Item2 = 0;
static int s_Item3 = 0;
static int s_Item4 = 0;
static int s_Item5 = 0;
static int s_Item6 = 0;

using namespace std;
using namespace BohgeNet;

ReceiptHandle*	g_ReceiptHandle = NULL;

//-------------------------------------------------------------------------------------------------------
ReceiptHandle::ReceiptHandle(void)
{
	//��ȡ���ﶩ��
	TCHAR strFilePath[MAX_PATH + 1]={0};
	GetModuleFileName(NULL, strFilePath, MAX_PATH);
	(strrchr(strFilePath, ('\\')))[1] = 0;
	SetCurrentDirectory( strFilePath );
	string path(strFilePath);
	path += "Receipt.xml";
	TiXmlDocument *myDocument = NET_NEW TiXmlDocument( path.c_str() );
	myDocument->LoadFile();
	TiXmlElement* RootElement = myDocument->RootElement();

	TiXmlElement* Item1 = RootElement->FirstChildElement();
	TiXmlElement* Item2 = Item1->NextSiblingElement();
	TiXmlElement* Item3 = Item2->NextSiblingElement();
	TiXmlElement* Item4 = Item3->NextSiblingElement();
	TiXmlElement* Item5 = Item4->NextSiblingElement();
	TiXmlElement* Item6 = Item5->NextSiblingElement();

	s_Item1 = atoi( Item1->Attribute( "Volume" ) );
	s_Item2 = atoi( Item2->Attribute( "Volume" ) );
	s_Item3 = atoi( Item3->Attribute( "Volume" ) );
	s_Item4 = atoi( Item4->Attribute( "Volume" ) );
	s_Item5 = atoi( Item5->Attribute( "Volume" ) );
	s_Item6 = atoi( Item6->Attribute( "Volume" ) );

	SAFE_DELETE( myDocument );


	// ��ʼ���̻߳�����
	InitializeCriticalSection(&m_csReslutlist);
	m_ProcessEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	g_ReceiptHandle = this;
	DWORD nThreadID;
	m_ReslutThreads = ::CreateThread(0, 0, PorcessReslut, (void *)this, 0, &nThreadID);

	m_pUrlResolve = NET_NEW CrackedUrl( ServerConfig::Instance().VerityDomain() );
}
//-------------------------------------------------------------------------------------------------------
ReceiptHandle::~ReceiptHandle(void)
{
	// ɾ���ͻ����б�Ļ�����
	DeleteCriticalSection(&m_csReslutlist);
	// �ر��¼����
	RELEASE_HANDLE(m_ProcessEvent);
	RELEASE_HANDLE(m_ReslutThreads);
	SAFE_DELETE( m_pUrlResolve );
}
//-------------------------------------------------------------------------------------------------------
bool ReceiptHandle::HanldeMessage( SmartPtr<ClientContext>& client, BohgeNet::NetPackage& pak, SQLDataBase& sql )
{
	pak.Begin();
	int message;
	pak>>message;
	switch (message)
	{
	case NM_RECEIPT_STATUS:
		{
			DEBUGLOG("Receive from %s port %d NM_RECEIPT_STATUS \n", client->BindIPString(), client->BindPort() );
			int vision;
			pak>>vision;
			if( SERVER_VISION == vision )
			{
				string udid;
				pak>>udid;
				stringstream str;
				str<<"SELECT * FROM Players WHERE UDID='"<<udid<<"'";
				sql.BeginQuery( str.str() );
				if( sql.hasRecord() )//�ҵ������Ϣ��
				{
					pak.Begin();
					pak<<NM_RECEIPT_STATUS<<true<<true;//��һ��bool�Ƿ�汾��ȷ���ڶ����Ƿ��д����
				}
				else//���û�������Ϣ
				{
					pak.Begin();
					pak<<NM_RECEIPT_STATUS<<true<<false;
				}
				sql.End();
			}
			else
			{
				pak.Begin();
				pak<<NM_RECEIPT_STATUS<<false<<false;
			}
			client->Send( pak );
		}return true;
	case NM_RECEIPT_DATA:
		{
			DEBUGLOG("Receive from %s port %d NM_RECEIPT_DATA \n", client->BindIPString(), client->BindPort() );
			string udid;
			string data;
			ushort countPacket, PacketID;
			int datasize;
			pak>>udid>>countPacket>>PacketID>>datasize;
			//��ȡdata
			data.resize(datasize);
			for (int i = 0 ; i < datasize ; i++ )
			{
				pak>>data[i];
			}
			DEBUGLOG("%d of %d\n",PacketID, countPacket );
			ReceiptMap::iterator it = Receipts.find( udid );
			if( it == Receipts.end() ) //����������
			{
				//����ְ�
				ReceiptFragment rf;
				rf.m_PacketCount = countPacket;
				rf.m_Fragment.insert( make_pair( PacketID, data ) );
				Receipts.insert( make_pair( udid, rf ) );
			}
			else
			{
				map<ushort, string>::iterator fragment = it->second.m_Fragment.find( PacketID );
				if ( fragment == it->second.m_Fragment.end() ) //��ȷû���ظ�����
				{
					it->second.m_Fragment.insert( make_pair( PacketID, data ) );
					if( it->second.m_Fragment.size() == it->second.m_PacketCount ) // ���������ˣ����
					{
						string receipt;
						for ( map<ushort, string>::iterator fit = it->second.m_Fragment.begin();
							fit != it->second.m_Fragment.end();
							fit ++ )
						{
							receipt += fit->second;
						}
						//������֤
						Receipts.erase( it );//ɾ���������
						pak.Begin();
						pak<<NM_RECEIPT_RECEVED;
						client->Send( pak );
						//��ʼ��һ���������ݣ�׼���������߳�
						ReceiptVerifyData* Receipt = NET_NEW ReceiptVerifyData;
						Receipt->m_pURL = m_pUrlResolve;
						Receipt->m_Receipt = receipt;
						//data->m_pEvent = &m_ReslutEvent;
						//��ʼ������ʱ��Ҫ������
						Receipt->m_pReslut = NET_NEW ReslutData;
						Receipt->m_pReslut->m_isSucess = false;
						Receipt->m_pReslut->m_isRealReceipt = false;
						Receipt->m_pReslut->m_Client = client;
						Receipt->m_pReslut->m_UDID = udid;
						DWORD nThreadID;
						::CreateThread(0, 0, HttpReceiptVerifyData, (void *)Receipt, 0, &nThreadID);//����һ���߳�
					}
				}
			}
		}return true;
	}
	return false;
}
//-------------------------------------------------------------------------------------------------------
DWORD WINAPI ReceiptHandle::HttpReceiptVerifyData( LPVOID lpParam )
{
	//�̳�
	//http://www.cppblog.com/kesalin/archive/2007/11/30/37567.aspx
	ReceiptVerifyData* Receipt = static_cast<ReceiptVerifyData*>(lpParam);
	Json::Value tosend;
	tosend["receipt-data"] = Receipt->m_Receipt;
	Json::StyledWriter styled_writer;
	string receipt = styled_writer.write( tosend );

	HINTERNET hSession = 0;
	HINTERNET hConnect = 0;
	HINTERNET hRequest = 0;
	//CrackedUrl crackedUrl( ServerConfig::Instance().VerityDomain() );
	// Open session.
	hSession = OpenSession(0);
	if (hSession != NULL) 
	{
		hConnect = Connect(hSession, Receipt->m_pURL->GetHostName(), Receipt->m_pURL->GetPort());// Connect.
		if (hConnect != NULL) 
		{
			hRequest = OpenRequest(hConnect, L"POST", Receipt->m_pURL->GetPath(), Receipt->m_pURL->GetScheme());// Open request.
			if (hRequest != NULL) 
			{
				if (SendRequest(hRequest, receipt.c_str(), receipt.length()))// Send post data.
				{
					if (EndRequest(hRequest)) // End request
					{
						DWORD Contentlength = 0;
						{//��ȡ�ı�����
							char szBuf[BUFFER_SIZE];
							DWORD dwSize = BUFFER_SIZE;
#ifdef USE_WINHTTP
							int contextLengthId = WINHTTP_QUERY_CONTENT_LENGTH;
#else
							int contextLengthId = HTTP_QUERY_CONTENT_LENGTH;
#endif
							if (QueryInfo(hRequest, contextLengthId, szBuf, &dwSize)) 
							{
								wstring len = (wchar_t*)szBuf;
								wstringstream ss;
								ss << len;
								ss >> Contentlength;
								if( Contentlength > 4096 )
								{
									Contentlength = 4096;
								}
							}
							else
							{
								Contentlength = 4096;
							}
						}
						char* reslut = NET_NEW char[Contentlength];
						if (ReadData(hRequest, reslut, Contentlength, &Contentlength)) 
						{
							if (Contentlength > 0) 
							{
								//����json
								Json::Reader reader;
								Json::Value jsonobject;
								Receipt->m_pReslut->m_isSucess = true;//�������лظ�
								if (reader.parse(reslut, jsonobject))
								{
									if( jsonobject["status"] == 0 )//�ɹ���
									{
										Receipt->m_pReslut->m_isRealReceipt = true;
									}
									Receipt->m_pReslut->m_ReslutJson = jsonobject["receipt"];
								}
								else
								{
									ShowMessage("Error:make json failed!\n");
								}
							}
							else
							{
								ShowMessage("Error:Contentlength < 0!\n");
							}
						}
						else
						{
							ShowMessage("Error:ReadData failed!\n");
						}
						SAFE_DELETE( reslut );
					}
					else
					{
						ShowMessage("Error:EndRequest failed!\n");
					}
				}
				else
				{
					ShowMessage("Error:SendRequest failed!\n");
				}
				CloseInternetHandle(hRequest);
			}
			else
			{
				ShowMessage("Error:OpenRequest failed!\n");
			}
			CloseInternetHandle(hConnect);
		}
		else
		{
			ShowMessage("Error:Connect failed!\n");
		}
		CloseInternetHandle(hSession);
	}
	else
	{
		ShowMessage("Error:Open session!\n");
	}
	//Receipt->m_pReslut;//������
	g_ReceiptHandle->InsertReslut( Receipt->m_pReslut );
	SAFE_DELETE( Receipt );
	return  0;
}
//-------------------------------------------------------------------------------------------------------
void ReceiptHandle::InsertReslut( ReslutData* data )
{
	EnterCriticalSection(&m_csReslutlist);
	m_Reslutlist.push_back( data );
	SetEvent( m_ProcessEvent );
	LeaveCriticalSection(&m_csReslutlist);
}
//-------------------------------------------------------------------------------------------------------
ReceiptHandle::ReslutData* ReceiptHandle::GetReslutData()
{
	ReslutData* result = NULL;
	EnterCriticalSection(&m_csReslutlist);
	if ( !m_Reslutlist.empty() )
	{
		result = (*m_Reslutlist.begin());
		m_Reslutlist.pop_front();
	}
	LeaveCriticalSection(&m_csReslutlist);
	return result;
}

//-------------------------------------------------------------------------------------------------------
DWORD WINAPI ReceiptHandle::PorcessReslut( LPVOID lpParam )
{
	SQLDataBase::COMInitializeEx();//��ʼ�����̵߳���COM
	SQLDataBase sql( ServerConfig::Instance().ConnectString() );
	NetPackage package;
	ReceiptHandle* rh = static_cast<ReceiptHandle*>(lpParam);
	ShowMessage("Receipt process thread is begin");
	DWORD dw;
	while(1)
	{
		dw = WaitForSingleObject(rh->GetEvent(), INFINITE);
		if(dw != WAIT_OBJECT_0)
		{
			ShowMessage("PorcessReslut error");
			return -1;
		}
		ReslutData* result = rh->GetReslutData();
		if ( NULL == result )
		{
			ResetEvent( rh->GetEvent() );
		}
		else
		{
			ShowMessage("%s's Receipt is %s verify and receipt is %s",
				result->m_UDID.c_str(),
				(result->m_isSucess ? "finshed" : "not finshed"),
				(result->m_isRealReceipt ? "real" : "fake") );

			//��ʼ���浽���ݿ�
			//NM_RECEIPT_RESULT���صĵ�һ��bool��ָapple���������߱����Ƿ���Ӧ�ˣ��ڶ���bool��ָ�ö����Ƿ���Ч����
			package.Begin();
			if( result->m_isSucess )//�лظ�����¼����
			{
				if( result->m_isRealReceipt )
				{
					string product_id = result->m_ReslutJson["product_id"].asString();
					string transaction_id = result->m_ReslutJson["transaction_id"].asString();
					stringstream ReceiptSQL;
					ReceiptSQL<<"SELECT * FROM Receipt WHERE RECEIPTID='"<<transaction_id<<"'";//���ȿ����Ƿ����Ѿ���ӹ��Ķ���
					sql.BeginQuery( ReceiptSQL.str() );
					if( !sql.hasRecord() )//û�ҵ�����������ҵ��������ˣ����ظ���¼��
					{
						cout<<"This receipt is new receipt recode it!\n";
						DEBUGLOG("New Receipt %s\n" , result->m_UDID.c_str());
						sql.AddNewRecord();///����¼�¼
						sql.InsertValue("UDID",_variant_t((_bstr_t)result->m_UDID.c_str()));
						sql.InsertValue("RECEIPTSTATUS",_variant_t(result->m_isRealReceipt));
						sql.InsertValue("PRODUCTID",_variant_t((_bstr_t)product_id.c_str()));
						sql.InsertValue("RECEIPTID",_variant_t((_bstr_t)transaction_id.c_str()));
						sql.End();
						{//��������Ӵ���
							stringstream PlayerSQL;
							PlayerSQL<<"SELECT * FROM Players WHERE UDID='"<<result->m_UDID<<"'";
							sql.BeginQuery( PlayerSQL.str() );
							if( sql.hasRecord() )//�ҵ������Ϣ��
							{
								int add = 0;
								switch( product_id[product_id.size()-1] )
								{
								case '1':add = s_Item1; break;
								case '2':add = s_Item2; break;
								case '3':add = s_Item3; break;
								case '4':add = s_Item4; break;
								case '5':add = s_Item5; break;
								case '6':add = s_Item6; break;
								default:cout<<"Undefined product id\n!!";
								}
								int buytimes = sql.GetValue("BuyTimes");
								sql.SetValue( "BuyTimes", buytimes + add );
								package<<NM_RECEIPT_RESULT<<true<<true;//���سɹ�
							}
							else
							{
								ShowMessage("Player %s not find!!", result->m_UDID.c_str() );
								package<<NM_RECEIPT_RESULT<<false<<false;
							}
						}
					}
					else
					{
						package<<NM_RECEIPT_RESULT<<true<<true;
						DEBUGLOG( "But this receipt is old receipt drop it!\n" );
					}
					sql.End();
				}
				else // ��¼һ��ʧ�ܵ���Ϣ
				{
					string str = "SELECT * FROM Receipt";
					sql.BeginQuery( str );
					sql.AddNewRecord();///����¼�¼
					sql.InsertValue("UDID",_variant_t((_bstr_t)result->m_UDID.c_str()));
					sql.InsertValue("RECEIPTSTATUS",_variant_t(result->m_isRealReceipt));
					sql.End();
					package<<NM_RECEIPT_RESULT<<true<<false;
				}
			}
			else
			{
				package<<NM_RECEIPT_RESULT<<false<<false;
			}
			result->m_Client->Send( package );
			result->m_Client->SendImmediately();
			SAFE_DELETE( result );
		}
	}
	SQLDataBase::COMUninitialize();//��ʼ�����̵߳���COM
}
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------