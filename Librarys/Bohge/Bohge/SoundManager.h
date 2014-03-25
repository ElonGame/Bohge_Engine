#pragma once
#include "VariableType.h"
#include "Predefine.h"



#include <map>
#include <string>



#ifdef WIN32
//#define OPENSL
#define _OPENAL
#endif

#ifdef ANDROID
#define _OPENSL
#endif

#ifdef IOS
#define _OPENAL
#endif


namespace BohgeEngine
{
	class Decoder;
	class SoundPlayer;
	class SoundResource;
	class SoundManager
	{
	public:
		enum ServerType
		{
			ST_OPENAL	= 0,
			ST_OPENSL	= 1,
			ST_FMOD		= 2,
		};
	private:
		enum Constant
		{
			SMC_MAX_REPEAT_SOUND_PLAYER		= 3,//���255
		};
	private:
		struct SharedSoundPlayer
		{
			SoundPlayer*		m_pSoundPlayer;//������Դ
			uint				m_ReferenceCounting;//���ü���
		};
		struct SoundPlayerMapManager
		{
		public:
			typedef std::map< uint, SharedSoundPlayer* >	SoundPlayerMap;
		public:
			SoundPlayerMap::iterator	m_bCurrentIndex;//��ǰ���䵽����Դ����
			byte						m_bCreatedPlayer;//�Ѿ���������Դ����
			SoundPlayerMap				m_SoundPlayerMap;
		public:
			SoundPlayerMapManager():m_bCreatedPlayer(0){};
		};
	private:
		typedef std::map< uint, SoundPlayerMapManager* >	SoundPlayerManagerMap;
	protected:
		static SoundManager*		m_pInstance;
	private:
		SoundPlayerManagerMap	m_SoundMapMap;
		float					m_fGlobalVolume;
		float					m_fGlobalPitch;
		bool					m_isGlobalMute;
		uint					m_nPlayerIndex;
	protected:
		SoundManager(void);
		virtual ~SoundManager(void);
	public:
		static BOHGE_FORCEINLINE SoundManager* Instance() { return m_pInstance; };
		static void Create();
		static void Destroy();
	private:
		virtual void _OnCreate( ) = 0;
		virtual void _OnDestroy( ) = 0;
	public:
		virtual SoundPlayer* CreatePlayer( int hash, int index, Decoder* res ) = 0;
	public:
		SoundPlayer* LoadSound( const std::string& path );
		void ReleaseSound( SoundPlayer* sound );
		void Update();
	public:
		BOHGE_FORCEINLINE void SetGlobalVolume( float volume )
		{
			m_fGlobalVolume = volume;//0-1
		}
		BOHGE_FORCEINLINE float GetGlobalVolume() const
		{
			return m_fGlobalVolume;
		}
		BOHGE_FORCEINLINE void SetGlobalPitch( float pitch )
		{
			m_fGlobalPitch = pitch;//0-1
		}
		BOHGE_FORCEINLINE float GetGlobalPitch() const
		{
			return m_fGlobalPitch;
		}
		BOHGE_FORCEINLINE bool SetGLobalMute( bool mute )
		{
			m_isGlobalMute = mute;
		}
		BOHGE_FORCEINLINE bool isGlobalMute() const
		{
			return m_isGlobalMute;
		}
	};
}