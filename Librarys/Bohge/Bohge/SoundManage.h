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



/////////////////////////////////////
//			�������������		   //
/////////////////////////////////////
#pragma once

#include "Predefine.h"
#include "VariableType.h"


#include <map>
#include <list>
#include <string>

#ifdef WIN32
//#define OPENSL
#define OPENAL
#endif

#ifdef ANDROID
#define OPENSL
#endif

#ifdef IOS
#define OPENAL
#endif






#ifdef OPENSL //openslʵ��
struct SLObjectItf_;
typedef const struct SLObjectItf_ * const * SLObjectItf;
struct SLEngineItf_;
typedef const struct SLEngineItf_ * const * SLEngineItf;
struct SLPlayItf_;
typedef const struct SLPlayItf_ * const * SLPlayItf;
struct SLSeekItf_;
typedef const struct SLSeekItf_ * const * SLSeekItf;
struct SLEnvironmentalReverbItf_;
typedef const struct SLEnvironmentalReverbItf_ * const * SLEnvironmentalReverbItf;
struct SLVolumeItf_;
typedef const struct SLVolumeItf_ * const * SLVolumeItf;
#endif

#ifdef OPENAL
typedef struct ALCdevice_struct ALCdevice;
typedef struct ALCcontext_struct ALCcontext;
#endif


namespace BohgeEngine
{
	class SoundManage
	{
	private:
		clamp		m_MusicVolume;
		clamp		m_SoundVolume;
#ifdef OPENAL	//openal ʵ��
	private:
		struct SoundData
		{
			int		format;
			byte*	data;
			int		size;
			int		ferq;
			~SoundData()
			{
				SAFE_DELETE_ARRAY(data);
			}
		};
	private:
		typedef std::pair<int, handle> SoundPair;
		typedef std::map<int,handle> SoundMap;//��һ����int������ö�ٵ�������handle���������
		typedef std::list<std::pair<int, handle> > EffectList;//����Ҫ���ŵ���Ч
	private:
		ALCdevice*		m_Device;//�豸
		ALCcontext*		m_Context;//������
		SoundMap		m_mSounds;	//�Ѿ���������ֵ�buffer��
		bool			m_isAnyMusicPlaying;
		int				m_hMusicKey;//���ֲ�key
		handle			m_hMusicSource;//���ֲ���Դ
		EffectList		m_lSoundSource;//ͬʱ���Դ��ڶ����Ч
	private:
		bool LoadWAV(const std::string& fileName, SoundData& out);
		bool LoadOGG(const std::string& fileName, SoundData& out);
#endif

#ifdef OPENSL //openslʵ��
	private:
		struct SoundData //����ɲ��ŵ����ֽṹ��
		{
			int				m_Key;
			SLObjectItf		m_uriPlayerObject;
			SLPlayItf		m_uriPlayerPlay;
			SLSeekItf		m_uriPlayerSeek;
			SLVolumeItf		m_uriPlayerVolume;
		};
		typedef std::pair<int, SoundData> SoundPair;
		typedef std::map<int, SoundData> SoundMap;//ȫ�����Բ��ŵ������б���һ����������ö������
		//typedef std::list<SoundData> EffectList;//���ڲ��ŵ�soundeffect
	private:
		bool							m_isAnyMusicPlaying;
		SLObjectItf						m_pEngineObject;
		SLEngineItf						m_pEngine;
		SLObjectItf						m_pMixObject;
		SLEnvironmentalReverbItf		m_pEnvironmental;

		SoundMap						m_mSounds;	//�Ѿ���������ֵ�buffer��
		SoundData						m_hMusicSource;//���ֲ���Դ
		//EffectList						m_PlayingEffect;
		BOHGE_FORCEINLINE int GetEngineSoundVolume()
		{
			return  -2000 * (1.0-m_SoundVolume);
		}
		BOHGE_FORCEINLINE int GetEngineMusicVolume()
		{
			return  -2000 * (1.0-m_MusicVolume);
		}
#endif
	private:
		/*�ͷ�������Դ*/
		bool CleanWave();
	public:
		SoundManage(void);
		~SoundManage(void);
		/*��ȡһ��wav�������ڴ���*/
		bool LoadSound(int Key, const std::string& fileName);
		bool UnloadSound(int key);
		/*�����Ĳ���һ����������*/
		bool PlayMusic(int Key);
		bool StopMusic();
		/*ֻ����һ�ε�*/
		bool PlaySoundEffect(int Key);
		/*��ⲥ��״̬��������ϵ�ɾ��*/
		void Update();
		void SetMusicVolume( clamp v);
		void SetSoundVolume( clamp v);
	public:
		BOHGE_FORCEINLINE clamp GetMusicVolume()
		{
			return m_MusicVolume;
		}
		BOHGE_FORCEINLINE clamp GetSoundVolume()
		{
			return m_SoundVolume;
		}
	};
}