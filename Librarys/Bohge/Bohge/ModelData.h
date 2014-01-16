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
//		ģ�ͻ�������	   //
/////////////////////////////
#pragma once
#include "3DMath.h"
#include "aabbox.hpp"

#include <string>
#include <vector>


namespace BohgeEngine
{
	class RendBuffer;
	class Material;
	class Serializer;

	typedef std::pair<std::vector<Quaternionf>, std::vector<Quaternionf> > AnimationDual;//���Ҫ����shader������

	struct Vertex_Attrs //��ȡģ��ʱ����Щ��������Щû��
	{
		int Attrs;
		bool vertex, normal, tangent, binormal, tex, bone;
		Vertex_Attrs():vertex(false),normal(false),tangent(false),binormal(false),tex(false),bone(false),Attrs(0){}
	};



	class SkinningKeyFrame	//ģ�Ͷ�����
	{
		friend class Model;
	private:
		struct KeyFrame
		{
			std::string											m_Name;//��Ӧ��������
			std::vector< std::pair<Quaternionf, Quaternionf> >	m_QuaternionArray;//�ؼ�֡����
		private:
			BOHGE_FORCEINLINE void _DoCopy( const KeyFrame& rhs )
			{
				m_Name = rhs.m_Name;
				m_QuaternionArray.resize( rhs.m_QuaternionArray.size() );
				for ( int i = 0 ; i < rhs.m_QuaternionArray.size(); i ++ )
				{
					m_QuaternionArray[i].first = rhs.m_QuaternionArray[i].first;
					m_QuaternionArray[i].second = rhs.m_QuaternionArray[i].second;
				}
			}
		public:
			KeyFrame()
			{

			}
			KeyFrame( const KeyFrame& rhs )
			{
				_DoCopy(rhs);
			}
			KeyFrame& operator = ( const KeyFrame& rhs )
			{
				_DoCopy(rhs);
				return *this;
			}
		};
	private://���������
		int							m_FrameBging;	//��ʼ֡
		int							m_FrameEnd;		//����֡
		int							m_Rate;			//֡��
		std::vector<KeyFrame>		m_BoneKeyFrames;//�йؼ���Ĺ���
	private:
		bool						m_isPlayAnimation;//�Ƿ񲥷Ŷ���
		bool						m_isLoop;//�Ƿ�ѭ������
		int							m_PlayBegin;	//��Ҫ���ŵĶ�������
		int							m_PlayEnd;	//��Ҫ���ŵĶ�������
		float						m_LastFrame;//ǰһ֡
		float						m_CurrentFrame;//��ǰ֡
	private:
		BOHGE_FORCEINLINE void _DoCopy( const SkinningKeyFrame& rhs )
		{
			m_FrameBging = rhs.m_FrameBging;
			m_FrameEnd = rhs.m_FrameEnd;
			m_Rate = rhs.m_Rate;
			m_isPlayAnimation = rhs.m_isPlayAnimation;
			m_isLoop = rhs.m_isLoop;
			m_PlayBegin = rhs.m_PlayBegin;
			m_PlayEnd = rhs.m_PlayEnd;
			m_LastFrame = rhs.m_LastFrame;
			m_CurrentFrame = rhs.m_CurrentFrame;
			m_BoneKeyFrames.resize( rhs.m_BoneKeyFrames.size() );
			for ( int i = 0 ; i < rhs.m_BoneKeyFrames.size(); i ++ )
			{
				m_BoneKeyFrames[i] = rhs.m_BoneKeyFrames[i];
			}
		}
	public:
		SkinningKeyFrame( const SkinningKeyFrame& rhs )
		{
			_DoCopy(rhs);
		}
		SkinningKeyFrame& operator = ( const SkinningKeyFrame& rhs )
		{
			_DoCopy(rhs);
			return *this;
		}
	private:
		BOHGE_FORCEINLINE bool isPlay()
		{
			return m_isPlayAnimation;
		}
	public:
		SkinningKeyFrame();
		void LoadKeyFrame(Serializer& file);//����ؼ���
		std::pair<Quaternionf, Quaternionf> GetFrame(int jointIndex);
		int	GetIndex(const std::string& jointName);
		void SetTime(uint timespan);
		std::vector< std::string > GetJointNameList();
	public:
		BOHGE_FORCEINLINE void SetAnimationInterval( int begin, int end)//������Ҫ���ŵ�֡����
		{
			ASSERT( begin >= m_FrameBging );
			ASSERT( end <= m_FrameEnd );
			m_PlayBegin = begin;
			m_PlayEnd = end;
		}
		BOHGE_FORCEINLINE void ResetAnimation()
		{
			m_LastFrame = m_CurrentFrame;
			m_CurrentFrame = m_PlayBegin;
		}
		BOHGE_FORCEINLINE void Play()
		{
			m_isPlayAnimation = true;
		}
		BOHGE_FORCEINLINE void Stop()
		{
			m_isPlayAnimation = false;
		}
		BOHGE_FORCEINLINE void Loop(bool b)
		{
			m_isLoop = b;
		}
	};


	class VertexBuffer;
	class IndicesBuffer;
	struct RendMesh
	{
		bool			m_isCopyed;
		mutable bool	m_isRender;//�����Ƿ���������У����ж��Ƿ����
		std::string		m_Name;
		aabbox3df		m_BindBox;//��Χ��
		RendBuffer*		m_pBuffer;
		VertexBuffer*	m_pVertexBuffer;
		IndicesBuffer*	m_pIndicesBuffer;
		int				m_Materialindex;//���ʵ�����
		Material*		m_pMaterial;//����ָ����ʵ�ָ��
	public:
		RendMesh():m_isCopyed(false),m_isRender(true),m_pBuffer(NULL),m_pMaterial(NULL),m_pVertexBuffer(NULL),m_pIndicesBuffer(NULL){}
		~RendMesh();
		RendMesh( const RendMesh& rm )
		{
			_DoCopy(rm);
		}
		const RendMesh& operator = ( const RendMesh& rm )
		{
			_DoCopy(rm);
			return *this;
		}
		BOHGE_FORCEINLINE void SetRend( const bool& b ) const
		{
			m_isRender = b;
		}
	private:
		void _DoCopy( const RendMesh& rm )
		{
			m_isCopyed = true;
			m_isRender = true;
			m_Name = rm.m_Name;
			m_BindBox = rm.m_BindBox;
			m_pBuffer = rm.m_pBuffer;
			m_pVertexBuffer = rm.m_pVertexBuffer;
			m_pIndicesBuffer = rm.m_pIndicesBuffer;
			m_Materialindex = rm.m_Materialindex;
			m_pMaterial = rm.m_pMaterial;
		}
	};



	struct Joint//������Ϣ
	{
		std::string								m_Name; //��������
		int										m_Parent;//�����ĸ�
		int										m_KeyFrameIndex;//�����ڹؼ�֡�����е�����
		Quaternionf								m_RealBind;
		Quaternionf								m_DaulBind;
		std::pair<Quaternionf, Quaternionf>		m_InversPair;//������ʼ״̬����
	private:
		BOHGE_FORCEINLINE void _DoCopy( const Joint& rhs )
		{
			//memcpy( this, &rhs, sizeof( Joint ) );//�����û�������ַ��������������std�ַ�������ʵ�ʸ��ƹ���ģ�������һ���ڴ�
			m_Name = rhs.m_Name;
			m_Parent = rhs.m_Parent;
			m_KeyFrameIndex = rhs.m_KeyFrameIndex;
			m_RealBind = rhs.m_RealBind;
			m_DaulBind = rhs.m_DaulBind;
			m_InversPair.first = rhs.m_InversPair.first;
			m_InversPair.second = rhs.m_InversPair.second;
		}
	public:
		Joint()
		{

		}
		Joint( const Joint& rhs )
		{
			_DoCopy(rhs);
		}
		Joint& operator = ( const Joint& rhs )
		{
			_DoCopy(rhs);
			return *this;
		}
	};




	class ModelData //��Դ���������ص�ģ������
	{
		friend class Model;
	public:
		//enum�е���ʼ���ֲ����޸ģ�����������
		enum Model_Chunk	//���������������Ҫ��ģ�͵���������ƥ��
		{
			CHUNK_BONES = 0,//ģ�͹���
			CHUNK_MATERIALS,//ģ�Ͳ���
			CHUNK_KEYFRAMES,//ģ�͹ؼ�֡
			//����µ�chunk����
			CHUNK_MESHES = 501,//ģ������
			MESHES_DATA,//��������
			MESHES_INDEX,//ģ������
			//����µ�mesh��������
			MESHES_BINDBOX,//����İ�Χ��
			MESHES_END = 999,//�������
			MODEL_END = 1000,//ģ�Ͷ�ȡ���
		};
		enum Vertex_Elements //ÿ����������������������Ҫ��ģ�͵���������ƥ��
		{
			VE_VERTEX = 1001,//����
			VE_NORMAL,//����
			VE_BINORMAL,//������
			VE_TANGENT,//����
			VE_TEX_COORD,//��������
			VE_BONEWEIGHT,//������Ϣ
			VE_END = 2000,//����
		};
	private:
		//c++��Ĭ�ϵĿ������죬�븳ֵ=���ǽ�ָ��ȫ����ֵ����������๲��
		std::string					m_FileName;
		bool						m_isBone;//�Ƿ��й���
		uint						m_MaxBoneIndexCount;//���������Ŀ������ȷ������ʲô���ӵ�shader
		std::vector<Material*>		m_MaterialVector;//���ֵ��ȫ��ͬ��ģ�͹��õģ�Ŀǰ���ÿ��Խ���
		std::vector<RendMesh*>		m_RendMeshVector;//���ֵ��ȫ��ͬ��ģ�͹��õģ�Ŀǰ���ÿ��Խ���
		std::vector<Joint>			m_JointVector;
		SkinningKeyFrame			m_KeyFrame;
		aabbox3df					m_BindBox;
	public:
		explicit ModelData();
		~ModelData();
	private:
		void _LoadBones(Serializer& file);
		void _LoadMaterials(Serializer& file, const std::string& folderpath);
		void _LoadMeshesData(Serializer& file, const Vertex_Attrs& att, vector2f& tex, vector3f& pos, vector3f& nor, vector3f& bin, vector4f& tat, vector4f& bone, vector4f& weight );
		void _LoadMeshes(Serializer& file);
		void _LoadKeyFrame(Serializer& file);
	public:
		void LoadModel(const std::string& name);
	};

}