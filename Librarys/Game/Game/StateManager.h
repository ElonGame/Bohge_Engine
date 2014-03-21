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
//			State Manage class		   //
/////////////////////////////////////////

#pragma once

#include "Predefine.h"
#include "VariableType.h"

#include <list>


namespace BohgeEngine
{
	class Engine;
	class UIGroup;
}

namespace BohgeGame
{
	class IGameState;
	class StateManager
	{
		friend class Game;
	public:
		enum GameState
		{
			State_None,
			State_Logo,
			State_Main_Menu,
		};
	private:
		struct StateInfo
		{
		public:
			IGameState*				m_pStatePtr;//״̬��ָ��
			GameState				m_eStateName;//״̬������
			StateInfo(IGameState* ptr, GameState state)
				:m_pStatePtr(ptr),
				m_eStateName(state)

			{
			}
		};
		typedef std::list<StateInfo>			StateList;//��Ϸ��״̬������
	private:
		bool					m_isSwitchState;
		bool					m_isJustSwitch;
		bool					m_isLoadResouce;//������
		bool					m_isReplace;//ֱ�Ӹ�������ѹջ
		bool					m_isShowLoading;
		bool					m_ClearOtherState;//�Ƿ��ڽ���ǰ����ȫ������״̬��
		GameState				m_ePreviousState;//��һ��״̬
		GameState				m_eNextState;//��һ��״̬
		StateList				m_lStateList;//��Ϸ״̬ջ
		BohgeEngine::UIGroup*	m_pLoading;
	private:
		StateManager();
		~StateManager();
	private:
		void SwitchState(BohgeEngine::Engine& engine);
		void Update(BohgeEngine::Engine& engine, uint millisecond);
		void Exit(BohgeEngine::Engine& engine);
	public:
		static StateManager& Instance()
		{
			static StateManager state;
			return state;
		}
		BOHGE_FORCEINLINE void ChangeState(GameState state, bool isReplace, bool isClearOther, bool isShowLoading)//�Ƿ��˳�֮ǰ״̬���Ƿ����ȫ��ѹջ�ģ��Ƿ���ʾloding����
		{
			m_isShowLoading = isShowLoading;
			m_isSwitchState = true;
			m_eNextState = state;
			m_isReplace = isReplace;
			m_ClearOtherState = isClearOther;
		}
		BOHGE_FORCEINLINE GameState CurrextState()
		{
			return m_lStateList.begin()->m_eStateName;
		}
		BOHGE_FORCEINLINE IGameState* CurrentStatePtr()
		{
			return m_lStateList.begin()->m_pStatePtr;
		}
	};
}