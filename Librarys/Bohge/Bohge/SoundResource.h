#pragma once
#include <string>



namespace BohgeEngine
{
	class SoundResourceDecoder;
	class SoundResource//������Դ
	{
		//������Դ����decoder���ϲ���������ڼ�����Դ�Ŀ��Խ�����ͬ��Դ�ļ��ع���
		//��һ��������Դ���ظ�ʹ�õ�ʱ��decoder������ʽ������Դ������֮������ȥ�ͷž���Դ
		//������������һ���Ŀռ���Դ��ȡʱ��
	private:
		SoundResourceDecoder*	m_pDecoder;
		int						m_nCurrentBufferPosition;//��ǰ����buffer�Ķ�ȡ��ַ
	public:
		SoundResource( SoundResourceDecoder* decoder );
		~SoundResource(void);
	public:
	};
}
