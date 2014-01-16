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
//				��IO���ݽṹ�嶨��(����ÿһ���ص������Ĳ���)
//
//====================================================================================
#pragma once
#include <winsock2.h>
#include <MSWSock.h>
#include "BohgeNet.h"




//////////////////////////////////////////////////////////////////
// ����ɶ˿���Ͷ�ݵ�I/O����������
typedef enum _OPERATION_TYPE  
{  
	ACCEPT_POSTED,                     // ��־Ͷ�ݵ�Accept����
	SEND_POSTED,                       // ��־Ͷ�ݵ��Ƿ��Ͳ���
	RECV_POSTED,                       // ��־Ͷ�ݵ��ǽ��ղ���
	NULL_POSTED                        // ���ڳ�ʼ����������

}OPERATION_TYPE;


class IOContext
{
private:
	OVERLAPPED     m_Overlapped;                               // ÿһ���ص�����������ص��ṹ(���ÿһ��Socket��ÿһ����������Ҫ��һ��)              
	SOCKET         m_sockAccept;                               // ������������ʹ�õ�Socket
	WSABUF         m_wsaBuf;                                   // WSA���͵Ļ����������ڸ��ص�������������
public:
	char*          m_szBuffer;									// �����WSABUF�������ַ��Ļ�����
	OPERATION_TYPE m_OpType;                                   // ��ʶ�������������(��Ӧ�����ö��)
public:
	IOContext(void)
	{
		ZeroMemory(&m_Overlapped, sizeof(m_Overlapped));
		m_szBuffer = NET_NEW char[ TCPBUFFER_SIZE ];
		ZeroMemory( m_szBuffer,TCPBUFFER_SIZE );
		m_sockAccept = INVALID_SOCKET;
		m_wsaBuf.buf = m_szBuffer;
		m_wsaBuf.len = TCPBUFFER_SIZE;
		m_OpType     = NULL_POSTED;
	}
	~IOContext(void)
	{
		SAFE_DELETE_ARRAY( m_szBuffer );
	}
	inline void ResetBuffer()
	{
		ZeroMemory( m_szBuffer,TCPBUFFER_SIZE );
	}
	inline OVERLAPPED& OverLopped()
	{
		return m_Overlapped;
	}
	inline SOCKET& Socket()
	{
		return m_sockAccept;
	}
	inline WSABUF& Buffer()
	{
		return m_wsaBuf;
	}
};