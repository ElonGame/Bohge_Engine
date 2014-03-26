#include "SoundManager.h"
#include "SoundManagerAL.h"
#include "SoundManagerSL.h"
#include "SoundPlayer.h"
#include "DecoderManager.h"
#include "Utility.h"


using namespace std;

namespace BohgeEngine
{
	SoundManager* SoundManager::m_pInstance = 0;


	//-------------------------------------------------------------------------------------------------------
	SoundManager::SoundManager(void)
		:m_nPlayerIndex(0),
		m_fGlobalVolume(1.0),
		m_fGlobalPitch(1.0),
		m_isGlobalMute(false)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	SoundManager::~SoundManager(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundManager::Create( )
	{
#ifdef _OPENAL
		m_pInstance = NEW SoundManagerAL;
#elif defined _OPENSL
		m_pInstance = NEW SoundManagerSL;
#else
		ASSERT( false && "unkonw platform!" );
#endif // _OPENSL
		m_pInstance->_OnCreate();
		//��ʼ����Դ������
		DecoderManager::Create();
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundManager::Destroy()
	{ 
		DecoderManager::Destroy();
		m_pInstance->_OnDestroy();
		SAFE_DELETE( m_pInstance );
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundManager::Update()
	{
		DecoderManager::Instance()->Update();
		for ( SoundPlayerManagerMap::iterator soundmap = m_SoundMapMap.begin();
			soundmap != m_SoundMapMap.end();
			soundmap ++ )
		{
			for ( SoundPlayerMapManager::SoundPlayerMap::iterator playmap = soundmap->second->m_SoundPlayerMap.begin();
				playmap != soundmap->second->m_SoundPlayerMap.end();
				playmap ++ )
			{
				playmap->second->m_pSoundPlayer->Update();
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	SoundPlayer* SoundManager::LoadSound( const std::string& path )
	{
		uint hash = Utility::HashCode( path );//�����ϣ��(α����)
		SoundPlayer* player = NULL;
		SoundPlayerMapManager* spvm = NULL;

		SoundPlayerManagerMap::iterator findspm = m_SoundMapMap.find( hash ); //�ҵ�����Դ�Ƿ񱻼��ع�
		if ( m_SoundMapMap.end() != findspm )
		{
			spvm = findspm->second;
		}
		else
		{
			spvm = NEW SoundPlayerMapManager;
			m_SoundMapMap.insert( make_pair( hash, spvm ) );
		}

		if ( spvm->m_bCreatedPlayer < SMC_MAX_REPEAT_SOUND_PLAYER ) //�����ͬ��Դ���ز����涨����ظ��ĸ���
		{
			int index = spvm->m_bCreatedPlayer ++;
			Decoder* decoder = DecoderManager::Instance()->LoadSoundDecoder( path );
			player = m_pInstance->CreatePlayer( hash, index, decoder );
			player->Initialization();
			SharedSoundPlayer* ssp = NEW SharedSoundPlayer();
			ssp->m_pSoundPlayer = player;
			ssp->m_ReferenceCounting = 1;
			spvm->m_SoundPlayerMap.insert( std::make_pair( (uint)player, ssp ) );
			spvm->m_bCurrentIndex = spvm->m_SoundPlayerMap.find( (uint)player );
		}
		else //�ﵽ���Լ��ص�����
		{
			spvm->m_bCurrentIndex->second->m_ReferenceCounting++;
			player = spvm->m_bCurrentIndex->second->m_pSoundPlayer;
			spvm->m_bCurrentIndex++;
			spvm->m_bCurrentIndex = spvm->m_bCurrentIndex == spvm->m_SoundPlayerMap.end() ? spvm->m_SoundPlayerMap.begin() : spvm->m_bCurrentIndex;
		}
		return player;
	}
	//-------------------------------------------------------------------------------------------------------
	void SoundManager::ReleaseSound( SoundPlayer** sound )
	{
		if ( NULL != *sound )
		{
			SoundPlayerManagerMap::iterator findspm = m_SoundMapMap.find( (*sound)->GetHashCode() ); //�ҵ�����Դ
			if( findspm != m_SoundMapMap.end() )
			{
				SoundPlayerMapManager::SoundPlayerMap::iterator spm = findspm->second->m_SoundPlayerMap.find( (uint)*sound );
				if ( 0 == -- spm->second->m_ReferenceCounting )
				{
					SAFE_DELETE( spm->second->m_pSoundPlayer );
					findspm->second->m_SoundPlayerMap.erase( spm );
					if( 0 == -- findspm->second->m_bCreatedPlayer )
					{
						SAFE_DELETE( findspm->second );
						m_SoundMapMap.erase( findspm );
					}
				}
			}
			*sound = NULL;
		}
	}
}