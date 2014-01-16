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
#include "ClientContext.h"
#include "IOCPModel.h"
#include "IOContext.h"
#include "NetBase.h"
#include <map>


class Listerner  : public ISocketContext//�������ӽ���Ŀͻ���
{
private:
	std::map<int, IOContext*>		m_IoContextMap;// �ͻ����������������������,Ҳ����˵����ÿһ���ͻ���Socket���ǿ���������ͬʱͶ�ݶ��IO�����
	HANDLE							m_hIOCompletionPortCopy;//������������ɶ˿ھ���Ŀ���
	LPFN_ACCEPTEX					m_lpfnAcceptEx;// AcceptEx �� GetAcceptExSockaddrs �ĺ���ָ�룬���ڵ�����������չ����
	LPFN_GETACCEPTEXSOCKADDRS		m_lpfnGetAcceptExSockAddrs; 
public:
	// ��ʼ��
	Listerner()
		:ISocketContext(ISocketContext::ST_LISTRNER),
		m_lpfnAcceptEx(NULL),
		m_lpfnGetAcceptExSockAddrs(NULL)
	{
	}
	// �ͷ���Դ
	~Listerner()
	{
		// �ͷŵ����е�IO����������
		for( std::map<int, IOContext*>::iterator it = m_IoContextMap.begin();
			it != m_IoContextMap.end();
			it ++ )
		{
			SAFE_DELETE( it->second );
		}
		m_IoContextMap.clear();
	}
	// ��ȡһ���µ�IoContext
	IOContext* GetNewIoContext()
	{
		IOContext* p = new IOContext;
		m_IoContextMap.insert( std::make_pair( (int)p, p ) );
		return p;
	}
	// ���������Ƴ�һ��ָ����IoContext
	void RemoveContext( IOContext* pContext )
	{
		std::map<int, IOContext*>::iterator it = m_IoContextMap.find( (int)pContext );
		if( it != m_IoContextMap.end() )
		{
			SAFE_DELETE_ARRAY( it->second );
			m_IoContextMap.erase(it);
		}
	}
	/////////////////////////////////////////////////////////////////
	// ��ʼ��Socket
	bool InitializeListenSocket( HANDLE& hIOCompletionPort, std::string& ip, ushort potr )
	{
		m_hIOCompletionPortCopy = hIOCompletionPort;
		// AcceptEx �� GetAcceptExSockaddrs ��GUID�����ڵ�������ָ��
		GUID GuidAcceptEx = WSAID_ACCEPTEX;  
		GUID GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS; 

		// ��������ַ��Ϣ�����ڰ�Socket
		struct sockaddr_in ServerAddress;

		// ��Ҫʹ���ص�IO�������ʹ��WSASocket������Socket���ſ���֧���ص�IO����
		m_Socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (INVALID_SOCKET == m_Socket ) 
		{
			ShowMessage(_T("Initialize Socket fail, error code: %d.\n"), WSAGetLastError());
			return false;
		}
		else
		{
			TRACE("WSASocket() compate.\n");
		}

		//����tcpΪ���ӳٷ��ͣ�����Ϣ��������
		const char chOpt=1;   
		int   nErr=setsockopt(  m_Socket,   IPPROTO_TCP,   TCP_NODELAY,   &chOpt,   sizeof(char));   
		if(nErr==-1)   
		{   
			ShowMessage(_T("cann't begin no delay TCP,error code: %d.\n"), WSAGetLastError());
			TRACE(_T("setsockopt()   error\n"),WSAGetLastError());   
			return false;   
		} 

		// ��Listen Socket������ɶ˿���
		//if( NULL== CreateIoCompletionPort( (HANDLE)m_Socket, m_hIOCompletionPortCopy,(DWORD)this, 0))  
		if( NULL== CreateIoCompletionPort( (HANDLE)m_Socket, m_hIOCompletionPortCopy,(DWORD)this->SelfSmartPtr(), 0))  
		{  
			ShowMessage(_T("bind Listen Socket to complation port! error code: %d/n"), WSAGetLastError());  
			RELEASE_SOCKET( m_Socket );
			return false;
		}
		else
		{
			TRACE("Listen Socket bind complation port success complate.\n");
		}

		// ����ַ��Ϣ
		ZeroMemory((char *)&ServerAddress, sizeof(ServerAddress));
		ServerAddress.sin_family = AF_INET;
		// ������԰��κο��õ�IP��ַ�����߰�һ��ָ����IP��ַ 
		//ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);                      
		ServerAddress.sin_addr.s_addr = inet_addr(ip.c_str());         
		ServerAddress.sin_port = htons(potr);                          

		// �󶨵�ַ�Ͷ˿�
		if (SOCKET_ERROR == bind(m_Socket, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress))) 
		{
			ShowMessage("bind() function fail.\n");
			return false;
		}
		else
		{
			TRACE("bind() Success.\n");
		}

		// ��ʼ���м���
		if (SOCKET_ERROR == listen(m_Socket,SOMAXCONN))
		{
			ShowMessage("Listen() funcion error.\n");
			return false;
		}
		else
		{
			TRACE("Listen() success.\n");
		}

		// ʹ��AcceptEx��������Ϊ���������WinSock2�淶֮���΢�������ṩ����չ����
		// ������Ҫ�����ȡһ�º�����ָ�룬
		// ��ȡAcceptEx����ָ��
		DWORD dwBytes = 0;  
		if(SOCKET_ERROR == WSAIoctl(
			m_Socket, 
			SIO_GET_EXTENSION_FUNCTION_POINTER, 
			&GuidAcceptEx, 
			sizeof(GuidAcceptEx), 
			&m_lpfnAcceptEx, 
			sizeof(m_lpfnAcceptEx), 
			&dwBytes, 
			NULL, 
			NULL))  
		{  
			ShowMessage("WSAIoctl cann't get AcceptEx founction ptr. error code: %d\n", WSAGetLastError()); 
			return false;  
		}  

		// ��ȡGetAcceptExSockAddrs����ָ�룬Ҳ��ͬ��
		if(SOCKET_ERROR == WSAIoctl(
			m_Socket, 
			SIO_GET_EXTENSION_FUNCTION_POINTER, 
			&GuidGetAcceptExSockAddrs,
			sizeof(GuidGetAcceptExSockAddrs), 
			&m_lpfnGetAcceptExSockAddrs, 
			sizeof(m_lpfnGetAcceptExSockAddrs),   
			&dwBytes, 
			NULL, 
			NULL))  
		{  
			ShowMessage("WSAIoctl cann't get GuidGetAcceptExSockAddrs founction ptr. error code: %d\n", WSAGetLastError());  
			return false; 
		}  
		// ΪAcceptEx ׼��������Ȼ��Ͷ��AcceptEx I/O����
		for( int i=0;i<MAX_POST_ACCEPT;i++ )
		{
			// �½�һ��IO_CONTEXT
			IOContext* pAcceptIoContext = this->GetNewIoContext();

			if( false==this->PostAccept( pAcceptIoContext ) )
			{
				this->RemoveContext(pAcceptIoContext);
				return false;
			}
		}
		ShowMessage( _T("post %d AcceptEx complate"),MAX_POST_ACCEPT );
		return true;
	}
private:
	/////////////////////////////////////////////////////
	// �����(Socket)�󶨵���ɶ˿���
	inline bool AssociateWithIOCP( ClientContext *pContext )
	{
		// �����ںͿͻ���ͨ�ŵ�SOCKET�󶨵���ɶ˿���
		//HANDLE hTemp = CreateIoCompletionPort((HANDLE)pContext->m_Socket, m_hIOCompletionPortCopy, (DWORD)pContext, 0);
		HANDLE hTemp = CreateIoCompletionPort((HANDLE)pContext->m_Socket, m_hIOCompletionPortCopy, (DWORD)pContext->SelfSmartPtr(), 0);
		if (NULL == hTemp)
		{
			ShowMessage(_T("run CreateIoCompletionPort() fail . error code: %d"),GetLastError());
			return false;
		}
		return true;
	}
	//////////////////////////////////////////////////////////////////
	// Ͷ��Accept����
	inline bool PostAccept( IOContext* io )
	{
		ASSERT( INVALID_SOCKET!=m_Socket );

		// ׼������
		DWORD dwBytes = 0;  
		io->m_OpType = ACCEPT_POSTED;  
		WSABUF *p_wbuf   = &io->Buffer();
		OVERLAPPED *p_ol = &io->OverLopped();

		// Ϊ�Ժ�������Ŀͻ�����׼����Socket( ������봫ͳaccept�������� ) 
		io->Socket()  = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);  
		if( INVALID_SOCKET==io->Socket() )  
		{  
			ShowMessage(_T("create Accept 's Socket fial! error code: %d"), WSAGetLastError()); 
			return false;  
		} 

		const char chOpt=1;   
		int   nErr=setsockopt(  io->Socket(),   IPPROTO_TCP,   TCP_NODELAY,   &chOpt,   sizeof(char));   
		if(nErr==-1)   
		{   
			TRACE(_T("setsockopt()   error\n"),WSAGetLastError());   
			return false;   
		} 

		// Ͷ��AcceptEx
		if(FALSE == m_lpfnAcceptEx( this->m_Socket, io->Socket(), p_wbuf->buf,
			p_wbuf->len - ((sizeof(SOCKADDR_IN)+16)*2),   //�����һ����������Ϊ0��ͻ���connect��ʱ������accpet
			sizeof(SOCKADDR_IN)+16, sizeof(SOCKADDR_IN)+16, &dwBytes, p_ol))  
		{  
			if(WSA_IO_PENDING != WSAGetLastError())  
			{  
				ShowMessage(_T("post AcceptEx fail, error code: %d"), WSAGetLastError());  
				return false;  
			}  
		} 
		return true;
	}
	inline SmartPtr<ISocketContext>* DoAccpet( IOContext* io )
	{
		SOCKADDR_IN* ClientAddr = NULL;
		SOCKADDR_IN* LocalAddr = NULL;  
		int remoteLen = sizeof(SOCKADDR_IN), localLen = sizeof(SOCKADDR_IN);  

		///////////////////////////////////////////////////////////////////////////
		// 1. ����ȡ������ͻ��˵ĵ�ַ��Ϣ
		// ��� m_lpfnGetAcceptExSockAddrs �����˰�~~~~~~
		// ��������ȡ�ÿͻ��˺ͱ��ض˵ĵ�ַ��Ϣ������˳��ȡ���ͻ��˷����ĵ�һ�����ݣ���ǿ����...
		m_lpfnGetAcceptExSockAddrs(io->Buffer().buf, io->Buffer().len - ((sizeof(SOCKADDR_IN)+16)*2),  
			sizeof(SOCKADDR_IN)+16, sizeof(SOCKADDR_IN)+16, (LPSOCKADDR*)&LocalAddr, &localLen, (LPSOCKADDR*)&ClientAddr, &remoteLen);  

		int protoclID = ((byte)io->Buffer().buf[0]) << 24 | ((byte)io->Buffer().buf[1]) << 16 | ((byte)io->Buffer().buf[2]) << 8 | ((byte)io->Buffer().buf[3]);
		DEBUGLOG( _T("client %s:%d connect. ID message: %d."),inet_ntoa(ClientAddr->sin_addr), ntohs(ClientAddr->sin_port), protoclID );
		if( protoclID != PROTOCOL_ID )
		{
			ShowMessage( _T(" ID message: %d is not match %d"), protoclID, PROTOCOL_ID );
			return NULL;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// 2. ������Ҫע�⣬���ﴫ��������ListenSocket�ϵ�Context�����Context���ǻ���Ҫ���ڼ�����һ������
		// �����һ���Ҫ��ListenSocket�ϵ�Context���Ƴ���һ��Ϊ�������Socket�½�һ��SocketContext
		ClientContext* pNewSocketContext = NET_NEW ClientContext;
		pNewSocketContext->m_Socket           = io->Socket();
		memcpy(&(pNewSocketContext->m_BindAddr), ClientAddr, sizeof(SOCKADDR_IN));

		// ����������ϣ������Socket����ɶ˿ڰ�(��Ҳ��һ���ؼ�����)
		if( false==this->AssociateWithIOCP( pNewSocketContext ) )
		{
			ShowMessage( _T("can not bind complation port") );
			SAFE_DELETE( pNewSocketContext );
			return NULL;
		} 

		///////////////////////////////////////////////////////////////////////////////////////////////////
		// 3. �������������µ�IoContext�����������Socket��Ͷ�ݵ�һ��Recv��������
		pNewSocketContext->MakeIOContext();
		// �����֮�󣬾Ϳ��Կ�ʼ�����Socket��Ͷ�����������
		if( false==pNewSocketContext->PostRecv( ) )
		{
			ShowMessage( _T("can not recev message on this port") );
			SAFE_DELETE( pNewSocketContext );
			return NULL;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////
		// 4. ʹ�����֮�󣬰�Listen Socket���Ǹ�IoContext���ã�Ȼ��׼��Ͷ���µ�AcceptEx
		io->ResetBuffer();
		this->PostAccept( io );		

		/////////////////////////////////////////////////////////////////////////////////////////////////
		// 5. ����ɹ�����ô�Ͱ������Ч�Ŀͻ�����Ϣ�����뵽ContextList��ȥ(��Ҫͳһ���������ͷ���Դ)
		pNewSocketContext->Actived();
		ClientConnect();
		return pNewSocketContext->SelfSmartPtr();
	}
public:
	virtual SmartPtr<ISocketContext>* DoWork( IOContext* io, BohgeNet::NetPackage& pak, SQLDataBase& sql, bool& isAdd )
	{
		if( io->m_OpType ==  ACCEPT_POSTED )
		{
			isAdd = true;
			return this->DoAccpet(io);
		}
		else
		{
			ASSERT(false);
		}
		return NULL;
	}
};