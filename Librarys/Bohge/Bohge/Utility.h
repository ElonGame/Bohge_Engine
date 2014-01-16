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



////////////////////
//	һЩ����	 //
///////////////////
#pragma once
#include "3DMath.h"

#include <string>
#include <vector>
#include <list>

namespace Utility
{
	//------------------------------------------------------------------------------------------------------------
	template<typename T>
	bool isInclude( const std::list<T>& lhs, const T& rhs )
	{
		typedef typename  std::list< T >::const_iterator ListCIT;
		for ( ListCIT it = lhs.begin();
			it != lhs.end();
			it ++ )
		{
			if( (*it) == rhs )
			{
				return true;
			}
		}
		return false;
	}
	//------------------------------------------------------------------------------------------------------------
	static unsigned int HashCode( const char * str )
	{
		unsigned int len = strlen(str);
		unsigned int hash = 0;
		for(size_t i = 0; i < len; ++i) 
			hash = 65599 * hash + str[i];
		return hash ^ (hash >> 16);
	}
	//------------------------------------------------------------------------------------------------------------
	static unsigned int HashCode( const std::string& str )
	{
		unsigned int hash = 0;
		for(size_t i = 0; i < str.size(); ++i) 
			hash = 65599 * hash + str[i];
		return hash ^ (hash >> 16);
	}
	//------------------------------------------------------------------------------------------------------------
	static void RemoveComment( const std::string& comsig, std::string& str )//ȥ����comsig��ͷ��ע�Ͷ�
	{
		int comment = 0;
		do 
		{
			comment = str.find( comsig );
			if ( std::string::npos != comment )
			{
				int end = str.find( "\r\n", comment );
				end = std::string::npos == end ? str.size() : end;
				str.erase( comment, end - comment );
			}
		} while ( std::string::npos != comment );
	}
	//------------------------------------------------------------------------------------------------------------
	static void RemoveTargetString( std::string& str, const std::string& target )
	{
		do
		{
			int begin = str.find( target );
			if ( -1 == begin )
			{
				break;
			}
			else
			{
				str.erase( begin, target.size() );
			}
		}while(true);
	}
	//------------------------------------------------------------------------------------------------------------
	static std::string RemoveTargetString( const std::string& str, const std::string& target )
	{
		std::string loacl = str;
		do
		{
			int begin = loacl.find( target );
			if ( -1 == begin )
			{
				break;
			}
			else
			{
				loacl.erase( begin, target.size() );
			}
		}while(true);
		return loacl;
	}
	//------------------------------------------------------------------------------------------------------------
	static void FindIncludes( std::vector< std::string >& includes, std::string& input )//����include����include���ļ�push�������У�ɾ��ԭ���ı��е�include�����﷨��
	{
		int include = 0;
		do 
		{
			include = input.find( "#include" );
			if ( std::string::npos != include )
			{
				int ifb = input.find( "\"", include );
				int ife = input.find( "\"", ifb+1 );
				std::string includefilename = input.substr( ifb+1, ife-ifb-1 );
				includes.push_back( includefilename );
				input.erase( include, ife - include + 1 );
			}
		} while ( std::string::npos != include );
	}
	//------------------------------------------------------------------------------------------------------------
	static bool ReplaceString(std::string& str, const std::string& from, const std::string& to) 
	{
		size_t start_pos = str.find(from);
		if(start_pos == std::string::npos)
			return false;
		str.replace(start_pos, from.length(), to);
		return true;
	}
	//------------------------------------------------------------------------------------------------------------
	static void ReplaceAllString(std::string& str, const std::string& from, const std::string& to) 
	{
		size_t start_pos = 0;
		do 
		{
			start_pos = str.find(from, start_pos);
			if( start_pos != std::string::npos )
			{
				str.replace(start_pos, from.length(), to);
				start_pos += to.size();//�����ظ��滻�Լ�
			}
		} while ( start_pos != std::string::npos );
	}
	//------------------------------------------------------------------------------------------------------------
	static bool NotVariablePart( char c )//�Ƿ��ǺϷ��ı���������һ����
	{
		if ( ( c >= 65 && c <= 90 ) //A~Z
			|| ( c >= 97 && c <= 122 )//a~z
			|| ( c >= 48 && c <= 57 )//0~9
			|| c == '_'
			)
		{
			return false;
		}
		return true;
	}
	//------------------------------------------------------------------------------------------------------------
	static void ReplaceAllParamterName(std::string& str, const std::string& from, const std::string& to, size_t begin = 0)//�޸�ȫ���Ĳ���������ģ���Ҫ�ж��ǲ��ǲ���,��Ҫ��ȫƥ��
	{
		size_t start_pos = begin;
		do 
		{
			start_pos = str.find(from, start_pos);
			if( start_pos != std::string::npos )
			{
				char first = start_pos == 0 ? ' ' : str[ start_pos - 1 ];
				char last = str[ start_pos + from.size() ];
				if ( NotVariablePart(last) && NotVariablePart(first) )
				{
					str.replace(start_pos, from.length(), to);
				}
				start_pos += to.size();//�����ظ��滻�Լ�
			}
		} while ( start_pos != std::string::npos );
	}
	//------------------------------------------------------------------------------------------------------------
	static void SeparateAllString( const std::string& begin, const std::string& end,//�﷨�εĿ�ʼ����
		std::vector< std::string >& parts, std::string& func )//����ĺ��������飬����������ı�
	{
		int define;
		do
		{
			define = func.find( begin );
			if ( std::string::npos != define )
			{
				int endparts = func.find( end );
				parts.push_back( func.substr( define, endparts - define + end.size() ) );
				func.erase( define, endparts - define + end.size() );
			}
		} while ( std::string::npos != define );
	}
	//------------------------------------------------------------------------------------------------------------
	static void SeparateString( const std::string& begin, const std::string& end,//�﷨�εĿ�ʼ����
		std::string& out, std::string& func )//����ĺ��������飬����������ı�
	{
		int define = func.find( begin );
		if ( std::string::npos != define )
		{
			int endparts = func.find( end );
			out = func.substr( define, endparts - define + end.size() );
			func.erase( define, endparts - define + end.size() );
		}
	}
	//------------------------------------------------------------------------------------------------------------
	static bool MacroName( const std::string& macro, const std::string& func, std::string& name )//�õ����ʵ��
	{
		int define = func.find( macro );
		if ( std::string::npos != define )
		{
			int firstspace = func.find_first_of( ' ', define );//�ո�ʼ
			int lastspace = func.find_first_not_of( ' ', firstspace );//������ʼ
			int funcend = func.find_first_of( " \r\n", lastspace );//��������
			name = func.substr( lastspace, funcend - lastspace );
			return true;
		}
		return false;
	}
	//------------------------------------------------------------------------------------------------------------
	static void MakeLuaFunction( const std::string& begin, const std::string& end,//�﷨�εĿ�ʼ����
		std::string& func )//�����ʺ�shader�﷨��
	{
		int define = func.find( begin );
		if ( std::string::npos != define )
		{
			int firstspace = func.find_first_of( ' ', define );//�ո�ʼ
			int lastspace = func.find_first_not_of( ' ', firstspace );//������ʼ
			int funcend = func.find_first_of( " \r\n", lastspace );//��������
			//���ɺ���
			func.insert( funcend, "()" );
			ReplaceString( func, end, "end" );
			ReplaceString( func, begin, "function" );
		}
	}
	//------------------------------------------------------------------------------------------------------------
	static void MakeLuaFunction( const std::string& begin, const std::string& end,//�﷨�εĿ�ʼ����
		std::string& funcname, std::string& func )//�����ʺ�shader�﷨�θĳ�lua����
	{
		int define = func.find( begin );
		if ( std::string::npos != define )
		{
			int firstspace = func.find_first_of( ' ', define );//�ո�ʼ
			int lastspace = func.find_first_not_of( ' ', firstspace );//������ʼ
			int funcend = func.find_first_of( " \r\n", lastspace );//��������
			funcname = func.substr( lastspace, funcend - lastspace );

			//���ɺ���
			func.insert( funcend, "()" );
			ReplaceString( func, end, "end" );
			ReplaceString( func, begin, "function" );
		}
	}
	//------------------------------------------------------------------------------------------------------------
	static uint CaculateRefreshLimit( float dis, float boxdiag )
	{
		float param = boxdiag / dis;
		return 1000 / (5 + 25 * BohgeEngine::Math::Clamp( param * 5.0f, 0.0f, 1.0f ) );
	}
}