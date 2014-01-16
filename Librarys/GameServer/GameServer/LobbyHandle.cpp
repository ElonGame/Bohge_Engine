//////////////////////////////////////
//			������������		   //
/////////////////////////////////////
#include "stdafx.h"
#include "Base.h"
#include "LobbyHandle.h"
#include "NetBase.h"
#include "ClientContext.h"
#include "SQLDataBase.h"
#include "GameRooms.h"
#include "GameSetting.h"

#include <sstream>
#include <string>


using namespace std;
using namespace BohgeNet;

int Index(uint l)
{
	switch(l)
	{
	case LOCATION_ASIA: return 0;
	case LOCATION_EURO: return 1;
	case LOCATION_USA:  return 2;
	}
	return 0;
}


//-------------------------------------------------------------------------------------------------------
LobbyHandle::LobbyHandle(void)
{
	//GameRooms::Instance().NewRoom(0, L"Bohge", L"1", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(0, L"�����", L"", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(0, L"Penn", L"3", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(0, L"�Ǹ��Ƥ", L"", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(0, L"������", L"4", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(0, L"������", L"", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(1, L"7", L"2", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(1, L"8", L"2", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(1, L"9", L"", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(1, L"10", L"2", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(1, L"11", L"2", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(1, L"12", L"", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(1, L"13", L"1", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(1, L"14", L"", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(2, L"15", L"2", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(2, L"16", L"3", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(2, L"17", L"4", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(2, L"18", L"2", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(2, L"19", L"", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(2, L"20", L"", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(2, L"21", L"", SmartPtr<ClientContext>() );
	//GameRooms::Instance().NewRoom(2, L"22", L"", SmartPtr<ClientContext>() );
}
//-------------------------------------------------------------------------------------------------------
LobbyHandle::~LobbyHandle(void)
{
}
//-------------------------------------------------------------------------------------------------------
bool LobbyHandle::HanldeMessage( SmartPtr<ClientContext>& client, BohgeNet::NetPackage& pak, SQLDataBase& sql )
{
	pak.Begin();
	int message;
	pak>>message;
	switch (message)
	{
	case NM_LOBBY_ENTER: //���͵�ǰ������Ϣ
	case NM_LOBBY_ROOM_REFESH: //���͵�ǰ������Ϣ
		{
			uint location;
			pak>>location;
			DEBUGLOG("Receive from %s  NM_LOBBY_ENTER  NM_LOBBY_ROOM_REFESH\n", client->BindIPString() );
			int i = 0;
			GameRooms::Instance().RoomMapBegin( location );
			for( GameRooms::RoomMap::iterator it = GameRooms::Instance().GetRoomMap(location).begin();
				it != GameRooms::Instance().GetRoomMap(location).end();
				it++ )
			{
				if( i >= 20 )//֮����ǰ20�����䣬ˢ����Ҫ�ͻ��ֶ�ˢ��
				{
					break;
				}
				if( !it->second->isFull() && it->second->isNotBegin() )
				{
					i++;
					pak.Begin();
					pak<<NM_LOBBY_ROOM_DATA<<it->second->m_ID<<it->second->m_Name<<(!it->second->m_Password.empty());
					client->Send( pak );
				}
			}
			GameRooms::Instance().RoomMapEnd( location );
			return true;
		}return true;
	case NM_LOBBY_ROOM_NEW://ת������������
		{
			DEBUGLOG("Receive from %s NM_LOBBY_ROOM_NEW\n", client->BindIPString() );
			uint local;
			wstring n,p;
			pak >> local;
			pak >> n;
			pak >> p;
			pak.Begin();
			pak<<NM_ROOM_PORT<<GameRooms::Instance().NewRoom(local, n, p, client ).m_ID;//��ӷ��䣬֪ͨ�ͻ���ת���˿ں������е�ID
			client->Send( pak );
			//���ݸ�room�߳��½���������Ϣ
			return true;
		}return true;
	case NM_LOBBY_ROOM_ENTER:
		{
			DEBUGLOG("Receive from %s NM_LOBBY_ROOM_ENTER\n", client->BindIPString() );
			bool isInputPassword;
			uint local;
			uint roomID = 0;
			wstring password;
			pak>>local>>roomID>>isInputPassword;
			NETDEBUGLOG("Room ID's %d\n", roomID);
			if( isInputPassword )
			{
				pak>>password;
			}
			const GameRooms::RoomInfo* proom = GameRooms::Instance().EntryRoom( local, roomID, password, client );
			if ( NULL == proom )//û���ҵ�����
			{
				pak.Begin();
				pak<<NM_LOBBY_ROOM_ENTER_REFUES;//�ܾ�����
				client->Send( pak );
			}
			else
			{
				DEBUGLOG("Password is Currect!\n");
				pak.Begin();
				pak<<NM_LOBBY_ROOM_ENTER_PASS
					<<roomID;
				client->Send( pak );
			}
		}return true;
	case NM_LOBBY_ROOM_FIND:
		{
			DEBUGLOG("Receive from %s NM_LOBBY_ROOM_FIND\n", client->BindIPString() );
			uint local, roomID = 0;
			pak>>local>>roomID;
			bool isFind = false;
			const GameRooms::RoomInfo* ri = GameRooms::Instance().FindEntryRoom(local, roomID, client, isFind );
			if ( false == isFind )//û���ҵ�����
			{
				pak.Begin();
				pak<<NM_LOBBY_ROOM_ENTER_REFUES;//�ܾ�����
				client->Send( pak );
			}
			else
			{
				if( NULL != ri )//������������ǿյľ�ֱ�ӽ���
				{
					pak.Begin();
					pak<<NM_LOBBY_ROOM_ENTER_PASS
						<<roomID;
					client->Send( pak );
					//֪ͨroom���˽�����
				}
				else
				{
					NETDEBUGLOG("Need the Password!\n");
					pak.Begin();
					pak<<NM_LOBBY_ROOM_PASSWORD;//��ͻ�����������
					client->Send( pak );
				}
			}
		}return true;
	case NM_LOBBY_ROOM_QUICK:
		{
			DEBUGLOG("Receive from %s NM_LOBBY_ROOM_QUICK\n", client->BindIPString() );
			uint local;
			pak>>local;
			const GameRooms::RoomInfo* ri = GameRooms::Instance().QuickBegin( local, client );
			if( NULL == ri )
			{
				pak.Begin();
				pak<<NM_LOBBY_ROOM_ENTER_REFUES;//�ܾ�����
				client->Send( pak );
			}
			else
			{
				pak.Begin();
				pak<<NM_LOBBY_ROOM_ENTER_PASS
					<<ri->m_ID;
				client->Send( pak );
			}
		}
	}
	return false;
}
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------