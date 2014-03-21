#pragma once
#include "VariableType.h"
#include "SoundResourceManager.h"
#include <string>



namespace BohgeEngine
{
	class IReadFile;
	class SoundResourceDecoder
	{
	protected:
		enum SoundType
		{
			ST_OGG		= 0,
			ST_MP3		= 1,
			ST_WAV		= 2,
		};
	public:
		class BufferChunk
		{
		private:
			byte*			m_pBuffer;//�����ڴ��
			uint		m_nSize;//���ݿ��С
		public:
			BufferChunk( byte* buffer, uint size)
				:m_pBuffer( buffer ),
				m_nSize( size )
			{}
		public:
			inline bool isValid( ) const
			{
				return NULL != m_pBuffer;
			}
			inline const byte* GetBuffer( ) const
			{
				return m_pBuffer;
			}
			inline uint GetSize( ) const
			{
				return m_nSize;
			}
		};
	private:
		unsigned long			m_nFrequency;//Ƶ��
		unsigned long			m_nFormat;//��ʽ
		unsigned long			m_nChannels;//����
		unsigned long			m_nBufferSize;//����buffer���ܴ�С
		SoundType				m_eDecoderTypes;
		bool					m_isDecodeing;//�Ƿ����ڽ�����
		bool					m_isStorageBuffer;//�Ƿ񱣴��Ѿ������buffer�����SoundResource��Ҫ�����Դ��������loop���ֵ�ʱ�����ֵΪtrue�������ظ���ѹ���ռ任ʱ�䣩
		bool					m_isActived;//��Դ�Ƿ���Խ��н��룬��ȡ�Ȳ�����������ReleaseDecoder������Ϊfalse
		IReadFile*				m_pFileStream;//io
		byte*					m_pBuffer;//��Դ��
	public:
		static SoundResourceDecoder* SoundResourceDecoderFactory( const std::string& path );
	protected:
		SoundResourceDecoder( IReadFile* stream, SoundType type );
		virtual ~SoundResourceDecoder(void);
	private:
		void _RequestDecode( uint form, uint to );//Ҫ��������ݶ�from to
	public:
		void ReleaseDecoder( );//�ͷ�io��buffer
		void StorageBuffer( );//����Ĭ��ʹ����ʽ���ط�ʽ������Դѭ�����Ż����ж��ʹ���ߵ�ʱ���л�����ģʽ�����ڴ滻ʱ��
		BufferChunk GetBufferChunk( int pos );//�õ���������
		void OnEndDecode( uint form, uint to );//��ѹ��ɺ�,buffer��Ҫ����һ��
	public:
		virtual void DoDecodeAsyn( uint form, uint to ) = 0;//�첽����
	private:
		inline uint _GetEndPosition( uint form )
		{
			return form + SoundResourceManager::SC_DEFUALT_SOUND_BUFFER_SIZE > m_nBufferSize ? m_nBufferSize : form + SoundResourceManager::SC_DEFUALT_SOUND_BUFFER_SIZE;
		}
	protected:
		inline IReadFile* _GetFileSteam()
		{
			return m_pFileStream;
		}
	public:
		inline unsigned long GetFrequency()
		{
			return m_nFrequency;
		}
		inline unsigned long GetFormat()
		{
			return m_nFormat;
		}
		inline unsigned long GetChannels()
		{
			return m_nChannels;
		}
		inline unsigned long GetBufferSize()
		{
			return m_nBufferSize;
		}
		inline bool isActived()
		{
			return m_isActived;
		}
		inline SoundType GetDecoderType()
		{
			return m_eDecoderTypes;
		}
	};
}
