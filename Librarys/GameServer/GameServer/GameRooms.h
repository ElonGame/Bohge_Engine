//////////////////////////////////////
//			��ҽ����ķ���		   //
/////////////////////////////////////

#pragma once
#include "ServerConfig.h"
#include "BohgeNet.h"
#include "AutoIndex.h"
#include "ClientContext.h"
#include "NetBase.h"
#include "GameSetting.h"
#include <string>
#include <map>



class SQLDataBase;
class GameRooms
{
public:
	enum
	{
		PLAYERS = 2,
	};
public:
	struct PlayerLocalRoom	//����ҷ������˳���ʱ����Ҫ�����������Ϣ�ж��Ƿ���Ҫ�Է������
	{
		uint	m_local;
		uint	m_room;
	};
	struct Player
	{
		std::string				m_UDID;
		uint					m_Win;
		uint					m_Lose;
		std::wstring			m_PlayerName;
		SmartPtr<ClientContext>	m_Client;
		//BohgeNet::Address		m_UDPAddress;//udp�Ķ˿�
		Player()
			:m_Client(NULL)
		{
		}
		inline bool isPlayer( SmartPtr<ClientContext>& client ) const
		{
			if( m_Client == NULL )
			{
				return false;
			}
			return m_Client->isClient( client );
		}
	};
	struct RoomInfo
	{
	private:
		bool				m_isNoBegin;//������Ϸ�Ƿ�ʼ
	public:
		bool				m_isGameOver;
		bool				m_isReady;
		uint				m_ID; //����ID
		uint				m_PlayerCount;
		uint				m_EnterCount;//������Ϸ������¼
		std::wstring		m_Name; //�������
		std::wstring		m_Password;
		Player				m_Players[PLAYERS];
		RoomInfo()
			:m_ID(0),
			m_PlayerCount(1),
			m_isReady(false),
			m_EnterCount(0),
			m_isNoBegin(true),
			m_isGameOver(false)
		{
		}
		bool isFull() const
		{
			return m_PlayerCount >= PLAYERS;
		}
		void RoomStart()
		{
			m_isNoBegin = false;
		}
		bool isNotBegin() const
		{
			return m_isNoBegin;
		}
		void OnGameOver( SmartPtr<ClientContext>& client, SQLDataBase& sql );
	};
public:
	typedef std::map<uint,RoomInfo*>		RoomMap;
private:
	typedef std::map<int, PlayerLocalRoom>		PlayerInfoMap;
private:
	AutoIndex			m_RoomID;//ÿ�������ID
	RoomMap				m_RoomInfoMap[LOCATION_COUNT];//׼��N��������������room
	PlayerInfoMap		m_PlayerInfoMap;
	CRITICAL_SECTION	m_csRoomMap[LOCATION_COUNT];// ����Worker�߳�ͬ���Ļ�����
	CRITICAL_SECTION	m_csPlayerMap;// ����Worker�߳�ͬ���Ļ�����
public:
	GameRooms(void);
	~GameRooms(void);
public:
	static inline GameRooms& Instance()
	{
		static GameRooms gr;
		return gr;
	}
public:
	const RoomInfo& NewRoom( uint local, const std::wstring& roomname, const std::wstring& password, SmartPtr<ClientContext>& client );
	const RoomInfo* EntryRoom( uint local, uint ID, const std::wstring& password, SmartPtr<ClientContext>& client );
	const RoomInfo* FindEntryRoom( uint local, uint ID, SmartPtr<ClientContext>& client, bool& isFind );//�ҵ����ҽ���
	const RoomInfo* QuickBegin( uint local, SmartPtr<ClientContext>& client );
	RoomInfo* FindRoom( uint local, uint ID );//ֻ�ҵ���������Ĵ���
	Player* FindPlayer( uint local, uint ID, SmartPtr<ClientContext>& client );
	SmartPtr<ClientContext>* PlayerLeave( uint local, uint ID, SmartPtr<ClientContext>& client );//��������뿪������û���뿪�����
	PlayerLocalRoom* FindPlayerLocalRoom( SmartPtr<ClientContext>& client );
	RoomInfo* EntryOne( uint local, uint ID );//��һ��������Ϸ��
	void DeleteRoom( uint local, uint ID );
private:
	inline int Index( uint local )
	{
		switch(local)
		{
		case LOCATION_ASIA: return 0;
		case LOCATION_EURO: return 1;
		case LOCATION_USA:  return 2;
		}
		return 0;
	}
public:
	inline void RoomMapBegin( uint local )
	{
		EnterCriticalSection(&m_csRoomMap[Index(local)]);
	}
	inline RoomMap& GetRoomMap( uint local ) //��Ϊˢ�·���ʹ�ã�����ʱ��Ҫ����
	{
		return m_RoomInfoMap[ Index(local) ];
	}
	inline void RoomMapEnd( uint local )
	{
		LeaveCriticalSection(&m_csRoomMap[Index(local)]);
	}
};
