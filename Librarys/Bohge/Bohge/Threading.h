#pragma once
#include "Predefine.h"

#include <pthread.h>



namespace BohgeEngine
{
	class Threading//�̻߳�����
	{
	public:
		enum ThreadPriority
		{
			TP_LOW = 0,
			TP_NORMAL,
			TP_HIGH,
		};
	private:
		pthread_t		m_pThreadID;
		ThreadPriority	m_ePriority;
		bool			m_isRunning;
	public:
		Threading(void);
		virtual ~Threading(void);
	public:
		static void* ThreadProc( void* param );
	private:
		virtual void _OnBeforeStart() = 0;//�����߳�ǰ���¼�
		virtual void _OnAfterStart() = 0;//�����̺߳���¼�
		virtual bool _StopWork() = 0;//ֹͣ�̹߳������߳�׼���˳�,�����Ƿ�ɹ��˳������ʧ�ܽ�ǿ���˳�
	public:
		virtual void* DoWork() = 0;//�����߳�
	public:
		void Start();//�����߳�
		void Stop();//�ر��߳�
		void SetPriority( ThreadPriority tp );
	public:
		BOHGE_FORCEINLINE bool isRunning() const
		{
			return m_isRunning;
		}
		BOHGE_FORCEINLINE ThreadPriority GetPriority() const
		{
			return m_ePriority;
		}
	};
}

