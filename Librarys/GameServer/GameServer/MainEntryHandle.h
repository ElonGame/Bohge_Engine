//////////////////////////////////////
//			������������		   //
/////////////////////////////////////

#pragma once
#include "IMessageHandle.h"
#include "Utility.h"

class MainEntryHandle : public IMessageHandle
{
private:
	int				m_AddTimeDay;
	YYYYMMDD		m_Date;//����
public:
	MainEntryHandle(void);
	~MainEntryHandle(void);
	virtual bool HanldeMessage( SmartPtr<ClientContext>& client, BohgeNet::NetPackage& pak, SQLDataBase& sql );
};
