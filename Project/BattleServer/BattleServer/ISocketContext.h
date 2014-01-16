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
#include "NetBase.h"
#include "SmartPtr.hpp"
#include "IOCPModel.h"
#include "Base.h"


class IOContext;
class SQLDataBase;
class ClientContext;
class ISocketContext
{
public:
	enum Type
	{
		ST_LISTRNER,
		ST_CLIENT,
	};
private:
	bool						m_Actived;//��һ��ʱ����ڣ��Ƿ������������ڷ����������Ͽ�����
	Type						m_eType;
	int							m_MapIndex;//������map���Լ���index�������Լ��ĵ�ַ��intֵ
	SmartPtr<ISocketContext>*	m_pSelfPtr;//���Լ�������ָ��ͬ�����Լ�����
protected:
	bool						m_isConnect;//�Ƿ�������״̬
	SOCKADDR_IN					m_BindAddr;// �ͻ��˵ĵ�ַ���߱��ذ󶨵ĵ�ַ
	SOCKET						m_Socket;// ÿһ���ͻ������ӵ�Socket
public:
	ISocketContext( Type t )
		:m_isConnect(true),
		m_eType( t ),
		m_Actived(false)
	{
		m_Socket = INVALID_SOCKET;
		memset(&m_BindAddr, 0, sizeof(m_BindAddr));
		m_MapIndex = (int)this;
		m_pSelfPtr = new SmartPtr<ISocketContext>( this );
	}
	virtual ~ISocketContext()
	{
		if( m_Socket!=INVALID_SOCKET 
			&& m_isConnect )
		{
			closesocket( m_Socket );
			m_Socket = INVALID_SOCKET;
		}
		m_pSelfPtr = NULL;//SAFE_DELETE( m_pSelfPtr );//���ﲻ���ٴ�delete�����Ǹ����ӵ��߼�˳����̣������͸���CIOCPModel::_RemoveContext
	}
	virtual SmartPtr<ISocketContext>* DoWork( IOContext* io, BohgeNet::NetPackage& pak, SQLDataBase& sql, bool& isAdd ) = 0;//����ֵ��ʾ�����Listen�ڴ���᷵��һ���¿ͻ��ˣ���Ҫ����������list�У�clinet��Զ����NULL
public:
	inline SOCKET Socket()
	{
		return m_Socket;
	}
	inline ulong BindIP()
	{
		return m_BindAddr.sin_addr.S_un.S_addr;
	}
	inline char* BindIPString()
	{
		return inet_ntoa( m_BindAddr.sin_addr );
	}
	inline ushort BindPort()
	{
		return m_BindAddr.sin_port;
	}
	//-------------------------------------------------------------------------------------------------------
	inline int GetIndex() const
	{
		return m_MapIndex;
	}
	inline SmartPtr<ISocketContext>* SelfSmartPtr()
	{
		return m_pSelfPtr;
	}
	inline bool isConnect()
	{
		return m_isConnect;
	}
	inline void Disconnect()
	{
		m_isConnect = false;
		if( m_Socket!=INVALID_SOCKET )
		{
			closesocket( m_Socket );
			m_Socket = INVALID_SOCKET;
		}
	}
	inline Type GetType()
	{
		return m_eType;
	}
	inline void Actived()
	{
		m_Actived = true;
	}
	inline bool isActived()
	{
		bool b = m_Actived;
		m_Actived = false;
		return b;
	}
};