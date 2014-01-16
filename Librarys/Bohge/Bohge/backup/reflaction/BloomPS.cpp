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


#include "BloomPS.h"
#include "IDriver.h"
#include "RenderTarget.h"
#include "PaperNode.h"
#include "Vertex.h"
#include "ShaderManage.h"
#include "Pipeline.h"

//-------------------------------------------------------------------------------------------------------
BloomPS::BloomPS()
{

	vector2d size = IDriver::Instance().GetViewprot();
	m_pDownSimple_1 = NEW RenderTarget(size / 4 , IDriver::FBO_texColor);
	m_pDownSimple_2 = NEW RenderTarget(size / 4 , IDriver::FBO_texColor);
}
//-------------------------------------------------------------------------------------------------------
BloomPS::~BloomPS()
{
	SAFE_DELETE( m_pDownSimple_1 );
	SAFE_DELETE( m_pDownSimple_2 );
}
//-------------------------------------------------------------------------------------------------------
void BloomPS::Process(IDriver& driver, const RenderTarget* const org, const RenderTarget* const scene, RenderTarget* const out, bool& isSwitch)
{
	float Range = 2; //����ģ����

	//������
	//driver.PushRenderTarget(m_pDownSimple_1);
	//driver.Clear( IDriver::COLOR_BUFFER );
	//QuadShader& qs = driver.GetShaderManage().GetShader<QuadShader>(ShaderManage::QuadShader);
	//driver.Draw( *m_pRendBuffer, qs, scene->GetColorBuffer() );
	//driver.PopRenderTarget();

	//������
	driver.PushRenderTarget(m_pDownSimple_1);
	driver.Clear( IDriver::COLOR_BUFFER );
	Luminance& le = driver.GetShaderManage().GetShader<Luminance>(ShaderManage::Luminance);
	driver.Draw( *m_pRendBuffer, le, scene->GetColorBuffer() );
	driver.PopRenderTarget();

	//x����ģ��
	driver.PushRenderTarget(m_pDownSimple_2);
	driver.Clear( IDriver::COLOR_BUFFER );
	Gaussian& gauss = driver.GetShaderManage().GetShader<Gaussian>(ShaderManage::Gaussian);
	gauss.SetParamStep( vector2f( Range / m_pDownSimple_1->GetSize().m_x, 0.0f ) );
	driver.Draw( *m_pRendBuffer, gauss, m_pDownSimple_1->GetColorBuffer() );
	driver.PopRenderTarget();

	//y����ģ��
	driver.PushRenderTarget(m_pDownSimple_1);
	driver.Clear( IDriver::COLOR_BUFFER );
	gauss.SetParamStep( vector2f( 0.0f, Range / m_pDownSimple_2->GetSize().m_y ) );
	driver.Draw( *m_pRendBuffer, gauss, m_pDownSimple_2->GetColorBuffer() );
	driver.PopRenderTarget();

	driver.PushRenderTarget( out );//���������������棬����Ҫ�����������һ��pe��������һ��rt
	isSwitch = true;
	driver.Clear( IDriver::COLOR_BUFFER );
	//if( 0 )
	{
		driver.EnableAlphaBlend(IDriver::BL_SRC_ALPHA, IDriver::BL_ONE_MINUS_SRC_ALPHA, IDriver::BL_SRC_ALPHA, IDriver::BL_ONE_MINUS_SRC_ALPHA );
		Combine& ce = driver.GetShaderManage().GetShader<Combine>(ShaderManage::Combine);
		ce.SetParamOrginial( scene->GetColorBuffer() );
		ce.SetParamSecen( m_pDownSimple_1->GetColorBuffer() );
		ce.SetParamOrginialWeight(1.0f);
		ce.SetParamSecenWeight(0.0f);//was 0.75
		driver.Draw( *m_pRendBuffer, ce, NULL );
	}
	//else
	//{
	//	QuadShader& qs = driver.GetShaderManage().GetShader<QuadShader>(ShaderManage::QuadShader);
	//	driver.Draw( *m_pRendBuffer, qs, m_pDownSimple_1->GetColorBuffer() );
	//	//driver.Draw( *m_pRendBuffer, ui, org->GetDepthBuffer() );
	//}
	driver.PopRenderTarget( );
}
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------