#pragma once
#include "SoundManager.h"

#ifdef _OPENAL


typedef struct ALCdevice_struct ALCdevice;
typedef struct ALCcontext_struct ALCcontext;
namespace BohgeEngine
{
	class SoundManagerAL : public SoundManager
	{
	private:
		ALCdevice*		m_pDevice;//�豸
		ALCcontext*		m_pContext;//������
	public:
		SoundManagerAL(void);
		~SoundManagerAL(void);
	private:
		virtual void _OnCreate( );
		virtual void _OnDestroy( );
	public:
		virtual SoundPlayer* CreatePlayer( int hash, int index, Decoder* res );
	};
}
#endif