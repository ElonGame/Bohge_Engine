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



/////////////////////////////////////
//			������Ʒ�м���		   //
/////////////////////////////////////
#include "ShopHelper.h"
#include "Predefine.h"
#include "BohgeNet.h"
#include "NetMessage.h"
#include "NetBase.h"
#include "NetHost.h"
#include "Serializer.h"
#include "Base64.h"
#include "Engine.h"
#include "NetHelper.h"
#include "IOSystem.h"

#include <sstream>
#include <math.h>


using namespace std;
using namespace BohgeNet;
using namespace BohgeEngine;

extern void RefreshItem();
extern void IAPBuyItem(int index);


namespace BohgeGame
{
	const string TempRecepitName = "receipt.tmp";

	//-------------------------------------------------------------------------------------------------------
	ShopHelper::ShopHelper()
		:m_isLoadDone(false),
		m_isAllowIAP(false),
		m_isIAPProcess(false),
		m_isResend(false),
		m_isSendedReceipt(false),
		m_isServerRecevr(false),
		m_isRecevCustomData(false),
		m_ShopStatus(NONE)
	{

	}
	//-------------------------------------------------------------------------------------------------------
	bool ShopHelper::CheckReceipt()
	{
		if( IOINSTANCE.isExist( TempRecepitName )//����ļ�Ŀ¼���д��ļ�˵���ϴ�û�д������
			&& !m_isIAPProcess)//�Ҳ��������ڴ����IAP
		{
			Serializer ser( IOINSTANCE.WriteFolder() + TempRecepitName );
			string receipt;
			ser>>receipt;
			m_Recepit = receipt;
			this->SendReceipt( receipt );
			m_isIAPProcess = true;
			return true;
		}
		return false;
	}
	//-------------------------------------------------------------------------------------------------------
	bool ShopHelper::isUnprocessReceipt()
	{
		if( IOINSTANCE.isExist( TempRecepitName )//����ļ�Ŀ¼���д��ļ�˵���ϴ�û�д������
			&& !m_isIAPProcess)//�Ҳ��������ڴ����IAP
		{
			//this->ShopServerStatus();
			return true;
		}
		return false;
	}
	//-------------------------------------------------------------------------------------------------------
	void ShopHelper::InsertItemString(int price, const std::wstring& pri, const std::wstring& str)
	{
		wstringstream ws;
		ws<<str<<" "<<pri;
		m_ItmeString.insert( make_pair(price, ws.str() ) );
		m_isLoadDone = m_ItmeString.size() == ITEM_COUNT ? true : false;
	}
	//-------------------------------------------------------------------------------------------------------
	const std::wstring& ShopHelper::GetString(int index)
	{
		ASSERT( index < ITEM_COUNT );
		int i = 0 ;
		for ( ItemMap::iterator it = m_ItmeString.begin();
			it != m_ItmeString.end();
			it ++)
		{
			if( index == i )
			{
				return it->second;
			}
			i++;
		}
		return m_ItmeString.begin()->second;
	}
	//-------------------------------------------------------------------------------------------------------
	void ShopHelper::BuyItem(int index)
	{
		//�����ʱ���鱾���Ƿ���û������˵����еĻ����ȴ������
		if( !this->CheckReceipt()
			&& !m_isIAPProcess)//�Ҳ��������ڴ����IAP
		{
			m_isIAPProcess = true;
			IAPBuyItem(index);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ShopHelper::Request()
	{
		if( false == m_isLoadDone )
		{
			m_isLoadDone = false;
			m_ItmeString.clear();
			RefreshItem();
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ShopHelper::PaymentTransactionStatePurchased( const std::string& receipt)
	{
		m_Recepit = Base64::Encode( receipt );;//���ܶ���
		Serializer ser( IOINSTANCE.WriteFolder() + TempRecepitName );
		ser<<m_Recepit;//������ܺ�Ķ���
		this->SendReceipt( m_Recepit );
	}
	//-------------------------------------------------------------------------------------------------------
	void ShopHelper::PaymentTransactionStateFailed()
	{
		//ֱ��֪ͨ����ʧ��
		m_isIAPProcess = false;
		m_OnReceiptResult.Multicast(false);
	}
	//-------------------------------------------------------------------------------------------------------
	void ShopHelper::ShopServerStatus(Engine& engine)
	{
		m_isResend = true;
		//m_Package.Begin();
		//m_Package<<NM_RECEIPT_STATUS<<SERVER_VISION<<NetContent::Instance().UDID();
		//m_pReceiptClient->Send( m_Package, true );
		engine.GetNet()->Package().Begin();
		engine.GetNet()->Package()<<NM_RECEIPT_STATUS<<SERVER_VISION<<NetHelper::Instance().UDID();
		engine.GetNet()->TCPSend( engine.GetNet()->Package() );
	}
	//-------------------------------------------------------------------------------------------------------
	bool ShopHelper::ListenReceipt(TCPCorrespondent& sender, NetPackage& pak)
	{
		//������
		//static uint Times = 0;
		//if( m_isSendedReceipt //���뷢�����������ظ�
		//	&& !m_isServerRecevr )//����������û���յ�����
		//{
		//	Times += timespan;
		//	if( Times >= 10000 )//10��һ���Զ��ظ����Ͷ���
		//	{
		//		DEBUGLOG("Receipt resend to service !\n");
		//		Times = 0;
		//		this->SendReceipt( m_Recepit );
		//	}
		//}
		//static uint StatusTime = 0;
		//if ( m_isResend )
		//{
		//	StatusTime += timespan;
		//	if( StatusTime >= 5000 )//5��һ���Զ��ظ�����
		//	{
		//		DEBUGLOG("StatusTime resend to service !\n");
		//		StatusTime = 0;
		//		this->ShopServerStatus( );
		//	}
		//}
		//if( 0 != m_pReceiptClient->Listen( m_Package ) )
		//{
		//	m_isSendedReceipt = false;
		//	m_Package.Begin();
		//	int message;
		//	m_Package>>message;
		//	switch( message )
		//	{
		//	case NM_RECEIPT_RECEVED:
		//		{
		//			m_isServerRecevr = true;
		//		}break;
		//	case NM_RECEIPT_STATUS:
		//		{
		//			bool isMatch;
		//			m_Package>>isMatch;
		//			if( isMatch )
		//			{
		//				bool isExites;
		//				m_Package>>isExites;
		//				if( isExites )
		//				{
		//					m_ShopStatus = NORMAL;
		//					this->CheckReceipt();
		//				}
		//				else
		//				{
		//					m_ShopStatus = NEW_USER;
		//				}
		//			}
		//			else
		//			{
		//				m_ShopStatus = VERSTION_NOT_MATCH;
		//			}
		//			m_isRecevCustomData = true;
		//			m_isResend = false;
		//			StatusTime = 0;
		//		}break;
		//	case NM_RECEIPT_RESULT:
		//		{
		//			bool isAppleRespones;
		//			bool isReal;
		//			m_Package>>isAppleRespones>>isReal;
		//			if( isAppleRespones )//�����֤��ɾ���������
		//			{
		//				File::Instance().DeleteLocalFile( File::Instance().WritePath() + TempRecepitName );
		//			}
		//			m_OnReceiptResult.Multicast(isReal);//����м����ߣ�֪ͨ�����ߴ�����
		//			m_isIAPProcess = false;
		//		}break;
		//	}
		//}
		m_isSendedReceipt = false;
		pak.Begin();
		int message;
		pak>>message;
		switch( message )
		{
		case NM_RECEIPT_RECEVED:
			{
				m_isServerRecevr = true;
			}break;
		case NM_RECEIPT_STATUS:
			{
				bool isMatch;
				pak>>isMatch;
				if( isMatch )
				{
					bool isExites;
					pak>>isExites;
					if( isExites )
					{
						m_ShopStatus = NORMAL;
						this->CheckReceipt();
					}
					else
					{
						m_ShopStatus = NEW_USER;
					}
				}
				else
				{
					m_ShopStatus = VERSTION_NOT_MATCH;
				}
				m_isRecevCustomData = true;
				m_isResend = false;
			}break;
		case NM_RECEIPT_RESULT:
			{
				bool isAppleRespones;
				bool isReal;
				pak>>isAppleRespones>>isReal;
				if( isAppleRespones )//�����֤��ɾ���������
				{
					IOINSTANCE.DeleteLocalFile( IOINSTANCE.WriteFolder() + TempRecepitName );
				}
				m_OnReceiptResult.Multicast(isReal);//����м����ߣ�֪ͨ�����ߴ�����
				m_isIAPProcess = false;
			}break;
		}
		return false;
	}
	//-------------------------------------------------------------------------------------------------------
	void ShopHelper::SendReceipt(const std::string& receiptEncode)
	{
		m_isServerRecevr = false;
		m_isSendedReceipt = true;
		int eachUDPdataszie = BUFFER_SIZE //�����ݶγ���
			- NetPackage::Sizeof( NetHelper::Instance().UDID() )//udid����Ҫ�����ݶγ���
			- sizeof(short)//�ܰ���
			- sizeof(short)//�ְ���
			- sizeof(int)//��Ҫ�����ݶϵĴ�С
			- sizeof(NM_RECEIPT_DATA);//��������
		ushort countPacket = (ushort)ceil( static_cast<float>(receiptEncode.size()) / eachUDPdataszie );//��Ҫ��UDP��������
		//NetPackage package;
		for ( ushort i = 0 ; i < countPacket; i ++ )
		{
			//package.Begin();
			Engine::Instance().GetNet()->Package().Begin();
			string data;
			if( ( receiptEncode.size() - eachUDPdataszie * i ) > eachUDPdataszie )
			{//����eachUDPdataszie��С����
				data = receiptEncode.substr( eachUDPdataszie * i, eachUDPdataszie );
			}
			else
			{
				data = receiptEncode.substr( eachUDPdataszie * i, receiptEncode.size() - eachUDPdataszie * i );
			}
			//package<<NM_RECEIPT_DATA<<NetHelper::Instance().UDID()<<countPacket<<i<<(int)data.size();
			Engine::Instance().GetNet()->Package()<<NM_RECEIPT_DATA<<NetHelper::Instance().UDID()<<countPacket<<i<<(int)data.size();
			for ( int j = 0; j < data.size() ; j++ )//���浥�����ַ���������ʹ��<<stringӦΪ������Ҫ�Զ����һ��0��Ϊ��ֹ�����ְ�����Ҫ
			{
				Engine::Instance().GetNet()->Package()<< data[j];
			}
			//m_pReceiptClient->SendUndropData( package, false );
			Engine::Instance().GetNet()->TCPSend( Engine::Instance().GetNet()->Package() );
		}
	}
}