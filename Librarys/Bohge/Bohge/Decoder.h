#pragma once
#include "3DMath.h"
#include "DecoderManager.h"
#include "IAsynJob.h"
#include <string>



namespace BohgeEngine
{
	class IReadFile;
	class Decoder : public IAsynJob
	{
	public:
		enum Format
		{
			DF_MONO_8 = 0,
			DF_STEREO_8,
			DF_MONO_16,
			DF_STEREO_16,
		};
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
			char*			m_pBuffer;//�����ڴ��
			uint			m_nSize;//���ݿ��С
			uint			m_nNextPosition;//��һ�����ݵ�����
			bool			m_isDone;
		public:
			BufferChunk( char* buffer, uint size, uint next, bool done )
				:m_pBuffer( buffer ),
				m_nSize( size ),
				m_nNextPosition( next ),
				m_isDone( done )
			{}
		public:
			BOHGE_FORCEINLINE bool isDone( ) const
			{
				return m_isDone;
			}
			BOHGE_FORCEINLINE const char* GetBuffer( ) const
			{
				return m_pBuffer;
			}
			BOHGE_FORCEINLINE uint GetSize( ) const
			{
				return m_nSize;
			}
			BOHGE_FORCEINLINE uint GetNextIndex( ) const
			{
				return m_nNextPosition;
			}
		};
	private:
		typedef std::vector<char*> BufferVector;
	private:
		int				m_nFrequency;//Ƶ��
		int				m_nChannels;//����
		int				m_nSourceTotalSize;//����pcm���ܴ�С
		double			m_dTime;
		Format			m_nFormat;//��ʽ
		uint			m_nLoadedBufferIndex;//�Ѿ����ع�������λ��
		std::string		m_FilePath;
		SoundType		m_eDecoderTypes;
		bool			m_isActived;//��Դ�Ƿ���Խ��н��룬��ȡ�Ȳ�����������ReleaseDecoder������Ϊfalse
		IReadFile*		m_pFileStream;//io
		BufferVector	m_BufferVector;//��Դ��
	public:
		static Decoder* DecoderFactory( const std::string& path );
		static void DestoryDecoder( Decoder* decoder );
	protected:
		Decoder( SoundType type );
		virtual ~Decoder(void);
	private:
		virtual void _DoInitialization( int& freq, Format& format, int& ch, int& buffersize, double& time ) = 0;
		virtual void _DoReleaseDecoder() = 0;//�ͷ���Դ
		virtual uint _DoDecodeAsyn( uint form, uint to ) = 0;//�첽����,���ؼ��ص�����
	private:
		void _RequestDecode();//Ҫ��������ݶ�from to
	public:
		void LoadResource( const std::string& path );
		void ReleaseDecoder( );//�ͷ�io��buffer
		BufferChunk GetBufferChunk( int index );//�õ���������,���Զ�������һ�μ��أ����getֻ��һ�ε���Ч��
	public:
		virtual void AsyncDoJob();//�̳���IAsynJob
	private:
		BOHGE_FORCEINLINE uint _GetEndPosition( uint form )
		{
			return form + DecoderManager::DC_DEFUALT_SOUND_BUFFER_SIZE > m_nSourceTotalSize ? m_nSourceTotalSize : form + DecoderManager::DC_DEFUALT_SOUND_BUFFER_SIZE;
		}
	protected:
		BOHGE_FORCEINLINE IReadFile* _GetFileSteam()
		{
			return m_pFileStream;
		}
		BOHGE_FORCEINLINE char* _GetInternalBuffer()
		{
			return m_BufferVector[m_nLoadedBufferIndex];
		}
	public:
		BOHGE_FORCEINLINE int isChunkReady( uint index ) const //���ݶ��Ƿ�׼����
		{
			return index < m_nLoadedBufferIndex;
		}
		BOHGE_FORCEINLINE int GetFrequency() const
		{
			return m_nFrequency;
		}
		BOHGE_FORCEINLINE Format GetFormat() const
		{
			return m_nFormat;
		}
		BOHGE_FORCEINLINE int GetChannels() const
		{
			return m_nChannels;
		}
		BOHGE_FORCEINLINE int GetSourceSize() const
		{
			return m_nSourceTotalSize;
		}
		BOHGE_FORCEINLINE bool isActived() const
		{
			return m_isActived;
		}
		BOHGE_FORCEINLINE SoundType GetDecoderType() const
		{
			return m_eDecoderTypes;
		}
		BOHGE_FORCEINLINE const std::string& GetFilePath() const
		{
			return m_FilePath;
		}
	};
}
