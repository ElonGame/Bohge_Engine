//////////////////////////////////////////////////////////////////////////////////////
//
//						The Bohge Engine License (BEL)
//
//	Copyright (c) 2011-2014 Peng Zhao
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in 
//	all copies or substantial portions of the Software. And the logo of 
//	Bohge Engine shall be displayed full screen for more than 3 seconds 
//	when the software is started. Copyright holders are allowed to develop 
//	game edit based on Bohge Engine, The edit must be released under the MIT 
//	open source license if it is going to be published. In no event shall 
//	copyright holders be prohibited from using any code of Bohge Engine 
//	to develop any other analogous game engines.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//
//////////////////////////////////////////////////////////////////////////////////////



#include "StdAfx.h"
#include "IOCPModel.h"
#include "MainDlg.h"
#include "ListenerContext.h"
#include "ClientContext.h"
#include "IOContext.h"
#include "SQLDataBase.h"
#include "ServerConfig.h"

using namespace std;

extern void OnClientQuit( SmartPtr<ClientContext>& client, BohgeNet::NetPackage& pak, SQLDataBase& sql );



CIOCPModel::CIOCPModel(void):
m_nThreads(0),
m_hShutdownEvent(NULL),
m_hIOCompletionPort(NULL),
m_phWorkerThreads(NULL),
m_strIP(DEFAULT_IP),
m_pListenContext( NULL ),
m_nPort(DEFAULT_PORT)
{
}


CIOCPModel::~CIOCPModel(void)
{
	// ȷ����Դ�����ͷ�
	this->Stop();
}




///////////////////////////////////////////////////////////////////
// �������߳�:   ΪIOCP�������Ĺ������߳�
//         Ҳ����ÿ����ɶ˿��ϳ�����������ݰ�,�ͽ�֮ȡ�������д�����߳�
///////////////////////////////////////////////////////////////////

DWORD WINAPI CIOCPModel::_WorkerThread(LPVOID lpParam)
{    
	THREADPARAMS_WORKER* pParam = (THREADPARAMS_WORKER*)lpParam;
	CIOCPModel* pIOCPModel = (CIOCPModel*)pParam->pIOCPModel;
	int nThreadNo = (int)pParam->nThreadNo;
	
	SQLDataBase::COMInitializeEx();//��ʼ�����̵߳���COM
	SQLDataBase sql( ServerConfig::Instance().ConnectString() );

	bool isAdd = false;

	BohgeNet::NetPackage pak;

	ShowMessage(_T("Working thread is begin,ID: %d."),nThreadNo);

	OVERLAPPED*						pOverlapped = NULL;
	SmartPtr<ISocketContext>*		pSmartPtrContext = NULL;
	DWORD							dwBytesTransfered = 0;

	// ѭ����������,֪�����յ�Shutdown��ϢΪֹ
	while (WAIT_OBJECT_0 != WaitForSingleObject(pIOCPModel->m_hShutdownEvent, 0))
	{
		if ( pIOCPModel->GetReleaser().isNeedRelease() )
		{
			pIOCPModel->GetReleaser().DoRelease();
		}
		BOOL bReturn = GetQueuedCompletionStatus(
			pIOCPModel->m_hIOCompletionPort,// ����������ǽ������Ǹ�Ψһ����ɶ˿�  
			&dwBytesTransfered,//����ǲ�����ɺ󷵻ص��ֽ���  
			(PULONG_PTR)&pSmartPtrContext,// ��������ǽ�����ɶ˿ڵ�ʱ��󶨵��Ǹ��Զ���ṹ�����  
			&pOverlapped,// ���������������Socket��ʱ��һ�������Ǹ��ص��ṹ
			INFINITE);// �ȴ���ɶ˿ڵĳ�ʱʱ��,����̲߳���Ҫ������������,�Ǿ�INFINITE������
		// ����յ������˳���־,��ֱ���˳�
		if ( EXIT_CODE==(DWORD)pSmartPtrContext )
		{
			break;
		}
		// �ж��Ƿ�����˴���
		if( !bReturn )  
		{  
			pIOCPModel->MutexClinetSmartPtrMap();
			if( pIOCPModel->_isInContextList( pSmartPtrContext ) )
			{
				DWORD dwErr = GetLastError();
				// ��ʾһ����ʾ��Ϣ
				OnClientQuit( check_pointer_cast<ClientContext>(*pSmartPtrContext), pak, sql );
				if( !pIOCPModel->HandleError( pSmartPtrContext, dwErr ) )
				{
					break;//��������ɶ˿ڳ�����,ֱ���Ƴ��߳���
				}
			}
			pIOCPModel->UnmutexClinetSmartPtrMap();
			continue;  
		}  
		else  
		{  	
			// ��ȡ����Ĳ���
			IOContext* pIoContext = CONTAINING_RECORD(pOverlapped, IOContext, OverLopped());  
			// �ж��Ƿ��пͻ��˶Ͽ���
			if((0 == dwBytesTransfered) && ( RECV_POSTED==pIoContext->m_OpType || SEND_POSTED==pIoContext->m_OpType))  
			{  
				pIOCPModel->MutexClinetSmartPtrMap();
				if( pIOCPModel->_isInContextList( pSmartPtrContext ) )
				{
					DEBUGLOG( _T("Client %s:%d disconnect."), (*pSmartPtrContext)->BindIPString(), ntohs((*pSmartPtrContext)->BindPort()) );
					OnClientQuit( check_pointer_cast<ClientContext>(*pSmartPtrContext), pak, sql );
					// �ͷŵ���Ӧ����Դ
					pIOCPModel->_RemoveContext( pSmartPtrContext );
				}
				pIOCPModel->UnmutexClinetSmartPtrMap();
				continue;  
			}  
			else
			{
				if( (*pSmartPtrContext)->isConnect() )
				{
					(*pSmartPtrContext)->Actived();//���ʱ�����ڱ�������
					SmartPtr<ISocketContext>* temp = (*pSmartPtrContext)->DoWork(pIoContext, pak, sql, isAdd);//����з���,ֻ������listener���ص�
					if ( NULL != temp )
					{
						if( isAdd )
						{
							pIOCPModel->_AddToContextList( temp );
						}
						else
						{
							pIOCPModel->MutexClinetSmartPtrMap();
							DEBUGLOG( _T("client error on clinetContext %s:%d disconnet."), (*pSmartPtrContext)->BindIPString(), ntohs((*pSmartPtrContext)->BindPort()) );
							OnClientQuit(check_pointer_cast<ClientContext>(*temp), pak, sql );
							pIOCPModel->_RemoveContext( temp );
							pIOCPModel->UnmutexClinetSmartPtrMap();
						}
					}
				}
			}//if
		}//if

	}//while

	TRACE(_T("Work thread %d exit.\n"),nThreadNo);
	SQLDataBase::COMUninitialize();//��ʼ�����̵߳���COM
	// �ͷ��̲߳���
	RELEASE(lpParam);	

	return 0;
}

void CIOCPModel::_DisconnectDeadClient()
{
	EnterCriticalSection( &m_csContextList );
	int disconnectCount = 0;
	for( ClinetSmartPtrMap::iterator it = m_ContextPtrMap.begin() ;
		it != m_ContextPtrMap.end() ;
		 )
	{
		if(		(*it->second)->GetType() == ISocketContext::ST_CLIENT
			&&	(*it->second)->isActived() == false )
		{
			disconnectCount ++;
			DEBUGLOG( _T("Client %s:%d has been kick."), (*it->second)->BindIPString(), ntohs((*it->second)->BindPort()) );
			OnClientQuit( check_pointer_cast<ClientContext>((*it->second)), m_Package, *m_pSql );
			(*it->second)->Disconnect();
			m_Releaser.AddToReleaseList( it->second );
			it = m_ContextPtrMap.erase( it );
		}
		else
		{
			it++;
		}
	}
	if( disconnectCount != 0 )
	{
		ClientDisonnect(disconnectCount);
	}
	LeaveCriticalSection( &m_csContextList );
}


//====================================================================================
//
//				    ϵͳ��ʼ������ֹ
//
//====================================================================================




////////////////////////////////////////////////////////////////////
// ��ʼ��WinSock 2.2
bool CIOCPModel::LoadSocketLib()
{    
	WSADATA wsaData;
	int nResult;
	nResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	// ����(һ�㶼�����ܳ���)
	if (NO_ERROR != nResult)
	{
		ShowMessage(_T(" Initialize WinSock 2.2 fail !\n"));
		return false; 
	}

	return true;
}

//////////////////////////////////////////////////////////////////
//	����������
bool CIOCPModel::Start()
{
	m_isBegin = true;

	ShowMessage( _T("SQL connect string is %s"), ServerConfig::Instance().ConnectString().c_str() );
	ShowMessage( _T("Verify domain is %s"), ServerConfig::Instance().VerifyString().c_str() );

	// ��ʼ���̻߳�����
	InitializeCriticalSection(&m_csContextList);

	// ����ϵͳ�˳����¼�֪ͨ
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// ��ʼ��IOCP
	if (false == _InitializeIOCP())
	{
		ShowMessage(_T("Initialize IOCP fail !\n"));
		return false;
	}
	else
	{
		ShowMessage(_T("\nIOCP Initialize success\n."));
	}

	// ��ʼ��Socket
	// �������ڼ�����Socket����Ϣ
	m_pListenContext = new Listerner;
	this->_AddToContextList( m_pListenContext->SelfSmartPtr() );
	if( false == m_pListenContext->InitializeListenSocket( m_hIOCompletionPort, m_strIP, m_nPort ) )
	{
		ShowMessage(_T("Listen Socket Initialize fail !\n"));
		this->_DeInitialize();
		return false;
	}
	else
	{
		ShowMessage(_T("Listen Socket Initialize success."));
	}

	SQLDataBase::COMInitializeEx();//��ʼ�����̵߳���COM
	m_pSql = NET_NEW SQLDataBase( ServerConfig::Instance().ConnectString() );

	ShowMessage(_T("all Initialize done, witing connect....\n"));

	return true;
}


////////////////////////////////////////////////////////////////////
//	��ʼ����ϵͳ�˳���Ϣ,�˳���ɶ˿ں��߳���Դ
void CIOCPModel::Stop()
{
	if( m_isBegin )
	{
		m_isBegin = false;
		if( m_pListenContext!=NULL && m_pListenContext->Socket()!=INVALID_SOCKET )
		{
			// ����ر���Ϣ֪ͨ
			SetEvent(m_hShutdownEvent);

			for (int i = 0; i < m_nThreads; i++)
			{
				// ֪ͨ���е���ɶ˿ڲ����˳�
				PostQueuedCompletionStatus(m_hIOCompletionPort, 0, (DWORD)EXIT_CODE, NULL);
			}

			// �ȴ����еĿͻ�����Դ�˳�
			WaitForMultipleObjects(m_nThreads, m_phWorkerThreads, TRUE, INFINITE);

			// ����ͻ����б���Ϣ
			this->_ClearContextList();

			// �ͷ�������Դ
			this->_DeInitialize();

			SAFE_DELETE( m_pSql );
			SQLDataBase::COMUninitialize();//��ʼ�����̵߳���COM

			ShowMessage(_T("Stoped!\n"));
		}	
	}
}


////////////////////////////////
// ��ʼ����ɶ˿�
bool CIOCPModel::_InitializeIOCP()
{
	// ������һ����ɶ˿�
	m_hIOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0 );

	if ( NULL == m_hIOCompletionPort)
	{
		ShowMessage(_T("creat complattion port fail ! error code: %d!\n"), WSAGetLastError());
		return false;
	}

	// ���ݱ����еĴ���������,������Ӧ���߳���
	m_nThreads = WORKER_THREADS_PER_PROCESSOR * _GetNoOfProcessors();

	// Ϊ�������̳߳�ʼ�����
	m_phWorkerThreads = new HANDLE[m_nThreads];

	// ���ݼ�����������������������߳�
	DWORD nThreadID;
	for (int i = 0; i < m_nThreads; i++)
	{
		THREADPARAMS_WORKER* pThreadParams = new THREADPARAMS_WORKER;
		pThreadParams->pIOCPModel = this;
		pThreadParams->nThreadNo  = i+1;
		m_phWorkerThreads[i] = ::CreateThread(0, 0, _WorkerThread, (void *)pThreadParams, 0, &nThreadID);
	}

	TRACE(" Create _WorkerThread %d .\n", m_nThreads );

	return true;
}


////////////////////////////////////////////////////////////
//	����ͷŵ�������Դ
void CIOCPModel::_DeInitialize()
{
	// ɾ���ͻ����б�Ļ�����
	DeleteCriticalSection(&m_csContextList);

	// �ر�ϵͳ�˳��¼����
	RELEASE_HANDLE(m_hShutdownEvent);

	// �ͷŹ������߳̾��ָ��
	for( int i=0;i<m_nThreads;i++ )
	{
		RELEASE_HANDLE(m_phWorkerThreads[i]);
	}

	RELEASE(m_phWorkerThreads);

	// �ر�IOCP���
	RELEASE_HANDLE(m_hIOCompletionPort);

	// �رռ���Socket
	//RELEASE(m_pListenContext);//�����������������List,����Ͳ���Ҫ�ٴ��ͷ���

	ShowMessage("All resource is released.\n");
}

//====================================================================================
//
//				    ContextList ��ز���
//
//====================================================================================


//////////////////////////////////////////////////////////////
// ���ͻ��˵������Ϣ�洢��������
void CIOCPModel::_AddToContextList( SmartPtr<ISocketContext> *pSocketContextPtr )
{
	EnterCriticalSection(&m_csContextList);
	m_ContextPtrMap.insert( make_pair( (int)pSocketContextPtr, pSocketContextPtr) );
	LeaveCriticalSection(&m_csContextList);
}
////////////////////////////////////////////////////////////////
//�ͻ����Ƿ���buff��
bool CIOCPModel::_isInContextList( SmartPtr<ISocketContext> *pSocketContextPtr )
{
	//EnterCriticalSection(&m_csContextList);//���ⲿ������
	ClinetSmartPtrMap::iterator it = m_ContextPtrMap.find( (int)pSocketContextPtr );
	//LeaveCriticalSection(&m_csContextList);
	return it != m_ContextPtrMap.end();
}
////////////////////////////////////////////////////////////////
//	�Ƴ�ĳ���ض���Context
void CIOCPModel::_RemoveContext( SmartPtr<ISocketContext> *pSocketContextPtr )
{
	//EnterCriticalSection(&m_csContextList);//���ⲿ������
	ClinetSmartPtrMap::iterator it = m_ContextPtrMap.find( (int)pSocketContextPtr );
	if( it != m_ContextPtrMap.end() )
	{
		(*it->second)->Disconnect();
		//SAFE_DELETE( it->second );//�����˾͸�һ������,����Ӧ��ISocketContext����������Delete���ָ��-----�ڶ����޸�,���ﲻֱ��ɾ��,����releaser����
		m_Releaser.AddToReleaseList( it->second );
		m_ContextPtrMap.erase( it );
		ClientDisonnect(1);
	}
	//LeaveCriticalSection(&m_csContextList);
}

////////////////////////////////////////////////////////////////
// ��տͻ�����Ϣ
void CIOCPModel::_ClearContextList()
{
	EnterCriticalSection(&m_csContextList);
	for( ClinetSmartPtrMap::iterator it = m_ContextPtrMap.begin() ;
		it != m_ContextPtrMap.end() ;
		it++ )
	{
		SAFE_DELETE( it->second );
		ClientDisonnect(1);
	}
	m_ContextPtrMap.clear();
	LeaveCriticalSection(&m_csContextList);
}



//====================================================================================
//
//				       ����������������
//
//====================================================================================



////////////////////////////////////////////////////////////////////
// ��ñ�����IP��ַ
std::string CIOCPModel::GetLocalIP()
{
	// ��ñ���������
	char hostname[MAX_PATH] = {0};
	gethostname(hostname,MAX_PATH);                
	struct hostent FAR* lpHostEnt = gethostbyname(hostname);
	if(lpHostEnt == NULL)
	{
		m_strIP = DEFAULT_IP;
		return DEFAULT_IP;
	}

	// ȡ��IP��ַ�б��еĵ�һ��Ϊ���ص�IP(��Ϊһ̨�������ܻ�󶨶��IP)
	LPSTR lpAddr = lpHostEnt->h_addr_list[0];      

	// ��IP��ַת�����ַ�����ʽ
	struct in_addr inAddr;
	memmove(&inAddr,lpAddr,4);
	m_strIP = std::string( inet_ntoa(inAddr) );        
	return m_strIP;
}

///////////////////////////////////////////////////////////////////
// ��ñ����д�����������
int CIOCPModel::_GetNoOfProcessors()
{
	SYSTEM_INFO si;

	GetSystemInfo(&si);

	return si.dwNumberOfProcessors;
}



/////////////////////////////////////////////////////////////////////
// �жϿͻ���Socket�Ƿ��Ѿ��Ͽ�,������һ����Ч��Socket��Ͷ��WSARecv����������쳣
// ʹ�õķ����ǳ��������socket��������,�ж����socket���õķ���ֵ
// ��Ϊ����ͻ��������쳣�Ͽ�(����ͻ��˱������߰ε����ߵ�)��ʱ��,�����������޷��յ��ͻ��˶Ͽ���֪ͨ��

bool CIOCPModel::_IsSocketAlive(SOCKET s)
{
	int nByteSent=send(s,"",0,0);
	if (-1 == nByteSent) return false;
	return true;
}

///////////////////////////////////////////////////////////////////
// ��ʾ��������ɶ˿��ϵĴ���
bool CIOCPModel::HandleError( SmartPtr<ISocketContext> *pSocketContextPtr,const DWORD& dwErr )
{
	// ����ǳ�ʱ��,���ټ����Ȱ�  
	if(WAIT_TIMEOUT == dwErr)  
	{  	
		// ȷ�Ͽͻ����Ƿ񻹻���...
		if( !_IsSocketAlive( (*pSocketContextPtr)->Socket()) )
		{
			ShowMessage( _T("_IsSocketAlive deteced client unhandle exit") );
			this->_RemoveContext( pSocketContextPtr );
			return true;
		}
		else
		{
			ShowMessage( _T("net work over time!retry...") );
			return true;
		}
	} 
	// �����ǿͻ����쳣�˳���
	else if( ERROR_NETNAME_DELETED==dwErr )
	{
		DEBUGLOG( _T("ERROR_NETNAME_DELETED  deteced client unhandle exit!") );
		this->_RemoveContext( pSocketContextPtr );
		return true;
	}

	else
	{
		ShowMessage( _T("opretor on complation port is fail,thread exit, error code: %d"),dwErr );
		return false;
	}
	return true;
}

void CIOCPModel::OnTimer()
{
	this->_DisconnectDeadClient();
}
