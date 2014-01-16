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



///////////////////////////////////////////
//�ֲ�klay�е�SceneObject��֧�ֱ任������//
///////////////////////////////////////////
#pragma once
#include <KlayGE/KlayGE.hpp>
#include <KlayGE/Util.hpp>
#include <KlayGE/ThrowErr.hpp>
#include <KlayGE/Math.hpp>
#include <KlayGE/Font.hpp>
#include <KlayGE/RenderableHelper.hpp>
#include <KlayGE/SceneObjectHelper.hpp>
#include <KlayGE/RenderEngine.hpp>
#include <KlayGE/FrameBuffer.hpp>
#include <KlayGE/SceneManager.hpp>
#include <KlayGE/Context.hpp>
#include <KlayGE/ResLoader.hpp>
#include <KlayGE/RenderSettings.hpp>
#include <KlayGE/Mesh.hpp>
#include <KlayGE/RenderEffect.hpp>
#include <KlayGE/Window.hpp>
#include <KlayGE/PostProcess.hpp>
#include <KlayGE/Camera.hpp>
#include <KlayGE/Light.hpp>
#include <KlayGE/DeferredRenderingLayer.hpp>
#include <KlayGE/XMLDom.hpp>


using namespace KlayGE;


class SceneObjectMoveable : public SceneObjectHelper
{
protected:
	AABBox			m_box;
	bool			m_isNeedUpdate;//�Ƿ���±任����
	float3			m_Forward;//����
	float3			m_Back;//���˷���
	float3			m_Up;//Up��

	Quaternion		m_Rotation;//��ת
	float3			m_Postion;//λ��
	float3			m_Scale;//����

	float			m_MoveSpeed;//�ƶ��ٶ� m/s
	float			m_RotationSpeed;//��ת�ٶ� r/s

	bool			m_isBeginBack;//�Ƿ�ʼ����
	float			m_BackTime;//���˵�ʱ��
public:
	explicit SceneObjectMoveable(float3 const & forward, float3 const & up );
	virtual ~SceneObjectMoveable()
	{
	}
public:
	//���ص��麯��������������£����ܻ��������
	virtual void Update(float app_time, float elapsed_time);
protected:
	//�Զ��ƶ���ָ����Ŀ�꣬���������������ת��ǰ��forward����
	void ProcessMove( float3 const & dir, float sec);
	//������ã���������ǰ���Ǻ��ˣ��Ƿ������ת
	void ProcessAvoid(float3 const & dir, float sec);
public:
	//����һ��λ��
	virtual void SetPostion(float3 const & pos);
	//�����ⷽ��ƽ�ƣ��������������������ϵ�е�����
	virtual void Move(float3 const & direction, float distance);
	//�ڵ�ǰ�ĳ��������ƶ�һ������
	virtual void MoveForward(float distance);
	//�ڵ�ǰ�ĳ��򷴷��������ƶ�һ������
	virtual void MoveBack(float distance);
	//����ģ�͵����������е�����ת���ȴ˴������direction���ں����г���ת���䵽ģ������ϵ
	virtual void MoveOnModelWorld(float3 const & direction, float distance);
	//����������ת���������������������ϵ�е�����
	//virtual void Rotaion(float3 const & axis, float radian);
	//����һ����ǰ�ƶ�����ģ�ͻᰴ�����������תһ���Ƕ�,������ת�ĽǶ�
	virtual void Rotaion( float3 const & faceto, float3 const & moveDir, float timespan);
	//��Up����ת
	virtual void RotaionByUp(float radian);
	//����ģ��
	virtual void Scale(float3 const & scale);
	AABBox const & BoundLocal() const;
public:
	inline const float3& GetPostion()
	{
		return m_Postion;
	}
	inline const float3& GetForward()
	{
		return m_Forward;
	}
	inline void SetMoveSpeed( float ms )
	{
		m_MoveSpeed = ms;
	}
	inline void SetRotateSpeed( float rs )
	{
		m_RotationSpeed = rs;
	}
};