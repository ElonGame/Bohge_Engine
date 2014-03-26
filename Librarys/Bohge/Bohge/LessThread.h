#pragma once
#include "JobBaseThread.h"


#include <map>


namespace BohgeEngine
{
	//�첽�̶߳���
	class IJob;
	class LessThread : public JobBaseThread
	{
	private:
		typedef std::multimap< int, IJob* > JobLessMap;
	private:
		JobLessMap		m_JobLessMap;//���ݶ���
	public:
		LessThread(void);
		~LessThread(void);
	private:
		virtual bool _isEmpty();//�����Ƿ�Ϊ��
		virtual IJob* _DoPopJob();//����һ������
		virtual void _DoPushJob( IJob* job );//д��һ������	
	};
}
