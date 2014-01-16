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
//		  ����ı�		   //
/////////////////////////////
#pragma once

#include "3DMath.h"
#include <string>
#include <list>



struct FT_LibraryRec_;
typedef struct FT_LibraryRec_  *FT_Library;
struct FT_FaceRec_;
typedef struct FT_FaceRec_*  FT_Face;
struct FT_GlyphRec_;
typedef struct FT_GlyphRec_*  FT_Glyph;

namespace BohgeEngine
{
	class StringManager;
	class Texture2D;
	class VertexTextureColor;
	class RendBuffer;
	class Engine;
	class VertexBuffer;
	class IndicesBuffer;
	//����ÿ�������ʾ��ͼ���������ڶ�ά�����
	//		y
	//		|
	//		|
	//-x----0---2-----x
	//		|   |
	//		1---3
	//		|
	//		-y

	class BString	//�ı���ӡ
	{
	public:
		enum Alignment
		{
			Center,
			Left,
			LeftTop,
			LeftBottom,
			Right,
			RightTop,
			RightBottom,
		};
		enum StringConst
		{
			COUNT_NODE			= 1,
			COUNT_OF_VERTEX		= 4,
			COUNT_OF_INDIES		= 4,
		};
	private:
		friend class StringManager;
	private:
		StringManager*					m_Father;
		bool							m_isManaged;//�����true��������ƣ������Լ�����
		Alignment						m_eAlign;
		vector2f						m_fScale;
		std::list<BString*>::iterator	m_Iterator;//��������Ϊ�˿���ɾ��
		bool							m_isShow;//�Ƿ����
		vector2f						m_ScreenFontSize;//����ʱ��Ҫ��ĳߴ�
		vector2d						m_StringTextureSize;//�ַ��������ؿ��
		int								m_MaxHeight;//�и�
		vector2f						m_Size;//�ı����������Ļ���صĴ�С[0,1]���䣬��Ӧ��������ϵ
		vector2f						m_Pos;//���Ƶص㣻
		VertexTextureColor*				m_pVertex;//��������
		RendBuffer*						m_pRendBuffer;
		Texture2D*						m_pTex;//�ı�����
		std::wstring					m_String;

	private:
		BString(StringManager* father, const std::wstring& wstr, int height, const vector2f& screensize, const vector2d& txtSize, const Color& clr, RendBuffer* buffer, const vector2d& texSize, const byte* pBuffer );
		~BString();
	public:
		void Render(Engine & engine);//ʹ��Ĭ��λ�û���
		void SetColor(const Color& clr);
		void SetScale(const vector2f& scale);
		vector4f GetStringCoordnate( const std::wstring& wstr );//�õ�ĳһ���ַ������ַ��������е����꣬xy��lt��zw��rb
	public:
		BOHGE_FORCEINLINE Texture2D& GetTexture()
		{
			return *m_pTex;
		}
		BOHGE_FORCEINLINE const vector2f& GetSize() const
		{
			return m_Size;
		}
		BOHGE_FORCEINLINE void SetIterator(const std::list<BString*>::iterator& it)
		{
			m_Iterator = it;
		}
		BOHGE_FORCEINLINE void SetPosition(const vector2f& pos, Alignment ali)
		{
			m_eAlign = ali;
			vector2f add;
			switch( ali )
			{
			case Center: add = m_Size/2.0f; add.m_x = -add.m_x; break;
			case Left: add = vector2f(0,m_Size.m_y/2.0f); break;
			case LeftTop: add = vector2f(0,0); break;
			case LeftBottom: add = vector2f(0,m_Size.m_y); break;
			case Right: add = vector2f(m_Size.m_x,m_Size.m_y/2.0f) /*  / 2.0f * 2.0f  */; add.m_x = -add.m_x; break;
			case RightTop: add = vector2f(m_Size.m_x,0) /*  / 2.0f * 2.0f  */; add.m_x = -add.m_x; break;
			case RightBottom: add = m_Size /*  / 2.0f * 2.0f  */; add.m_x = -add.m_x; break;
			}
			m_Pos = pos + add;
		}
		BOHGE_FORCEINLINE void Hide()
		{
			m_isShow = false;
		}
		BOHGE_FORCEINLINE void Show()
		{
			m_isShow = true;
		}
		BOHGE_FORCEINLINE void SetManaged(bool b)
		{
			m_isManaged = b;
		}
		BOHGE_FORCEINLINE bool isManaged()
		{
			return m_isManaged;
		}
	};



	/////////////////////////
	//		  ����		   //
	/////////////////////////
	//�ı��е�#�������ǻ���
	class StringManager//�ı�����������
	{
	private:
		class BChar //��ȡ��ÿһ��char
		{
		public:
			FT_Glyph	m_pGlyph;
			vector2d	m_PixelSize;
			int			m_nBeginY;
			bool		m_isSpace;//�Ƿ��ǿո�
			BChar(int beginy, const vector2d& size, FT_Glyph glyph, bool isspace );
			~BChar();
		};
		typedef std::list<RendBuffer*> RendBufferList;//BString��rendbuffer��StringManager����
		typedef std::list<BChar*> BCharList;//��ȡÿһ��string�е�charʹ��
		typedef std::list<BString*> BStringList;//��ȡ������ÿһ��string
	private:
		bool				m_isLoad;
		vector2f			m_FaceSize;//Ĭ�ϵ������С
		BStringList			m_StringList;//�Ѿ����ص�ȫ�����ı�
		FT_Library			m_FT2Lib;
		FT_Face				m_FT_Face;
		RendBufferList		m_FreeBufferList;//���е�buffer���Ͳ���Ҫ������
	private:
		BChar* _LoadChar(wchar_t wchar);
		BString* _InternalMakeString(bool isScale, const vector2f& fontsize, const Color& clr, const std::wstring& wstr);//����װ��һ���ı�������һ���ɴ�ӡ���ַ���
		vector2d _InternalGetStringSize(bool isScale, const vector2f& fontsize, const std::wstring& wstr);//����װ��һ���ı�������һ���ɴ�ӡ���ַ���
		RendBuffer* _AllocBuffer();
	public:
		StringManager();
		~StringManager();
		void OnSizeChange( const vector2d& size );
		void LoadFont(const std::string& filename);
		BString* MakeString(const std::wstring& wstr);
		BString* MakeString(const vector2f& fontsize, const std::wstring& wstr);
		BString* MakeString(const vector2f& fontsize, const Color& clr,const std::wstring& wstr);
		vector2d GetStringSize( const std::wstring& wstr ); //�õ��ַ��������س���
		vector2d GetStringSize( const vector2f& fontsize, const std::wstring& wstr ); //�õ��ַ��������س���
		void DestroyString(BString* bstr);
		void Render(Engine & engine);
	public:
		BOHGE_FORCEINLINE bool isLoaded()
		{
			return m_isLoad;
		}
	};

}