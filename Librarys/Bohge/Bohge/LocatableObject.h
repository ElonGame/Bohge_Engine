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
//		��λ��ģ��		   //
/////////////////////////////
//˵��ȫ������ģ�͵�up���������(0,1,0),Froward�������Ҫ(1,0,0)
#pragma once
#include "3DMath.h"
#include "ISceneNode.h"
#include "aabbox.hpp"


class btRigidBody;
class btCollisionShape;
class btDefaultMotionState;
namespace BohgeEngine
{
	static const vector3f UP(0,1,0);
	static const vector3f FORWARD(1,0,0);

	class LocatableObject
	{
	private:
		IAttributeNode* const	m_pHost;
	private:
		bool					m_isChange;//�Ƿ�仯��

		mutable bool			m_isPositionChange;
		vector3f				m_LocalPosition;//λ��

		mutable bool			m_isRotationChange;
		Quaternionf				m_LocalRotation;//��ת

		mutable bool			m_isScaleChange;
		vector3f				m_LocalScale;//����

		mutable vector3f		m_WorldPosition;//λ��
		mutable Quaternionf		m_WorldRotation;//��ת
		mutable vector3f		m_WorldScale;//����

		mutable bool			m_isWorldTransfromChange;
		mutable Matrix44f		m_WorldTransfrom;//����
		mutable bool			m_isWorldRotationChange;
		mutable Matrix33f		m_WorldRotationMatrix;//��ת����

		mutable bool			m_isWorldTransfromInversChange;
		mutable Matrix44f		m_WorldTransfromInvers;//����
	private:
		void _WorldTransfromUpdata();
	public:
		explicit LocatableObject( IAttributeNode* const host );
		virtual ~LocatableObject(void);
	public:
		virtual void Update( uint milisecond );//����и���
		virtual void SetLocalRotation( const Quaternionf& r );
		//���ҽӵ�ʱ�򸸽ڵ�����Ż�Ӱ���ֽڵ����λ��
		virtual void SetLocalPosition( const vector3f& p );
		virtual void SetLocalScale( const vector3f& s );
	public:
		const vector3f& GetWorldPosition() const;
		const vector3f& GetWorldScale() const;
		const Quaternionf& GetWorldRotation() const;
		const Matrix44f& GetWorldTransform() const;
		const Matrix33f& GetWorldRotationMatrix() const;
		const Matrix44f& GetWorldTransformInvers() const;
	public:
		const void* LocatableAttribute(const IAttributeNode::Node_Attribute& name) const;
	protected:
		BOHGE_FORCEINLINE void _SetChangeFlag( bool b )
		{
			m_isChange = b;
		}
	public:
		BOHGE_FORCEINLINE IAttributeNode* GetAttributeHost( ) const
		{
			return m_pHost;
		}
		BOHGE_FORCEINLINE bool isChange( ) const
		{
			return m_isChange;
		}
		BOHGE_FORCEINLINE const vector3f& GetLocalPosition( ) const
		{
			return m_LocalPosition;
		}
		BOHGE_FORCEINLINE const vector3f& GetLocalScale( ) const
		{
			return m_LocalScale;
		}
		BOHGE_FORCEINLINE const Quaternionf& GetLocalRotation( ) const
		{
			return m_LocalRotation;
		}
	};





	class PhysicalObject : public LocatableObject
	{
	protected:
		btRigidBody*			m_pBody;
		btCollisionShape*		m_pShape;
		btDefaultMotionState*	m_pMotion;
	public:
		PhysicalObject ( IAttributeNode* const host );
		virtual ~PhysicalObject();
	protected:
		void _InitPhysical( aabbox3df& colShape, float mass, const vector3f& localInertia );
	public:
		virtual void Update( uint milisecond );//����и���
		virtual void SetLocalRotation( const Quaternionf& r );//��ת����ģ��ԭʼ�ĵĻ�������ת����ת�������
		virtual void SetLocalPosition( const vector3f& p );
		virtual void SetLocalScale( const vector3f& s );
	};

}