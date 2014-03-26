#pragma once
#include "JobBaseThread.h"


#include <queue>


namespace BohgeEngine
{
	//�첽�̶߳���
	class IJob;
	class QueueThread : public JobBaseThread
	{
	private:
		typedef std::queue< IJob* > JobQueue;
	private:
		JobQueue		m_JobQueue;//���ݶ���
	public:
		QueueThread(void);
		~QueueThread(void);
	private:
		virtual bool _isEmpty();//�����Ƿ�Ϊ��
		virtual IJob* _DoPopJob();//����һ������
		virtual void _DoPushJob( IJob* job );//д��һ������	
	};
}
