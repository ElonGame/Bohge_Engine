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



//====================================================================================
//
//				��������ݽṹ�嶨��(����ÿһ����ɶ˿ڣ�Ҳ����ÿһ��Socket�Ĳ���)
//
//====================================================================================
#pragma once
#include "ISocketContext.h"
#include "IOContext.h"
#include "NetBase.h"
#include "Packer.h"
#include <map>



class ClientContext;
extern void OnGetMessage( SmartPtr<ClientContext>& client, BohgeNet::NetPackage& pak, SQLDataBase& sql );//�ⲿ��������ͨѶ���֮��ģ�������յ�����Ϣ�ĺ���

class ClientContext : public ISocketContext
{ 
	friend class SmartPtr<ClientContext>;
	friend class CIOCPModel;
	friend class Listerner;
	// �ͻ����������������������,Ҳ����˵����ÿһ���ͻ���Socket���ǿ���������ͬʱͶ�ݶ��IO�����
private:
	bool					m_isCopying;//���͵���Ϣ�Ƿ��ڿ�����
	bool					m_isActive;//��work�߳��б����õģ�ֹͣ��Ӧ�ͻ��˵�DoSend��ӦΪ�����Լ�����Ӧ
	bool					m_isSending;//������
	IOContext				m_IoContext;
	BohgeNet::Packer		m_Packer;//��������շ���ȫ�ģ���ͬ�߳̿���ͬʱ�����շ�
	uint					m_PlayerIndex;//��������ݿ��е�����
	SmartPtr<ClientContext>	m_CopyBasePtr;//����ǿ��������Ļ�����Ǹ�����ָ�룬����Ϊ�˿��ٴ��ݸ�OnGetMessage
private:
	// ��ʼ��
	ClientContext()
		:ISocketContext(ISocketContext::ST_CLIENT),
		m_isSending(false),
		m_isActive(false),
		m_PlayerIndex(-1),
		m_isCopying(false)
	{ 
		m_CopyBasePtr.Copy( check_pointer_cast<ClientContext>( (*this->SelfSmartPtr()) ) );//�����ǿ��������������ü���
	}
	// �ͷ���Դ
	~ClientContext()
	{
	}
	//-------------------------------------------------------------------------------------------------------
	inline bool Receive( BohgeNet::NetPackage& pak )
	{
		return m_Packer.GetPackage(pak); //��������ݾͷ���true�����Ҳ����ͺͽ�����Ϣ
	}
public:
	//-------------------------------------------------------------------------------------------------------
	inline uint& PlayerIndex()
	{
		return m_PlayerIndex;
	}
	inline void Send( BohgeNet::NetPackage& pak )
	{
		while(true)
		{
			if( m_isCopying == false )//��ֹ���߳�ͬʱд��
			{
				break;
			}
			//Sleep( 1 );
		}
		m_isCopying = true;
		m_Packer.CopyPackage( pak );
		m_isCopying = false;
	}
	//-------------------------------------------------------------------------------------------------------
	inline void SendImmediately() //ʵ�ʵķ��ͳ�ȥ,������������ظ����ã�һ������ֻ�ܵ���һ��
	{
		if( ! m_isActive )//�����������ٵ���Ŀǰ�����⣬���Ÿİɣ����¾���Ŀǰ��client�ǻ״̬���ٵ��������������
		{
			this->DoSend();
		}
	}
	//-------------------------------------------------------------------------------------------------------
	inline bool isClient ( const SmartPtr<ClientContext>& client ) const
	{
		return (this->GetIndex() == client->GetIndex()) && (this->m_PlayerIndex == client->m_PlayerIndex ) ;
	}
private:
	void MakeIOContext()
	{
		m_IoContext.Socket() = m_Socket;
	}
	//-------------------------------------------------------------------------------------------------------
	// Ͷ�ݽ�����������
	bool PostRecv( )
	{
		// ��ʼ������
		DWORD dwFlags = 0;
		DWORD dwBytes = 0;
		m_IoContext.m_OpType = RECV_POSTED;
		m_IoContext.Buffer().len = TCPBUFFER_SIZE;

		// ��ʼ����ɺ󣬣�Ͷ��WSARecv����
		int nBytesRecv = WSARecv(	m_IoContext.Socket(),
									&m_IoContext.Buffer(),
									1, &dwBytes, &dwFlags,
									&m_IoContext.OverLopped(),
									NULL );

		// �������ֵ���󣬲��Ҵ���Ĵ��벢����Pending�Ļ����Ǿ�˵������ص�����ʧ����
		if ((SOCKET_ERROR == nBytesRecv) && (WSA_IO_PENDING != WSAGetLastError()))
		{
			ShowMessage("post WSARecv fail ! error code: %d!\n", WSAGetLastError());
			return false;
		}
		return true;
	}
	//-------------------------------------------------------------------------------------------------------
	bool PostSend( )
	{
		// ��ʼ������
		DWORD dwFlags = 0;
		DWORD dwBytes = 0;
		m_IoContext.m_OpType = SEND_POSTED;

		DEBUGLOG("Send Message to %s : %d", this->BindIPString(), ntohs(this->BindPort()));
		// ��ʼ����ɺ󣬣�Ͷ��WSASend����
		int nBytesSend = WSASend(	m_IoContext.Socket(),
									&m_IoContext.Buffer(),
									1, &dwBytes, dwFlags,
									&m_IoContext.OverLopped(), 
									NULL );
		//�������ֵ���󣬲��Ҵ���Ĵ��벢����Pending�Ļ����Ǿ�˵������ص�����ʧ����
		if ((SOCKET_ERROR == nBytesSend) && (WSA_IO_PENDING != WSAGetLastError()))
		{
			ShowMessage("Post SASend fail, error code: %d!\n", WSAGetLastError());
			return false;
		}
		return true;
	}
	//-------------------------------------------------------------------------------------------------------
	// ���н��յ����ݵ����ʱ�򣬽��д���
	inline bool DoRecv( BohgeNet::NetPackage& pak, SQLDataBase& sql )
	{
		if( BohgeNet::Packer::isRecevSignal( (byte*)m_IoContext.Buffer().buf ) )//����ǶԷ����յ�����Ϣ
		{
			m_isSending = false;
			return DoSend();//�������Է���
		}
		else//������������Ϣ
		{
			if( m_isSending ) //�����ǰ�Ƿ���״̬�ͽ��յ����Ǽ������͵������źţ���������ͣ���������Ϣ
			{
				m_isSending = false;
				m_isActive = false;
				m_Packer.Reset();
			}
			switch( m_Packer.RecevData( (byte*)m_IoContext.Buffer().buf ) )
			{
			case BohgeNet::Packer::PS_BADPACKAGE :
				{
					DEBUGLOG("%s:%d recev a incorrcet package", this->BindIPString(), this->BindPort() );
					return PostRecv();//��Ч���ݰ������������տͻ�����Ϣ
				}break;
			case BohgeNet::Packer::PS_FRAGMENT :
				{
					m_isActive = true;
					BohgeNet::Packer::GetRecevSignal( (byte*)m_IoContext.Buffer().buf, (int&)m_IoContext.Buffer().len );//�õ����յ��ź�
					return PostSend();//������źŷ��ͳ�ȥ
				}break;
			case BohgeNet::Packer::PS_ALLDONE :
				{
					DEBUGLOG("%s:% dRecev a package", this->BindIPString(), this->BindPort() );
					m_isActive = true;
					while(1)
					{
						if ( m_Packer.GetPackage( pak ) )
						{
							OnGetMessage( m_CopyBasePtr, pak, sql );
						}
						else
						{
							break;
						}
					}
					return DoSend();//����ֱ�Ӽ���Ƿ�����Ҫ���͵�����
				}break;
			}
			return PostRecv();//����ִ�е�����
		}
	}
	//-------------------------------------------------------------------------------------------------------
	bool DoSend( )	//����Ҫ�������ݵ�ʱ�򣬽��д���
	{
		if( ! m_isSending  //�����ǰ�ǷǷ���״̬
			&& m_isConnect )//����Ҫ������״̬
		{
			if( m_Packer.SendData( (byte*)m_IoContext.Buffer().buf, (int&)m_IoContext.Buffer().len ) )//������Ƕಥ������Ͱ�imulitsend״̬������
			{
				if( m_Packer.isMulitSend() )//ֻ���Ƕಥ����Ҫ����sending���ȴ��Է�����ȷ����Ϣ
				{
					m_isSending = true;
				}
				// Ȼ��ʼͶ����һ��WSARecv����
				return this->PostSend( );
			}
		}
		m_isActive = false; //���������
		return PostRecv( );//�����л�������״̬���ȴ���ִ
	}
	//-------------------------------------------------------------------------------------------------------
	virtual SmartPtr<ISocketContext>* DoWork( IOContext* io,  BohgeNet::NetPackage& pak, SQLDataBase& sql, bool& isAdd )
	{
		//ASSERT( &m_IoContext == io );
		bool Success = false;
		switch( io->m_OpType )  
		{ 
			// RECV
		case RECV_POSTED:
			{
				// Ϊ�����Ӵ���ɶ��ԣ�������ר�ŵ�_DoRecv�������д����������
				Success = DoRecv( pak, sql );
			}
			break;
			// SEND
		case SEND_POSTED:
			{
				//����Ϣ���ͺ���Ҫ�û�Ϊ׼�����ܵ�״̬
				Success = DoSend( );
			}
			break;
		default:
			// ��Ӧ��ִ�е�����
			ASSERT(false);
			break;
		}
		if( !Success ) //���ִ��ʧ���ˣ��ͷ����Լ���Ȼ��ɾ���Լ�
		{
			isAdd = false;
			return this->SelfSmartPtr();
		}
		return NULL;
	}
};