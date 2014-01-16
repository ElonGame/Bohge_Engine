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
//		�����Ĳ���   //
/////////////////////////
#pragma once
#include "SecenProperty.h"
#include "ISceneNode.h"
#include "aabsphere.hpp"
#include "Frustum.hpp"
#include "ShaderManage.h"
#include <vector>


namespace BohgeEngine
{
	class MaterialShader;
	class Engine;
	class Material;
	class RendBuffer;
	class IndicesBuffer;
	class TerrainChunk : public IAttributeNode
	{
	private:
		enum DETAIL_LEVEL //ϸ�ڵȼ���ÿһ���ڵ����һ���ؾ��ȣ������ĸ��ڵ㾫����ͬ����һ���߾��ȣ���������еľ��ȵȼ������ڵ��ڵ���ѵ�Ϊ4�����жϸɽڵ�������һ�����ȵȼ�
		{
			DL_HIGHT	= 0,
			DL_LOW		= 1,
			DL_COUNT,
		};
	private:
		struct DrawData//���Ƶ�����
		{
			int					m_DetailIndex;//ϸ�ڵĵȼ���8��Ӧ0���ȵȣ�����ƽ������
			//������ֵҪ���ݸ�shader,���������ݣ��ֱ���
			vector4f					m_MorphXY_SteperScaleInvers_DoubleChunkSize;//����ϵ��, ��ǰ�ĵ������������ŵĳ˻����棬����ȷ����ǰ�ǵڼ���mesh�ȼ�ʹ�ã������㻹ԭ��0,1,2,3,4,5����1������ʽ,��ǰchunk�ĳߴ�
			ShaderManage::ShaderName	m_RenderShader;//�������ʱ��Ĳ���
			IndicesBuffer*				m_IndicesBuffer;////��ǰ���ȵ�����������
			//������chunk��ɾ��
		};
	private:
		typedef std::vector< DrawData >	DrawDataVector;//����buffer����������
	private:
		vector2f			m_DetailDisance_Tile;//ϸ�������ɼ���ϸ��,ϸ����������ϵ��

		const bool			m_isFirstNode;	//�Ƿ����׽��
		const bool			m_isLastNode;	//�Ƿ����ս��

		bool				m_isCanSee;//����������ܽ���ˮ�µ�ʱ������������޳���Щ�޷�����ʾ�ĵ�Ƥ
		bool				m_isSplit;//�Ƿ���Ҫ���µݹ�
		bool				m_isDraw;//�Ƿ���Ҫ���Ƹýڵ�

		vector2f			m_BeginEnd; //��chunkϸ�ڵĿɼ�����
		vector2f			m_OffsetScale; //��chunkƫ��
		aabbox3df			m_BindBox;	//�ڵ�󶨺�

		vector2f			m_SizeScaleInv_RepeatSizeInv;//���γߴ�*���ŵĵ����������ظ��ĵ�Ԫ��С�ĵ���,����ȷ����������
		int					m_DrawLevel;//���Ƶ�ϸ�ڵȼ����п��ܴνڵ�����ˣ���������һ��ڵ㲻����߾��ȣ��������ĸ��ڵ�ľ��Ȼ���
		DrawDataVector		m_DrawData;

		RendBuffer*			m_Buffer;
		Material*			m_Material;

		//�ӽڵ�
		TerrainChunk*		m_LeafChunk_UL;  //����
		TerrainChunk*		m_LeafChunk_UR;  //����
		TerrainChunk*		m_LeafChunk_DL;  //����
		TerrainChunk*		m_LeafChunk_DR;  //����
	public:
		TerrainChunk( const vector2d& begin, const vector2d& end, int iterDepth, const TerrainProperty& ppy );//size����ʼ�ڽ���,��İ󶨺�,�Ѿ����������,ԭʼ�ĵ�������
		~TerrainChunk( );
	public:
		void LoadBindBox( Serializer& loader );
		void OffsetScale( const vector2f& os );
		void Update( const Frustumf& view, aabspheref& sphere, std::multimap< float, TerrainChunk* >& tmap );
		void Render( Engine& engine) const;
		void SetBuffer( RendBuffer* buffer );
		void SetMaterial( Material* m );
		void AssignMaterial( const bool& isCast, const ICamera& camera );//ͨ����Ӱ�����ȷ����Ⱦÿһ��Ĳ���
	public:
		const void *Attribute(const IAttributeNode::Node_Attribute &att) const;
	};
}