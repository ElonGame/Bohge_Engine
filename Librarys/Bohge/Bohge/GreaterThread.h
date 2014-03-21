#pragma once
#include "JobBaseThread.h"


#include <map>


namespace BohgeEngine
{
	//�첽�̶߳���
	class IAsynJob;
	class GreaterThread : public JobBaseThread
	{
	private:
		typedef std::multimap< int, IAsynJob*, std::greater<int> > JobGreaterMap;
	private:
		JobGreaterMap		m_JobGreaterMap;//���ݶ���
	public:
		GreaterThread(void);
		~GreaterThread(void);
	private:
		virtual bool _isEmpty();//�����Ƿ�Ϊ��
		virtual IAsynJob* _DoPopJob();//����һ������
		virtual void _DoPushJob( IAsynJob* job );//д��һ������	
	};
}
