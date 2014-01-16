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
//			ģ��				//
/////////////////////////////
#pragma once
#include "3DMath.h"
#include "ISceneNode.h"
#include "LocatableObject.h"
#include "aabbox.hpp"
#include "ModelData.h"

#include <string>
#include <vector>


namespace BohgeEngine
{
	class RendBuffer;
	class Material;
	class Serializer;
	class Model : public ISceneNode
	{
	private:
		class JointObject : public IAttributeNode
		{
		public:
			int				m_JointIndex;//��Ӧ������Ԫ��������
			Quaternionf		m_Rotation;
			vector3f		m_Scale;
			vector3f		m_Position;
			Matrix44f		m_Transfrom;//����
			Matrix33f		m_RotationMatrix;//��ת����
		public:
			JointObject():m_JointIndex(0){}
		public:
			virtual const void* Attribute( const Node_Attribute& name ) const
			{

				switch( name )
				{
				case IAttributeNode::ATT_WORLDSCALE : return &m_Scale;
				case IAttributeNode::ATT_WORLDROTATION : return &m_Rotation;
				case IAttributeNode::ATT_WORLDPOSITION : return &m_Position;
				case IAttributeNode::ATT_WORLDTRANSFORM : return &m_Transfrom;
				case IAttributeNode::ATT_NORMALTRANSFORM : return &m_RotationMatrix;
				}
				//ASSERT(false);
				return NULL;
			}
		};
	private:
		//c++��Ĭ�ϵĿ������죬�븳ֵ=���ǽ�ָ��ȫ����ֵ����������๲��
		bool								m_isLoaded;
		std::string							m_FileName;
		bool								m_isBone;//�Ƿ��й���
		bool								m_isInview;
		bool								m_isUsingBasiMatrixOnDrawDepth;//�������Լ�����Ӱ��ʱ��ʹ��ƫ�ƾ���
		uint								m_MaxBoneIndexCount;//���������Ŀ������ȷ������ʲô���ӵ�shader
		std::vector<Material>				m_MaterialVector;
		std::vector<RendMesh>				m_RendMeshVector;
		std::vector<Joint>					m_JointVector;
		SkinningKeyFrame					m_KeyFrame;
		AnimationDual						m_AnimationDual;
		//����֡����
		uint								m_RefreshLimit;//ˢ�¼��
		uint								m_PassedTime;//������ʱ��
	protected:
		aabbox3df							m_OgiBindBox;
		//����ģ�͹����ҽڵ�
	private:
		typedef std::map< int, JointObject* >	JointNode;//���Թ�ģ�͵Ĺ����ڵ�
	private:
		bool						m_isFroceUpdate;//ǿ�Ƹ���
		JointNode					m_JointNodes;
	private://��ȡ�İ����������ⲿ��ֹ����
		void _BuildBones(uint timespan);//�ӽ���������
		void _UpDataJoint();//���¹ҽڵ�Ĺ���
	public:
		Model();
		virtual ~Model(void);
	public:
		virtual void Update( uint milisecon );
		virtual bool isInview( const Frustumf& cFru );
		virtual void Render( Engine& engine );
		virtual const void* Attribute(const Node_Attribute& name) const;
		virtual IAttributeNode* GetNodeByIndex( int index );//�õ������ڵ�
	public:
		void LoadModel( const std::string& name );
		void LoadLightMap( int index, const std::string& path );
	protected:
		BOHGE_FORCEINLINE void _SetForceUpdataJoint( const bool& b )
		{
			m_isFroceUpdate = b;
		}
		BOHGE_FORCEINLINE void _SetRefreshLimit( uint time )
		{
			m_RefreshLimit = time;
		}
		BOHGE_FORCEINLINE bool _isBone()
		{
			return m_isBone;
		}
		BOHGE_FORCEINLINE bool _isInview()
		{
			return m_isInview;
		}
	public:
		BOHGE_FORCEINLINE const std::vector<Material>& GetMaterialVector() const
		{
			return m_MaterialVector;
		}
		BOHGE_FORCEINLINE const std::vector<RendMesh>& GetRendMeshVector() const
		{
			return m_RendMeshVector;
		}
		BOHGE_FORCEINLINE void SetUseingBaisMatrix( const bool& is )
		{
			m_isUsingBasiMatrixOnDrawDepth = is;
		}
		BOHGE_FORCEINLINE const std::string& GetName()
		{
			return m_FileName;
		}
		BOHGE_FORCEINLINE void SetAnimationInterval( int begin, int end )
		{
			m_KeyFrame.SetAnimationInterval( begin, end );
		}
		BOHGE_FORCEINLINE void ResetAnimation()
		{
			m_KeyFrame.ResetAnimation();
			this->_BuildBones(0);
		}
		BOHGE_FORCEINLINE void AnimationPlay()
		{
			m_KeyFrame.Play();
		}
		BOHGE_FORCEINLINE void AnimationStop()
		{
			m_KeyFrame.Stop();
		}
		BOHGE_FORCEINLINE void AnimationLoop(bool b)
		{
			m_KeyFrame.Loop( b );
		}
		//����ģ�͹����ҽڵ�
		BOHGE_FORCEINLINE std::vector< std::string > GetJointNameList()//�õ����Թ�����Ĺ����ڵ�
		{
			return m_KeyFrame.GetJointNameList();
		}
	};



	/////////////////////////////
	//		���Զ�λ��ģ��	   //
	/////////////////////////////
	class LocatableModel : public Model , public LocatableObject
	{
	public:
		LocatableModel( );
	public:
		virtual void Update( uint milisecon );
		virtual const void* Attribute(const Node_Attribute& name) const;
	};





	/////////////////////////////////////
	//		���Զ�λ������ģ��	   //
	/////////////////////////////////////
	class PhysicalModel : public Model , public PhysicalObject
	{
	public:
		PhysicalModel( );
		void InitPhysical( float mass, const vector3f& localInertia );
	public:
		virtual void Update( uint milisecon );
		virtual const void* Attribute(const Node_Attribute& name) const;
	};

	//
	///////////////////////////////////////
	////		���Զ�λ������ģ��	   //
	///////////////////////////////////////
	//class ScriptPhysicalModel : public PhysicalModel, public ScriptBase
	//{
	//public:
	//	ScriptPhysicalModel( const std::string& name, const std::string& script );
	//	void InitPhysical( float mass, const vector3f& localInertia );
	//public:
	//	virtual void Update( uint milisecon );
	//	virtual const void* Attribute(const Node_Attribute& name) const;
	//};

}