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



/////////////////////////
//		��������	   //
/////////////////////////
#pragma once
#include "Correspondent.h"
#include "Event.hpp"
#include <string>
#include <list>
#include <string>
#include <pthread.h>


//�����࣬��ʼ��ͨѶ���ṩͨѶ�ܵ�
namespace BohgeNet
{
	enum ICorrespondentType
	{
		CT_UDP,
		CT_TCP,
	};
	class NetHost //�����������Ϊ�ͻ��˿��ǵģ��������Ͳ�����
	{
	private:
		struct TCPConnect
		{
			bool*				m_isProcess;
			bool				m_isDomain;
			TCPCorrespondent*	m_Tcp;
			Address				m_Addres;
			Domain				m_Domain;
		};
	private:
		bool											m_isInitialized;
		NetPackage*										m_pLocalPackage;
		bool											m_isConnectingTcp;//�Ƿ���ù�TCP���ӣ�û�����Ͳ��ܷ������Debug�õ�
		bool											m_isTCPConnecting;//tcp���������첽��һ��ʱ��ֻ������һ�����ڶ���ֱ�ӷ����ˣ���ʱ������ô�鷳
		pthread_t										m_TcpConnectThread;
		TCPCorrespondent*								m_pTcp;//�ͻ��˴�һ��tcp
		Event2<bool, TCPCorrespondent&, NetPackage&>	m_TcpEvent;
		UDPCorrespondent*								m_pUdp;//�ͻ��˴�һ��udp
		Event2<bool, UDPCorrespondent&, NetPackage&>	m_UdpEvent;
	public:
		NetHost(void);
		~NetHost(void);
		void Initialization();//ÿһ���̶߳���Ҫ��ʼ��һ�Σ���������ú���
		void Destry();//��Ӧ��ʼ��
		void OnFarmeBegin();//��Driver����,��ȡ��Ϣ
		void OnFarmeEnd();//��Driver����,������Ϣ
		static void* TCPConncetThread(void* prama);
	public:
		void ConnectTCP( const Address& server_address );//���ӷ�����
		bool ConnectUDP( const Address& server_address);//���ӷ�����
		void ConnectTCP( const Domain& domain );//���ӷ�����
		bool ConnectUDP( const Domain& domain );//���ӷ�����
		void DisConnectTCP();//�Ͽ�����
		void DisConnectUDP();
	public:
		BOHGE_FORCEINLINE FunctionPtr AddTCPResponse(IFunction2<bool, TCPCorrespondent&, NetPackage&>* const func)//��ӽ��յ���Ϣ��Ӧ�¼�
		{
			return m_TcpEvent.Connect( func );
		}
		BOHGE_FORCEINLINE FunctionPtr AddUDPResponse(IFunction2<bool, UDPCorrespondent&, NetPackage&>* const func)
		{
			return m_UdpEvent.Connect( func );
		}
		BOHGE_FORCEINLINE void RemoveTCPResponse(FunctionPtr ptr)//���
		{
			m_TcpEvent.Remove(ptr);
		}
		BOHGE_FORCEINLINE void RemoveUDPResponse(FunctionPtr ptr)
		{
			m_UdpEvent.Remove(ptr);
		}
		BOHGE_FORCEINLINE bool TCPSend( NetPackage& pak )
		{
			return m_pTcp->Send( pak );
		}
		BOHGE_FORCEINLINE bool UDPSend( NetPackage& pak )
		{
			if( m_pUdp->isReady() )
			{
				return m_pUdp->Send( pak );
			}
			return false;
		}
		BOHGE_FORCEINLINE NetPackage& Package()
		{
			return *m_pLocalPackage;
		}
		BOHGE_FORCEINLINE bool isTCPReady()
		{
			return m_pTcp->isReady();
		}
		BOHGE_FORCEINLINE bool isUDPReady()
		{
			return m_pUdp->isReady();
		}
	};
}
