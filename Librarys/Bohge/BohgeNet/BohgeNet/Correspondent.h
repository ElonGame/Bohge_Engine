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
//	 ������/�ͻ���   //
/////////////////////////
#pragma once
#include "BohgeNet.h"
#include "NetBase.h"
#include "NetUtility.h"
#include "Packer.h"
#include <vector>
#include <list>
#include <map>


namespace BohgeNet
{
	//////////////////////
	//		��������	//
	//////////////////////
	class ICorrespondent
	{
	protected:
		Address			m_Server;
		Domain			m_Domain;
		bool			m_isReady;
	protected:
		ICorrespondent();
		virtual ~ICorrespondent();
	protected:
		bool ConnectWithDomain( const Domain& domain );
		virtual bool ConnectWithAddress( const Address& server ) = 0;
		virtual void Close() = 0;
		virtual bool Receive( NetPackage& pak ) = 0; //����ѶϢ
		virtual void OnFrameEnd(  ) = 0;//��ÿ֡������ʱ�򣬷��ͱ�������
	public:
		virtual bool Send( NetPackage& pak ) = 0;//����ѶϢ
	public:
		BOHGE_FORCEINLINE bool isReady()
		{
			return m_isReady;
		}
	public:
		BOHGE_FORCEINLINE const Address& ServerAddress()
		{
			return m_Server;
		}
		BOHGE_FORCEINLINE const Domain& ServerDomain()
		{
			return m_Domain;
		}
	};

	//////////////////////
	//		UDP����		//
	//////////////////////
	class UDPCorrespondent : public ICorrespondent // udp�ϲ�������
	{
		friend class NetHost;
	protected:
		std::vector<byte>	m_vBuffer;//�������ȡ������
		uint				m_SendID;//��������ID
		uint				m_ReciveID;//���ܵ�ID
		SocketUDP*			m_pSocket;
	protected:
		UDPCorrespondent( ); //������˿�
		UDPCorrespondent( ushort port );//��ָ���˿�
		virtual ~UDPCorrespondent();
	protected:
		virtual bool ConnectWithAddress( const Address& server );
		virtual void Close();
		virtual bool Receive( NetPackage& pak ); //����ѶϢ
		virtual void OnFrameEnd( ){ }//udpʵʱ���ͣ�������ǿյ�
	public:
		virtual bool Send( NetPackage& pak );//����ѶϢ
	};

	//////////////////////
	//	UDP Server		//
	//////////////////////
	class UDPServer
	{
	private:
		std::vector<byte>	m_vBuffer;//�������ȡ������
		const uint			m_SendID;//��������ID
		SocketUDP*			m_pSocket;
	public:
		UDPServer( ushort port );
		~UDPServer();
	public:
		bool RecevieFrom( Address& ad, NetPackage& pak);
		bool SendTo( Address& ad, NetPackage& pak );
	};


	//////////////////////
	//		TCP����		//
	//////////////////////
	class TCPCorrespondent : public ICorrespondent // TCP�ϲ�������
	{
		friend class NetHost;
	private:
		bool				m_isGeting;//������Ͽ���ȡ��������
		bool				m_isMulitSending;//��ηְ����ͣ��ȴ����ؽ���ȷ����
		uint				m_RecevedCount;//�յ����ݰ�Ƭ�εĸ���
		SocketTCP*			m_pSocket;
		Packer				m_Packer;
		Packer::Buffer		m_vSendRecev;//����õķ��ͣ�����buffer
	protected:
		TCPCorrespondent( );
		~TCPCorrespondent();
	protected:
		virtual bool ConnectWithAddress( const Address& server );
		virtual void Close();
		virtual bool Receive( NetPackage& pak ); //����ѶϢ
		virtual void OnFrameEnd();
	public:
		virtual bool Send( NetPackage& pak );//����ѶϢ
        BOHGE_FORCEINLINE bool isConnect()
        {
            return m_pSocket->isConnect();
        }
	protected:
		BOHGE_FORCEINLINE bool DoSend()//ʵ�ʷ������ݵĴ���
		{
			if ( !m_isMulitSending )
			{
				int size = 0;
				if( m_Packer.SendData( &m_vSendRecev[0], size ) )
				{
					if( m_Packer.isMulitSend() ) //ֻ�е��Ƕ�η��͵�ʱ��
					{
						m_isMulitSending = true;
					}
					return m_pSocket->Send( &m_vSendRecev[0], size );
				}
			}
			return true;
		}
	};


}
