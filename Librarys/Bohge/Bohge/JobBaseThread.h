#pragma once
#include "Threading.h"



namespace BohgeEngine
{
	//�첽�̶߳���
	class IAsynJob;
	class ThreadMutex;
	class ThreadCondition;
	class JobBaseThread : public Threading
	{
	private:
		ThreadMutex*		m_pMutex;
		ThreadCondition*	m_pCondition;
		bool				m_isQuitQueue;
		bool				m_isWorking;
	public:
		JobBaseThread(void);
		virtual ~JobBaseThread(void);
	private:
		void _SendSignal();
	private:
		virtual bool _isEmpty() = 0;//�����Ƿ�Ϊ��
		virtual IAsynJob* _DoPopJob() = 0;//����һ������
		virtual void _DoPushJob( IAsynJob* job ) = 0;//д��һ������
	private:
		virtual void _OnBeforeStart();//�����߳�ǰ���¼�
		virtual void _OnAfterStart();//�����̺߳���¼�
		virtual bool _StopWork();//ֹͣ�̹߳������߳�׼���˳�,�����Ƿ�ɹ��˳������ʧ�ܽ�ǿ���˳�
	public:
		virtual void* DoWork();//�����߳�
	public:
		void PushJob( IAsynJob* job );//д��һ������
	};
}
