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
//		ֲ���Ĳ���	   //
/////////////////////////
#pragma once
#include "ISceneNode.h"
#include "PlantBaseInfo.h"
#include "Frustum.hpp"
#include "aabsphere.hpp"
#include "aabbox.hpp"
#include <string>
#include <vector>
#include <map>



namespace BohgeEngine
{
	class Engine;
	class Material;
	class RendBuffer;
	class Serializer;
	class VertexBuffer;
	class IndicesBuffer;
	class PlantChunk : public IAttributeNode
	{
		enum FADE_STATE
		{
			FS_FADEOUT	= -1,
			FS_SHOW		= 1001,
			FS_HIDE		= 1002,
			FS_FADEIN	= 1,
		};
	private:
		typedef std::vector< IndicesBuffer* >	IndicesVector;//����buffer����������
	private:
		struct EachPlantBuffer
		{
			PlantBaseInfo::Plant_Shader			m_eShader;
			Material*							m_PlantMaterial;
			RendBuffer*							m_PlantBuffer;//ֲ����buffer
			VertexBuffer*						m_pPlantVertexBuffer;
			IndicesVector						m_LodIndices;
			EachPlantBuffer()
				:m_PlantMaterial(NULL),
				m_PlantBuffer(NULL),
				m_pPlantVertexBuffer(NULL)
			{

			}
		};
		struct PlantAlphaFadeInOut //���뵭��
		{
			FADE_STATE		m_InOut;//1�ǵ��룬-1�ǵ���
			float			m_Alpha;
			PlantAlphaFadeInOut()
				:m_InOut( FS_SHOW ),
				m_Alpha(1.0)
			{

			}
		};
	private:
		typedef std::vector< EachPlantBuffer >		RenderbufferVector;
		typedef std::vector< PlantAlphaFadeInOut >	FadeVector;
	private:
		int					m_Depth;
		bool				m_isDraw;//�Ƿ���Ҫ���Ƹýڵ�
		aabbox3df			m_BindBox;//������ľ����������������İ󶨺в�����������ǹ����ģ�����ط���Ҫÿ���ڵ㶼��һ���󶨺���
		vector4f			m_VisableRadius;//��ϸ�ڼ���Ŀɼ�����

		float				m_DistancePow;
		int					m_CurrentDrawLevel;//��ǰ�����ĵȼ�

		RenderbufferVector	m_BufferVector;
		FadeVector			m_FadeVector;

		PlantChunk*			m_LeftTop;
		PlantChunk*			m_RightTop;
		PlantChunk*			m_LeftBottom;
		PlantChunk*			m_RightBottom;
	public:
		PlantChunk( int depth );
		~PlantChunk(void);
	public:
		void LoadStruct( Serializer& loader );
		void LoadBindBox( Serializer& loader );
		void LoadBuffer( Serializer& loader, const std::string& texPath );
		void Update( const Frustumf& view, aabspheref& sphere, float fadetimespan, std::multimap< float, PlantChunk* >& plantmap  );
		void RenderOpaque( Engine& engine);
		void RenderTransparent( Engine& engine);
	public:
		virtual const void* Attribute(const Node_Attribute& name) const;
	};

}