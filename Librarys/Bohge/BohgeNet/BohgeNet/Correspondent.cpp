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
//		ͨѶԱ		   //
/////////////////////////
#include "Correspondent.h"
#include "NetBase.h"


namespace BohgeNet
{



	//-------------------------------------------------------------------------------------------------------
	ICorrespondent::ICorrespondent()
		:m_isReady(false)
	{

	}
	//-------------------------------------------------------------------------------------------------------
	ICorrespondent::~ICorrespondent()
	{

	}
	//-------------------------------------------------------------------------------------------------------
	bool ICorrespondent::ConnectWithDomain( const Domain& domain )
	{
		m_Domain = domain;
		return this->ConnectWithAddress( m_Domain.ToAddress() );
	}




	//////////////////////
	//		UDP����		//
	//////////////////////
	UDPCorrespondent::UDPCorrespondent( ushort port )
		:m_SendID(0),
		m_ReciveID(0)
	{
		m_pSocket = NET_NEW SocketUDP( port );
		m_vBuffer.resize( UDPBUFFER_SIZE );
	}
	//-------------------------------------------------------------------------------------------------------
	UDPCorrespondent::UDPCorrespondent( )
		:m_SendID(0),
		m_ReciveID(0)
	{
		m_pSocket = NET_NEW SocketUDP( 0 );
		m_vBuffer.resize( UDPBUFFER_SIZE );
	}
	//-------------------------------------------------------------------------------------------------------
	UDPCorrespondent::~UDPCorrespondent(void)
	{
		SAFE_DELETE( m_pSocket );
	}
	//-------------------------------------------------------------------------------------------------------
	bool UDPCorrespondent::ConnectWithAddress( const Address& server )
	{
		m_SendID = 0;//ת��Ҫ��ֵ���ֵ
		m_ReciveID = 0;
		if( !m_pSocket->isConnect( ) )
		{
			m_pSocket->Connect( false );
			m_isReady = m_pSocket->isConnect();
		}
		m_Server = server;
		return m_isReady;
	}
	//-------------------------------------------------------------------------------------------------------
	void UDPCorrespondent::Close()
	{
		m_Server.Clear();
		m_isReady = false;
		m_pSocket->Close();
	}
	//-------------------------------------------------------------------------------------------------------
	bool UDPCorrespondent::Receive( NetPackage& pak )
	{
		uint RecevID(0);
		Address address;
		int recsize = m_pSocket->Receive( address, &m_vBuffer[0], m_vBuffer.size() );
		if( 0 == recsize )
		{
			return 0;
		}
		else // ��ѯͨѶ��
		{
			if( address != m_Server )//���ǶԷ�������
			{
				return 0;
			}
			else
			{
				int protoclID = m_vBuffer[0] << 24 | m_vBuffer[1] << 16 | m_vBuffer[2] << 8 | m_vBuffer[3];
				if( protoclID != PROTOCOL_ID ) //���Э����벻һ��
				{
					return 0;
				}
				else
				{
					RecevID = m_vBuffer[4] << 24 | m_vBuffer[5] << 16 | m_vBuffer[6] << 8 | m_vBuffer[7];
					if( RecevID < m_ReciveID //������ܵ����ǹ��ڵ����ݰ���
						&& RecevID != uint(-1) ) //uint(-1)�Ƿ�����������
					{
						return 0;
					}
				}
			}
		}
		m_ReciveID = RecevID;//�ı䵽���µ�ID
		recsize -= ( sizeof(ID) + sizeof(RecevID) ); //ȥ��ʶ����Ͱ���ID��ʵ������
		ASSERT( BUFFER_SIZE >= recsize );
		memcpy( pak.GetBuffer(),  &m_vBuffer[sizeof(ID) + sizeof(RecevID)], recsize);
		pak.Begin();
		return recsize > 0;
	}
	//-------------------------------------------------------------------------------------------------------
	bool UDPCorrespondent::Send( NetPackage& pak )
	{
		m_SendID++;
		const byte Pid[sizeof(uint)] =
		{
			(byte)(m_SendID >> 24),
			(byte)(m_SendID >> 16),
			(byte)(m_SendID >> 8),
			(byte)m_SendID
		};
		memcpy( &m_vBuffer[0], ID, sizeof(ID) );//����ʶ���
		memcpy( &m_vBuffer[sizeof(ID)], Pid, sizeof(Pid) );//������������
		memcpy( &m_vBuffer[sizeof(ID)+sizeof(Pid)], pak.GetBuffer(), pak.Size() );
		int SizeSended = ( pak.Size()+sizeof(ID)+sizeof(Pid) );
		return m_pSocket->Send( m_Server, &m_vBuffer[0], SizeSended );
	}



	//////////////////////
	//	UDP Server		//
	//////////////////////
	UDPServer::UDPServer( ushort port )
		:m_SendID( uint(-1) )
	{
		m_pSocket = NET_NEW SocketUDP( port );
		m_pSocket->Connect( true );
		m_vBuffer.resize( UDPBUFFER_SIZE );
	}
	//-------------------------------------------------------------------------------------------------------
	UDPServer::~UDPServer()
	{
		SAFE_DELETE( m_pSocket );
	}
	//-------------------------------------------------------------------------------------------------------
	bool UDPServer::RecevieFrom( Address& ad, NetPackage& pak)
	{
		int recsize = m_pSocket->Receive( ad, &m_vBuffer[0], m_vBuffer.size() );
		if( 0 == recsize )
		{
			return 0;
		}
		else // ��ѯͨѶ��
		{
			int protoclID = m_vBuffer[0] << 24 | m_vBuffer[1] << 16 | m_vBuffer[2] << 8 | m_vBuffer[3];
			if( protoclID != PROTOCOL_ID ) //���Э����벻һ��
			{
				return 0;
			}
		}
		recsize -= ( sizeof(ID) + sizeof(m_SendID) ); //ȥ��ʶ����Ͱ���ID��ʵ������
		ASSERT( BUFFER_SIZE >= recsize );
		memcpy( pak.GetBuffer(),  &m_vBuffer[sizeof(ID) + sizeof(m_SendID)], recsize);
		pak.Begin();
		return recsize > 0;
	}
	//-------------------------------------------------------------------------------------------------------
	bool UDPServer::SendTo( Address& ad, NetPackage& pak )
	{
		const byte Pid[sizeof(uint)] =
		{
			(byte)(m_SendID >> 24),
			(byte)(m_SendID >> 16),
			(byte)(m_SendID >> 8),
			(byte)m_SendID
		};
		memcpy( &m_vBuffer[0], ID, sizeof(ID) );//����ʶ���
		memcpy( &m_vBuffer[sizeof(ID)], Pid, sizeof(Pid) );//������������
		memcpy( &m_vBuffer[sizeof(ID)+sizeof(Pid)], pak.GetBuffer(), pak.Size() );
		int SizeSended = ( pak.Size()+sizeof(ID)+sizeof(Pid) );
		return m_pSocket->Send( ad, &m_vBuffer[0], SizeSended );
	}




	//////////////////////
	//		TCP����		//
	//////////////////////
	//-------------------------------------------------------------------------------------------------------
	TCPCorrespondent::TCPCorrespondent( )
		:m_isGeting(false),
		m_RecevedCount(0),
		m_isMulitSending(false)
	{
		m_vSendRecev.resize( TCPBUFFER_SIZE );//ÿ�η��͵����ݳ��Ȳ��������������
		m_pSocket = NET_NEW SocketTCP( );
	}
	//-------------------------------------------------------------------------------------------------------
	TCPCorrespondent::~TCPCorrespondent(void)
	{
		//this->DoSend();//�˳�ǰ���Ȱ����ݷ�����
		SAFE_DELETE( m_pSocket );
	}
	//-------------------------------------------------------------------------------------------------------
	bool TCPCorrespondent::ConnectWithAddress( const Address& server )
	{
		if( false == m_pSocket->isConnect() //��û�����ӣ�����Ŀ���ַ�����ڵĲ���ͬ��ת��
			|| server != m_Server )
		{
			if( m_pSocket->isConnect() )
			{
				m_pSocket->Close();
			}
			m_pSocket->Connect( server );
			m_isReady = m_pSocket->isConnect();
			if( m_isReady )
			{
				m_Server = server;//����Է���ַ
				m_pSocket->Send( ID, sizeof(ID) );//����ʶ���룬��������
			}
		}
		return m_isReady;
	}
	//-------------------------------------------------------------------------------------------------------
	void TCPCorrespondent::Close()
	{
		m_Server.Clear();
		m_isReady = false;
		m_pSocket->Close();
	}
	//-------------------------------------------------------------------------------------------------------
	bool TCPCorrespondent::Receive( NetPackage& pak ) //����ѶϢ
	{
		if( m_pSocket->Receive( &m_vSendRecev[0], TCPBUFFER_SIZE ) > 0 )
		{
			if( m_Packer.isRecevSignal( &m_vSendRecev[0] ) )//����ǶԷ����յ�����Ϣ
			{
				if( m_isMulitSending )
				{
					m_isMulitSending = false;
					this->DoSend();//�������Է���
					return false;
				}
			}
			else//������������Ϣ
			{
				if( m_isMulitSending ) //�����ǰ�Ƿ���״̬�ͽ��յ����Ǽ������͵������źţ���������ͣ���������Ϣ
				{
					m_isMulitSending = false;
					m_Packer.Reset();
				}
				switch( m_Packer.RecevData( &m_vSendRecev[0] ) )
				{
				case BohgeNet::Packer::PS_BADPACKAGE : break;//��Ч���ݰ������������տͻ�����Ϣ
				case BohgeNet::Packer::PS_ALLDONE : m_isGeting = true; break;//��������ˣ���ʼ������Ϣ
				case BohgeNet::Packer::PS_FRAGMENT :
					{
						int size = 0;//���û����ɣ��������ͱ��������ˣ�Ҫ������һ��
						m_Packer.GetRecevSignal( &m_vSendRecev[0], size );
						m_pSocket->Send( &m_vSendRecev[0], size );
						return false;
					}break;
				}
				return false;//����ִ�е�����
			}
		}
		if( m_isGeting )
		{
			if ( m_Packer.GetPackage(pak) )
			{
				return true;
			}
			else
			{
				m_isGeting = false;
				return false;
			}
		}
		return false;
	}
	//-------------------------------------------------------------------------------------------------------
	bool TCPCorrespondent::Send( NetPackage& pak )//����ѶϢ
	{
		m_Packer.CopyPackage( pak );
		return true;
	}
	//-------------------------------------------------------------------------------------------------------
	void TCPCorrespondent::OnFrameEnd()
	{
		if( false == this->DoSend() )
		{
			this->Close();//close socket and connect;
		}
	}
}