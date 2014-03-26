#pragma once
#include "Decoder.h"


#include <string>



namespace BohgeEngine
{
	class Decoder;
	class SoundResource//������Դ
	{
		//������Դ����decoder���ϲ���������ڼ�����Դ�Ŀ��Խ�����ͬ��Դ�ļ��ع���
		//��һ��������Դ���ظ�ʹ�õ�ʱ��decoder������ʽ������Դ������֮������ȥ�ͷž���Դ
		//������������һ���Ŀռ���Դ��ȡʱ��
	private:
		Decoder*		m_pDecoder;
		const char*		m_BufferAddress;
		uint			m_nBufferSize;
		uint			m_nCurrentBufferIndex;//��ǰ����buffer��index
		bool			m_isDone;
	public:
		SoundResource( Decoder* decoder );
		~SoundResource(void);
	public:
		void FlushBufferData();
	public:
		BOHGE_FORCEINLINE const std::string& GetFilePath() const
		{
			return m_pDecoder->GetFilePath();
		}
		BOHGE_FORCEINLINE void RequestDecode()//Ҫ��������ݶ�
		{
			m_pDecoder->RequestDecode();
		}
		BOHGE_FORCEINLINE void ReleaseResource()//sl��Ҫ�ͷ���Դ��ӦΪsl����Ҫ��ȡ
		{
			m_pDecoder->ReleaseDecoder();
		}
		BOHGE_FORCEINLINE int GetFrequency() const
		{
			return m_pDecoder->GetFrequency();
		}
		BOHGE_FORCEINLINE Decoder::Format GetFormat() const
		{
			return m_pDecoder->GetFormat();
		}
		BOHGE_FORCEINLINE int GetChannels() const
		{
			return m_pDecoder->GetChannels();
		}
		BOHGE_FORCEINLINE int GetBufferSize() const
		{
			return m_nBufferSize;
		}
		BOHGE_FORCEINLINE int GetSourceSize() const
		{
			return m_pDecoder->GetSourceSize();
		}
		BOHGE_FORCEINLINE const char* GetBufferChunk() const//�õ����ݶ�
		{
			return m_BufferAddress;
		}
		BOHGE_FORCEINLINE bool isChunkReady() const
		{
			return m_pDecoder->isChunkReady( m_nCurrentBufferIndex );
		}
		BOHGE_FORCEINLINE bool isDone() const //�Ƿ񲥷����
		{
			return m_isDone;
		}
	};
}
