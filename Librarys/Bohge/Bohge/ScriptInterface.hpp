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



/////////////////////////////
//		�ű���Ľӿ�	   //
/////////////////////////////
#pragma once
#include "3DMath.h"
#include "ScriptBase.h"

////���ں����ӿڴ�������������
//void FlushTiggerInterface( ScriptBase* ptr )
//{
//	ptr->RegClass<ITigger>( "ITigger" ); //ע��һ���࣬ʵ����table
//	ptr->RegClassFunc<ITigger>( "StartTigger", &ITigger::Script_StartTigger );//ע��table��Ա
//	ptr->RegClassFunc<ITigger>( "StopTigger", &ITigger::Script_StopTigger );//ע��table��Ա
//}
//������ӻᵼ�µ��øú����ı���
//ԭ���ǵ�c++�˵���lua��ʱ���������tableע��ĺ���ָ����Tigger�ĺ�����ַ������������Ǹ��麯��
//c++ʵ���麯����һ���麯�����������ϼ��ĺ�����ַ
//Ȼ������ע�����ITigger�ĵ�ַ�������ַָ��һ�����麯����������runtime���õ�ʱ���ָ����һ������ĵ�ַ
//������Ҫ����ģ��ķ��ͱ�̣�������������Ҫinclude "ScriptBase.h"
//��������ע��ĺ���ָ����ʵ�ʵ�engine��ĺ�����ַ������Ҳ��ӵ�ʱlua֧���˶�̬

//��Ҫ
//luatinker��֧�����ã�����������luatinker�ṹ���Ը�local��������ص��ºܶ�����
//������ע�ắ����ʱ��ؼ���������һ������������

//ע����ļ̳е���Ҫע��ĺ�������ʹ�û���ĳ�Ա������ȫ����Ŀ�꺯����Ҫ������д��
//�ں�����ǰ�����Lua_xxx������μ�ScriptEmeitter


namespace BohgeEngine
{
	//�ӿ����
	template< typename T1 >
	void AsIAnimation( T1* ptr )
	{
		ptr->T1::template RegClassFunc< T1 >( "StartAnimation", &T1::Lua_StartAnimation );//ע��table��Ա
		ptr->T1::template RegClassFunc< T1 >( "StopAnimation", &T1::Lua_StopAnimation );//ע��table��Ա
	}

	template< typename T1 >
	void AsILocatable( T1* ptr )
	{
		ptr->T1::template RegClassFunc<T1>( "SetLocalRotation", &T1::Lua_SetLocalRotation );//ע��table��Ա
		ptr->T1::template RegClassFunc<T1>( "SetLocalPosition", &T1::Lua_SetLocalPosition );//ע��table��Ա
		ptr->T1::template RegClassFunc<T1>( "SetLocalScale", &T1::Lua_SetLocalScale );//ע��table��Ա
		ptr->T1::template RegClassFunc<T1>( "GetLocalPosition", &T1::Lua_GetLocalPosition );//ע��table��Ա
		ptr->T1::template RegClassFunc<T1>( "GetLocalScale", &T1::Lua_GetLocalScale );//ע��table��Ա
		ptr->T1::template RegClassFunc<T1>( "GetLocalRotation", &T1::Lua_GetLocalRotation );//ע��table��Ա
		ptr->T1::template RegClassFunc<T1>( "GetWorldPosition", &T1::Lua_GetWorldPosition );//ע��table��Ա
		ptr->T1::template RegClassFunc<T1>( "GetWorldScale", &T1::Lua_GetWorldScale );//ע��table��Ա
		ptr->T1::template RegClassFunc<T1>( "GetWorldRotation", &T1::Lua_GetWorldRotation );//ע��table��Ա
	}


	//ȡ���ӿڶ��壬�˷�����
	////�ӿڶ���
	//class ITigger //����ײ��������һЩʱ��ʱ������
	//{
	//public:
	//	virtual ~ITigger(){}
	//public:
	//	virtual void Script_StartTigger() = 0; //�ɴ������߼�������
	//	virtual void Script_StopTigger() = 0;//ֹͣ
	//};
	//
	//class ILocatable
	//{
	//public:
	//	virtual ~ILocatable(){}
	//public:
	//	virtual void Script_SetRotate( const Quaternionf& r ) = 0;
	//	virtual void Script_SetPosition( const vector3f& p ) = 0;
	//	virtual void Script_SetScale( const vector3f& s ) = 0;
	//	virtual const vector3f& Script_GetPosition( ) = 0;
	//	virtual const vector3f& Script_GetScale( ) = 0;
	//	virtual const Quaternionf& Script_GetRotation( ) = 0;
	//};

}