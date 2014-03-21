#pragma once
#include "Predefine.h"

#include <pthread.h>

namespace BohgeEngine
{
	class ThreadRWLock
	{
	private:
		pthread_rwlock_t	m_pRWLock;
	public:
		ThreadRWLock(void);
		~ThreadRWLock(void);
	public:
		//------------------------------------------------------------------------------------------------------
		inline void ReadLock()//����ʽ�Ķ�ȡ��
		{
			pthread_rwlock_rdlock( &m_pRWLock );
		}
		//------------------------------------------------------------------------------------------------------
		inline void WriteLock()//��ռ��д����
		{
			pthread_rwlock_wrlock( &m_pRWLock );
		}
		//------------------------------------------------------------------------------------------------------
		inline void Unlock()//����
		{
			pthread_rwlock_unlock( &m_pRWLock );
		}
	};
}
