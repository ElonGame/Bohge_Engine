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
//			ˮ��ͳһ����		   //
/////////////////////////////////////
#include "WaterManager.h"
#include "WaterGrid.h"
#include "RenderTarget.h"
#include "Pipeline.h"
#include "CopyCurrentScreen.h"
#include "WaterTile.h"
#include "Serializer.h"
#include "SceneDataIndex.h"
#include "Camera.h"
#ifndef COPYSCREEN
#include "TerrainManager.h"
#endif

using namespace std;


namespace BohgeEngine
{
	//-------------------------------------------------------------------------------------------------------
	WaterManager::WaterManager(void)
		:m_pWater(NULL),
		m_isRender(false),
		m_LastTileIndex(-1)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	WaterManager::~WaterManager(void)
	{
		SAFE_DELETE( m_pWater );
#ifndef COPYSCREEN
		SAFE_DELETE( m_Refraction );
#endif
		this->Unload();
	}
	//-------------------------------------------------------------------------------------------------------
	void WaterManager::BeginUpdate( Engine& engine )
	{
		m_isRender = false;
		Frustumf frustum = engine.GetCamera()->GetFrustum();
		for ( WaterMeshes::iterator it = m_WaterDatas.begin();
			it != m_WaterDatas.end();
			it ++ )
		{
			if( frustum.Intersect( (*it)->BindBox() ) )
			{
				m_isRender = true;
				if( m_LastTileIndex != (*it)->GetIndex() )
				{
					m_LastTileIndex = (*it)->GetIndex();
					m_pWater->WaterSurface( (*it)->GetMaterial(), (*it)->GetWaterParameter() );
				}
				break;
			}
		}
		if( m_isRender )
		{
			m_pWater->BeginUpdate( engine );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void WaterManager::EndUpdate( Engine& engine )
	{
		if( m_isRender )
		{
			m_pWater->EndUpdate( engine );
		}
	}
	//-------------------------------------------------------------------------------------------------------
#ifdef COPYSCREEN
	void WaterManager::Render( Engine& engine )
	{
		if( m_isRender )
		{
			engine.GetPipeline()->CopyScreenColor();
			engine.GetPipeline()->CopyScreenDepth();
			m_pWater->Render( engine );
		}
	}
#else
	void WaterManager::Render( Engine& engine, TerrainManager* world )
	{
		if( m_isRender )
		{
			RenderTarget* now = engine.GetDevice()->PopRenderTarget( );
			engine.GetDevice()->PushRenderTarget( m_Refraction );
			engine.GetDevice()->Clear( Device::COLOR_BUFFER | Device::DEPTH_BUFFER | Device::STENCIL_BUFFER );
			world->Render( engine );
			engine.GetDevice()->PopRenderTarget( );
			engine.GetDevice()->PushRenderTarget( now );
		}
	}
#endif
	//-------------------------------------------------------------------------------------------------------
	void WaterManager::RenderNormal( Engine& engine )
	{
		if( m_isRender )
		{
			m_pWater->Render( engine );
		}
	}
	//-------------------------------------------------------------------------------------------------------
#ifdef TERRAINMANAGER
	void WaterManager::Load( const std::string& path, const TerrainManager& terrain )
#endif
#ifdef TERRAINEDIT
		void WaterManager::Load( const std::string& path, const TerrainEdit& terrain )
#endif
	{
		if( NULL == m_pWater )//���û�й���grid����newһ��
		{
#ifdef COPYSCREEN
			m_pWater = NEW WaterGrid( *((Texture2D*)Engine::Instance().GetPipeline()->GetCopyColorTexture()), *((Texture2D*)Engine::Instance().GetPipeline()->GetCopyDepthTexture()), terrain );
#else
			vector2d size = Engine::Instance().GetDevice()->GetResolutionSize();
			m_Refraction = NEW RenderTarget(size , Device::FBO_TCOLOR_TDEPTH);
			m_pWater = NEW WaterGrid( *((Texture2D*)m_Refraction->GetColorBuffer()), *((Texture2D*)m_Refraction->GetDepthBuffer()), terrain );
#endif
		}
		this->Unload();
		//��ʼ��ȡ
		string LocalPath = path + WaterFolder;
		Serializer loader( LocalPath + WaterInfoList );
		bool isContinue = true;
		while( isContinue )
		{
			int index;
			loader>>index;
			switch( index )
			{
			case SceneDataIndex::Water_Tile: 
				{
					string teerainpath;
					vector2f offset;
					loader>>offset.m_x>>offset.m_y>>teerainpath;
					teerainpath += PLANTFORMDELIMITER;
					WaterTile* temp = new WaterTile( );
					temp->Load( LocalPath + teerainpath, LocalPath );
					m_WaterDatas.push_back( temp );
				}break;
			case SceneDataIndex::Save_Done: isContinue = false; break;
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void WaterManager::Unload()
	{
		for ( WaterMeshes::iterator it = m_WaterDatas.begin();
			it != m_WaterDatas.end();
			it ++ )
		{
			SAFE_DELETE( *it );
		}
	}
	//-------------------------------------------------------------------------------------------------------
	float WaterManager::GetHeight( float x, float y ) const
	{
		float depth = -100000000;
		for ( WaterMeshes::const_iterator it = m_WaterDatas.begin();
			it != m_WaterDatas.end();
			it ++ )
		{
			if( (*it)->GetHeight( x, y, depth ) )
			{
				break;
			}
		}
		return depth;
	}

}