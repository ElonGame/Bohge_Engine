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
		uint			m_nCurrentBufferPosition;//��ǰ����buffer�Ķ�ȡ��ַ
	public:
		SoundResource( Decoder* decoder );
		~SoundResource(void);
	private:
		void _CopyBufferData();
	public:
		void DecoderNextChunk();//������һ������
	public:
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
		BOHGE_FORCEINLINE bool isDecoding()
		{
			bool res = m_pDecoder->isDecoding();
			if ( !res )
			{
				_CopyBufferData();
			}
			return res;
		}
	};
}
