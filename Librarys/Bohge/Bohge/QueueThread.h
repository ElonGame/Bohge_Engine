#pragma once
#include "JobBaseThread.h"


#include <queue>


namespace BohgeEngine
{
	//�첽�̶߳���
	class IAsynJob;
	class QueueThread : public JobBaseThread
	{
	private:
		typedef std::queue< IAsynJob* > JobQueue;
	private:
		JobQueue		m_JobQueue;//���ݶ���
	public:
		QueueThread(void);
		~QueueThread(void);
	private:
		virtual bool _isEmpty();//�����Ƿ�Ϊ��
		virtual IAsynJob* _DoPopJob();//����һ������
		virtual void _DoPushJob( IAsynJob* job );//д��һ������	
	};
}
