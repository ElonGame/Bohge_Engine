//////////////////////////////////////
//			������������		   //
/////////////////////////////////////
#include "stdafx.h"
#include "Base.h"
#include "RoomHandle.h"
#include "NetBase.h"
#include "ClientContext.h"
#include "SQLDataBase.h"
#include "GameRooms.h"
#include "Correspondent.h"

#include <sstream>
#include <string>


using namespace std;
using namespace BohgeNet;


//-------------------------------------------------------------------------------------------------------
RoomHandle::RoomHandle(void)
{
	//DWORD nThreadID;
	//m_hUDPThreads = ::CreateThread(0, 0, UDPThread, (void *)NULL, 0, &nThreadID);
}
//-------------------------------------------------------------------------------------------------------
RoomHandle::~RoomHandle(void)
{
	//Ҫ��ֹ�̲߳�Ȼ���ڴ�й¶��bug
	//RELEASE_HANDLE( m_hUDPThreads );
}
//-------------------------------------------------------------------------------------------------------
//DWORD WINAPI RoomHandle::UDPThread(LPVOID lpParam)
//{
//	ShowMessage("Room UDP Thread is begin");
//	NetPackage pak;
//	Address from;
//	UDPServer server( UDP_ENTRT_PORT );
//	while( 1 )
//	{
//		if ( server.RecevieFrom( from, pak ) )
//		{
//			pak.Begin();
//			int message;
//			pak>>message;
//			switch (message)
//			{
//			case NM_ROOM_ENTER: //���뷿�䣬��ȡID����
//				{
//					DEBUGLOG("UDPThread Recev UDP Address form %d,%d,%d,%d:%d\n", from.A(),from.B(),from.C(),from.D(),from.Port());
//					uint ID, local;
//					string udid;
//					wstring name;
//					pak>>udid>>local>>ID>>name;
//					GameRooms::RoomInfo* room = GameRooms::Instance().FindRoom( local, ID );
//					if( NULL != room )
//					{
//						bool isSend0 = false;
//						bool isSend1 = false;
//						pak.Begin();
//						pak<<NM_ROOM_UDP_ADDRESS_GET;
//						if ( room->m_Players[0].m_UDID == udid )
//						{
//							if( room->m_Players[0].m_UDPAddress.IP() == 0 )
//							{
//								room->m_Players[0].m_UDPAddress = from;
//							}
//							if( room->m_Players[0].m_UDPAddress == from )
//							{
//								room->m_Players[0].m_Client->Send( pak );
//								isSend0 = true;
//							}
//						}
//						if ( room->m_Players[1].m_UDID == udid )
//						{
//							if( room->m_Players[1].m_UDPAddress.IP() == 0 )
//							{
//								room->m_Players[1].m_UDPAddress = from;
//							}
//							if( room->m_Players[1].m_UDPAddress == from )
//							{
//								room->m_Players[1].m_Client->Send( pak );
//								isSend1 = true;
//							}
//						}
//						if( room->isFull() 
//							&& room->m_Players[0].m_UDPAddress.IP() != 0
//							&& room->m_Players[1].m_UDPAddress.IP() != 0 )
//						{
//							pak.Begin();
//							pak<<NM_ROOM_DATA
//								<<(uint)room->m_Players[1].m_UDPAddress.IP()
//								<<room->m_Players[1].m_UDPAddress.Port()
//								<<room->m_Players[1].m_PlayerName;
//							room->m_Players[0].m_Client->Send(pak);
//							isSend0 = true;
//							
//
//							pak.Begin();
//							pak<<NM_ROOM_DATA
//								<<(uint)room->m_Players[0].m_UDPAddress.IP()
//								<<room->m_Players[0].m_UDPAddress.Port()
//								<<room->m_Players[0].m_PlayerName;
//							room->m_Players[1].m_Client->Send(pak);
//							isSend1 = true;
//							
//						}
//						if( isSend0 )
//						{
//							room->m_Players[0].m_Client->SendImmediately();
//						}
//						if( isSend1 )
//						{
//							room->m_Players[1].m_Client->SendImmediately();
//						}
//					}
//				}break;
//			}
//
//		}
//	}
//}
//-------------------------------------------------------------------------------------------------------
bool RoomHandle::HanldeMessage( SmartPtr<ClientContext>& client, BohgeNet::NetPackage& pak, SQLDataBase& sql )
{
	pak.Begin();
	int message;
	pak>>message;
	switch (message)
	{
	case NM_ROOM_ENTER: //���뷿�䣬��ȡID����
		{
			DEBUGLOG("Receive from %s NM_ROOM_ENTER\n", client->BindIPString() );
			uint ID, local, win, lose;
			string udid;
			wstring name;
			pak>>udid>>local>>ID>>win>>lose>>name;
			GameRooms::RoomInfo* room = GameRooms::Instance().FindRoom( local, ID );
			if( NULL != room 
				&&
					( room->m_Players[0].isPlayer(client)
					||room->m_Players[1].isPlayer(client)
					) )
			{
				bool isHost = false;
				if( room->m_Players[0].isPlayer( client ) )
				{
					isHost = true;
					room->m_Players[0].m_UDID = udid;
					room->m_Players[0].m_Win = win;
					room->m_Players[0].m_Lose = lose;
					room->m_Players[0].m_PlayerName = name;
				}
				else if ( room->m_Players[1].isPlayer( client ) )
				{
					room->m_Players[1].m_UDID = udid;	
					room->m_Players[1].m_Win = win;
					room->m_Players[1].m_Lose = lose;
					room->m_Players[1].m_PlayerName = name;
				}
				room->m_isReady = false;//����δ׼��
				DEBUGLOG( "pass test, enter room\n" );
				pak.Begin();
				pak<<NM_ROOM_ENTER_PASS<<isHost;
				client->Send( pak );

				//�����˾ͷ���֪ͨ,�޳�UDP���¼ӵ�
				if( room->isFull() )
				{
					pak.Begin();
					pak<<NM_ROOM_DATA
						<<room->m_Players[1].m_Win
						<<room->m_Players[1].m_Lose
						<<room->m_Players[1].m_PlayerName;
					room->m_Players[0].m_Client->Send(pak);
					room->m_Players[0].m_Client->SendImmediately();
					pak.Begin();
					pak<<NM_ROOM_DATA
						<<room->m_Players[0].m_Win
						<<room->m_Players[0].m_Lose
						<<room->m_Players[0].m_PlayerName;
					room->m_Players[1].m_Client->Send(pak);
					room->m_Players[1].m_Client->SendImmediately();
				}
			}
			else
			{
				GameRooms::Instance().PlayerLeave(local, ID, client);
				DEBUGLOG( "fail test, return lobby\n" );
				pak.Begin();
				pak<<NM_ROOM_ENTER_REFUES<<MAIN_ENTER_PORT;
				client->Send( pak );
			}
		}return true;
	//case NM_ROOM_SUCCESS_PUNCHING://�ͻ��˴򶴳ɹ�
	//	{
	//		DEBUGLOG("Receive from %s NM_ROOM_UDP_PUNCHING\n", client->BindIPString() );
	//		uint ID, local;
	//		pak>>local>>ID;
	//		GameRooms::RoomInfo* room = GameRooms::Instance().FindRoom( local, ID );
	//		if( NULL != room )
	//		{
	//			if( room->isFull() )
	//			{
	//				pak.Begin();
	//				pak<<NM_ROOM_SUCCESS_PUNCHING;//֪ͨ��һ���ͻ���ֹͣ����punching
	//				if( room->m_Players[0].isPlayer( client ) )
	//				{
	//					if( NULL != room->m_Players[1].m_Client )
	//					{
	//						room->m_Players[1].m_Client->Send( pak );
	//						room->m_Players[1].m_Client->SendImmediately();
	//					}
	//				}
	//				else if( room->m_Players[1].isPlayer( client ) )
	//				{
	//					if( NULL != room->m_Players[0].m_Client )
	//					{
	//						room->m_Players[0].m_Client->Send( pak );
	//						room->m_Players[0].m_Client->SendImmediately();
	//					}
	//				}
	//			}
	//		}
	//	}return true;
	case NM_ROOM_ACTION:
		{
			uint ID, local;
			pak>>local>>ID;
			GameRooms::RoomInfo* room = GameRooms::Instance().FindRoom( local, ID );
			if( NULL != room )
			{
				if( room->isFull() )
				{
					DEBUGLOG("Recive Action ID %d ", ID);
					if( room->m_Players[1].isPlayer( client ) ) //����Ƕ���λ�����Ǹ���׼������
					{
						DEBUGLOG("2nd Ready\n");
						room->m_isReady = true;
						pak.Begin();
						pak<<NM_GAME_READY;
						client->Send( pak );
						if( NULL != room->m_Players[0].m_Client )
						{
							room->m_Players[0].m_Client->Send(pak);
							room->m_Players[0].m_Client->SendImmediately();
						}
					}
					else if( 
						room->m_Players[0].isPlayer(client) //��1��λ
						&& room->m_isReady //��Ҷ�׼����
						&& room->isFull()  //��������
						&& room->isNotBegin() )
					{
						DEBUGLOG("Game Begin\n");
						room->RoomStart();
						pak.Begin();
						pak<<NM_GAME_BEGIN;
						//�ı����ݿ�
						for (int i = 0 ; i < GameRooms::PLAYERS ; i ++)
						{
							stringstream str;
							str<<"SELECT * FROM Players WHERE UDID='"<<room->m_Players[i].m_UDID<<"'";
							sql.BeginQuery( str.str() );
							if( sql.hasRecord() )//�ҵ������Ϣ��
							{
								int freetimes = sql.GetValue( "FreeTimes" );
								int buytimes = sql.GetValue( "BuyTimes" );
								if ( freetimes > 0 )
								{
									sql.SetValue( "FreeTimes", --freetimes );
								}
								else
								{
									if( buytimes <= 0 )
									{
										ShowMessage("No free time, Buy time < 0 !!");
									}
									else
									{
										sql.SetValue( "BuyTimes", --buytimes );
									}
								}
							}
							sql.End();
							if ( room->m_Players[i].m_Client != NULL )
							{
								room->m_Players[i].m_Client->Send( pak );
								room->m_Players[i].m_Client->SendImmediately();
							}
						}
					}
					else
					{
						bool isInRoom = false;//�ڷ��������
						for(int i = 0 ; i < GameRooms::PLAYERS ; i ++ )
						{
							if( room->m_Players[i].isPlayer( client ) )
							{
								isInRoom = true;
							}
						}
						if( isInRoom )
						{
							NETDEBUGLOG("Game Cant Begin\n");
							std::wstring str = L"Game Cant Begin";
							pak.Begin();
							pak<<NM_TEXT_CONTENT<<str;
							client->Send( pak );
						}
						else
						{
							ShowMessage("action sent by someone who are not in the room!!\n");
							pak.Begin();
							pak<<NM_ROOM_ENTER_REFUES;//�ܾ�����,ת�ص�������
							client->Send( pak );
						}
					}
				}
				else
				{
					bool isHost = false;
					if( room->m_Players[0].isPlayer( client ) )
					{
						isHost = true;
					}
					room->m_isReady = false;//����δ׼��
					DEBUGLOG( "ready or action not use\n" );
					pak.Begin();
					pak<<NM_ROOM_ENTER_PASS<<isHost;
					client->Send( pak );
				}
			}
		}return true;
	case NM_TEXT_CONTENT:
		{
			uint local,id;
			pak>>local>>id;
			GameRooms::RoomInfo* room = GameRooms::Instance().FindRoom( local,id );
			if( NULL != room )
			{
				if( room->isFull() )
				{
					if( room->m_Players[0].isPlayer( client ) )
					{
						DEBUGLOG( "Send message to %s:%d", room->m_Players[1].m_Client->BindIPString(), room->m_Players[1].m_Client->BindPort() );
						if ( room->m_Players[1].m_Client != NULL )
						{
							room->m_Players[1].m_Client->Send( pak );
							room->m_Players[1].m_Client->SendImmediately();
						}
					}
					else if( room->m_Players[1].isPlayer( client ) )
					{
						DEBUGLOG( "Send message to %s:%d", room->m_Players[0].m_Client->BindIPString(), room->m_Players[1].m_Client->BindPort() );
						if ( room->m_Players[0].m_Client != NULL )
						{
							room->m_Players[0].m_Client->Send( pak );
							room->m_Players[0].m_Client->SendImmediately();
						}
					}
				}
			}
		}return true;
	case NM_ROOM_LEAVE:
		{
			DEBUGLOG(" Player Leave " );
			uint local,ID;
			pak>>local>>ID;
			bool isCanLeave = false;
			GameRooms::RoomInfo* room = GameRooms::Instance().FindRoom( local,ID );
			if( NULL != room )
			{
				if( room->isNotBegin() )
				{
					isCanLeave = true;
				}
			}
			else
			{
				isCanLeave = true;
			}

			if( isCanLeave )
			{
				pak.Begin();
				pak<<NM_ROOM_LEAVE_PASS;//֪ͨ����ң�������뿪��
				client->Send( pak );
				GameRooms::Player* player = GameRooms::Instance().FindPlayer( local, ID, client );
				if( NULL != player )
				{
					wstring name = player->m_PlayerName;
					SmartPtr<ClientContext>* others = GameRooms::Instance().PlayerLeave( local, ID, client );
					if( NULL != others )
					{
						//֪ͨ��һ����ң��Է��뿪��
						pak.Begin();
						pak<<NM_ROOM_LEAVE<<name;
						(*others)->Send( pak );
						(*others)->SendImmediately();
					}
				}
			}
		}return true;
	}
	return false;
}
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------