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

#pragma once
#include "Event.hpp"
#include "Correspondent.h"
#include "NetBase.h"
#include <map>
#include <string>


namespace BohgeEngine
{
	class Engine;
}
namespace BohgeGame
{
	class ShopHelper
	{
	public:
		enum ShopStatus
		{
			NONE,
			VERSTION_NOT_MATCH,
			NEW_USER,
			NORMAL,
		};
	private:
		enum
		{
			ITEM_COUNT	=	6,//������Ʒ����
		};
	private:
		typedef std::map<int, std::wstring> ItemMap;
	private:
		bool				m_isResend;
		bool				m_isAllowIAP;
		bool				m_isLoadDone;
		bool				m_isIAPProcess;//�Ƿ������ⶩ���ڵȴ�����״̬
		ItemMap				m_ItmeString;

		//���粿��
		ShopStatus				m_ShopStatus;
		bool					m_isRecevCustomData;//���յ���Ҫshop������ź�
		bool					m_isSendedReceipt;//���Ͷ������ݺ�����Ϊtrue�����������������ݺ�����Ϊfalse
		bool					m_isServerRecevr;//�������Ƿ��յ��˶���
		std::string				m_Recepit;
		Event<bool,bool>		m_OnReceiptResult;//����������Ӧ����
	private:
		ShopHelper();
		bool CheckReceipt();
	public:
		void Request();//��������
		void InsertItemString(int price, const std::wstring& pri, const std::wstring& str);//��������ַ���
		const std::wstring& GetString(int index);
		void BuyItem(int index);
		void PaymentTransactionStatePurchased( const std::string& receipt);
		void PaymentTransactionStateFailed();
		bool isUnprocessReceipt();//����ʱ����û�д���Ķ���
		void ShopServerStatus(BohgeEngine::Engine& engine);
	public://��Զ����Ĳ�����
		bool ListenReceipt( BohgeNet::TCPCorrespondent& sender, BohgeNet::NetPackage& pak);
		void SendReceipt(const std::string& receiptEncode);
	public:
		static BOHGE_FORCEINLINE ShopHelper& Instance()
		{
			static ShopHelper sh;
			return sh;
		}
		BOHGE_FORCEINLINE bool isLoadDone()
		{
			return m_isLoadDone;
		}
		BOHGE_FORCEINLINE bool& isAllowIAP()
		{
			return m_isAllowIAP;
		}
		BOHGE_FORCEINLINE bool isWaitReslut()//�Ƿ��ڵȴ�����
		{
			return m_isIAPProcess;
		}
		BOHGE_FORCEINLINE Event<bool,bool>& ReceiptReslutCallBack()
		{
			return m_OnReceiptResult;
		}
		BOHGE_FORCEINLINE ShopStatus GetShopStatus()
		{
			return m_ShopStatus;
		}
		BOHGE_FORCEINLINE bool isRecevData()
		{
			if( m_isRecevCustomData )
			{
				m_isRecevCustomData = false;
				return true;
			}
			return false;
		}
	};
}