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

#include "3DMath.h"
#include "aabrect.hpp"
#include "Device.h"
#include "ISceneNode.h"
#include "Bfont.h"
#include "Event.hpp"
#include "UIManager.h"
#include <vector>
#include <list>
#include <string>



namespace BohgeEngine
{
	class ScriptBase;
	class UIGroup;
	class VertexBuffer;
	class IndicesBuffer;
	class UIBase : public IGetBuffer
	{
	public:
		const static int Invalid;
	protected:
		enum Const
		{
			COUNT_OF_VERTEX		= 4,
			COUNT_OF_INDIES		= 6,
		};
	protected:
		typedef std::list< std::pair< ScriptBase*, std::string> >	LuaCallBackList;
	private:
		int					m_Identification;
		bool				m_isShow;//�Ƿ���ʾ
		bool				m_isResponse;//�԰�������Ӧ��UI
		bool				m_isTouched;//��ǰ������
		bool				m_isDonw;//�����º�Ϊtrue������OnMouseDown�ĵ��ô˴������ٿ���
		std::string			m_Name;//UI����
		UIManager::Type		m_eType;//UI����
		UIManager::Layer	m_eLayer;//��
	protected:
		bool				m_isUpdata;//�Ƿ���Ҫ��
		bool				m_isActive;//�Ƿ�
		IUIFather*			m_Father;	//ӵ������Father������Ӧ����ָ��
		vector2f			m_Center;//���Ľڵ�
		vector2f			m_Size;//��С
		BString*			m_pString;//�ı�
		Color				m_StringColor;
		VertexTextureColor*	m_pVertexArray;
		aabrectf*			m_pBox;//��ײ���
		vector2f			m_vScale;
	protected:
		//begin----uve
		//|			|
		//|			|
		//|			|
		//uvb------end
		UIBase(IUIFather* father, bool isResponse, const UIData& data);
	public:
		virtual ~UIBase();
	public:
		void MakeBindBox();//UITool���ã����ɰ󶨺���
		void SetCenter(const vector2f& center);//��ֻUI��Group�����ģ�����UIGroup��SetCenter��ͬ�����������UI�Լ������ģ���UIGroup�Ľ�UI����ƫ��
		void SetCenterForce(const vector2f& center);//����һ������ˢ�µ�����
		void SetScale(const vector2f& scale);
		void Move(const vector2f& dis);//�ƶ�����UIһ�ξ���
		bool isSelected(bool isDown, const vector2f& point);//�Ƿ�ѡ��, isReflush��ʾ�Ƿ���Ҫˢ��buffer, isDown��ʾ�ǰ��»���̧�����
		bool isUI(const std::string& name) const;//�Ƿ������UI
		void SetString(const vector2f& fontsize, const Color& clr, bool isCover, const std::wstring& wstr);//isCover������������Ƿ�����UI�Լ����ƣ�������UI���ܻᱻ����UI���ǣ�������ѡ��true
		void SetString(const vector2f& fontsize, const Color& clr, bool isCover, const char* file );//ΪLua�����
		void SetShow(bool show);
		void Reset();//���ã����µ���Ϣ����
		void SetStringShow(bool isShow);
	public://UIGroup����
		virtual void Update();//UIGroup���ã�ˢ���ڲ�����
		virtual void OnMouseDown(const vector2f& pos){}//����ʱ�ı�����
		virtual void OnMouseMove(const vector2f& pos){}//���º��������ƶ�,���ݵ������λ��[-1,1]
		virtual void OnMouseUp(const vector2f& pos){}//�����Ǹı�����
		virtual void OnAction(const vector2f& pos){}//����
		virtual void OnDraw(Engine& engine, const Color& clr, const Texture* tex);
	public:
		BOHGE_FORCEINLINE bool isUpdata()
		{
			return m_isUpdata;
		}
	public://�̳еĽӿ�
		virtual void SetActive(bool isActive);//�Ƿ�
		virtual void GetVertexBuffer(SharedBuffer& input) const ;
		virtual uint GetCountOfNode();//���ص�ǰ��node����
	public:
		BOHGE_FORCEINLINE const int& GetIdentification() const//��ȡʶ���
		{
			return m_Identification;
		}
		BOHGE_FORCEINLINE void SetIdentification( const int& id )//����ʶ���
		{
			m_Identification = id;
		}
		BOHGE_FORCEINLINE UIManager::Type UIType()
		{
			return m_eType;
		}
		BOHGE_FORCEINLINE bool isShow()
		{
			return m_isShow;
		}
		BOHGE_FORCEINLINE bool isResponse()
		{
			return m_isResponse;
		}
		BOHGE_FORCEINLINE bool isPushDonw()
		{
			return m_isDonw;
		}
		BOHGE_FORCEINLINE bool isActive()
		{
			return m_isActive;
		}
		BOHGE_FORCEINLINE UIManager::Layer GetLayer()
		{
			return m_eLayer;
		}
		BOHGE_FORCEINLINE aabrectf GetBindBox()
		{
			if ( NULL != m_pBox )
			{
				return m_vScale.m_x >0.99f && m_vScale.m_y >0.99f ? *m_pBox : *m_pBox * m_vScale;
			} 
			else
			{
				return aabrectf( );
			}
		}
		BOHGE_FORCEINLINE const vector2f& GetSize()
		{
			return m_Size;
		}
		BOHGE_FORCEINLINE const vector2f& GetCenter()
		{
			return m_Center;
		}
		BOHGE_FORCEINLINE void SetStringPostion( const vector2f& input)
		{
			if( NULL != m_pString )
			{
				m_pString->SetPosition( input, BString::Center );
			}
		}
		BOHGE_FORCEINLINE void SetStringPostion( const vector2f& input, BString::Alignment ali)
		{
			if( NULL != m_pString )
			{
				m_pString->SetPosition( input, ali );
			}
		}
		BOHGE_FORCEINLINE const BString& GetString()
		{
			return *m_pString;
		}
	public:
		static uint GetElementCountPerVertex();//�õ�ÿ������floatԪ�ظ���
		static BOHGE_FORCEINLINE uint GetCountOfVertex()
		{
			return COUNT_OF_VERTEX;
		}
		static BOHGE_FORCEINLINE uint GetCountOfIndes()
		{
			return COUNT_OF_INDIES;
		}
		static BOHGE_FORCEINLINE Device::RenderMode GetRenderMode()
		{
			return Device::RM_TRIANGLES;
		}
	};



	class UIStatic : public UIBase
	{
	public:
		UIStatic(IUIFather* father, const UIData& data);
		~UIStatic();
	};


	class UIButton : public UIBase
	{
	private:
		CoordinatePair			m_CoordNormal;
		CoordinatePair			m_CoordHighLight;
		Event<bool,UIBase&>		m_Event;//��Ӧ����
		LuaCallBackList			m_LuaCallBack;//lua��callback����
	public:
		UIButton(IUIFather* father, const UIData& data);
		~UIButton();
	protected:
		virtual void OnMouseDown(const vector2f& pos);
		virtual void OnMouseUp(const vector2f& pos);
		virtual void OnAction(const vector2f& pos);
	public:
		virtual void SetActive(bool isActive);
	public:
		void AddResponse(IFunction<bool,UIBase&>* const func);
		void AddResponse( ScriptBase* scrip, const char* func );//Luaʹ�õ�callback,c++��ֹʹ��
	};


	class UISlider : public UIBase
	{
	private:
		float					m_fPercentage;//�������ٷֱ�
		float					m_fMinPercentage;
		uint					m_SliderCount;
		uint					m_VisuableSliderCount;
		CoordinatePair			m_CoordElement;
		vector2f				m_ElementSize;
		VertexTextureColor*		m_pSliderVertexArray;
		Event<bool,UIBase&>		m_Event;//��Ӧ����
		LuaCallBackList			m_LuaCallBack;//lua��callback����
	public:
		UISlider(IUIFather* father, const UIData& data);
		~UISlider();
	protected:
		virtual void OnMouseMove(const vector2f& pos);
		virtual void OnAction(const vector2f& pos);
		virtual void Update();//UIGroup���ã�ˢ���ڲ�����
	public://�̳еĽӿ�
		virtual void SetActive(bool isActive);
		virtual void GetVertexBuffer(SharedBuffer& input) const ;
		virtual uint GetCountOfNode();//���ص�ǰ��node����
	public:
		void AddResponse(IFunction<bool,UIBase&>* const func);
		void AddResponse( ScriptBase* scrip, const char* func );//Luaʹ�õ�callback,c++��ֹʹ��
		void SetPercentage(clamp percentage);
	public:
		BOHGE_FORCEINLINE float Percentage () //�õ��ٷֱ�
		{
			return m_fPercentage;
		}
		BOHGE_FORCEINLINE void SetMinPercentage( clamp percentage )//��С��ֵ
		{
			m_fMinPercentage = Math::Clamp0to1( percentage );
		}
	};


	//list ר�õ�
	class IListItem
	{
		friend class UIList;
	public:
		virtual ~IListItem(){}
	private:
		virtual void OnVisable() = 0;//������ʾ��Χ
		virtual void OnInvisable() = 0;//�Ƴ���ʾ��Χ
		virtual void SetPostion(const vector2f& ItemCenter) = 0;
		virtual void Draw(Engine& engine) = 0;
	};
	class UIList : public UIBase , public IUIFather//���Ҳ��һ��UIFather�����ڲ���staticˢ��
	{
	private:
		typedef std::list< IListItem* > ItemList;//������������
		typedef std::vector< std::pair<UIStatic*,ItemList::iterator> >	ContainerVector;
	private:
		uint					m_ContainerCount;//��Ҫ��Item����������ಢ�������ÿһ��Item����һ���ɻ��Ƶ�Item��������һ����Ҫ����С����Ȼ��ѭ������
		vector2f				m_ListItemSize;//list����Ҫ��Item�ĳߴ�
		Event<bool,IListItem&>	m_Event;//��Ӧ����
		LuaCallBackList			m_LuaCallBack;//lua��callback����

		ItemList				m_ItemList;//��������
		ContainerVector			m_ContainerVector;//���������ɻ��Ƶ�
		int						m_CurrentIndex;//��ǰ�ĵ�һ����������
		SharedBuffer*			m_pBuffer;
		RendBuffer*				m_pRender;
		VertexBuffer*			m_pVertexBuffer;
		IndicesBuffer*			m_pIndicesBuffer;
		RendBuffer*				m_pStencilRender;
		VertexBuffer*			m_pStencilVertexBuffer;
		IndicesBuffer*			m_pStencilIndicesBuffer;

		bool					m_isUp;//����ƶ��ķ����Ƿ�����
		float					m_LastMouseY;//�������Yֵ
		bool					m_isAction;//ʱ���ǵ���¼�
		float					m_DownMouseY;//����ʽ�����λ��
	public:
		UIList(IUIFather* father, const UIData& data);
		~UIList();
	private:
		void FlushItem();
	protected:
		virtual void OnMouseDown(const vector2f& pos);
		virtual void OnMouseMove(const vector2f& pos);
		virtual void OnAction(const vector2f& pos);//����
		virtual void OnDraw(Engine& engine, const Color& clr, const Texture* tex);
		virtual void Update();
	public:
		virtual void SetActive(bool isActive);
		virtual void Renew();
	public:
		void AddResponse(IFunction<bool,IListItem&>* const func);
		void AddResponse( ScriptBase* scrip, const char* func );//Luaʹ�õ�callback,c++��ֹʹ��
		void AddItem(IListItem* item);
		void ShowLastItem();//ֱ����ʾ���һ��Item
		void Clear();
	};


	class UIEdit : public UIBase //�����
	{
	private:
		bool			m_isNumOnly;
		uint			m_MaxInput;	//�������ĸ���
		vector2f		m_FontSize;
		Color			m_FontClr;
		std::wstring	m_Content;//���������
		vector2f		m_FontOffset;//�ı�λ�õ�ƫ��
		RendBuffer*		m_pStencilRender;
		VertexBuffer*	m_pStencilVertexBuffer;
		IndicesBuffer*	m_pStencilIndicesBuffer;
		Event<bool,const std::wstring&>	m_OnReciveText;
	public:
		UIEdit(IUIFather* father, const UIData& data);
		~UIEdit();
	protected:
		virtual void OnAction(const vector2f& pos);//����������ϵͳ���̣���������
		virtual void OnDraw(Engine& engine, const Color& clr, const Texture* tex);
	public:
		void AddResponse(IFunction<bool,const std::wstring&>* const func);
		bool OnReciveText(const std::wstring& str);
	public:
		BOHGE_FORCEINLINE std::wstring& GetContent()
		{
			return m_Content;
		}
		BOHGE_FORCEINLINE void Clear()
		{
			m_Content.clear();
			if( NULL != m_pString )
			{
				Engine::Instance().GetStringManager()->DestroyString( m_pString );
				m_pString = NULL;
			}
		}
		BOHGE_FORCEINLINE void SetFontSize( const vector2f& size )
		{
			m_FontSize = size;
		}
		BOHGE_FORCEINLINE void SetFontColor( const Color& clr )
		{
			m_FontClr = clr;
		}
		BOHGE_FORCEINLINE void SetMaxInput( uint m )//���Ŀ�������ַ�����,0��û������
		{
			m_MaxInput = m;
		}
		BOHGE_FORCEINLINE void NumOnly( bool b )
		{
			m_isNumOnly = b;
		}
		BOHGE_FORCEINLINE void FontOffset( const vector2f& off )
		{
			m_FontOffset = off;
		}
	};
}