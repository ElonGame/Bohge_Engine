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




/*
==========================================================================

Purpose:

* �����CIOCPModel�Ǳ�����ĺ����࣬����˵��WinSock�������˱��ģ���е�
��ɶ˿�(IOCP)��ʹ�÷�������ʹ��MFC�Ի�����������������ʵ���˻�����
����������ͨ�ŵĹ��ܡ�

* ���е�PER_IO_DATA�ṹ���Ƿ�װ������ÿһ���ص������Ĳ���
PER_HANDLE_DATA �Ƿ�װ������ÿһ��Socket�Ĳ�����Ҳ��������ÿһ����ɶ˿ڵĲ���

* ��ϸ���ĵ�˵����ο� http://blog.csdn.net/PiggyXP

Notes:

* ���彲���˷������˽�����ɶ˿ڡ������������̡߳�Ͷ��Recv����Ͷ��Accept����ķ�����
���еĿͻ��������Socket����Ҫ�󶨵�IOCP�ϣ����дӿͻ��˷��������ݣ�����ʵʱ��ʾ��
��������ȥ��

Author:

* Bohge

Date:

* 20012/10/22

==========================================================================
*/

#pragma once

// winsock 2 ��ͷ�ļ��Ϳ�
#include <winsock2.h>
#include <MSWSock.h>
#include <map>
#include <list>
#include <string>

#include "AutoIndex.h"
#include "SmartPtr.hpp"
#include "Releaser.h"

#pragma comment(lib,"ws2_32.lib")




//====================================================================================
//
//				CIOCPModel�ඨ��
//
//====================================================================================

// �������̵߳��̲߳���
class CIOCPModel;
typedef struct _tagThreadParams_WORKER
{
	CIOCPModel* pIOCPModel;                                   // ��ָ�룬���ڵ������еĺ���
	int         nThreadNo;                                    // �̱߳��

} THREADPARAMS_WORKER,*PTHREADPARAM_WORKER; 


class SQLDataBase;
class IOContext;
class Listerner;
class ISocketContext;
class ClientContext;
class CIOCPModel// CIOCPModel��
{
public:
	CIOCPModel(void);
	~CIOCPModel(void);

public:

	// ����������
	bool Start();

	//	ֹͣ������
	void Stop();

	void OnTimer();

	// ����Socket��
	bool LoadSocketLib();

	// ж��Socket�⣬��������
	void UnloadSocketLib() { WSACleanup(); }

	// ��ñ�����IP��ַ
	std::string GetLocalIP();

private:

	// ��ʼ��IOCP
	bool _InitializeIOCP();

	// ����ͷ���Դ
	void _DeInitialize();

	// ���ͻ��˵������Ϣ�洢��������
	void _AddToContextList( SmartPtr<ISocketContext> *pSocketContextPtr );

	//�ͻ����Ƿ���buff��
	bool _isInContextList( SmartPtr<ISocketContext> *pSocketContextPtr );

	// ���ͻ��˵���Ϣ���������Ƴ�
	void _RemoveContext( SmartPtr<ISocketContext> *pSocketContextPtr );

	// ��տͻ�����Ϣ
	void _ClearContextList();

	// ������ɶ˿��ϵĴ���
	bool HandleError( SmartPtr<ISocketContext> *pSocketContextPtr,const DWORD& dwErr );

	// �̺߳�����ΪIOCP�������Ĺ������߳�
	static DWORD WINAPI _WorkerThread(LPVOID lpParam);

	//�����������Ͽ�����
	void _DisconnectDeadClient();

	// ��ñ����Ĵ���������
	int _GetNoOfProcessors();

	// �жϿͻ���Socket�Ƿ��Ѿ��Ͽ�
	bool _IsSocketAlive(SOCKET s);

	void inline MutexClinetSmartPtrMap()
	{
		EnterCriticalSection( &m_csContextList );
	}

	void inline UnmutexClinetSmartPtrMap()
	{
		LeaveCriticalSection( &m_csContextList );
	}

	inline Releaser& GetReleaser()
	{
		return m_Releaser;
	}

private:
	typedef std::list<int>	FreeIDList;
	//typedef std::map<int,ISocketContext*> ClinetMap;
	typedef std::map<int,SmartPtr<ISocketContext>*> ClinetSmartPtrMap;
public:
	bool						 m_isBegin;

	HANDLE                       m_hShutdownEvent;              // ����֪ͨ�߳�ϵͳ�˳����¼���Ϊ���ܹ����õ��˳��߳�

	HANDLE                       m_hIOCompletionPort;           // ��ɶ˿ڵľ��

	HANDLE*                      m_phWorkerThreads;             // �������̵߳ľ��ָ��

	int		                     m_nThreads;                    // ���ɵ��߳�����

	std::string                  m_strIP;                       // �������˵�IP��ַ

	unsigned short				 m_nPort;						// �������˵ļ����˿�

	CRITICAL_SECTION             m_csContextList;               // ����Worker�߳�ͬ���Ļ�����
     
	ClinetSmartPtrMap			 m_ContextPtrMap;				// �ͻ���Socket��Context��Ϣ     

	Releaser					 m_Releaser;					//ȫ���ĶϿ����ӵĿͻ��˲�����ɾ�����ŵ���������У���ɾ��

	Listerner*					 m_pListenContext;              // ���ڼ�����Socket��Context��Ϣ

	SQLDataBase*				 m_pSql;

	BohgeNet::NetPackage		 m_Package;
};

