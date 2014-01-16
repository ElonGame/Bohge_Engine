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
//		shader��		   //
/////////////////////////
#include "ShaderSegment.h"
#include "ShaderLibrary.h"
#include "Utility.h"
#include "TextureState.h"


using namespace std;


static string SHADER_CODE("#SHADER_CODE");
static string END_CODE("#END_CODE");
static string SHADER_DEFINE("#SHADER_DEFINE");
static string END_DEFINE("#END_DEFINE");
static const char* FUNCTION_INPUT = "FUNCTION_INPUT";
static const char* FUNCTION_OUTPUT = "FUNCTION_OUTPUT";


namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	ShaderSegment::ShaderSegment( const std::string& name, const std::string& code )
		:m_isOriginal(true),
		m_SegmentName(name),
		m_Function(code)//��ʱ�洢
	{
	}
	//-------------------------------------------------------------------------------------------------------
	ShaderSegment::ShaderSegment( const ShaderSegment& ss )
	{
		_Docopy( ss );
	}
	//-------------------------------------------------------------------------------------------------------
	ShaderSegment::~ShaderSegment(void)
	{
		ReleaseResource();
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderSegment::ReleaseResource()
	{
		FOR_EACH( ParameterMap, it, m_InputParameter )
		{
			SAFE_DELETE( it->second );
		}
		FOR_EACH( ParameterMap, it, m_OutputParameter )
		{
			SAFE_DELETE( it->second );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderSegment::_ReadParameter( ShaderLibrary& host, ParameterMap& into, const char* which )
	{
		LuaTable input = host.GetGlobal<LuaTable>( which );
		for ( int i = 1; i <= input.size(); i ++  )
		{
			LuaTable param = input.get<LuaTable>(i);
			ShaderParameter::InputsType type = param.get<ShaderParameter::InputsType>(1);
			ASSERT( ShaderParameter::IT_ERRORCODE != type );
			into.insert( 
				make_pair( 
					type, 
					NEW ShaderParameter( type,
						param.get<ShaderParameter::FloatPrecision>(2),
						param.get<ShaderParameter::VariablesType>(3),
						param.get<char*>(4) )
						)
					);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderSegment::ProcessSegment( ShaderLibrary& host )
	{
		m_isOriginal = false;
		Utility::SeparateString( SHADER_CODE, END_CODE, m_SegmentCode, m_Function );
		Utility::ReplaceString( m_SegmentCode, SHADER_CODE, "" );
		Utility::ReplaceString( m_SegmentCode, END_CODE, "" );
		Utility::MakeLuaFunction( SHADER_DEFINE, END_DEFINE, m_SegmentName, m_Function );
		host.DoScript( m_Function );
		host.CallLua<void>( m_SegmentName.c_str() );
		_ReadParameter( host, m_InputParameter, FUNCTION_INPUT );//�������
		_ReadParameter( host, m_OutputParameter, FUNCTION_OUTPUT );//�������
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderSegment::RenameInputParameter( const ShaderParameter::InputsType& ty, const std::string& to )
	{
		ParameterMap::iterator it = m_InputParameter.find( ty );
		if ( m_InputParameter.end() != it )
		{
			Utility::ReplaceAllParamterName( m_SegmentCode, it->second->GetName(), to );
			it->second->SetName( to );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderSegment::RenameOutputParameter( const ShaderParameter::InputsType& ty, const std::string& to )
	{
		ParameterMap::iterator it = m_OutputParameter.find( ty );
		if ( m_OutputParameter.end() != it )
		{
			Utility::ReplaceAllParamterName( m_SegmentCode, it->second->GetName(), to );
			it->second->SetName( to );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	bool ShaderSegment::isNeedInput( const ShaderParameter::InputsType& ty )
	{
		ParameterMap::iterator it = m_InputParameter.find( ty );
		if ( m_InputParameter.end() != it )
		{
			return true;
		}
		return false;
	}
	//-------------------------------------------------------------------------------------------------------
	bool ShaderSegment::isProvider( const ShaderParameter::InputsType& ty )
	{
		ParameterMap::iterator it = m_OutputParameter.find( ty );
		if ( m_OutputParameter.end() != it )
		{
			return true;
		}
		return false;
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderSegment::operator=( const ShaderSegment& ss )
	{
		_Docopy( ss );
	}
	//-------------------------------------------------------------------------------------------------------
	void ShaderSegment::_Docopy( const ShaderSegment& ss )
	{
		m_isOriginal = ss.m_isOriginal;
		m_SegmentName = ss.m_SegmentName;
		m_SegmentCode = ss.m_SegmentCode;
		//m_Function = ss.m_Function; //�������ˣ���ߵ�Ч��
		FOR_EACH_CONST( ParameterMap, cit, ss.m_InputParameter )
		{
			m_InputParameter.insert( make_pair( cit->first, NEW ShaderParameter( *cit->second ) ) );//ShaderParameterû��ָ������ʹ�õ���ϵͳ�ṩ��Ĭ�ϵĿ�������
		}
		FOR_EACH_CONST( ParameterMap, cit, ss.m_OutputParameter )
		{
			m_OutputParameter.insert( make_pair( cit->first, NEW ShaderParameter( *cit->second ) ) );
		}
	}
}