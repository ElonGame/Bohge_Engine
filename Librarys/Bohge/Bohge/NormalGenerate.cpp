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
//			������Ⱦ��Ч		   //
/////////////////////////////////////
#include "NormalGenerate.h"
#include "Device.h"
#include "RendBuffer.h"
#include "RenderTarget.h"
#include "ShaderManage.h"
#include "Material.h"


namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	NormalGenerate::NormalGenerate( const vector2d& size, Texture2D& n1, Texture2D& n2, IAttributeNode* node  )
	{
		VertexTexture Vertex[4];
		Vertex[0] = VertexTexture(vector4f(-1,1,0), vector2f(0,1));
		Vertex[1] = VertexTexture(vector4f(-1,-1,0), vector2f(0,0));
		Vertex[2] = VertexTexture(vector4f(1,1,0), vector2f(1,1));
		Vertex[3] = VertexTexture(vector4f(1,-1,0), vector2f(1,0));
		ushort Indies[4]={0,1,2,3};
		m_pRendBuffer = NEW RendBuffer( Device::RM_TRIANGLE_STRIP);
		m_pVertexBuffer = NEW VertexBuffer( Device::MU_STATIC );
		m_pIndicesBuffer = NEW IndicesBuffer( Device::MU_STATIC );
		m_pRendBuffer->SetVertexBuffer( m_pVertexBuffer );
		m_pRendBuffer->SetIndicesBuffer( m_pIndicesBuffer );
		m_pVertexBuffer->FlushVertexBuffer( 4, &Vertex[0] );
		m_pIndicesBuffer->FlushIndiesBuffer( 4, &Indies[0] );

		m_pHighmapRT = NEW RenderTarget( size, Device::FBO_TCOLOR);
		m_pHighmapMaterial = NEW Material;
		//m_pHighmapMaterial->SetShader( ShaderManage::HighmapGenerateShader );
		m_pHighmapMaterial->LoadTexture( Material::ATT_TEX_USER0, n1 );
		m_pHighmapMaterial->LoadTexture( Material::ATT_TEX_USER1, n2 );
		m_pHighmapMaterial->SetNode( node );

		m_pNormalmapRT = NEW RenderTarget( size, Device::FBO_TCOLOR);
		m_pNormalmapMaterial = NEW Material;
		//m_pNormalmapMaterial->SetShader( ShaderManage::NormalmapGenerateShader );
		m_pNormalmapMaterial->LoadTexture( Material::ATT_TEX_USER0, *((Texture2D*)m_pHighmapRT->GetColorBuffer()) );
		m_pNormalmapMaterial->SetNode( node );
	}
	//-------------------------------------------------------------------------------------------------------
	NormalGenerate::~NormalGenerate(void)
	{
		SAFE_DELETE( m_pNormalmapMaterial );
		SAFE_DELETE( m_pNormalmapRT );
		SAFE_DELETE( m_pHighmapMaterial );
		SAFE_DELETE( m_pHighmapRT );
		SAFE_DELETE( m_pRendBuffer );
		SAFE_DELETE( m_pVertexBuffer );
		SAFE_DELETE( m_pIndicesBuffer );
	}
	//-------------------------------------------------------------------------------------------------------
	void NormalGenerate::GenerateNormal( Engine& dirver )
	{
		RenderTarget* now = dirver.GetDevice()->PopRenderTarget( );

		dirver.GetDevice()->PushRenderTarget( m_pHighmapRT );
		dirver.GetDevice()->Draw( *m_pRendBuffer, *m_pHighmapMaterial );
		dirver.GetDevice()->PopRenderTarget();

		dirver.GetDevice()->PushRenderTarget( m_pNormalmapRT );
		dirver.GetDevice()->Draw( *m_pRendBuffer, *m_pNormalmapMaterial );
		dirver.GetDevice()->PopRenderTarget();

		dirver.GetDevice()->PushRenderTarget(now);
	}
	//-------------------------------------------------------------------------------------------------------
	Texture2D& NormalGenerate::Normalmap()
	{
		return *((Texture2D*)m_pNormalmapRT->GetColorBuffer());
	}

	//-------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------


	//#include <pthread.h>
	//#include <unistd.h>
	//#include <stdio.h>
	//#include <string.h>
	//#include <stdlib.h>
	//static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
	//static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
	//struct node
	//{
	//	int n_number;
	//	struct node *n_next;
	//} *head = NULL; /*[thread_func]*/
	///*�ͷŽڵ��ڴ� */
	//static void cleanup_handler(void *arg)
	//{
	//	printf("Cleanup handler of second thread.\n");
	//	free(arg);
	//	(void)pthread_mutex_unlock(&mtx);
	//}
	//void *thread_func(void *arg)
	//{node *p = NULL;
	//(cleanup_handler, p);
	//while (1)
	//{
	//	pthread_mutex_lock(&mtx);
	//	����//���mutex_lock��Ҫ����������wait�ȴ��ٽ�ʱ�ڵ������
	//	����//����waitΪ�������ʱ����ʱ���Ѿ�����Head�ȴ������������������ʱ���ܻ�©�����ִ���
	//	����//���whileҪ�ر�˵��һ�£�����pthread_cond_wait���ܺ����ƣ�
	//	����//Ϊ������Ҫ��һ��while (head == NULL)�أ���Ϊpthread_cond_wait����߳̿��ܻᱻ���⻽�ѣ�������ʱ��head != NULL��
	//	����//����������Ҫ����������ʱ��Ӧ�����̼߳�������pthread_cond_wait
	//	while (head == NULL)
	//	����{
	//		  pthread_cond_wait(&cond, &mtx);
	//		  ����// pthread_cond_wait���Ƚ��֮ǰ��pthread_mutex_lock������mtx��Ȼ�������ڵȴ����������ߣ�ֱ���ٴα�����
	//		  ����//�������������ǵȴ������������������ѣ����Ѻ󣬸ý��̻���������pthread_mutex_lock(&mtx);��
	//		  ����// �ٶ�ȡ��Դ ����������ǱȽ������/*block-->unlock-->wait() return-->lock*/
	//		  ����p = head;
	//			head = head->n_next;
	//			printf("Got %d from front of queue\n", p->n_number);
	//			free(p);
	//			pthread_mutex_unlock(&mtx); //�ٽ������ݲ�����ϣ��ͷŻ�����
	//	����}
	//����}
	//pthread_cleanup_pop(0);
	//return 0;
	///*EC_CLEANUP_BGN (void)pthread_mutex_unlock(&mtx); EC_FLUSH("thread_func") return 1; EC_CLEANUP_END*/
	//}
	//int main(void)
	//{
	//	pthread_t tid;
	//	int i;
	//	struct node *p;
	//	pthread_create(&tid, NULL, thread_func, NULL);
	//	����//���̻߳�һֱ�ȴ���Դ�����������ߺ������ߣ���������������߿����Ƕ�������ߣ���������֧����ͨ�ĵ��������ߣ����ģ����Ȼ�򵥣����Ǻ�ǿ�� /*[tx6-main]*/
	//	for (i = 0; i < 10; i++)
	//	{
	//		p = (struct node *)malloc(sizeof(struct node));
	//		p->n_number = i;
	//		pthread_mutex_lock(&mtx); //��Ҫ����head����ٽ���Դ���ȼ�����
	//		p->n_next = head;
	//		head = p;
	//		pthread_cond_signal(&cond);
	//		pthread_mutex_unlock(&mtx); //����
	//		sleep(1);
	//	}
	//	����printf("thread 1 wanna end the cancel thread 2.\n");
	//	  pthread_cancel(tid);
	//	  ����//����pthread_cancel����һ������˵�������Ǵ��ⲿ��ֹ���̣߳�
	//	  ����//���̻߳��������ȡ���㣬�˳��̣߳��������ǵĴ���������ȡ����϶�����pthread_cond_wait()�ˡ�
	//	  ����pthread_join(tid, NULL);
	//		����printf("All done -- exiting\n");
	//		  return 0; /*[]*/ /*EC_CLEANUP_BGN return EXIT_FAILURE; EC_CLEANUP_END*/
	//����}

}