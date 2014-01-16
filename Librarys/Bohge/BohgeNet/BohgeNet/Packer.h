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



//--------------------------------------------------------------------------------
//Ŀǰ���ֻ���m_isBufferChange�ᵼ�������ڷ������˵Ķѻ�
//�����������Ӧ�������������Ϣ�ѻ��������ȴ�����
//����Ŀǰ���ֻ��Ʊ���һ���ԵĴ����������ݰ�
//��������շ���ȫ�ģ���ͬ�߳̿���ͬʱ�����շ�
//--------------------------------------------------------------------------------

#pragma once
#include "NetMessage.h"
#include "NetBase.h"
#include <list>


namespace BohgeNet
{

	class Packer	//���ڽ���Ҫ���͵İ���
	{
	public:
		enum PackStatus
		{
			PS_BADPACKAGE,//������
			PS_FRAGMENT,//ֻ��Ƭ��
			PS_ALLDONE,//�����
		};
	private:
		enum
		{
			Recev_Alloc_Size = 5,//һ��׼��5���������ݵİ���
		};
	public:
		typedef std::vector<byte>	Buffer;
	private:
		struct RecevBuffer
		{
			uint		m_size;
			Buffer		m_Buffer;
			RecevBuffer()
				:m_size(0)
			{
				m_Buffer.resize( BUFFER_SIZE );
			}
		};
	private:
		typedef std::vector<RecevBuffer>	RecevBufferVector;
	private:

		//���������������
		TCPHead								m_Head;//tcp�İ�ͷ
		ushort								m_PackageCount;
		std::vector<ushort>					m_EachSize;//ÿһ�������ĳ���
		Buffer								m_vInternalBuffer;//����Ҫ���͵����������ַ���
		ushort								m_CopySize;//��ǰ��������ĳ���

		//���͹�������µ�������
		bool								m_isBufferChange;

		//�������
		int									m_FragamentCount;//Ƭ����������
		int									m_NeedSend;//��Ҫ���͵�����
		int									m_SendedSize;//�ѽ������˵Ĵ�С
		int									m_SendedPackage;//���͵İ�������

		//������ص�
		uint								m_RecevFragment;//���ܵ���Ƭ������
		uint								m_ReadIndex;
		uint								m_WriteIndex;//��ǰ�յ�������
		RecevBufferVector					m_RecevedVector;//���յ���
	public:
		Packer()
			:m_CopySize(0),
			m_PackageCount(0),
			m_WriteIndex(0),
			m_ReadIndex(0),
			m_SendedSize(0),
			m_SendedPackage(0),
			m_NeedSend(0),
			m_RecevFragment(0),
			m_FragamentCount(0),
			m_isBufferChange(false)
		{
			m_EachSize.resize( TCPHead::Max_Package_Count );
			m_vInternalBuffer.resize( BUFFER_SIZE );
			m_RecevedVector.resize( Recev_Alloc_Size );
		}
	public:
		void Reset()
		{
			m_isBufferChange = false;
			m_FragamentCount = 0;
			m_NeedSend = 0;//��Ҫ�����
			m_CopySize = 0;//��Ҫ�����
			m_PackageCount = 0;//��Ҫ�����
			m_SendedPackage = 0;//��Ҫ�����
			m_SendedSize = 0;//��Ҫ�����
			memset( &m_EachSize[0], 0, m_EachSize.size() );
			memset( &m_vInternalBuffer[0], 0, m_vInternalBuffer.size() );
		}
		BOHGE_FORCEINLINE bool isMulitSend()
		{
			return m_FragamentCount > 1;
		}
		//�����յ�����ʱ�����ô˺��������ݲ��,����ֵ��ָ����������Ƿ��������
		//���ؽ���״̬�����������ˣ����Խ��ж�ȡ
		PackStatus RecevData( byte* buffer )
		{
			memcpy( &m_Head, buffer, sizeof(m_Head) );//��ͷ����������ȥ
			if ( m_Head.m_PackageCount > TCPHead::Max_Package_Count
				|| m_Head.Size() > BUFFER_SIZE )
			{//��Ч���ݰ����˳�,�������ǲ����ܵģ���ֹ���˺ڷ�����
				NETDEBUGLOG("Bad Head\n",1);
				ASSERT(false);
				return PS_BADPACKAGE;
			}
			m_RecevFragment ++;
			int copyedSize = sizeof(m_Head);
			for ( int i = 0 ; i < m_Head.m_PackageCount; i ++ )
			{
				if( m_WriteIndex >= m_RecevedVector.size() )//�Ų�����
				{
					m_RecevedVector.resize( m_RecevedVector.size() + Recev_Alloc_Size );//���ٷ���һ��
				}
				m_RecevedVector[m_WriteIndex].m_size = m_Head.m_LengthEachPackage[i];
				memcpy( &(m_RecevedVector[m_WriteIndex].m_Buffer[0]), &buffer[copyedSize], m_Head.m_LengthEachPackage[i]);
				copyedSize += m_Head.m_LengthEachPackage[i];
				m_WriteIndex++;
			}
			if ( m_RecevFragment >= m_Head.m_FragmentCount )
			{
				m_RecevFragment = 0;
				return PS_ALLDONE;
			}
			return PS_FRAGMENT;
		}
		bool GetPackage( BohgeNet::NetPackage& pak )
		{
			if ( m_WriteIndex > 0 ) //���д���������0�ˣ���˵��������
			{
				if( m_ReadIndex >= m_WriteIndex )
				{
					m_WriteIndex = 0;
					m_ReadIndex = 0;
					return false;
				}
				memcpy( pak.GetBuffer(), &m_RecevedVector[m_ReadIndex].m_Buffer[0], m_RecevedVector[m_ReadIndex].m_size );
				pak.SetSize( m_RecevedVector[m_ReadIndex].m_size );
				m_ReadIndex ++;
				return true;
			}
			return false;
		}
		void CopyPackage( BohgeNet::NetPackage& pak )//���ư���
		{
			m_NeedSend ++;
			m_isBufferChange = true;
			while( pak.Size() > ( m_vInternalBuffer.size() - m_CopySize ) ) //���ʣ��Ŀռ䲻�������·�����buffer��������ǰ�ķ���buffer�ڴ��
			{
				m_vInternalBuffer.resize( m_vInternalBuffer.size() + BUFFER_SIZE );
			}
			memcpy( &m_vInternalBuffer[m_CopySize] , pak.GetBuffer(), pak.Size() );
			m_CopySize += pak.Size();
			if( m_PackageCount == m_EachSize.size() )
			{
				m_EachSize.resize( m_EachSize.size() + TCPHead::Max_Package_Count );
			}
			m_EachSize[m_PackageCount] = pak.Size();
			m_PackageCount++;
		}
		bool SendData( byte* out_buffer, int& out_size) //���͵�ǰbuffer�е�����,���ص��������Ƿ�����Ҫ���͵Ķ���
		{
			if( m_NeedSend > 0 )
			{
				m_Head.Reset();
				if(	m_isBufferChange )//���ı������buffer,�����Ҫ����һ����Ҫ���͵�fragment����
				{//�����������
					m_isBufferChange = false;
					uint sizeCount = 0;//�ֲ�����,����ÿ��������������
					uint PackageCount = 0;//����İ�������
					for ( int i = m_SendedPackage ; i < m_NeedSend ; i ++ )
					{
						if (	PackageCount + 1 > TCPHead::Max_Package_Count
							||	sizeCount + this->m_EachSize[i] > BUFFER_SIZE )
						{
							sizeCount = 0;
							PackageCount = 0;
							m_FragamentCount ++;
						}
						{//ͳ����Ҫ���͵�ѶϢ
							sizeCount += this->m_EachSize[i];
							PackageCount ++;
						}
					}//for
					if( 0 != PackageCount )//���û�����ˣ������������ݰ���������
					{
						m_FragamentCount ++;
					}
				}
				{ //����Ŀ�ʼ����
					uint sizeCount = 0;//�ֲ�����,����ÿ��������������
					uint PackageCount = 0;//����İ�������
					uint packageIndex = 0;//��ǰ�����ļ���
					m_Head.m_FragmentCount = m_FragamentCount;//д����һ����������Ƭ������
					for ( int i = m_SendedPackage ; i < m_NeedSend ; i ++ )
					{
						if (	PackageCount + 1 > TCPHead::Max_Package_Count
							||	sizeCount + this->m_EachSize[i] > BUFFER_SIZE )
						{
							m_Head.m_PackageCount = PackageCount;
							memcpy( out_buffer, &m_Head, sizeof(m_Head) );//��ͷ����������ȥ
							memcpy( &out_buffer[sizeof(m_Head)], &this->m_vInternalBuffer[m_SendedSize], sizeCount );//�������ݶ�
							out_size = sizeCount + sizeof(m_Head);
							m_SendedSize += sizeCount;
							m_SendedPackage = i;//�´ο�ʼ��i+1��ʼ����
							return true;
						}
						{//ͳ����Ҫ���͵�ѶϢ
							sizeCount += this->m_EachSize[i];
							m_Head.m_LengthEachPackage[packageIndex] = this->m_EachSize[i];
							PackageCount ++;
							packageIndex ++;
						}
					}//for
					if( 0 != PackageCount )//���û�����ˣ������������ݰ���������
					{
						m_Head.m_PackageCount = PackageCount;
						memcpy( out_buffer, &m_Head, sizeof(m_Head) );//��ͷ����������ȥ
						memcpy( &out_buffer[sizeof(m_Head)], &this->m_vInternalBuffer[m_SendedSize], sizeCount );//�������ݶ�
						out_size = sizeCount + sizeof(m_Head);
						this->Reset();//������Ҫreset
						return true;
					}
				}
			}
			return false;
		}
	public:
		static BOHGE_FORCEINLINE void GetRecevSignal( byte* out_buffer, int& out_size )
		{
			const static byte Signal[ sizeof( SM_RECEV_FRAGMENT ) ] = {
				(byte)(SM_RECEV_FRAGMENT >> 24),
				(byte)(SM_RECEV_FRAGMENT >> 16),
				(byte)(SM_RECEV_FRAGMENT >> 8),
				(byte)SM_RECEV_FRAGMENT
			};
			out_size = sizeof(Signal);
			memcpy( out_buffer, Signal, sizeof(Signal) );
		}
		static BOHGE_FORCEINLINE bool isRecevSignal( byte* buffer )
		{
			int s = buffer[0]<<24 | buffer[1]<<16 | buffer[2]<<8 | buffer[3];
			return s == SM_RECEV_FRAGMENT;
		}
	};

}