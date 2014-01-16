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



//////////////////////////////////
//		����ʱ����������	   //
/////////////////////////////////
#pragma once

namespace BohgeEngine
{
#define SceneListExpand ".list"
#define SceneEntityExpand ".oed"
#define SceneFolderExpand ".sfd"
#define SceneName "Scene.sd"
#define SceneExpand ".sd"
#define ObjectFolder "Object/"
#define ObjectListFolder "ObjectList/"
#define ObjectEntityFolder "ObjectEntity/"
#define TerrainFolder "Terrain/"
#define TerrainTextureFolder "TerrainTexture/"
#define WaterFolder "Water/"
#define WaterTextureFolder "WaterTexture/"
#define PlantFolder "Plant/"
#define PlantTextureFolder "PlantTexture/"
#define ObjectInfoList "Object.list"
#define ObjectEntityList "ObjectEntiyt.list"
#define TerrainInfoList "Terrain.list"
#define TerrainMeshName "TerrainMesh.btm"
#define TerrainWeight "Weight.png"
#define TerrainDiffuse "Diffuse.png"
#define WaterInfoList "Water.list"
#define WaterData "Water.bwd"
#define PlantInfoList "Plant.list"
#define PlantMeshName "PlantMesh.bpm"


namespace SceneDataIndex
{
		enum
		{
			Save_Begin = 10000,
				//��������			
				Scene_Property	= 11000,
					Main_Property = 11200,//�ƹ��������
						Main_LightDir =11201,
						Main_LightPos =11202,
						Main_LightClr =11203,
						Main_AmbieClr =11204,
						Main_FogDista =11205, //�������
						Main_FogColor =11206,
						Main_Unwater  =11207,
						Main_FogParam  =11208,

					Terrain_Property	= 11100, //��������
						Terrain_TileSize	= 11101,
						Terrain_Split		= 11102,
						Terrain_Scale		= 11103,
						Terrain_TextureSize = 11104,
						Terrain_Repeat		= 11105,
				//�����������
				Terrain_List	= 12000,
					Terrain_Tile = 12100,
						Terrain_Offset	= 12101,
						Terrain_Vertex	= 12102,//����߶Ⱥͷ���
						Terrain_Diffuse	= 12103,//ֱ������;��ȵ���������ͼ����ϸ����ͼ��
						Terrain_Materal	= 12104,
						Terrain_Bind	= 12105,
						Terrain_CDLOD	= 12106,//��ֵ���ݵ��ƶ�����ԭĿ���ĸ߶Ȳ�ֵ
						Terrain_Normal	= 12107,
						Terrain_Weight  = 12108,//ֻ�Ǳ���ʱ����Ч
				//����ˮ������
				Water_List	= 13000,
					Water_Tile	= 13100,
						Water_Offset	= 13101,
						Water_Materal	= 13102,
						Water_Param		= 13103,
						Water_Bind		= 13104,

				//�������ṹ
				Trunk_List = 14000,
					Trunk_Tile = 14100,
						Trunk_Offset	= 14101,

				//ֲ��
				Plant_List = 15000,
					Plant_Tile = 15100,
						Plant_Offset	= 15101,
						Plant_Plant		= 15102,//�༭���е�ÿһ��ֲ��
						Plant_Trans		= 15103,//��������
						Plant_Buffr		= 15104,//�༭������

						//�����ṩapp��ȡ������
						Plant_BindBox	= 15105,
						Plant_Materal	= 15106,
						Plant_Vertex	= 15107,
						Plant_Indices	= 15108,
						Plant_Shader	= 15109,//���ɺ���Ҷ�ݴԣ���Ҫ��ͬ��shader,0�����ɣ�1����Ҷ��2�ǲݴ�
						Plant_Quda_Struct = 15110,//�Ĳ����ṹ
						Plant_Quda_Buffer = 15111,//ȫ����ͬbuffer�ĸ���
						Plant_Buffer_End = 15112,//������һ��buffer��

				//Object
				Object_List = 16000,
					ObjectListVersion = 1,

			Save_Done = -1,//�������
		};
	}

}