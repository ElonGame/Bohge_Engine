//////////////////////////////////////
//			������������		   //
/////////////////////////////////////

#pragma once
#include "IMessageHandle.h"
#include "Utility.h"

class RoomHandle : public IMessageHandle
{
private:
	//HANDLE				m_hUDPThreads;
public:
	RoomHandle(void);
	~RoomHandle(void);
	virtual bool HanldeMessage( SmartPtr<ClientContext>& client, BohgeNet::NetPackage& pak, SQLDataBase& sql );

	// �̺߳�����ΪIOCP�������Ĺ������߳�
	//static DWORD WINAPI UDPThread(LPVOID lpParam);
};
