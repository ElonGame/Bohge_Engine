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
//		���ƹ���	   //
/////////////////////////

#pragma once

#include <list>
#include <vector>
#include "3DMath.h"
#include <string>
#include <map>

class SkyBox;
class IDriver;
class Color;
class Environment;
class IShadowable;
class IBeShadowable;
class IRenderNode;
class SceneMain;
class ITransparent;
class IUpdateNode;
class EmitterManager;
class SecenManage
{
	friend class Emitter;
	friend class IUpdateNode;
	friend class IRenderNode;
private:
	typedef std::list< IRenderNode* > ListRenderable;	//�ɻ��ƵĽڵ�
	typedef std::list< IUpdateNode* > ListUpdateable;	//��Ҫ���µĽڵ�
	//typedef std::list<IShadowable*> ListShadowable;	//�ɲ�����Ӱ�Ľڵ�
	//typedef std::list<IBeShadowable*> ListBeShadowable;	//�ɱ���ӰӰ��Ľڵ�
	typedef std::vector<IRenderNode*> NeedRenders;//��Ҫ���Ƶ�
private:
	SkyBox*				m_pSky;
	//ListShadowable		m_lShadowable;
	//ListBeShadowable	m_lBeShadowable;
	ListRenderable		m_lRenderable;
	ListUpdateable		m_lUpdateable;

	//�޳���ص�
	NeedRenders			m_NeedRenders;//��Ҫ���Ƶģ�û�б��޳���
	NeedRenders			m_ShadowRenders;//��Ҫ���ƵĲ�����Ӱ��
	//�༭���ĳ���
	SceneMain*			m_pWorld;
	//����
	EmitterManager*		m_pEmitters;
	//û����ȵ�
	std::map<float, ITransparent*, std::greater<float> >		m_TransparentMap;
public:
	SecenManage();
	~SecenManage();
	void RenderBackground(IDriver& driver);
	void RenderSkyBox(IDriver& driver);
	void OnBeginUpdate( IDriver& driver );
	void Update( uint milisecond );//���³����������Ƿ���ư����������������󶨰�
	void Render(IDriver& driver);
	void RenderDepth(IDriver& driver);
	void RenderReflection(IDriver& driver);
	//void RenderShadow(IDriver& driver);
	//void RemoveShadowObject(IShadowable* input);
	//void RemoveBeShadowObject(IBeShadowable* input);
	void LoadScene( const std::string& path );//��ȡһ������
	float GetWaterMiddleHeight();
private:
	BOHGE_FORCEINLINE EmitterManager& GetEmitterManage()
	{
		return *m_pEmitters;
	}
private:
	BOHGE_FORCEINLINE void AttachObject(IRenderNode* rn)
	{
		m_lRenderable.push_back( rn );
	}
	BOHGE_FORCEINLINE void RemoveObject(IRenderNode* rn)
	{
		m_lRenderable.remove(rn);
	}
	BOHGE_FORCEINLINE void AttachObject(IUpdateNode* rn)
	{
		m_lUpdateable.push_back( rn );
	}
	BOHGE_FORCEINLINE void RemoveObject(IUpdateNode* rn)
	{
		m_lUpdateable.remove(rn);
	}
public:
	//BOHGE_FORCEINLINE void AddShadowObject(IShadowable* input)
	//{
	//	m_lShadowable.push_back(input);
	//}
	//BOHGE_FORCEINLINE void AddBeShadowObject(IBeShadowable* input)
	//{
	//	m_lBeShadowable.push_back(input);
	//}
	BOHGE_FORCEINLINE SkyBox& GetSkyBox()
	{
		return *m_pSky;
	}
	BOHGE_FORCEINLINE bool isCastShadow()
	{
		return !m_ShadowRenders.empty();
	}
	BOHGE_FORCEINLINE SceneMain& GetWorld()
	{
		return *m_pWorld;
	}
};
