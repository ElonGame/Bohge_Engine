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
//			����ϵͳ���ӷ�����		   //
/////////////////////////////////////////
#pragma once
#include "ISceneNode.h"
#include "aabbox.hpp"
#include "3DMath.h"
#include "LocatableObject.h"
#include "ScriptBase.h"
#include <string>



namespace BohgeEngine
{
	//���ӻ��Ƶ�ʱ����м�ϵĸо�������ӦΪ�������Ƶ���һ������ʱ�����ȱ����ƣ�������alpha���򸲸ǵĺ�û������ɫ������
	//Ȼ���ٻ���
	class Particle_ColorSizeForce;
	class RendBuffer;
	//class Texture2D;
	class Engine;
	class Material;
	class VertexBuffer;
	class Emitter : public ISceneNode, public LocatableObject
	{
		friend class EmitterMain;
	private:
		class Index
		{
		private:
			int			m_nMax;
			int			m_nIndex;
		public:
			Index()
				:m_nMax(0),m_nIndex(0)
			{
			}
			void SetMax(int max)
			{
				m_nMax = max;
			}
			int operator++ (int)
			{
				m_nIndex++;
				if( m_nIndex > m_nMax )
				{
					m_nIndex = 0;
				}
				return m_nIndex;
			}
			BOHGE_FORCEINLINE int Max()
			{
				return m_nMax;
			}
			BOHGE_FORCEINLINE operator int()
			{
				return m_nIndex;
			}
		};
	private:
		bool			m_isInview;//�Ƿ�����׶��

		bool			m_isForceStop;//ǿ������ֹͣ
		bool			m_isEmit;//�Ƿ���
		vector3f		m_PositionNoise;//������λ��ƫ�����
		bool			m_isBeginChangeColor;//��ʼ����ɫ�Ƿ��Ǳ仯��
		bool			m_isEndChangeColor;//��������ɫ�Ƿ��Ǳ仯��
		Color			m_BeginColorA;//��ɫ���
		Color			m_BeginColorB;//��ɫ���
		Color			m_EndColorA;//��ɫ�յ�
		Color			m_EndColorB;//��ɫ�յ�
		vector2f		m_ColorPow;//��ɫ�仯������
		vector3f		m_Direction;//���䷽��
		vector3f		m_Acceleration;//���ٶ�
		vector2f		m_Speed;//�ٶ�
		float			m_EndSpeed;
		vector3f		m_Angle;//��xyz�ϵĸ����Ƕ�
		vector2d		m_LifeSpan;//������Χ
		vector2f		m_AlphaPow;//alpha�仯������
		vector2f		m_BeginSize;//��ʼ��С��Χ��Ե�
		vector2f		m_EndSize;//������С��Χ
		vector2f		m_SizePow;//�ߴ�仯������
		vector2d		m_CountSec;//ÿ�����緢������
		float			m_fResistance;//���������ֵ����ĩβ�ٶ�-��С��ʼ�ٶ�/�������ʱ�䣬���뱣֤���ֵ*�������ʱ��С�ڵ���1
		//Texture2D*		m_Tex;//����

		Index			m_Index;//��ǰ������

		RendBuffer*		m_pRendBuffer;//����buffer
		VertexBuffer*	m_pVertexBuffer;
		Particle_ColorSizeForce*		m_pParticle;//��������

		uint			m_uTimeSpan;//û�з������ӵ�ʱ���ܳ���
		uint			m_uLifedTime;//�Ѿ������ʱ��
		uint			m_ActiveTime;//��Ч�أ���أ����ǵ�ʱ��
		int				m_EmitterLife;//������������

		int				m_ArraySize;

		Material*		m_pMaterial;
	public:
		Emitter(const vector3f& pos,//���ӵķ���λ��
			const vector3f& posNoise,//���ӵĶ������
			const vector3f& dir,//���ӵķ�������
			const vector3f& acce,//���ӵļ��ٶ�
			const vector2f& speedPmill,//���ӵ��ٶ�
			float endspeed,//���ӽ����ٶ�
			const vector3f& angle,//���ӵĽǶ�
			const Color& begin1,//���ӵ���ɫ
			const Color& begin2,
			const Color& end1,
			const Color& end2,
			const vector2f& clrpow,//������ɫ����
			const vector2d& life,//��������ʱ��
			const vector2f& alphaPow,//����alpha������
			const vector2f& sizebegin,//���ӳߴ�
			const vector2f& sizeend,
			const vector2f& sizepow,//���ӳߴ�����
			const vector2d& countSec,//ÿ������
			const int emitterlife,//��������������
			std::string texName );//����
		virtual ~Emitter();
	public:
		virtual void Update( uint milisecon );
	public:
		void Move(const vector3f& dir);
		void CaculateBindBox();//����󶨺���,���ڼ���ĸ����ԣ�������ó������ķ������������������ֹ��runtime��������
	public:
		virtual bool isInview( const Frustumf& cFru );//�жϵ�ǰ֡�Ƿ�ɼ��Ƿ���Ҫ����
		virtual void Render( Engine& engine );
		virtual const void* Attribute(const Node_Attribute& name) const;
	public:
		void SetTexture( const std::string& texname );
	public:
		BOHGE_FORCEINLINE void SetPositionNoise( const vector3f& noise )
		{
			m_PositionNoise = noise;
		}
		BOHGE_FORCEINLINE void SetDirect( const vector3f& dir )
		{
			m_Direction = dir;
		}
		BOHGE_FORCEINLINE void SetAcceleration( const vector3f& acce )
		{
			m_Acceleration = acce;
		}
		BOHGE_FORCEINLINE void SetAngle( const vector3f& a )
		{
			m_Angle = a;
		}
		BOHGE_FORCEINLINE void SetBeginSpeedRange( const vector2f& range )
		{
			m_Speed = range;
			m_fResistance = ((m_EndSpeed / m_Speed.m_y) - 1.0f )/static_cast<float>(m_LifeSpan.m_y);
		}
		BOHGE_FORCEINLINE void SetEndSpeed( const float& speed )
		{
			m_EndSpeed = speed;
			m_fResistance = ((m_EndSpeed / m_Speed.m_y) - 1.0f )/static_cast<float>(m_LifeSpan.m_y);
		}
		BOHGE_FORCEINLINE void SetBeginColor1( const Color& clr )
		{
			m_BeginColorA = clr;
		}
		BOHGE_FORCEINLINE void SetBeginColor2( const Color& clr )
		{
			m_BeginColorB = clr;
		}
		BOHGE_FORCEINLINE void SetEndColor1( const Color& clr )
		{
			m_EndColorA = clr;
		}
		BOHGE_FORCEINLINE void SetEndColor2( const Color& clr )
		{
			m_EndColorB = clr;
		}
		BOHGE_FORCEINLINE void SetColorPow( const vector2f& ver )
		{
			m_ColorPow = ver;
		}
		BOHGE_FORCEINLINE void SetAlphaPow( const vector2f& ver )
		{
			m_AlphaPow = ver;
		}
		BOHGE_FORCEINLINE void SetBeginSizeRange( const vector2f& ver )
		{
			m_BeginSize = ver;
		}
		BOHGE_FORCEINLINE void SetEndSizeRange( const vector2f& ver )
		{
			m_EndSize = ver;
		}
		BOHGE_FORCEINLINE void SetSizePow( const vector2f& ver )
		{
			m_SizePow = ver;
		}
	public:
#ifdef DRAWAABBOX
		BOHGE_FORCEINLINE void RenderBind( Engine& dirver )
		{
			if( m_isEmit || m_uLifedTime <= m_EmitterLife + m_LifeSpan.m_y )//������ӻ�����
			{
				if ( m_isInview )//����׶��
				{
					m_BindBox.Render( dirver );
				}
			}
		}
#endif
		BOHGE_FORCEINLINE aabbox3df& WorldBindBox()
		{
			return m_BindBox;
		}
		BOHGE_FORCEINLINE void Relife()
		{
			m_isForceStop = false;
			m_isEmit = true;
			m_uLifedTime = 0;
			m_uTimeSpan = 0;
			m_ActiveTime = 0;
		}
		BOHGE_FORCEINLINE void StopEmit() //ֹͣ����
		{
			m_uLifedTime = 0;
			m_isEmit = false;
		}
		BOHGE_FORCEINLINE void ForceStop()//ǿ��ֹͣ��ֹͣ�����Լ�����
		{
			m_isForceStop = true;
			m_isEmit = false;
		}
		BOHGE_FORCEINLINE bool isLife()
		{
			return m_uLifedTime < m_EmitterLife + m_LifeSpan.m_y || m_EmitterLife < 0;
		}
		BOHGE_FORCEINLINE bool isForceStop()
		{
			return m_isForceStop;
		}
		BOHGE_FORCEINLINE bool isEmit()
		{
			return m_isEmit;
		}
	};
}