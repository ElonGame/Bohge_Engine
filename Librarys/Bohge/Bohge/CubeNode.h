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



/////////////////////////////////////////
//			State Manage class		   //
/////////////////////////////////////////

#pragma once

#include "VariableType.h"
#include "Device.h"
#include "3DMath.h"
#include "ISceneNode.h"



namespace BohgeEngine
{
	class Vertex;
	class VertexTextureNormal;
	class Textrue;
	class SharedBuffer;
	class CubeNode : public IGetBuffer
	{
	private:
		enum Const
		{
			//ֻ��Ҫ�˸�����ӦΪ�������⣬�������������һ���ģ�
			//ֻ��Ҫ���Խ��ߵ�����������Ϊ��ͬ��ֵ����,
			//���Ƕ��͵ױ��붨���µĶ���,һ��16������
			COUNT_OF_VERTEX			= 24,
			COUNT_OF_INDIES			= 36,
			COUNT_OF_EDGE_VERTEX	= 8,
			COUNT_OF_EDGE_INDIES	= 30,
		};
	private:
		VertexTextureNormal*		m_pVertexArray;
		vector3f					m_vPosition;
		vector3f					m_vSize;
		vector2f					m_BeginCoord;
		vector2f					m_EndCoord;
	public:
		CubeNode();
		CubeNode( const CubeNode& in);
		CubeNode(const vector3f& size, const vector2f& beginCoord, const vector2f& endCoord);
		/*�˴���pos����size�Ӻ�ֱ�ӷŵ�vertexArray�в���������pos��ԭ��Ȼ��ͨ���任����ƽ��*/
		CubeNode(const vector3f& pos, const vector3f& size, const vector2f& beginCoord, const vector2f& endCoord);
		~CubeNode();
		/*��������������*/
		void CopyCoordinate(const CubeNode& input);
		void GetMashEdge(SharedBuffer& input);
		Vertex& GetVertex( int index );
		void SetPostion(const vector3f& pos, const vector3f& size);
		CubeNode& operator = (const CubeNode& in);
	public://�̳еĽӿ�
		virtual void GetVertexBuffer(SharedBuffer& input) const ;
	public:
		BOHGE_FORCEINLINE const vector3f& GetSize() const
		{
			return m_vSize;
		}
		BOHGE_FORCEINLINE const vector3f& GetPosition() const
		{
			return m_vPosition;
		}
		BOHGE_FORCEINLINE void GetCoordnate(vector2f& begin, vector2f& end)
		{
			begin = m_BeginCoord;
			end = m_EndCoord;
		}
	public:
		static uint GetElementCountPerVertex();//�õ�ÿ������floatԪ�ظ���
		static BOHGE_FORCEINLINE uint GetCountOfVertex()
		{
			return static_cast<uint>(COUNT_OF_VERTEX);
		}
		static BOHGE_FORCEINLINE uint GetCountOfIndes()
		{
			return static_cast<uint>(COUNT_OF_INDIES);
		}
		static BOHGE_FORCEINLINE uint GetCountOfEdgeVertex()
		{
			return static_cast<uint>(COUNT_OF_EDGE_VERTEX);
		}
		static BOHGE_FORCEINLINE uint GetCountOfEdgeIndes()
		{
			return static_cast<uint>(COUNT_OF_EDGE_INDIES);
		}
		static BOHGE_FORCEINLINE Device::RenderMode GetRenderMode()
		{
			return Device::RM_TRIANGLES; 
		}
	};
}