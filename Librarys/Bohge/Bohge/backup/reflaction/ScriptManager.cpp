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
//		Lua�ű�����	   //
/////////////////////////////
#include "ScriptManager.h"
#include "lua.hpp"
#include "FileSystem.h"
#include "Log.h"
#include <string>

using namespace std;


//--------------------------------------------------------------------------------------------------------------------------------------
ScriptManager::ScriptManager(void)
{
	m_Lua = luaL_newstate();
	luaL_openlibs (m_Lua);//��ȫ���Ŀ�
	//luaopen_base( m_Lua );//������
	//luaopen_coroutine( m_Lua );//�߳̿�
	//luaopen_table( m_Lua );//����
	//luaopen_io( m_Lua );//io��
	//luaopen_os( m_Lua );//����ϵͳ��صģ�ʱ�䰡�ȵ�
	//luaopen_string( m_Lua );//�ַ���
	//luaopen_bit32( m_Lua );//32λ
	//luaopen_math( m_Lua );//��ѧ
	//luaopen_debug( m_Lua );//����
	//luaopen_package( m_Lua );//��װ��
	/*{
		int check = luaL_loadfile(m_Lua, File::Instance().MakeFilePath("Test.lua").c_str() );
		if ( check == 0 )  
		{  
			check = lua_pcall(m_Lua, 0, LUA_MULTRET, 0);  
			if (check != 0)  
			{  
				_PrintLuaError(check);  
			} 
		}  
		else  
		{  
			_PrintLuaError(check);  
		} 

		check = luaL_loadfile(m_Lua, File::Instance().MakeFilePath("Test2.lua").c_str() );
		if ( check == 0 )  
		{  
			check = lua_pcall(m_Lua, 0, LUA_MULTRET, 0);  
			if (check != 0)  
			{  
				_PrintLuaError(check);  
			} 
		}  
		else  
		{  
			_PrintLuaError(check);  
		}

		Add(10, 15);
	} */
}
//--------------------------------------------------------------------------------------------------------------------------------------
ScriptManager::~ScriptManager(void)
{
	lua_close( m_Lua );
}

int ScriptManager::Add( int x, int y )
{
	int sum;
	lua_getglobal(m_Lua, "add");
	lua_pushnumber(m_Lua, x);
	lua_pushnumber(m_Lua, y);
	lua_call(m_Lua, 2, 1);
	sum = (int)lua_tonumber(m_Lua, -1);
	lua_pop(m_Lua, 1);
	return sum;
}

//--------------------------------------------------------------------------------------------------------------------------------------
void ScriptManager::_PrintLuaError( int sErr )
{
	if (sErr==0)  
	{  
		return;  
	}  
	const char* error;  
    string sErrorType;
	switch(sErr)  
	{  
	case LUA_ERRSYNTAX://����ʱ����  
		/*const char *buf = "mylib.myfun()2222";���������������������ʱ����*/  
		sErrorType += string("Syntax error during pre-compilation\n");  
		break;  
	case LUA_ERRMEM://�ڴ����  
		sErrorType += string("Memory allocation error\n");  
		break;  
	case LUA_ERRRUN://����ʱ����  
		/*const char *buf = "my222lib.myfun()";����������������������ʱ����my222libʵ���ϲ����������Ŀ⣬���ص�ֵ��nil*/  
		sErrorType += string("A runtime error\n");  
		break;  
	case LUA_YIELD://�̱߳��������  
		sErrorType += string("Thread has Suspended\n");  
		break;  
	case LUA_ERRERR://�ڽ��д�����ʱ��������  
		sErrorType += string("Error while running the error handler function\n");  
		break;  
	default:  
		break;  
	}  
	error = lua_tostring(m_Lua, -1);//��ӡ������  
	DEBUGLOG("%s:%s\n",sErrorType.c_str(),error);  
	lua_pop( m_Lua, 1);   
}
