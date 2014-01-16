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



/////////////////////////////////////////
//		��Ϸ��post effect�Ĺ�����	   //
/////////////////////////////////////////

#include "Pipeline.h"
#include "IDriver.h"
#include "IPostEffect.h"
#include "SecenManage.h"
#include "Environment.h"
#include "Bfont.h"
#include "EmitterManager.h"
#include "UIManager.h"
#include "Environment.h"
#include "ActionManage.h"
#include "Camera.h"
#include "ShaderManage.h"
#include "SecenManage.h"
#include "SoundManage.h"
#include "UIManager.h"
#include "ShaderManage.h"
#include "SoundManage.h"
#include "RenderTarget.h"
#include "PaperNode.h"
#include "Vertex.h"
#include "RendBuffer.h"
#include "CopyCurrentScreen.h"


//-------------------------------------------------------------------------------------------------------
Pipeline::Pipeline(void)
:m_isPostEffectActive(true),
m_pOriginalColorDepthStencil(NULL)
{
	vector2d size = IDriver::Instance().GetViewprot();
	m_pOriginalColorDepthStencil = NEW RenderTarget(size, IDriver::FBO_texColor_texDepthStencil);
	//m_pBottomUISkyBoxColor = NEW RenderTarget(size, IDriver::FBO_texColor);
	m_pPreviousSceneColor[0] = NEW RenderTarget(size, IDriver::FBO_texColor);
	m_pPreviousSceneColor[1] = NEW RenderTarget(size, IDriver::FBO_texColor);
	m_pReflection = NEW RenderTarget(size, IDriver::FBO_texColor_bufDepth);
	m_pCopyer = NEW CopyCurrentScreen( m_pPreviousSceneColor[0], m_pPreviousSceneColor[1] );
#ifdef IOS
	m_EndSecen = NEW RenderTarget();
#endif
	VertexTexture Vertex[COUNT_OF_VERTEX];
	Vertex[0] = VertexTexture(vector4f(-1,1,0), vector2f(0,1));
	Vertex[1] = VertexTexture(vector4f(-1,-1,0), vector2f(0,0));
	Vertex[2] = VertexTexture(vector4f(1,1,0), vector2f(1,1));
	Vertex[3] = VertexTexture(vector4f(1,-1,0), vector2f(1,0));
	ushort Indies[COUNT_OF_INDIES]={0,1,2,3};
	m_pRendBuffer = NEW RendBuffer(VertexTexture::Layout(), sizeof( VertexTexture ), IDriver::RM_TRIANGLE_STRIP);
	m_pRendBuffer->MakeVertexBuffer( &Vertex[0], COUNT_OF_VERTEX , IDriver::MU_STATIC );
	m_pRendBuffer->MakeIndiesBuffer( &Indies[0], COUNT_OF_INDIES, IDriver::MU_STATIC );
}
//-------------------------------------------------------------------------------------------------------
Pipeline::~Pipeline(void)
{
#ifdef IOS
	SAFE_DELETE( m_EndSecen );
#endif
	SAFE_DELETE( m_pReflection );
	SAFE_DELETE( m_pRendBuffer );
	SAFE_DELETE( m_pOriginalColorDepthStencil );
	//SAFE_DELETE( m_pBottomUISkyBoxColor );
	SAFE_DELETE( m_pPreviousSceneColor[0] );
	SAFE_DELETE( m_pPreviousSceneColor[1] );
	SAFE_DELETE( m_pCopyer );
}
//-------------------------------------------------------------------------------------------------------
void Pipeline::RemovePostEffect( IPostEffect* ipe )
{
	for ( PostEffectList::iterator it = m_PostList.begin();//������Ч
		it != m_PostList.end();
		)
	{
		if ( ipe == (*it) )
		{
			it = m_PostList.erase( it );
		}
		else
		{
			it++;
		}
	}
}
//-------------------------------------------------------------------------------------------------------
void Pipeline::Render(IDriver& driver)
{
	bool swtich = false;
	//m_pPreviousSceneColor[0]->Clear(driver);
	//m_pPreviousSceneColor[1]->Clear(driver);

	driver.EnableDepthTest();
	driver.EnableCullFace();
	driver.DepthMask(true);

	if( driver.hasLight() )
	{
		if( driver.GetEnvironment().isCastShadow() ) //�����Ҫ��Դ��ͼ��
		{
			if( driver.GetSecenManage().isCastShadow() )
			{
				m_CurrentPassType = PT_DEPTH;
				driver.ClearScreenColor( 1.0f, 1.0f, 1.0f, 1.0f );//��Ҫ��֤�������ĵ�ɫ�Ǵ��ף��ȣ������ֵΪ1�����
				driver.GetEnvironment().BeginRenderDepth( driver );
				driver.GetSecenManage().RenderDepth(driver);
				driver.GetEnvironment().EndRenderDepth( driver );
			}
			else
			{
				driver.ClearScreenColor( 1.0f, 1.0f, 1.0f, 1.0f );//��Ҫ��֤�������ĵ�ɫ�Ǵ��ף��ȣ������ֵΪ1�����
				driver.GetEnvironment().ClearDepth( driver );
			}
		}
	}

    
	driver.EnableAlphaBlend(IDriver::BL_SRC_ALPHA, IDriver::BL_ONE_MINUS_SRC_ALPHA, IDriver::BL_SRC_ALPHA, IDriver::BL_ONE_MINUS_SRC_ALPHA );
	m_CurrentPassType = PT_LIGHTING;
	{//���Ʒ���
		ICamera* nowcame = &driver.GetCamera();
		StaticCamera refle;
		refle.CreatePerspectiveProjection( nowcame->GetNear(), nowcame->GetFar() );
		vector3f pos = nowcame->GetPosition();
		vector3f lookat = nowcame->GetPosition() + nowcame->GetForward();
		pos.m_y = driver.GetSecenManage().GetWaterMiddleHeight() - pos.m_y;
		lookat.m_y = driver.GetSecenManage().GetWaterMiddleHeight() - lookat.m_y;

		refle.LookAt( pos, lookat, -nowcame->GetUp() );
		refle.Update();
		refle.AttachCamera();
		driver.PushRenderTarget( m_pReflection );
		driver.Clear( IDriver::COLOR_BUFFER | IDriver::DEPTH_BUFFER );
		driver.GetSecenManage().RenderBackground(driver);
		driver.GetSecenManage().RenderSkyBox(driver);
		driver.GetSecenManage().RenderReflection(driver);
		driver.PopRenderTarget();
		nowcame->AttachCamera();
	}
	if ( m_PostList.empty() || !m_isPostEffectActive )	//���û�к��ڣ�ֱ�ӻ�������Ļ��
	{
#ifdef IOS
		driver.PushRenderTarget( m_EndSecen );
#endif
        driver.ClearScreenColor( );//�ı�������ɫ,ʹ�ÿͻ����õ���ɫ
        driver.Clear( IDriver::COLOR_BUFFER | IDriver::DEPTH_BUFFER | IDriver::STENCIL_BUFFER ); //������Ļ
		driver.GetSecenManage().RenderBackground(driver);
		driver.GetUIManager().Render(driver, UIManager::UL_BOTTOM);//���Ʊ�������
		driver.GetSecenManage().RenderSkyBox(driver);
		driver.GetSecenManage().Render(driver);
		//driver.GetSecenManage().RenderShadow(driver);//������Ӱ
		//driver.GetEmitterManager().Render(driver);//��Ⱦ����
	}
	else
	{
        
		{//����������
			//driver.isUseLightCamera(true);
			driver.ClearScreenColor( 0.0f, 0.0f, 0.0f, 0.0f );//��Ҫalpha������0����volumelightpost��ʵ��
			driver.PushRenderTarget( m_pOriginalColorDepthStencil );//�Ƚ�����������ԭʼ������
			driver.Clear( IDriver::COLOR_BUFFER | IDriver::DEPTH_BUFFER | IDriver::STENCIL_BUFFER );
			driver.GetSecenManage().RenderSkyBox(driver);
			driver.GetSecenManage().Render(driver);
			//driver.GetSecenManage().RenderShadow(driver);//������Ӱ
			//driver.GetEmitterManager().Render(driver);//��Ⱦ����
			driver.PopRenderTarget( );
			//driver.isUseLightCamera(false);
		}

        driver.DisableDepthTest();
		driver.DepthMask( false );
		{//���Ʊ���
			//driver.PushRenderTarget( m_pBottomUISkyBoxColor );//�Ƚ�����������ԭʼ������
			//driver.Clear( IDriver::COLOR_BUFFER );
#ifdef IOS
            driver.PushRenderTarget( m_EndSecen );
#endif
            //driver.ClearScreenColor( );//�ı�������ɫ,ʹ�ÿͻ����õ���ɫ
            driver.Clear( IDriver::COLOR_BUFFER | IDriver::DEPTH_BUFFER | IDriver::STENCIL_BUFFER ); //������Ļ
			//driver.EnableAlphaBlend(IDriver::BL_SRC_ALPHA, IDriver::BL_ONE_MINUS_SRC_ALPHA, IDriver::BL_SRC_ALPHA, IDriver::BL_ONE_MINUS_SRC_ALPHA );
			driver.GetSecenManage().RenderBackground(driver);
			driver.GetUIManager().Render(driver, UIManager::UL_BOTTOM);//���Ʊ�������
			//driver.PopRenderTarget( );
#ifdef IOS            
            driver.PopRenderTarget( );
#endif
		}
        
		bool isNeedOrig = true;
		for ( PostEffectList::iterator it = m_PostList.begin();//������Ч
			it != m_PostList.end();
			it++)
		{
			if ( (*it)->isActive() )
			{
				bool isSwitch = false;
				if( isNeedOrig )//����ǵ�һ��posteffect�Ͱѳ�ʼogi����secee
				{
					isNeedOrig = false;
					(*it)->Process(driver, m_pOriginalColorDepthStencil,m_pOriginalColorDepthStencil, m_pPreviousSceneColor[static_cast<int>(swtich)], isSwitch);
				}
				else
				{
					(*it)->Process(driver, m_pOriginalColorDepthStencil,m_pPreviousSceneColor[static_cast<int>( !swtich/*ע��̾��*/ )], m_pPreviousSceneColor[static_cast<int>(swtich)], isSwitch);
				}
				if( isSwitch )
				{
					swtich = !swtich; //����
				}
			}
		}
		//��ʼ�ϳɵ���Ļ��

		//if( 0 )
		//{
		//	PipelineCombine& pc = driver.GetShaderManage().GetShader<PipelineCombine>(ShaderManage::PipelineCombine);
		//	pc.SetParamOrginial( m_pBottomUISkyBoxColor->GetColorBuffer() );
		//	pc.SetParamSecen( m_pPreviousSceneColor[static_cast<int>( !swtich/*ע��̾��*/ )]->GetColorBuffer() );
		//	driver.Draw( *m_pRendBuffer, pc, NULL );
		//}
		//else
		//{
#ifdef IOS
            driver.PushRenderTarget( m_EndSecen );
#endif
			driver.EnableAlphaBlend(IDriver::BL_SRC_ALPHA, IDriver::BL_ONE_MINUS_SRC_ALPHA, IDriver::BL_SRC_ALPHA, IDriver::BL_ONE_MINUS_SRC_ALPHA );
			QuadShader& qs = driver.GetShaderManage().GetShader<QuadShader>(ShaderManage::QuadShader);
			driver.Draw( *m_pRendBuffer, qs, m_pPreviousSceneColor[static_cast<int>( !swtich/*ע��̾��*/ )]->GetColorBuffer() );
		//}
	}
	//��Ⱦ2D
	driver.DisableDepthTest();
	driver.GetUIManager().Render(driver, UIManager::UL_TOP);
	driver.GetStringManager().Render(driver);
}
//-------------------------------------------------------------------------------------------------------
void Pipeline::MakeCopyScreen()
{
	m_pCopyer->Process( IDriver::Instance(), m_pOriginalColorDepthStencil );
}
//-------------------------------------------------------------------------------------------------------
Texture* Pipeline::GetCopyColorTexture()
{
	return m_pCopyer->ColorTexture();
}
//-------------------------------------------------------------------------------------------------------
Texture* Pipeline::GetCopyDepthTexture()
{
	return m_pCopyer->DepthTexture();
}
//-------------------------------------------------------------------------------------------------------
#ifdef IOS
void Pipeline::SetRendTargetHandle( handle h, const vector2d& size )
{
	m_EndSecen->SetRendTargetHandle( h, size);
}
#endif
//-------------------------------------------------------------------------------------------------------
void Pipeline::OnSizeChange( const vector2d& size )
{
	if ( size != vector2d::Zero() )
	{
		m_pOriginalColorDepthStencil->Resize( size );
		//m_pBottomUISkyBoxColor->Resize( size );
		m_pPreviousSceneColor[0]->Resize( size );
		m_pPreviousSceneColor[1]->Resize( size );
	}
}
//-------------------------------------------------------------------------------------------------------
Texture* Pipeline::GetReflection()
{
	return m_pReflection->GetColorBuffer();
}
