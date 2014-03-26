#pragma once
#include "JobBaseThread.h"


#include <map>


namespace BohgeEngine
{
	//�첽�̶߳���
	class IJob;
	class GreaterThread : public JobBaseThread
	{
	private:
		typedef std::multimap< int, IJob*, std::greater<int> > JobGreaterMap;
	private:
		JobGreaterMap		m_JobGreaterMap;//���ݶ���
	public:
		GreaterThread(void);
		~GreaterThread(void);
	private:
		virtual bool _isEmpty();//�����Ƿ�Ϊ��
		virtual IJob* _DoPopJob();//����һ������
		virtual void _DoPushJob( IJob* job );//д��һ������	
	};
}
