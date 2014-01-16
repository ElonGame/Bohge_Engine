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
//			����		   //
/////////////////////////////

#pragma once

#include <cstdlib>
#include <string>
#include <vector>

#include "VariableType.h"
#include "3DMath.h"

namespace BohgeNet
{
	class NetHost;
}
namespace BohgeEngine
{
	class Device;
	class SoundManage;
	class ActionManage;
	class ShaderManage;
	class SceneManage;
	class UIManager;
	class ICamera;
	class Environment;
	class StringManager;
	class ResourceManager;
	class PhysicManager;
	class ScriptManager;
	class Pipeline;
	class MaterialManager;
	class Engine
	{
		//�������ʱ������engine����
	private:
		ulong			m_tBeginTiem;//��ʼ��ʱ��
		ulong			m_tLastTime;
		uint			m_uTimeSpan;
		uint			m_uFPS;
	public:
		BOHGE_FORCEINLINE int GetTimeSpan()
		{
			return m_uTimeSpan;
		}
		BOHGE_FORCEINLINE int GetFPS()
		{
			return m_uFPS;
		}
	public://�豸�ȼ�
		enum DeviceLevel//�豸��ͼ�ο��ȼ����漰��һЩshader�ļ���
		{
			LEL_LOW,
			LEL_BASE,
			LEL_MEDIUM,
			LEL_HIGH,
			LEL_ULTRA,
		};
	public:
		friend class ICamera;
	private:
		DeviceLevel			m_eDeviceLevel;
		Device*				m_pDevice;
		Environment*		m_pEnvironment;	//������
		bool				m_isUseLightCamera;//�����Ƶƹ�����ͼ��ʱ�л�
		ICamera*			m_pViewCamera;	//���������
		ICamera*			m_pActivCamera;	//��ǰ�����������������ǻ��������ͶӰ�����
		ActionManage*		m_pAction;	//�����������
		ShaderManage*		m_pShader;	//shader����
		SceneManage*		m_pSecen;	//���ƹ���
		SoundManage*		m_Sound;	//����
		UIManager*			m_pUI;		//UI
		StringManager*		m_pString;	//�ı�
		//EmitterManager*		m_pParticle;//���ӹ�����
		Pipeline*			m_pPipeline;//��Ⱦ����
		ResourceManager*	m_pResource;//��Դ�ļ�����
		PhysicManager*		m_pPhysic;//����
		ScriptManager*		m_pScript;//�ű�
		MaterialManager*	m_pMaterial;//�����Զ�������
		BohgeNet::NetHost*	m_pNet;//������ص�
	private:
		void Initialization(const vector2d& viewport, const vector2d& resolution, DeviceLevel l);
		void Drop();
		void _AttachCamera( ICamera* cptr );
	public:
		void ResizeViewport( const vector2d& size );
		void OnFramebegin();
		void Update();	//����engine�еĸ���manage
		void Render();
		uint CurrentTime();
		BOHGE_FORCEINLINE void AttachLightCamera ( ICamera* cptr )
		{
			m_isUseLightCamera = true;
			m_pActivCamera = cptr;
		}
		BOHGE_FORCEINLINE void DetachLightCamera()
		{
			m_isUseLightCamera = false;
			m_pActivCamera = m_pViewCamera;
		}
		BOHGE_FORCEINLINE Environment* GetEnvironment()
		{
			return m_pEnvironment;
		}
		BOHGE_FORCEINLINE uint Time()//�������е�ʱ�䳤�ȣ�����
		{
			return m_tLastTime;
		}
		BOHGE_FORCEINLINE DeviceLevel GetDeviceLevel()
		{
			return m_eDeviceLevel;
		}
		BOHGE_FORCEINLINE Device* GetDevice()
		{
			return m_pDevice;
		}
		BOHGE_FORCEINLINE ResourceManager* GetResourceManage()
		{
			return m_pResource;
		}
		BOHGE_FORCEINLINE ScriptManager* GetScriptManage()
		{
			return m_pScript;
		}
		BOHGE_FORCEINLINE Pipeline* GetPipeline()
		{
			return m_pPipeline;
		}
		BOHGE_FORCEINLINE SceneManage* GetSceneManage()
		{
			return m_pSecen;
		}
		BOHGE_FORCEINLINE ICamera* GetCamera()
		{
			return m_pActivCamera;
		}
		BOHGE_FORCEINLINE ActionManage* GetActionManage()
		{
			return m_pAction;
		}
		BOHGE_FORCEINLINE ShaderManage* GetShaderManage()
		{
			return m_pShader;
		}
		BOHGE_FORCEINLINE MaterialManager* GetMaterialManager()
		{
			return m_pMaterial;
		}
		BOHGE_FORCEINLINE SoundManage* GetSoundManage()
		{
			return m_Sound;
		}
		BOHGE_FORCEINLINE UIManager* GetUIManager()
		{
			return m_pUI;
		}
		BOHGE_FORCEINLINE StringManager* GetStringManager()
		{
			return m_pString;
		}
		BOHGE_FORCEINLINE PhysicManager* GetPhysicManager()
		{
			return m_pPhysic;
		}
		//BOHGE_FORCEINLINE EmitterManager& GetEmitterManager()
		//{
		//	return *m_pParticle;
		//}
		BOHGE_FORCEINLINE BohgeNet::NetHost* GetNet()
		{
#ifdef _DEBUG
			if ( NULL == m_pNet ) ASSERT(false);
#endif
			return m_pNet;
		}
		//����Ϊ�ڲ�����������
	private:
		static Engine*		m_pEngine;
	private:
		Engine();
		Engine( const Engine& r){};
		Engine& operator= ( const Engine& r){};
	public:
		~Engine();
		static Engine* CreateEngine(const vector2d& viewport, const vector2d& resolution, DeviceLevel l);
		static void DropEngine(Engine** engine);
		static BOHGE_FORCEINLINE Engine& Instance()
		{
			return *m_pEngine;
		}
	};
}