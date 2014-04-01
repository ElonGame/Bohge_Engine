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

#include "Bfont.h"
#include "Vertex.h"
#include "Texture.h"
#include "ShaderManager.h"
#include "string.h"
#include "IOSystem.h"
#include "Log.h"
#include "Shapes.h"
#include "RendBuffer.h"
#include "Shaders2D.h"
#include "Engine.h"
#include "Device.h"

#include "ft2build.h"
#include FT_FREETYPE_H 
#include FT_GLYPH_H

using namespace std;

namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	BString::BString(StringManager* father, const wstring& wstr, int height, const vector2f& screensize, const vector2d& orgiSize, const Color& clr, RendBuffer* buffer, const vector2d& texSize, const byte* pBuffer )
		:m_String(wstr),
		m_Father( father ),
		m_MaxHeight(height),
		m_isManaged(true),
		m_isShow(true),
		m_fScale(1,1),
		m_StringTextureSize(orgiSize),
		m_ScreenFontSize( screensize ),
		m_eAlign(Center)
	{
		m_pTex = NEW Texture2D();
		m_pTex->MakeTexture( Device::PF_A8, 1, texSize, pBuffer );

		const vector2d& resolution = Engine::Instance().GetDevice()->GetResolutionSize();
		m_Size = vector2f(
			static_cast<float>(orgiSize.m_x) / static_cast<float>(resolution.m_x),
			static_cast<float>(orgiSize.m_y) / static_cast<float>(resolution.m_y)
			);
		m_Size *= 2.0f;//ת������Ļ����
		vector2f scrrenPos = m_Size;// * 2.0f;


		vector2f coord;
		coord.m_x = static_cast<float>(orgiSize.m_x) / texSize.m_x;
		coord.m_y = static_cast<float>(orgiSize.m_y) / texSize.m_y;

		m_pVertex = NEW VertexTextureColor[COUNT_OF_VERTEX];
		m_pVertex[0] = VertexTextureColor(vector4f(0,0,0), vector2f(0,0), clr);
		m_pVertex[1] = VertexTextureColor(vector4f(0,-scrrenPos.m_y,0), vector2f(0,coord.m_y), clr);
		m_pVertex[2] = VertexTextureColor(vector4f(scrrenPos.m_x,0,0), vector2f(coord.m_x,0), clr);
		m_pVertex[3] = VertexTextureColor(vector4f(scrrenPos.m_x,-scrrenPos.m_y,0), vector2f(coord.m_x,coord.m_y), clr);

		ASSERT( NULL != buffer );
		m_pRendBuffer = buffer;
		m_pRendBuffer->GetVertexBuffer()->FlushVertexBuffer( COUNT_OF_VERTEX, m_pVertex );
	}
	//-------------------------------------------------------------------------------------------------------
	BString::~BString()
	{
		SAFE_DELETE(m_pTex);
		SAFE_DELETE_ARRAY(m_pVertex);
		//SAFE_DELETE(m_pBuffer);
	}
	//-------------------------------------------------------------------------------------------------------
	void BString::Render(Engine & engine)
	{
		if( m_isShow )
		{
			FontShader& font = engine.GetShaderManager()->GetShader<FontShader>(ShaderManager::FontShader);
			font.SetParamPos(m_Pos * m_fScale );
			font.SetParamScale(m_fScale);
			engine.GetDevice()->Draw( *m_pRendBuffer, font, m_pTex );

			//vector2f scrrenPos = m_Size ;
			//vector2f lt( m_pVertex[0].GetPostion().m_x, m_pVertex[0].GetPostion().m_y );
			//vector2f rb( m_pVertex[3].GetPostion().m_x, m_pVertex[3].GetPostion().m_y );
			//Rect2D test( m_Pos + lt, m_Pos + rb, Color::BloodRed );
			//test.Render(engine);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void BString::SetColor(const Color& clr)
	{
		for (int i = 0 ; i < COUNT_OF_VERTEX ; i ++ )
		{
			m_pVertex[i].SetColor(clr);
		}
		m_pRendBuffer->GetVertexBuffer()->FlushVertexBuffer( COUNT_OF_VERTEX, &m_pVertex[0] );
	}
	//-------------------------------------------------------------------------------------------------------
	void BString::SetScale(const vector2f& scale)
	{
		m_fScale = scale;
	}
	//-------------------------------------------------------------------------------------------------------
	vector4f BString::GetStringCoordnate( const wstring& wstr )
	{
		if ( -1 != m_String.find( wstr ) )
		{
			int beginpos = m_String.find( wstr );
			vector2d begin;
			vector2d tgsize = m_Father->GetStringSize( m_ScreenFontSize, wstr );
			if ( 0 != beginpos )
			{
				int hangCount = 0;
				wstring forwardstr = m_String.substr( 0, beginpos );//�õ�����ǰ����ı�
				for ( int i = 0 ; i < forwardstr.size(); i ++ )
				{
					if ( L'\n' == forwardstr[i] )
					{
						hangCount ++;//��¼�ж�����
					}
				}
				int lastenter = forwardstr.find_last_of('\n');
				wstring substr = forwardstr.substr( lastenter+1, forwardstr.size()-lastenter );
				vector2d offset = m_Father->GetStringSize( m_ScreenFontSize, substr );//ȡ��ǰ����ı��ĳߴ�
				begin.m_y = m_MaxHeight * hangCount;
				begin.m_x = offset.m_x;
			}
			vector2d TextureSize = m_pTex->GetSize();
			vector4f coord;
			coord.m_x = static_cast<float>(begin.m_x) / TextureSize.m_x;
			coord.m_y = static_cast<float>(begin.m_y) / TextureSize.m_y;
			coord.m_z = static_cast<float>(begin.m_x+tgsize.m_x) / TextureSize.m_x;
			coord.m_w = static_cast<float>(begin.m_y+tgsize.m_y) / TextureSize.m_y;
			return coord;
		}
		return vector4f();
	}









	//-------------------------------------------------------------------------------------------------------
	StringManager::BChar::BChar(int beginy, const vector2d& size, FT_Glyph glyph, bool isspace )
		:m_PixelSize(size), m_pGlyph(glyph), m_nBeginY(beginy),m_isSpace(isspace)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	StringManager::BChar::~BChar()
	{
		FT_Done_Glyph(m_pGlyph);
	}



	//-------------------------------------------------------------------------------------------------------
	StringManager::StringManager()
		:m_isLoad(false),
		m_FaceSize(0.5f, 0.5f)//Ĭ�ϵ������С�ǵ�ǰ��Ļ��һ���һ��
	{
	}
	//-------------------------------------------------------------------------------------------------------
	StringManager::~StringManager()
	{
		if( m_isLoad )
		{
			FT_Done_Face(m_FT_Face);
			FT_Done_FreeType(m_FT2Lib);
			for ( BStringList::iterator it = m_StringList.begin(); //�ͷ�ȫ��������
				it != m_StringList.end();
				it++)
			{
				this->DestroyString( *it );
			}
			for ( RendBufferList::iterator it = m_FreeBufferList.begin();
				it != m_FreeBufferList.end();
				it++ )
			{
				delete (*it)->GetVertexBuffer();
				delete (*it)->GetIndicesBuffer();
				SAFE_DELETE( *it );
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void StringManager::OnSizeChange( const vector2d& size )
	{
		FT_Set_Pixel_Sizes(m_FT_Face, size.m_x * m_FaceSize.m_x, size.m_y * m_FaceSize.m_y);
	}
	//-------------------------------------------------------------------------------------------------------
	void StringManager::LoadFont(const string& filename)
	{
		ASSERT( !m_isLoad );
		m_isLoad = true;
		FT_Init_FreeType( &m_FT2Lib);
		//����һ������,ȡĬ�ϵ�Face,һ��ΪRegualer
		FT_New_Face( m_FT2Lib,
			( IOINSTANCE.FontFolder() + filename).c_str(),
			0, &m_FT_Face ); 
		FT_Select_Charmap(m_FT_Face, FT_ENCODING_UNICODE);
		m_FT_Face->num_fixed_sizes;
		const vector2d& resolution = Engine::Instance().GetDevice()->GetResolutionSize();
		FT_Set_Pixel_Sizes(m_FT_Face, resolution.m_x * m_FaceSize.m_x, resolution.m_y * m_FaceSize.m_y);
		DEBUGLOG("Load Font %s\n", filename.c_str());
	}
	//-------------------------------------------------------------------------------------------------------
	StringManager::BChar* StringManager::_LoadChar(wchar_t wchar)
	{
		//�ο�http://my.unix-center.net/~Simon_fu/?p=385
		bool space = false;
		if( L' ' == wchar )
		{
			space = true;
			wchar = L'_';
		}
		FT_Load_Glyph(m_FT_Face,  FT_Get_Char_Index( m_FT_Face, wchar ), FT_LOAD_DEFAULT);//FT_LOAD_FORCE_AUTOHINT| FT_LOAD_TARGET_NORMAL); 

		//�õ���ģ
		FT_Glyph glyph;
		FT_Get_Glyph( m_FT_Face->glyph, &glyph );
		//ת����λͼ
		FT_Render_Glyph( m_FT_Face->glyph,   FT_RENDER_MODE_LCD );

		FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
		//ȡ��λͼ����
		FT_Bitmap& bitmap=bitmap_glyph->bitmap;

		//���¼�������
		float scale = static_cast<float>(bitmap.rows) / static_cast<float>(m_FT_Face->glyph->metrics.height);//�����ı��߿����ŵ�bitmap�߿�����ű�
		int beginY =  Math::Ceil( m_FT_Face->glyph->metrics.horiBearingY * scale );//yƫ��

		//FT_Done_Glyph(glyph);
		vector2d size = vector2d(bitmap.width, bitmap.rows);
		return NEW BChar(beginY, size, glyph, space);
	}
	//-------------------------------------------------------------------------------------------------------
	BString* StringManager::MakeString(const wstring& wstr)
	{
		return _InternalMakeString(false, vector2f(1.0f, 1.0f), Color(1.0f,1.0f,1.0f,1.0f), wstr);
	}
	//-------------------------------------------------------------------------------------------------------
	BString* StringManager::MakeString(const vector2f& fontsize, const wstring& wstr)
	{
		return MakeString(fontsize, Color(1.0f,1.0f,1.0f,1.0f), wstr);
	}
	//-------------------------------------------------------------------------------------------------------
	BString* StringManager::MakeString(const vector2f& fontsize, const Color& clr,const wstring& wstr)
	{
		bool isScale = ! ( fontsize.m_x == 1.0 && fontsize.m_y == 1.0 );
		return _InternalMakeString(isScale, fontsize, clr, wstr);
	}
	//-------------------------------------------------------------------------------------------------------
	vector2d StringManager::GetStringSize( const wstring& wstr )
	{
		return _InternalGetStringSize( false, vector2f(1,1), wstr );
	}
	//-------------------------------------------------------------------------------------------------------
	vector2d StringManager::GetStringSize( const vector2f& fontsize, const wstring& wstr )
	{
		bool isScale = ! ( fontsize.m_x == 1.0 && fontsize.m_y == 1.0 );
		return _InternalGetStringSize( isScale, fontsize, wstr );
	}
	//-------------------------------------------------------------------------------------------------------
	vector2d StringManager::_InternalGetStringSize( bool isScale, const vector2f& fontsize, const wstring& wstr )
	{
		if (!m_isLoad)
		{
			DEBUGLOG("Font not loaded!!\n",1);
			return vector2d();
		}
		BCharList list;
		vector< pair<int,int> > LineElements;
		vector2d TexSize;//�ı�ͼƬ�ĳԴ�

		//��һ������ȡȫ�������壬�ҵ�����BeginY
		{
			int tempWidth = 0;
			int CharCount = 0; //ÿ�����ֵĸ���
			int MaxBeginY = 0; //����BeginY
			FT_Matrix scale;
			if( isScale )
			{//����������ڰ����ر��������ŵģ�
				scale.xx = fontsize.m_x / m_FaceSize.m_x * 0x10000L;
				scale.xy = 0.0;
				scale.yy = fontsize.m_y / m_FaceSize.m_y * 0x10000L;
				scale.yx = 0.0;
			}

			for( int i = 0 ; i < wstr.size() ; )//����ȫ����char
			{
				if( L'\n' != wstr[i] && L'#' != wstr[i] )
				{
					if( isScale )
					{
						FT_Set_Transform( m_FT_Face, &scale, NULL );
					}
					CharCount++;
					BChar* pBChar = _LoadChar(wstr[i]);
					MaxBeginY = MaxBeginY < pBChar->m_nBeginY ? pBChar->m_nBeginY : MaxBeginY;
					tempWidth += pBChar->m_PixelSize.m_x;
					list.push_back(pBChar);
				}
				i++;//����
				if( L'\n' == wstr[i] || L'#' == wstr[i] || wstr.size() == i )//����ǻ��л����ı�����
				{
					TexSize.m_x = TexSize.m_x < tempWidth ? tempWidth : TexSize.m_x;
					LineElements.push_back(make_pair(CharCount, MaxBeginY));//ÿ�����ֵĸ�������������beginY
					CharCount = 0;
					MaxBeginY = 0;
					tempWidth = 0;
				}		
			}

		}
		//�ڶ������ҵ�ÿһ�����ĸ߶ȣ��߶ȵ��ڣ����BeginY��ȥ��ǰ�����BeginY�ټ��ϵ�ǰ����ĸ߶�
		{
			int LineIndex = 0; //����������
			int CharIndex = 0; //ÿ���ֵ�����
			int maxHeight = 0; //ÿ�����߶�
			for( BCharList::iterator it = list.begin();
				it != list.end();
				it ++)
			{
				CharIndex ++;
				int height = LineElements[LineIndex].second - (*it)->m_nBeginY + (*it)->m_PixelSize.m_y;
				maxHeight = maxHeight < height ? height : maxHeight;
				if( CharIndex == LineElements[LineIndex].first )//��������Ѿ�����
				{
					TexSize.m_y = TexSize.m_y < maxHeight ? maxHeight : TexSize.m_y;
					maxHeight = 0;
					CharIndex = 0;
					LineIndex ++;
				}
				SAFE_DELETE( *it );
			}
		}
		//�����µ�Y
		{
			int NewHeight = TexSize.m_y * 1.2;
			int SkyEarth = ( NewHeight - TexSize.m_y ) * 0.5 ;//����Ԥ��0.2����������
			TexSize.m_y = NewHeight;
			TexSize.m_y *= LineElements.size();
		}
		return TexSize;
	}
	//-------------------------------------------------------------------------------------------------------

	int PowerOfTwo( int num )
	{
		int i = 2;
		do 
		{
			i = i << 1;
		} while ( i < num );
		return i;
	}
	BString* StringManager::_InternalMakeString(bool isScale, const vector2f& fontsize, const Color& clr, const wstring& wstr)
	{
		if (!m_isLoad)
		{
			DEBUGLOG("Font not loaded!!\n",1);
			return NULL;
		}
		BCharList list;
		vector< pair<int,int> > LineElements;
		vector2d TexSize;//�ı�ͼƬ�ĳԴ�

		//��һ������ȡȫ�������壬�ҵ�����BeginY
		{
			int tempWidth = 0;
			int CharCount = 0; //ÿ�����ֵĸ���
			int MaxBeginY = 0; //����BeginY
			FT_Matrix scale;
			if( isScale )
			{//����������ڰ����ر��������ŵģ�
				scale.xx = fontsize.m_x / m_FaceSize.m_x * 0x10000L;
				scale.xy = 0.0;
				scale.yy = fontsize.m_y / m_FaceSize.m_y * 0x10000L;
				scale.yx = 0.0;
			}

			for( int i = 0 ; i < wstr.size() ; )//����ȫ����char
			{
				if( L'\n' != wstr[i] && L'#' != wstr[i] )
				{
					if( isScale )
					{
						FT_Set_Transform( m_FT_Face, &scale, NULL );
					}
					CharCount++;
					BChar* pBChar = _LoadChar(wstr[i]);
					MaxBeginY = MaxBeginY < pBChar->m_nBeginY ? pBChar->m_nBeginY : MaxBeginY;
					tempWidth += pBChar->m_PixelSize.m_x;
					list.push_back(pBChar);
				}
				i++;//����
				if( L'\n' == wstr[i] || L'#' == wstr[i] || wstr.size() == i )//����ǻ��л����ı�����
				{
					TexSize.m_x = TexSize.m_x < tempWidth ? tempWidth : TexSize.m_x;
					LineElements.push_back(make_pair(CharCount, MaxBeginY));//ÿ�����ֵĸ�������������beginY
					CharCount = 0;
					MaxBeginY = 0;
					tempWidth = 0;
				}		
			}

		}
		//�ڶ������ҵ�ÿһ�����ĸ߶ȣ��߶ȵ��ڣ����BeginY��ȥ��ǰ�����BeginY�ټ��ϵ�ǰ����ĸ߶�
		{
			int LineIndex = 0; //����������
			int CharIndex = 0; //ÿ���ֵ�����
			int maxHeight = 0; //ÿ�����߶�
			for( BCharList::iterator it = list.begin();
				it != list.end();
				it ++)
			{
				CharIndex ++;
				int height = LineElements[LineIndex].second - (*it)->m_nBeginY + (*it)->m_PixelSize.m_y;
				maxHeight = maxHeight < height ? height : maxHeight;
				if( CharIndex == LineElements[LineIndex].first )//��������Ѿ�����
				{
					TexSize.m_y = TexSize.m_y < maxHeight ? maxHeight : TexSize.m_y;
					maxHeight = 0;
					CharIndex = 0;
					LineIndex ++;
				}
			}

		}
		//�����������ͼ����
		vector2d orgiSize;//ԭʼ�����óߴ�
		BString* Result;
		int ResultHeight;//�и�
		{
			int NewHeight = TexSize.m_y * 1.2;
			ResultHeight = NewHeight;
			int SkyEarth = ( NewHeight - TexSize.m_y ) * 0.5 ;//����Ԥ��0.2����������
			TexSize.m_y = NewHeight;
			int Height = TexSize.m_y;
			TexSize.m_y *= LineElements.size();
			//��TextSize�ı�֮power of two
			orgiSize = TexSize;
			TexSize.m_x = PowerOfTwo( TexSize.m_x );
			TexSize.m_y = PowerOfTwo( TexSize.m_y );
			byte* pBuffer = NEW byte[ TexSize.m_x * TexSize.m_y ];
			int LineIndex = 0; //����������
			int CharIndex = 0; //ÿ���ֵ�����
			vector2d offset; //��ȡÿһ����������µ�buffer�е�ƫ��
			memset(pBuffer,0,TexSize.m_x * TexSize.m_y );
			for ( BCharList::iterator it = list.begin();
				it != list.end();
				it++)
			{
				int horBeginPos = LineElements[LineIndex].second - (*it)->m_nBeginY + SkyEarth ; //��ͼƬ�����л����ı�����ʵ��ֱ��ʼλ��
				int horEndPos = horBeginPos + (*it)->m_PixelSize.m_y;//��ͼƬ�����л����ı�����ʵ��ֱ�������λ��
				FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)(*it)->m_pGlyph;
				for(int j=0; j  < Height ; j++) //����ͼƬ�Ŀ�ȣ��������߶����ı������Ŷ���һ�����ֵ�����
				{
					if( j < horBeginPos || j >= horEndPos || (*it)->m_isSpace ) //���������Ҫ���Ƶķ�Χ��
					{
						continue;
					}
					for(int i=0; i < (*it)->m_PixelSize.m_x; i++)
					{
						int index = (i + offset.m_x) + ( j + offset.m_y ) * TexSize.m_x;
						//bite alpha = j >= horBeginPos && j < horEndPos && !(*it)->m_isSpace //���ڻ��Ʒ�Χ֮��ʱ,�Ҳ��ǿո��ʱ��
						//	? bitmap_glyph->bitmap.buffer[ i + (*it)->m_PixelSize.m_x* ( j - horBeginPos ) ] : 0;				
						byte alpha = bitmap_glyph->bitmap.buffer[ i + (*it)->m_PixelSize.m_x* ( j - horBeginPos ) ];
						pBuffer[index] = alpha;
					}
				}
				offset.m_x += (*it)->m_PixelSize.m_x;//����xλ�õ�offset
				SAFE_DELETE(*it);//˳���������
				CharIndex++;//����һ��
				if( CharIndex >= LineElements[LineIndex].first ) //��������Ѿ�������
				{
					offset.m_y += Height;
					offset.m_x = 0; //ˮƽ��ƫ������
					CharIndex = 0;
					LineIndex ++;
				}
			}
			Result = NEW BString( this, wstr, ResultHeight, fontsize, orgiSize, clr, this->_AllocBuffer() , TexSize, pBuffer);
			SAFE_DELETE_ARRAY(pBuffer);
			m_StringList.push_back(Result);
			Result->SetIterator( (++m_StringList.rbegin()).base() );
		}
		//DEBUGLOG("String size %d, %d\n", TextSize.m_x, TextSize.m_y);
		return Result;
	}
	//-------------------------------------------------------------------------------------------------------
	RendBuffer* StringManager::_AllocBuffer()
	{
		if( 0 != m_FreeBufferList.size() )
		{
			RendBufferList::iterator it = m_FreeBufferList.begin();
			RendBuffer* reslut = (*it);
			m_FreeBufferList.erase( it );
			return reslut;
		}
		static ushort Indies[BString::COUNT_OF_INDIES]={0,1,2,3};

		RendBuffer*			rb;
		VertexBuffer*		vb;
		IndicesBuffer*		ib;
		rb = NEW RendBuffer(Device::RM_TRIANGLE_STRIP);
		vb = NEW VertexBuffer( Device::MU_DYNAMIC );
		ib = NEW IndicesBuffer( Device::MU_STATIC );
		rb->SetVertexBuffer( vb );
		rb->SetIndicesBuffer( ib );
		ib->FlushIndiesBuffer( BString::COUNT_OF_INDIES, &Indies[0] );
		return rb;
	}
	//-------------------------------------------------------------------------------------------------------
	void StringManager::DestroyString(BString* bstr)
	{
		//for ( BStringList::iterator it = m_StringList.begin();
		//	it != m_StringList.end();
		//	it ++)
		//{
		//	if( *it == bstr )
		//	{
		//		SAFE_DELETE(*it);
		//		m_StringList.erase(it);
		//		break;
		//	}
		//}

		//����ɾ��
		if ( NULL != bstr )
		{
			BStringList::iterator it = bstr->m_Iterator;
			m_FreeBufferList.push_back( (*it)->m_pRendBuffer );//������õ�buffer��������
			SAFE_DELETE( *it );
			m_StringList.erase(it);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void StringManager::Render(Engine & engine)
	{
		for ( BStringList::iterator it = m_StringList.begin();
			it != m_StringList.end();
			it ++)
		{
			if( (*it)->isManaged() )
			{
				(*it)->Render(engine);
			}
		}
	}
//-------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------
}