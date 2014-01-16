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
//		Lua�ű�����		   //
/////////////////////////////
#pragma once
#include "lua_tinker.hpp"
#include "Event.hpp"
#include "ScriptManager.h"
#include "Engine.h"
#include "ScriptNodeInterface.h"

#include <set>
#include <vector>
#include <string>

typedef lua_tinker::table LuaTable;//lua��table�õ������Ǵ�1��ʼ

namespace BohgeEngine
{
	class ScriptBase
	{
	private:
		typedef std::vector< LuaTable* > TableVector;
		typedef std::set< int > LoadedSet;
	private:
		int				m_TableIndex;
		lua_State*		m_LuaState;
		LoadedSet		m_LoadedSet;
		std::string		m_Scriptname;
		TableVector		m_TableVector;
		bool			m_isLoaded;//�Ƿ������
#ifdef LUA_DEBUG_MODE
	private:
		bool			m_isError;
		FunctionPtr		m_OnErrorFunc;
#endif
	public:
		ScriptBase( );
		virtual ~ScriptBase( );
	protected:
		void _NewLuaState();
		void _CloseLuaState();
	public:
		virtual void LoadScript( const std::string& filename );
		virtual void DoScript( const std::string& buffer );
		virtual void UnLoadScript();
#ifdef LUA_DEBUG_MODE
		bool OnScriptError( const std::string& text );
#endif
	private:
		virtual void _OnScriptLoaded() = 0;//���ű����غ�
		virtual void _SetGlobalClassesIndex();//����ȫ�ֵ��ཨ�����������ʵļ̳�����Ҫ��������������������Լ���ȫ��ֵ
	private:
		template<typename T> int _GetIndex()//������ģ�庯���������ڲ�ͨTʱ�򱣴�ÿ��T��index
		{
			static bool isFirst = true;
			static int index = m_TableIndex;
			if ( isFirst )
			{
				isFirst = false;
				m_TableIndex++;
			}
			return index;
		}
	public:
		void LoadClasses( int name ); //������Ҫ���صĿ����
	public:
		BOHGE_FORCEINLINE lua_State* LuaState()
		{
			return m_LuaState;
		}
	public:
		template<typename CLASS>
		BOHGE_FORCEINLINE void RegTable( const char *name )
		{
			_GetIndex<CLASS>();
			LuaTable* newtable = new LuaTable( m_LuaState, name );
			m_TableVector.push_back( newtable );
		}
		template< typename CLASS, typename T >
		BOHGE_FORCEINLINE void SetTableVolume( const char *name, const T& v )
		{
			int index = _GetIndex<CLASS>();
			m_TableVector[index]->set( name, v );
		}
		//HACK ʹ�����ô��ݣ�
		template<typename T>
		BOHGE_FORCEINLINE void SetGlobal(const char* name, T object)
		{
#ifdef LUA_DEBUG_MODE
			if ( !m_isError && m_isLoaded )
			{
				Engine::Instance().GetScriptManage()->OnScriptErrorEvent( m_OnErrorFunc );
				lua_tinker::setglobal(m_LuaState, name, object);
			}
#else
			lua_tinker::setglobal(m_LuaState, name, object);
#endif
		}

		template<typename T>
		BOHGE_FORCEINLINE T GetGlobal(const char* name) const
		{
#ifdef LUA_DEBUG_MODE
			if ( !m_isError && m_isLoaded )
			{
				Engine::Instance().GetScriptManage()->OnScriptErrorEvent( m_OnErrorFunc );
				return lua_tinker::getglobal<T>(m_LuaState, name);
			}
#else
			return lua_tinker::getglobal<T>(m_LuaState, name);
#endif
		}

		//�����lua_CFunction ���͵ĺ���
		template<typename F>
		BOHGE_FORCEINLINE void RegFunc(const char *funcName, F func) 
		{
#ifdef LUA_DEBUG_MODE
			if ( !m_isError && m_isLoaded )
			{
				Engine::Instance().GetScriptManage()->OnScriptErrorEvent( m_OnErrorFunc );
				lua_tinker::def<F>(m_LuaState, funcName, func);
			}
#else
			lua_tinker::def<F>(m_LuaState, funcName, func);
#endif
		}

		//����class 
		template<typename Class>//ע������
		BOHGE_FORCEINLINE void RegClass(const char *name)
		{
#ifdef LUA_DEBUG_MODE
			if ( !m_isError && m_isLoaded )
			{
				Engine::Instance().GetScriptManage()->OnScriptErrorEvent( m_OnErrorFunc );
				lua_tinker::class_add<Class>(m_LuaState, name);
			}
#else
			lua_tinker::class_add<Class>(m_LuaState, name);
#endif
		} 

		//�����̳У����캯����������Ա��������������ں���ע��ĺ��棬ӦΪ�ڲ��ұ����ֵ�ʱ�����õ�static�ַ����������ֵ���ں���ע���ʱ��д���

		template<typename Classengine, typename ClassBase>//ע����ļ̳й�ϵ
		BOHGE_FORCEINLINE void RegClassInh()
		{
#ifdef LUA_DEBUG_MODE
			if ( !m_isError && m_isLoaded )
			{
				Engine::Instance().GetScriptManage()->OnScriptErrorEvent( m_OnErrorFunc );
				lua_tinker::class_inh<Classengine,ClassBase>(m_LuaState);
			}
#else
			lua_tinker::class_inh<Classengine,ClassBase>(m_LuaState);
#endif
		}

		template<typename Class >//�๹�죬�ṩ��lua�й�����,���캯��ֻ����һ��
		BOHGE_FORCEINLINE void RegClassCons()
		{
#ifdef LUA_DEBUG_MODE
			if ( !m_isError && m_isLoaded )
			{
				Engine::Instance().GetScriptManage()->OnScriptErrorEvent( m_OnErrorFunc );
				lua_tinker::class_con<Class>( m_LuaState, lua_tinker::constructor<Class> );
			}
#else
			lua_tinker::class_con<Class>( m_LuaState, lua_tinker::constructor<Class> );
#endif
		}

		template<typename Class, typename T1>//�๹�죬�ṩ��lua�й�����,���캯��ֻ����һ��
		BOHGE_FORCEINLINE void RegClassCons()
		{
#ifdef LUA_DEBUG_MODE
			if ( !m_isError && m_isLoaded )
			{
				Engine::Instance().GetScriptManage()->OnScriptErrorEvent( m_OnErrorFunc );
				lua_tinker::class_con<Class>( m_LuaState, lua_tinker::constructor<Class, T1> );
			}
#else
			lua_tinker::class_con<Class>( m_LuaState, lua_tinker::constructor<Class, T1> );
#endif
		}

		template<typename Class, typename T1, typename T2>
		BOHGE_FORCEINLINE void RegClassCons()
		{
#ifdef LUA_DEBUG_MODE
			if ( !m_isError && m_isLoaded )
			{
				Engine::Instance().GetScriptManage()->OnScriptErrorEvent( m_OnErrorFunc );
				lua_tinker::class_con<Class>( m_LuaState, lua_tinker::constructor<Class, T1, T2> );
			}
#else
			lua_tinker::class_con<Class>( m_LuaState, lua_tinker::constructor<Class, T1, T2> );
#endif
		}

		template<typename Class, typename T1, typename T2, typename T3>
		BOHGE_FORCEINLINE void RegClassCons()
		{
#ifdef LUA_DEBUG_MODE
			if ( !m_isError && m_isLoaded )
			{
				Engine::Instance().GetScriptManage()->OnScriptErrorEvent( m_OnErrorFunc );
				lua_tinker::class_con<Class>( m_LuaState, lua_tinker::constructor<Class, T1, T2, T3> );
			}
#else
			lua_tinker::class_con<Class>( m_LuaState, lua_tinker::constructor<Class, T1, T2, T3> );
#endif
		}

		template<typename Class, typename T1, typename T2, typename T3, typename T4>
		BOHGE_FORCEINLINE void RegClassCons()
		{
#ifdef LUA_DEBUG_MODE
			if ( !m_isError && m_isLoaded )
			{
				Engine::Instance().GetScriptManage()->OnScriptErrorEvent( m_OnErrorFunc );
				lua_tinker::class_con<Class>( m_LuaState, lua_tinker::constructor<Class, T1, T2, T3, T4> );
			}
#else
			lua_tinker::class_con<Class>( m_LuaState, lua_tinker::constructor<Class, T1, T2, T3, T4> );
#endif
		}

		template<typename Class, typename T1, typename T2, typename T3, typename T4, typename T5>
		BOHGE_FORCEINLINE void RegClassCons()
		{
#ifdef LUA_DEBUG_MODE
			if ( !m_isError && m_isLoaded )
			{
				Engine::Instance().GetScriptManage()->OnScriptErrorEvent( m_OnErrorFunc );
				lua_tinker::class_con<Class>( m_LuaState, lua_tinker::constructor<Class, T1, T2, T3, T4, T5> );
			}
#else
			lua_tinker::class_con<Class>( m_LuaState, lua_tinker::constructor<Class, T1, T2, T3, T4, T5> );
#endif
		}

		template<typename Class, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
		BOHGE_FORCEINLINE void RegClassCons()
		{
#ifdef LUA_DEBUG_MODE
			if ( !m_isError && m_isLoaded )
			{
				Engine::Instance().GetScriptManage()->OnScriptErrorEvent( m_OnErrorFunc );
				lua_tinker::class_con<Class>( m_LuaState, lua_tinker::constructor<Class, T1, T2, T3, T4, T5, T6> );
			}
#else
			lua_tinker::class_con<Class>( m_LuaState, lua_tinker::constructor<Class, T1, T2, T3, T4, T5, T6> );
#endif
		}

		template<typename Class, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
		BOHGE_FORCEINLINE void RegClassCons()
		{
#ifdef LUA_DEBUG_MODE
			if ( !m_isError && m_isLoaded )
			{
				Engine::Instance().GetScriptManage()->OnScriptErrorEvent( m_OnErrorFunc );
				lua_tinker::class_con<Class>( m_LuaState, lua_tinker::constructor<Class, T1, T2, T3, T4, T5, T6, T7> );
			}
#else
			lua_tinker::class_con<Class>( m_LuaState, lua_tinker::constructor<Class, T1, T2, T3, T4, T5, T6, T7> );
#endif
		}

		template<typename Class, typename F>//ע�����Ա����
		void RegClassFunc(const char* name, F func)//���ﲻ�ܼ�inline��ӦΪ�ᵼ��inline�ĵݹ飬gcc������
		{
#ifdef LUA_DEBUG_MODE
			if ( !m_isError && m_isLoaded )
			{
				Engine::Instance().GetScriptManage()->OnScriptErrorEvent( m_OnErrorFunc );
				lua_tinker::class_def<Class>(m_LuaState, name, func);
			}
#else
			lua_tinker::class_def<Class>(m_LuaState, name, func);
#endif
		}

		template<typename Class, typename BASE, typename VAR>//ע�����Ա����
		BOHGE_FORCEINLINE void RegClassVar(const char* name, VAR BASE::*val)
		{
#ifdef LUA_DEBUG_MODE
			if ( !m_isError && m_isLoaded )
			{
				Engine::Instance().GetScriptManage()->OnScriptErrorEvent( m_OnErrorFunc );
				lua_tinker::class_mem<Class>(m_LuaState, name, val);
			}
#else
			lua_tinker::class_mem<Class>(m_LuaState, name, val);
#endif
		}

	public:
		// Call Lua Func {
		template <typename R>
		BOHGE_FORCEINLINE R CallLua(const char *func)
		{
#ifdef LUA_DEBUG_MODE
			if ( !m_isError && m_isLoaded )
			{
				Engine::Instance().GetScriptManage()->OnScriptErrorEvent( m_OnErrorFunc );
				return lua_tinker::call<R>(m_LuaState, func);
			}
#else
			return lua_tinker::call<R>(m_LuaState, func);
#endif
		}

		template <typename R, typename T1>
		BOHGE_FORCEINLINE R CallLua(const char *func, const T1& t1)
		{
#ifdef LUA_DEBUG_MODE
			if ( !m_isError && m_isLoaded )
			{
				Engine::Instance().GetScriptManage()->OnScriptErrorEvent( m_OnErrorFunc );
				return lua_tinker::call<R, T1>(m_LuaState, func, t1);
			}
#else
			return lua_tinker::call<R, T1>(m_LuaState, func, t1);
#endif
		}

		template <typename R, typename T1, typename T2>
		BOHGE_FORCEINLINE R CallLua(const char *func, const T1& t1,  const T2& t2)
		{
#ifdef LUA_DEBUG_MODE
			if ( !m_isError && m_isLoaded )
			{
				Engine::Instance().GetScriptManage()->OnScriptErrorEvent( m_OnErrorFunc );
				return lua_tinker::call<R, T1, T2>(m_LuaState, func, t1, t2);
			}
#else
			return lua_tinker::call<R, T1, T2>(m_LuaState, func, t1, t2);
#endif
		}

		template <typename R, typename T1, typename T2, typename T3>
		BOHGE_FORCEINLINE R CallLua(const char *func, const T1& t1,  const T2& t2, const T3& t3)
		{
#ifdef LUA_DEBUG_MODE
			if ( !m_isError && m_isLoaded )
			{
				Engine::Instance().GetScriptManage()->OnScriptErrorEvent( m_OnErrorFunc );
				return lua_tinker::call<R, T1, T2, T3>(m_LuaState, func, t1, t2, t3);
			}
#else
			return lua_tinker::call<R, T1, T2, T3>(m_LuaState, func, t1, t2, t3);
#endif
		}

		template <typename R, typename T1, typename T2, typename T3, typename T4>
		BOHGE_FORCEINLINE R CallLua(const char *func, const T1& t1,  const T2& t2, const T3& t3, const T4& t4)
		{
#ifdef LUA_DEBUG_MODE
			if ( !m_isError && m_isLoaded )
			{
				Engine::Instance().GetScriptManage()->OnScriptErrorEvent( m_OnErrorFunc );
				return lua_tinker::call<R, T1, T2, T3, T4>(m_LuaState, func, t1, t2, t3, t4);
			}
#else
			return lua_tinker::call<R, T1, T2, T3, T4>(m_LuaState, func, t1, t2, t3, t4);
#endif
		}

		template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
		BOHGE_FORCEINLINE R CallLua(const char *func, const T1& t1,  const T2& t2, const T3& t3, const T4& t4, const T5& t5)
		{
#ifdef LUA_DEBUG_MODE
			if ( !m_isError && m_isLoaded )
			{
				Engine::Instance().GetScriptManage()->OnScriptErrorEvent( m_OnErrorFunc );
				return lua_tinker::call<R, T1, T2, T3, T4, T5>(m_LuaState, func, t1, t2, t3, t4, t5);
			}
#else
			return lua_tinker::call<R, T1, T2, T3, T4, T5>(m_LuaState, func, t1, t2, t3, t4, t5);
#endif
		}

		template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
		BOHGE_FORCEINLINE R CallLua(const char *func, const T1& t1,  const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6)
		{
#ifdef LUA_DEBUG_MODE
			if ( !m_isError && m_isLoaded )
			{
				Engine::Instance().GetScriptManage()->OnScriptErrorEvent( m_OnErrorFunc );
				return lua_tinker::call<R, T1, T2, T3, T4, T5, T6>(m_LuaState, func, t1, t2, t3, t4, t5, t6);
			}
#else
			return lua_tinker::call<R, T1, T2, T3, T4, T5, T6>(m_LuaState, func, t1, t2, t3, t4, t5, t6);
#endif
		}

		template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
		BOHGE_FORCEINLINE R CallLua(const char *func, const T1& t1,  const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7)
		{
#ifdef LUA_DEBUG_MODE
			if ( !m_isError && m_isLoaded )
			{
				Engine::Instance().GetScriptManage()->OnScriptErrorEvent( m_OnErrorFunc );
				return lua_tinker::call<R, T1, T2, T3, T4, T5, T6, T7>(m_LuaState, func, t1, t2, t3, t4, t5, t6, t7);
			}
#else
			return lua_tinker::call<R, T1, T2, T3, T4, T5, T6, T7>(m_LuaState, func, t1, t2, t3, t4, t5, t6, t7);
#endif
		}

		template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
		BOHGE_FORCEINLINE R CallLua(const char *func, const T1& t1,  const T2& t2, const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8)
		{
#ifdef LUA_DEBUG_MODE
			if ( !m_isError && m_isLoaded )
			{
				Engine::Instance().GetScriptManage()->OnScriptErrorEvent( m_OnErrorFunc );
				return lua_tinker::call<R, T1, T2, T3, T4, T5, T6, T7, T8>(m_LuaState, func, t1, t2, t3, t4, t5, t6, t7, t8);
			}
#else
			return lua_tinker::call<R, T1, T2, T3, T4, T5, T6, T7, T8>(m_LuaState, func, t1, t2, t3, t4, t5, t6, t7, t8);
#endif
		}
		// Call lua Func End}
	};

	////һ���໥���õ�����
	//Test.lua
	//
	//function add(x, y)
	//return x + y
	//end
	//
	//function diff(x,y)
	//return State:diff(x,y)
	//end
	//
	//c++
	//engine.GetScriptManage()->RegClass<StateMainMenu>( "StateMainMenu" ); //ע��һ���࣬ʵ����table
	//engine.GetScriptManage()->RegClassFunc<StateMainMenu>( "diff", &StateMainMenu::diff );//ע��table��Ա
	//engine.GetScriptManage()->Dofile( "Test.lua" );//����lua
	//engine.GetScriptManage()->SetGlobal<StateMainMenu>( "State", this );//����ȫ�ֱ����� <StateMainMenu>����ʡ��
	//int a = engine.GetScriptManage()->CallLua<int>( "add", 1,5 );//���ú���
	//int d = engine.GetScriptManage()->CallLua<int>( "diff", 1,5 );

}