//////////////////////////////////////////////////////////////////////////////////////
//
//						The Bohge Engine License (BEL)
//
//	Copyright (c) 2011-2014 Peng Zhao
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in 
//	all copies or substantial portions of the Software. And the logo of 
//	Bohge Engine shall be displayed full screen for more than 3 seconds 
//	when the software is started. Copyright holders are allowed to develop 
//	game edit based on Bohge Engine, The edit must be released under the MIT 
//	open source license if it is going to be published. In no event shall 
//	copyright holders be prohibited from using any code of Bohge Engine 
//	to develop any other analogous game engines.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//
//////////////////////////////////////////////////////////////////////////////////////



/////////////////////////
//		��Ϣ����	   //
/////////////////////////
#pragma once


namespace BohgeNet
{
	enum SystemMessage
	{
		SM_RECEV_FRAGMENT = 0xffffffff,
	};
	enum NetMessage
	{
		NM_CONNECT_SERVER = 0x80000001, //�ͻ��˷��͵Ľ�������
		NM_SERVER_NOTICE,//������ͨ��
		NM_CHANGE_LOCATION,//�ı��������ַ
		NM_NEW_USER,//�������û�����Ҫ�û���Ϣ
		NM_USER_DATA,//���ص��û���Ϣ
		NM_LOBBY_INFO, //����lobby��Ϣ
		NM_NO_GAME_TIMES,//û����Ϸʱ���ˣ��ܾ�����
		NM_LOBBY_PORT, //������������LOBBY��ַ
		NM_LOBBY_ENTER,//����lobby
		NM_LOBBY_ROOM_REFESH,//ˢ�·�����Ϣ
		NM_LOBBY_ROOM_DATA,//�������񷢳��ķ���ĵ�ǰ�������Ϣ
		NM_LOBBY_ROOM_NEW,//�㲥һ���·���
		NM_LOBBY_ROOM_CLOSE,//��Ҫ�㲥�ķ�����ʧ��Ϣ
		NM_LOBBY_ROOM_ENTER,//���뷿������
		NM_LOBBY_ROOM_FIND,//��ѯָ������
		NM_LOBBY_ROOM_PASSWORD,//Ҫ����������
		NM_LOBBY_ROOM_QUICK,//���ٿ�ʼ
		NM_LOBBY_ROOM_ENTER_PASS,//׼����뷿��
		NM_LOBBY_ROOM_ENTER_REFUES,//�ܾ����㷿��
		NM_ROOM_PORT,//����������˿�
		NM_ROOM_ENTER,//���뷿��
		NM_ROOM_UDP_ADDRESS_GET,//�õ������UDP��ַ
		NM_ROOM_UDP_PUNCHING,//UDP��
		NM_ROOM_SUCCESS_PUNCHING,//UDP�򶴳ɹ�
		NM_ROOM_ENTER_PASS,//���뷿��ɹ�
		NM_ROOM_ENTER_REFUES,//���뷿��ʧ��
		NM_ROOM_DATA,//���ͷ�������
		NM_ROOM_ACTION,//׼�����߿�ʼ��Ϸ
		NM_ROOM_LEAVE_PASS,//����ͻ����˳�
		NM_ROOM_HEART,//������
		NM_ROOM_LEAVE,//�ͻ��˷����������ģ���ϢҪ����
		NM_TEXT_CONTENT,//�����ı�

		NM_GAME_READY,//����׼������
		NM_GAME_BEGIN,//��Ϸ��ʼ
		NM_GAEM_IM_READY,//���׼������
		NM_GAME_IM_START,//�Է�֪����׼�����ˣ��Ѿ���ʼ��
		NM_GAME_STACK_DATA,//���ϵ�����
		NM_GAME_SKILL_DATA,//����ʹ�õ�����
		NM_GAME_DEAD,//ʧ����
		NM_GAME_LEAVE,//����Ϸ���뿪��

		NM_RECEIPT_STATUS,//���ͻ����Ƿ�汾��ͬ���û��Ƿ����
		NM_RECEIPT_DATA,//�վ�����
		NM_RECEIPT_RECEVED,//���յ������ˣ���Ҫ�����ˣ��ȴ���֤��
		NM_RECEIPT_RESULT,//����������֤���

		NM_LEAVE,//�뿪�������������ͻ��ˣ����ݵ�ǰ״̬��������ô����
	};
}