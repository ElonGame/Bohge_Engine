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



//////////////////////////////////////
//			�������ݿ�����		   //
/////////////////////////////////////

#pragma once
#include "Base.h"
#include <string>
#include <ICRSINT.H>//ado ���ݿ�����
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF")rename("BOF","adoBOF")





class SQLDataBase
{
private:
	static bool			m_isCoInitialize;
	bool				m_isConnect;
	bool				m_isQuery;//��ѯ
	bool				m_isInsert;//����
	bool				m_isChange;//�ı�
	_ConnectionPtr		m_pConnect;
	_RecordsetPtr		m_pRecordest;
public:
	SQLDataBase( const std::string& connectStr );
	~SQLDataBase(void);
public:
	static void COMInitializeEx();//����ǰ�����ʼ��SQL��com���û���,���̵߳��ã�ÿ�������̵߳���һ��
	static void COMUninitialize();
public:
	inline void BeginQuery( const std::string & source )//��ʼ��ѯ
	{
		ASSERT( m_isQuery == false );
		m_isQuery = true;
		m_isChange = false;
		try{
			m_pRecordest->Open( source.c_str(), m_pConnect.GetInterfacePtr(), adOpenDynamic,adLockOptimistic,adCmdUnspecified);
		}
		catch(_com_error e)
		{
			m_isQuery = false;
			ShowMessage(_T("SQL BeginQuery Erorr source is %s ------- error code %s"), e.Description(), source);
		}
	}
	inline bool hasRecord()
	{
		if( m_isQuery )
		{
			return !m_pRecordest->adoEOF;
		}
		return false;
	}
	inline _variant_t GetValue( const _variant_t & Index )
	{
		if( m_isQuery )
		{
			return m_pRecordest->Fields->GetItem(Index)->Value;
		}
		return 0;
	}
	inline void SetValue( const _variant_t & Index, const _variant_t& v )
	{
		if( m_isQuery )
		{
			m_isChange = true;
			m_pRecordest->Fields->GetItem(Index)->Value = v;
		}
	}
	inline void AddNewRecord()
	{
		m_isInsert = true;
		m_pRecordest->AddNew();///����¼�¼
	}
	inline void InsertValue( const _variant_t & Index, _variant_t& v )
	{
		if( m_isInsert )
		{
			m_pRecordest->PutCollect( Index, v);
		}
	}
	inline void End( )//ȫ���Ľ���
	{
		if( m_isChange || m_isInsert )
		{
			m_pRecordest->Update();
			m_pRecordest->Close();
		}
		else if( m_isQuery )
		{
			m_pRecordest->Close();
		}
		m_isQuery = false;
		m_isChange = false;
		m_isInsert = false;
	}
};
