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



///////////////////////
//	shader�ڲ��Ĳ���	 //
///////////////////////
#pragma once
#include "Predefine.h"


#include <string>
#include <map>

namespace BohgeEngine
{
	class ShaderLibrary;
	class ShaderParameter//����಻����ָ�룬��ָ��Ļ�����Ҫʵ�ֿ�������͸�ֵ����
	{
	public:
		enum FloatPrecision
		{
			FP_ERRORCODE = 0,//Lua�ò������ݵ�ʱ�����������Ϊ0
			FP_NONE,
			FP_LOW,
			FP_MEDIUM,
			FP_HIGH,
		};
		enum VariablesType
		{
			VT_ERRORCODE = 0,//Lua�ò������ݵ�ʱ�����������Ϊ0
			VT_FLOAT,
			VT_VEC2,
			VT_VEC3,
			VT_VEC4,
			VT_VEC2_ARRAY,
			VT_VEC3_ARRAY,
			VT_VEC4_ARRAY,
			VT_MAT3,
			VT_MAT4,
			VT_TEXTURE1D,
			VT_TEXTURE2D,
			VT_TEXTURE3D,
			VT_TEXTURECUBE,
		};
		enum InputsType//����Ĳ�������
		{
			//��������
			IT_ERRORCODE = 0,//Lua�ò������ݵ�ʱ�����������Ϊ0
			IT_LOCALSPACE_POSITION,//ԭʼλ��
			IT_LOCALSPACE_COORDNATE,//ԭʼ��������,һ��ʹ��������������һ��������������
			IT_LOCALSPACE_NORMAL,//ԭʼ����
			IT_LOCALSPACE_TANGENT,//ԭʼ����
			IT_LOCALSPACE_BINORMAL,//ԭʼ������
			IT_LOCALSPACE_BONE_INEX,//ԭʼ��������
			IT_LOCALSPACE_BONE_WEIGHT,//ԭʼ����Ȩ��

			IT_WORLDSPACE_POSITION,//���������µ�λ��
			IT_WORLDSPACE_NORMAL,//���������µķ���
			IT_WORLDSPACE_TANGENT,//���������µ�����
			IT_WORLDSPACE_BINORMAL,//���������µĸ�����
			IT_WORLDSPACE_VIEWDIRECTION,//���������µ��Ӿ����߷���

			IT_SCREENSPACE_POSITION,//��Ļ�ռ�λ��[-1,1]
			IT_SCREENSPACE_COORDNATE,//��Ļ�ռ��������꣬����������Ļ[0,1]
			IT_SCREENSPACE_NORMAL,//��Ļ�ռ�ķ��ߣ�һ��û��
			IT_SCREENSPACE_TANGENT,//��Ļ�ռ�����ߣ�һ��û��
			IT_SCREENSPACE_BINORMAL,//��Ļ�ռ�ĸ����ߣ�һ��û��

			IT_WORLDSPACE_LIGHT_DISTANCE,//��Դ�Ͷ���ľ���
			IT_WORLDSPACE_DEPTH,//���

			IT_USER_ATTRIBUTE_0,//�Զ��嶨����
			IT_USER_ATTRIBUTE_1,
			IT_USER_ATTRIBUTE_2,
			IT_USER_ATTRIBUTE_3,
			IT_USER_ATTRIBUTE_4,
			IT_USER_ATTRIBUTE_5,
			IT_USER_ATTRIBUTE_6,
			IT_USER_ATTRIBUTE_7,

			//ģ�ͼĴ���
			IT_LOCALSPACE_REAL_ARRAY,//��������
			IT_LOCALSPACE_DUAL_ARRAY,//��������
			IT_LOCALWORLD_TRANSFORM,//���ص�����ı任����
			IT_LOCALWORLD_ROTATION,//���ص���ת����
			IT_LOCALSCREEN_TRANSVIEWPROJ,//���ص���Ļ��mvp

			//������Ĵ�������
			IT_CAMERA_WORLDPOSITION,//�����λ��
			IT_CAMERA_WORLDDIRECTION,//���������
			IT_CAMERA_VIEW,//�����view����
			IT_CAMERA_PROJECTION,//�����ͶӰ
			IT_CAMERA_VIEWPROJ,//�����vp����
			IT_CAMERA_LINERPARAM,//��������Ի�����

			//��Դ������
			IT_LIGHT_POSITION,
			IT_LIGHT_COLOR,
			IT_AMBIENT_COLOR,
			IT_LOCALSPACE_LIGHTDIRECTION,//�ֲ������¹��յķ������һ����������
			IT_WORLDSPACE_LIGHTDIRECTION,//���������µĹ�Դ����

			//���ʽ��ܹ���ǿ��
			IT_MATERIAL_AMBIENT,//������
			IT_MATERIAL_DIFFUSE,//������
			IT_MATERIAL_SPECULAR,//����
			IT_MATERIAL_SPECULAR_LEVEL,//����ǿ��
			IT_MATERIAL_SHININESS,//�������

			//���պͲ��ʵĸ����Ͳ��������ǰ����ݶ���ˣ��ṩpsʹ�ã�һ��ʹ���������
			IT_LIGHT_MATERIAL_AMBIENT,//������
			IT_LIGHT_MATERIAL_LIGHT,//��������ɫ
			IT_LIGHT_MATERIAL_SPECULAR,//����

			//������ɫ��������
			IT_SURFACE_COLOR,//������ɫ

			//����
			IT_TEXTURE_AMBIENT,//������ͼ
			IT_TEXTURE_DIFFUSE,//��������
			IT_TEXTURE_SPECULAR,//������ɫ
			IT_TEXTURE_SPLEVEL,//����ȼ�
			IT_TEXTURE_GLOSSINESS,//����
			IT_TEXTURE_ILLUMINATION,//�Է���
			IT_TEXTURE_OPACITY,//͸����ͼ
			IT_TEXTURE_FITER,//������ͼ
			IT_TEXTURE_BUMP,//������ͼ
			IT_TEXTURE_REFLECTION,//������ͼ
			IT_TEXTURE_REFRACTION,//������ͼ
			IT_TEXTURE_DISPLACEMENT,//�Ӳ���ͼ
			IT_TEXTURE_TERRAIN_WEIGHT,//���ε���ͼ
			IT_TEXTURE_TERRAIN_DETAL,
			IT_TEXTURE_TERRAIN_BASE,
			IT_TEXTURE_TERRAIN_R,
			IT_TEXTURE_TERRAIN_G,
			IT_TEXTURE_TERRAIN_B,
			IT_TEXTURE_USER0,//�û��Զ����
			IT_TEXTURE_USER1,//�û��Զ����
			IT_TEXTURE_USER2,//�û��Զ����
			IT_TEXTURE_USER3,//�û��Զ����
			IT_TEXTURE_USER4,//�û��Զ����
			IT_TEXTURE_USER5,//�û��Զ����
			IT_TEXTURE_USER6,//�û��Զ����
			IT_TEXTURE_USER7,//�û��Զ����
		};
	private:
		VariablesType	m_eType;
		FloatPrecision	m_ePrecision;
		InputsType		m_eInputType;
		std::string		m_ParamName;//shader�е�����
	public:
		ShaderParameter( const InputsType& it, const FloatPrecision& fp, const VariablesType& vt, const std::string& name );
	public:
		std::string AsParameterString() const;//����һ��shader�����������߼Ĵ��������ַ���
		std::string AsVaryingString( const std::string& name ) const;//����һ��������������
		std::string AsVariables() const;//���һ������
		bool isUniform() const;
	public:
		static void RegisterShaderParameter( ShaderLibrary& library );
	public:
		BOHGE_FORCEINLINE const std::string& GetName() const
		{
			return m_ParamName;
		}
		BOHGE_FORCEINLINE void SetName( const std::string& to )
		{
			m_ParamName = to;
		}
		BOHGE_FORCEINLINE const InputsType& GetInputsType() const
		{
			return m_eInputType;
		}
		BOHGE_FORCEINLINE const VariablesType& GetVariableType() const
		{
			return m_eType;
		}
	};


	typedef std::map< ShaderParameter::InputsType, ShaderParameter*> ParameterMap;
}