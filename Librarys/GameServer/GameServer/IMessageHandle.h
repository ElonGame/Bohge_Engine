//////////////////////////////////////
//			������������		   //
/////////////////////////////////////

#pragma once
#include "SmartPtr.hpp"
#include "BohgeNet.h"

class ClientContext;
class SQLDataBase;
class IMessageHandle
{
public:
	virtual ~IMessageHandle(){}
	virtual bool HanldeMessage( SmartPtr<ClientContext>& client, BohgeNet::NetPackage& pak, SQLDataBase& sql ) = 0;//�����������true
};