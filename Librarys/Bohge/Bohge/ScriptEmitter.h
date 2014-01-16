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
//			����ϵͳ���ӷ�����	   //
/////////////////////////////////////////
#pragma once
#include "Emitter.h"


namespace BohgeEngine
{
	//֧�ֽű������ӷ�����
	class ScriptEmitter : public Emitter, public ScriptBase
	{
	public:
		ScriptEmitter(const vector3f& pos,//���ӵķ���λ��
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
	public:
		virtual void Update(uint millisecond);
	private:
		virtual void _OnScriptLoaded();//���ű����غ�
	public:
		void Lua_StartAnimation() //�ɴ������߼�������
		{
			Relife();
		}
		void Lua_StopAnimation()//ֹͣ
		{
			StopEmit();
		}
	public:
		BOHGE_FORCEINLINE void Lua_SetLocalRotation( const Quaternionf& r )
		{
			SetLocalRotation( r );
		}
		BOHGE_FORCEINLINE void Lua_SetLocalPosition( const vector3f& p )
		{
			SetLocalPosition( p );
		}
		BOHGE_FORCEINLINE void Lua_SetLocalScale( const vector3f& s )
		{
			SetLocalScale( s );
		}
		BOHGE_FORCEINLINE const vector3f& Lua_GetLocalPosition( ) const
		{
			return GetLocalPosition();
		}
		BOHGE_FORCEINLINE const vector3f& Lua_GetLocalScale( ) const
		{
			return GetLocalScale();
		}
		BOHGE_FORCEINLINE const Quaternionf& Lua_GetLocalRotation( ) const
		{
			return GetLocalRotation();
		}
		BOHGE_FORCEINLINE const Matrix44f& Lua_GetWorldTransform() const
		{
			return GetWorldTransform();
		}
		BOHGE_FORCEINLINE const Matrix33f& Lua_GetWorldRotationMatrix() const
		{
			return GetWorldRotationMatrix();
		}
		BOHGE_FORCEINLINE const vector3f& Lua_GetWorldPosition() const
		{
			return GetWorldPosition();
		}
		BOHGE_FORCEINLINE const vector3f& Lua_GetWorldScale() const
		{
			return GetWorldScale();
		}
		BOHGE_FORCEINLINE const Quaternionf& Lua_GetWorldRotation() const
		{
			return GetWorldRotation();
		}
	};
}