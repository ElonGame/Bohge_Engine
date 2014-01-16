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



//////////////////////////
//		��������		//
//////////////////////////
#pragma once
#include "Predefine.h"

#include <map>
#include <string>

#define BOHGE_STRING_EXPAND ".str"

namespace BohgeEngine
{
#define  STRING LanguageControl::Instance()
	class LanguageControl
	{
	public:
		enum LanguageType
		{
			L_Chinese,
			L_English,
			L_Count,
		};
	private:
		typedef std::map< int , std::wstring >	WstringMap;
	private:
		static LanguageControl*		m_psSelf;
		std::string					m_CurrentPackage;
		std::string					m_FolderPathName;
		LanguageType				m_eLanguage;
		WstringMap					m_WstringMap;
	private:
		LanguageControl();
		~LanguageControl();
		void _LanguageRootPath();
		std::wstring _LoadString( const std::string& path );
	public:
		void Setlanguage( LanguageType l );
		void LoadPackage( );//��ȡ��ǰ��Ŀ¼�µ�ȫ���ı�
		void LoadPackage( const char* dir );//��ȡĿ¼�µ�ȫ���ı�
		std::wstring GetFromatString( const char* filename, ...);//ע��ƻ��ϵͳ��...ֻ֧�ֹؼ���
		std::wstring GetString( const char* filename );
	public:
		static BOHGE_FORCEINLINE void Create()
		{
			ASSERT( NULL == m_psSelf );
			m_psSelf = NEW LanguageControl;
		}
		static BOHGE_FORCEINLINE void Destroy()
		{
			SAFE_DELETE( m_psSelf );
		}
		static BOHGE_FORCEINLINE LanguageControl& Instance()
		{
			return *m_psSelf;
		}
		BOHGE_FORCEINLINE LanguageType GetLanguage()
		{
			return m_eLanguage;
		}
	};

}