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



/////////////////////////////
//		��������		   //
/////////////////////////////
#pragma once


#define SERVER_VISION	1//�������汾��
//���������� (1024*8)
//֮����Ϊʲô����8K��Ҳ��һ�������ϵľ���ֵ
//���ȷʵ�ͻ��˷�����ÿ�����ݶ��Ƚ��٣���ô�����õ�СһЩ��ʡ�ڴ�
//548��һЩ����˿�MTU�����ޣ��������512��
#define BUFFER_SIZE		512//ʵ��������NetPackage�е����ݴ�С���������ڷ���ʱ��Ĵ�Сû�й�ϵ
#define MAIN_ENTER_PORT	56423//��Ҫ�Ľ���˿�
#define UDP_ENTRT_PORT	56420//��ͨ�õ�


typedef unsigned short	ushort;
typedef unsigned int	uint;
typedef unsigned long	ulong;
typedef unsigned char	byte;
typedef unsigned int	bitmask;
typedef int				sint;
typedef float			clamp;
typedef unsigned int	handle;



//TCP�����ݰ�ͷ��Ϊ�˽�����Ƿ�������һ��pass�з��Ͷ�����ݰ������⣬
//��������ʹ�����ַ�����ȫ�����ݴ����һ�����ݰ��У�
//Ȼ����ն��յ�֮�����ݲ�ֿ���
struct TCPHead
{
	enum
	{
		Max_Package_Count = 16,	//ÿ�����������������ݰ��������ܳ���14��
	};
	ushort			m_FragmentCount;//��֪�Է���Ҫ���ܶ��ٸ�Ƭ�Σ��������һ��������������
	ushort			m_PackageCount;//���buffer�����İ�������
	ushort			m_LengthEachPackage[Max_Package_Count];//ÿ�������ĳ���
	TCPHead()
	{
		this->Reset();
	}
	void Reset()
	{
		m_FragmentCount = 0;
		m_PackageCount = 0;
		for ( int i = 0 ; i < Max_Package_Count ; i ++ )
		{
			m_LengthEachPackage[i] = 0;
		}
	}
	ushort Size()
	{
		ushort r = 0;
		for ( int i = 0 ; i < m_PackageCount ; i ++ )
		{
			r += m_LengthEachPackage[i];
		}
		return r;
	}
};


#define TCPBUFFER_SIZE (BUFFER_SIZE + sizeof(TCPHead) )//ÿһ�η��͵İ���TCP��ͷ
#define UDPBUFFER_SIZE (BUFFER_SIZE + sizeof(PROTOCOL_ID) + sizeof(uint) )//���һ���ǰ�����ID����


#define PROTOCOL_ID			0x19851022//ͨѶ��ʶ����
const static byte	ID[sizeof(PROTOCOL_ID)] =
										{
											(byte)(PROTOCOL_ID >> 24),
											(byte)(PROTOCOL_ID >> 16),
											(byte)(PROTOCOL_ID >> 8),
											(byte)PROTOCOL_ID
										};


#define PLATFORM_WINDOWS	1
#define PLATFORM_IOS		2
#define PLATFORM_ANDROID	3


#ifdef _WIN32
#define PLATFORM PLATFORM_WINDOWS
#elif _WIN64
#define PLATFORM PLATFORM_WINDOWS
#elif IOS
#define PLATFORM PLATFORM_IOS
#elif ANDROID
#define PLATFORM PLATFORM_ANDROID
#else
#error unknown platform!
#endif



#if PLATFORM == PLATFORM_WINDOWS

#include <winsock2.h>
#include <windows.h>

#pragma comment( lib, "wsock32.lib" )

#elif PLATFORM == PLATFORM_IOS || PLATFORM == PLATFORM_ANDROID
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <fcntl.h>

#else

#error unknown platform!

#endif




//���
#ifdef APP
#include "Log.h"
#define NETDEBUGLOG(x,...) Tracker::PrintDebugInfo("",__VA_ARGS__)
#else
#ifdef ANDROID
#include <android/log.h>
#define TODO(x)
#define LOG_TAG    "Bohge"
#define NETDEBUGLOG(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__);
#endif
#ifdef IOS
#include <stdio.h>
#define NETDEBUGLOG(x,...)  printf(x,__VA_ARGS__)
#endif
#ifdef WIN32
#include <stdio.h>
#define NETDEBUGLOG(x,...)  printf(x,__VA_ARGS__)
#endif
#endif

#ifndef  ASSERT
#ifdef _DEBUG
#include <assert.h>
#define ASSERT(f) assert(f);
#else
#define ASSERT(f) ;
#endif
#endif


#ifdef _DEBUG
//#define NET_TRACK
#endif

#ifdef NET_TRACK
#define NET_NEW new(__FILE__, __LINE__) 
#else 
#define NET_NEW new 
#endif

#ifdef WIN32
#define NULL 0
#endif


#ifndef SAFE_DELETE
#define SAFE_DELETE(p) if(p){delete p; p = NULL;}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) if(p){delete[] p; p = NULL;}
#endif


namespace BohgeNet
{
	class NetPackage;
	class NetHost;
	class TCPCorrespondent;
	class UDPCorrespondent;
}


#ifndef BOHGE_FORCEINLINE
#ifdef WIN32
#define BOHGE_FORCEINLINE __forceinline
#else
#define BOHGE_FORCEINLINE __attribute__((always_inline))
#endif
#endif