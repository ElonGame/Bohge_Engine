#pragma once
#include "JobBaseThread.h"


#include <map>


namespace BohgeEngine
{
	//�첽�̶߳���
	class IAsynJob;
	class LessThread : public JobBaseThread
	{
	private:
		typedef std::multimap< int, IAsynJob* > JobLessMap;
	private:
		JobLessMap		m_JobLessMap;//���ݶ���
	public:
		LessThread(void);
		~LessThread(void);
	private:
		virtual bool _isEmpty();//�����Ƿ�Ϊ��
		virtual IAsynJob* _DoPopJob();//����һ������
		virtual void _DoPushJob( IAsynJob* job );//д��һ������	
	};
}
