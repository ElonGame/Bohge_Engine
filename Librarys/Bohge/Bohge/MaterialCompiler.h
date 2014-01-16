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



/////////////////////////////////
//			���ʱ���			   //
/////////////////////////////////
#pragma once
#include "ScriptBase.h"
#include "Pipeline.h"
#include "Device.h"
#include "ShaderParameter.h"

#include <string>
#include <list>
#include <vector>

namespace BohgeEngine
{
	class Material;
	class ShaderAssembler;
	class ShaderLibrary;
	class ShaderSegment;
	typedef std::vector< ShaderSegment* > Segments;
	class MaterialCompiler : public ScriptBase
	{
		enum Material_Constant
		{
			//alpha������
			ERRORCODE = 0,//Lua�ò������ݵ�ʱ�����������Ϊ0
			ALPAH_OFF,
			ALPAH_COVERAGE,
			ALPAH_BLEND,
			ZERO,
			ONE,
			SRC_COLOR,
			ONE_MINUS_SRC_COLOR,
			SRC_ALPHA,
			ONE_MINUS_SRC_ALPHA,
			DST_ALPHA,
			ONE_MINUS_DST_ALPHA ,
			DST_COLOR,
			ONE_MINUS_DST_COLOR,

			//��Ⱦ״̬���
			DEPTH_TEST_ON,
			DEPTH_TEST_OFF,
			DEPTH_MASK_ON,
			DEPTH_MASK_OFF,
			DEPTH_FUNCTION_NEVER,
			DEPTH_FUNCTION_LESS,
			DEPTH_FUNCTION_GREATER,
			DEPTH_FUNCTION_ALWAYS,
			CULL_FACE_NONE,
			CULL_FACE_BACK,
			CULL_FACE_FRONT,

			MC_COUNT,
		};
	private:
		ShaderLibrary*		m_pShaderLibrary;//Ƭ�Ͽ�
		ShaderAssembler*	m_pShaderAssembler;//�����
	public:
		MaterialCompiler( );
		~MaterialCompiler( );
	private:
		Device::Blend _MaterialConstant2Blend( const Material_Constant& mc ) const;
		Device::Function _MaterialConstant2Function( const Material_Constant& mc ) const;
		Device::CullFaceMode _MaterialConstant2CullFaceMode( const Material_Constant& mc ) const;
		void _MaterialAlphaDrawState( const Pipeline::PassType& pass,  Material& mat ) const;
		void _MaterialSahder( const Pipeline::PassType& pass,  Material& mat );
		void _ReadTextureState( Material& mat, const std::list<ShaderParameter::InputsType>& list );
		void _ReadTextureState( Material& mat, int type, const char* name );
	private:
		virtual void _OnScriptLoaded();//���ű����غ�
		virtual void _SetGlobalClassesIndex();
	public:
		virtual void LoadScript( const std::string& filename );
		virtual void DoScript( const std::string& buffer );
		virtual void UnLoadScript();
	public:
		void ReleaseResource();//ȫ���ͷ�
		void Initialization( );//��ʼ��
		Material* CompilerMaterial( std::string& matstr );
	};
}
