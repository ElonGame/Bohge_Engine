#pragma once
#include "Decoder.h"



namespace BohgeEngine
{
	class WAVDecoder : public Decoder
	{
	private:
		uint			m_DataPosition;
	public:
		WAVDecoder(void);
		~WAVDecoder(void);
	private:
		virtual void _DoInitialization( int& freq, Format& format, int& ch, int& buffersize, double& time );
		virtual uint _DoDecodeAsyn( uint form, uint to );//�첽����
		virtual void _DoReleaseDecoder();//�ͷ���Դ
	};
}
