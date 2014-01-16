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
//			����		   //
/////////////////////////////

#pragma once

#include "VariableType.h"
#include "Device.h"


#include <string>


//ע�⣬��ȡ������ȫ���ǹ�������������޸�����һ����ȡ�������ᵼ�������Զ�ȡ�ĸ��������ͬ���ı仯

namespace BohgeEngine
{
	struct OutPutTextureData;
	class Texture
	{
	private:
		struct WarpType
		{
			Device::TextureWarp m_SWarp;
			Device::TextureWarp m_TWarp;
			WarpType()
				:m_SWarp(Device::TW_CLAMP_TO_EDGE),
				m_TWarp(Device::TW_CLAMP_TO_EDGE)
			{}
		};
		struct FilterType
		{
			Device::TextureFilter m_MagFilter;
			Device::TextureFilter m_MinFilter;
			FilterType()
				:m_MagFilter( Device::TF_NEAREST ),
				m_MinFilter( Device::TF_NEAREST )
			{}
		};
		friend class Device;
	protected:
		handle					m_hTextureHandle;

		Device::TextureType	m_eType;
		bool					m_isLoadFormFile;//�����Ǳ����ɵĻ��Ƕ�ȡ��
		bool					m_isLoaded;
		bool					m_hasMipMap;
		uint					m_Anisotropic;
		WarpType				m_Warp;
		FilterType				m_Filter;
		vector2d				m_vSize;//�ߴ�
		Device::PixelFormat	m_eFormat;
		std::string				m_Name;
	public:
		Texture(Device::TextureType type);
		virtual ~Texture();
		void UnloadTexture();
		void SetWarp( Device::TextureWarp s, Device::TextureWarp t );
		void SetFilter( Device::TextureFilter mag, Device::TextureFilter min );
		void SetAnisotropic( uint x );
		void GenerateMipmap();
	public:
		BOHGE_FORCEINLINE const handle& GetTextureHandle() const
		{
			return m_hTextureHandle;
		}
		BOHGE_FORCEINLINE const handle* GetTextureHandlePointer() const
		{
			return &m_hTextureHandle;
		}
		BOHGE_FORCEINLINE bool HasMipmap() const
		{
			return m_hasMipMap;
		}
		BOHGE_FORCEINLINE const std::string& GetName() const
		{
			return m_Name;
		}
		BOHGE_FORCEINLINE const vector2d& GetSize() const
		{
			return m_vSize;
		}
		BOHGE_FORCEINLINE Device::TextureType TextureType() const
		{
			return m_eType;
		}
		BOHGE_FORCEINLINE Device::PixelFormat GetPixelFormat() const
		{
			return m_eFormat;
		}
		BOHGE_FORCEINLINE bool isLoadFormFile()//�Ƿ��Ƕ�ȡ��
		{
			return m_isLoadFormFile;
		}
		BOHGE_FORCEINLINE bool isLoaded()
		{
			return m_isLoaded;
		}
		static void SaveTexture( const OutPutTextureData& saveto );
	};




	class Texture2D : public Texture
	{
	public:
		Texture2D();
		~Texture2D();
		bool LoadTexture( Device::PixelFormat f, const std::string& strFileName );
		bool MakeTexture( Device::PixelFormat pf, byte pixelsize, const vector2d& size, const byte* buffer);
		//bool CopyFragment( const RenderTarget* fbo );//��ȡ��ǰFbo�е�����,Ч�ʵ���
		bool SubTexture( const byte* buffer );//��������
	};



	class TextureCube : public Texture//�������������ȫ�������size������ͬ
	{
	public:
		TextureCube();
		~TextureCube();
		//ǰ�����ϣ��£�����
		bool LoadTexture( Device::PixelFormat pf, const std::string& f, const std::string& b, const std::string& u, const std::string& d, const std::string& l, const std::string& r );
	};
}