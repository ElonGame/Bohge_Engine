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


#include "TextureState.h"
#include "ShaderLibrary.h"


#define  MAKE_STRING_ENUM(x,y) #y , x##y

namespace BohgeEngine
{

	//-------------------------------------------------------------------------------------------------------
	TextureState::TextureState( Device::TextureWarp s, Device::TextureWarp t, Device::TextureFilter mag, Device::TextureFilter min )
		:m_SWarp( s ),
		m_TWarp( t ),
		m_MagFilter( mag ),
		m_MinFilter( min )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	TextureState::~TextureState( void )
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void BohgeEngine::TextureState::RegisterShaderParameter( ScriptBase& script )
	{
		script.SetGlobal( MAKE_STRING_ENUM( Device::TW_, REPEAT) );
		script.SetGlobal( MAKE_STRING_ENUM( Device::TW_, CLAMP_TO_EDGE) );
		script.SetGlobal( MAKE_STRING_ENUM( Device::TW_, MIRRORED_REPEAT) );

		script.SetGlobal( MAKE_STRING_ENUM( Device::TF_, NEAREST) );
		script.SetGlobal( MAKE_STRING_ENUM( Device::TF_, LINEAR) );
		script.SetGlobal( MAKE_STRING_ENUM( Device::TF_, NEAREST_MIPMAP_NEAREST) );
		script.SetGlobal( MAKE_STRING_ENUM( Device::TF_, LINEAR_MIPMAP_NEAREST) );
		script.SetGlobal( MAKE_STRING_ENUM( Device::TF_, NEAREST_MIPMAP_LINEAR) );
		script.SetGlobal( MAKE_STRING_ENUM( Device::TF_, LINEAR_MIPMAP_LINEAR) );
	}

}