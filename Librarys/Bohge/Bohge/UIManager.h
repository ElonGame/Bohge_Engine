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
//		UI������	   //
/////////////////////////
#pragma once
#include "aabrect.hpp"
#include "3DMath.h"
#include "Event.hpp"
#include <string>
#include <vector>
#include <list>


namespace BohgeEngine
{
	typedef std::pair<vector2f, vector2f> CoordinatePair;
	typedef std::vector< CoordinatePair > CoordinateVector;

	class UIBase;
	class UIData;
	class Engine;
	class Texture2D;
	class SharedBuffer;
	class RendBuffer;
	class UIGroup;
	class UIAnimation;
	class VertexBuffer;
	class IndicesBuffer;
	class UIManager
	{
		friend class Engine;
		friend class ActionManage;
		friend class Pipeline;
		friend class UIGroup;
	public:
		enum Type
		{
			UI_STATIC,
			UI_BUTTON,
			UI_SLIDER,
			UI_LIST,
			UI_EDIT,
			Type_Count,
		};
		enum Layer //���ƵĲ�
		{
			UL_TOP,
			UL_BOTTOM,
			Layer_Count,
		};
	private:
		typedef std::list<UIGroup*>	GroupList;
	private:
		bool                            m_isKeyboard;
		GroupList						m_GroupList;
		Event<bool,const std::wstring&>	m_eEditCallBakc;
	public:
		UIManager();
		~UIManager();
		std::string GetTypeName(int type);
		UIManager::Type GetType(int index);
		std::string GetLayerName(int type);
		UIManager::Layer GetLayer(int index);
	private:
		void Update(uint millisecond);//updataÿ��UI��ˢ��ʲô��
		void Render(Engine& engine, Layer layer);
		bool TouchResponse(bool isDown, const vector2f& point);//UI��Ӧ
		void TopMost(UIGroup* ui);//��������public://�ⲿ�����ӿ�
	public:
		UIBase* Factory(UIGroup* group, const UIData& data);
		void PauseResponseAll();
		void ResumeResponseAll();
		UIGroup* CreateUIGroup( const char* file );//�������UIEdit�У�����Ϊ���ֽṹҲ��̫�ã�Ӧ�÷ŵ�һ����ʱ������
		void DeleteUIGroup( UIGroup* ui );//�ͷ�
	public:
		void OnActiveKeyborad(UIBase* needer, const vector4f& rect, uint maxsize, const std::wstring& was, bool isNumberOnly);//��UI���ã���Ҫ����������
		void OnReceiveText(const std::wstring& text);
	};





	class IUIFather //������Ϊ���ڵ㣬��ӦRennew����,�Ƿ��ڻ���isShow
	{
	public:
		virtual void Renew() = 0;
		//virtual bool isShow() = 0;
	};

	bool CheckName(const UIBase& ui, const std::string& name);//���android��ֲ����
	class UIGroup : public IUIFather//UI�壬managerÿ��load�Ͳ���һ��UIȺ��
	{
		friend class UIManager;
	private:
		struct RendStruct
		{
			SharedBuffer*	m_pShared;
			RendBuffer*		m_pRender;
			VertexBuffer*	m_pVertex;
			IndicesBuffer*	m_pIndices;
			explicit RendStruct()
				:m_pShared(NULL),
				m_pRender(NULL),
				m_pVertex(NULL),
				m_pIndices(NULL)
			{}
			~RendStruct();
		};
	private:
		typedef std::vector<UIBase*> UIVector;
		typedef std::vector< RendStruct* > RendStructVector;
	private:
#ifdef _DEBUG
		std::string				m_Name;
#endif // _DEBUG
		bool						m_isManaged;//�Ƿ��йܻ���
		bool						m_isShow;//�Ƿ���ʾ���UIGroup,����ʾ�Ͳ����¶���
		bool						m_isResponse;//�Ƿ���ͣ��Group����Ӧ
		uint						m_PassedFrame;//�����߹���֡��
		bool						m_isAnimation;//�Ƿ��ж���
		UIAnimation*				m_pAnimation;
		bool						m_isSetAnimation;//�Ƿ����õ��¶���
		IFunction<bool,UIGroup&>*	m_pFuncPtrTemp;//��ʱ��ָ�룬��ֹ�ڶ��������Ļص������У�����һ���µ�Animation���µ�����ʹ�õ�ָ�뱻�ı�ȴ���
		Event<bool,UIGroup&>		m_Event;//����������ϵ���Ӧ����
		aabrectf*					m_pBox;//����UI����ײ���
		RendStructVector			m_pUIRendStructVector;//����buffer��Vector
		UIVector					m_UIVectorArray[UIManager::Layer_Count];//UI������
		Texture2D*					m_pTexture;//����
		vector2f					m_Center;//UI���ģ�Ĭ��Ϊ��Ļ����
		Color						m_Color;//������ɫ
		vector2f					m_vScale;//����
		bool						m_isReflush;
	private:
		//UI�����ļ���������ָ��
		UIGroup(const std::string& filename);
		~UIGroup();
	private:
		void Update(uint millisecond);//updataÿ��UI��ˢ��ʲô��
		void AddUI(UIBase* ui);
		void Render(Engine& engine, UIManager::Layer layer);
		bool TouchResponse(bool isDown, const vector2f& point);//�Ƿ����������ӦUI������
		void MakeBuffer();//����һ��Buffer
#ifdef _DEBUG
		BOHGE_FORCEINLINE void SetName(const std::string& name)
		{
			m_Name = name;
		}
#endif // _DEBUG
	public://����Ϊ�ⲿ���Է��ʵĹ��ýӿ�
		void SetCenter(const vector2f& center);//����UIGroup������λ�ã��������йأ�UIGroup��SetCenter�ǽ�UIȺ�������
		void SetScale(const vector2f& scale);//��������
		void SetShow(bool b);//������showΪfalse��ʱ������ж�������ֹͣ���¶���
		void setResponse(bool response);//ֹͣ������Ӧ��ֹͣ��ʾ����
		void SetUIAnimation(const UIAnimation& animation, IFunction<bool,UIGroup&>* const func );
		void ResetAnimation();
		void TopMost();//�ö�
	public:
		virtual void Renew();
		BOHGE_FORCEINLINE void SetCorlor(const Color& clr)
		{
			m_Color = clr;
		}
		BOHGE_FORCEINLINE bool isResponse()
		{
			return m_isResponse;
		}
		BOHGE_FORCEINLINE bool isAnimation()
		{
			return m_isAnimation;
		}
		BOHGE_FORCEINLINE bool isShow()
		{
			return m_isShow;
		}
		BOHGE_FORCEINLINE const vector2f& GetPostion()
		{
			return m_Center;
		}
		BOHGE_FORCEINLINE UIAnimation& GetUIAnimation()
		{
			return *m_pAnimation;
		}
		BOHGE_FORCEINLINE void PauseAnimation()
		{
			m_isAnimation = false;
		}
		BOHGE_FORCEINLINE bool isManaged()
		{
			return m_isManaged;
		}
		BOHGE_FORCEINLINE void SetManaged(bool b)
		{
			m_isManaged = b;
		}
		BOHGE_FORCEINLINE void Draw(Engine& engine)
		{
			this->Render(engine, UIManager::UL_TOP);
		}
	public:
		template <typename T> T* Control(const char* name)
		{
			for(int i = 0 ; i < UIManager::Layer_Count ; i ++ )
			{
				for( UIVector::iterator it = m_UIVectorArray[i].begin() ;
					it != m_UIVectorArray[i].end() ;
					it++ )
				{
					if ( CheckName(**it, name) )
					{
						ASSERT( NULL != dynamic_cast<T*>(*it) );//���Ҫ��ת����ָ�벻��Ŀ��ָ�룬�˴��ᱨ��
						return static_cast<T*>(*it);
					}
				}
			}
			ASSERT( false );
			return NULL;
		}
		template <typename T> T* Control(UIManager::Layer layer, int index)
		{
			ASSERT( NULL != dynamic_cast<T*>( m_UIVectorArray[static_cast<int>(layer)][index] )  );
			return static_cast<T*>(m_UIVectorArray[static_cast<int>(layer)][index]);
		}
	};


	class Serializer;
	class UIData	//���湹��һ��UI����Ҫ������
	{
	public:
		std::string						m_Name;
		UIManager::Type				m_Type;
		UIManager::Layer			m_Layer;
		vector2f					m_Center;
		CoordinatePair				m_Tex1;
		CoordinatePair				m_Tex2;
		vector2f					m_Size;//���size��00-11�ķ�Χ��
		//float						m_Reservation1;//Ԥ��������
		//float						m_Reservation2;
		vector2f					m_Tex2Size;//�ڶ�������Ĵ�С��00-11�ķ�Χ��
		float						m_Reservation3;
		float						m_Reservation4;
		float						m_Reservation5;
		float						m_Reservation6;
		float						m_Reservation7;
		float						m_Reservation8;
		float						m_Reservation9;
		float						m_Reservation10;
		float						m_Reservation11;
		float						m_Reservation12;
		float						m_Reservation13;
		float						m_Reservation14;
		float						m_Reservation15;
		float						m_Reservation16;
	public:
		UIData();
		void Save(Serializer& file);
		void Load(Serializer& file);
	};
}