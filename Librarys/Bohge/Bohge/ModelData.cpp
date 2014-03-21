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



/////////////////////////////
//		ģ�ͻ�������	   //
/////////////////////////////
#include "ModelData.h"
#include "Serializer.h"
#include "3DMath.h"
#include "Texture.h"
#include "Material.h"
#include "Vertex.h"
#include "RendBuffer.h"
#include "FilePath.h"
#include "ObjectDataIndex.h"
#include <vector>

using namespace std;



namespace BohgeEngine
{

	RendMesh::~RendMesh()
	{
		if ( !m_isCopyed )
		{
			SAFE_DELETE( m_pBuffer );
			SAFE_DELETE( m_pIndicesBuffer );
			SAFE_DELETE( m_pVertexBuffer );
		}
	}



	//--------------------------------------------------------------------------------------------------------------------------------------
	SkinningKeyFrame::SkinningKeyFrame()
		:m_FrameBging(0),
		m_FrameEnd(0),
		m_PlayBegin(0),
		m_PlayEnd(0),
		m_Rate(0),
		m_LastFrame(0.0),
		m_CurrentFrame(0.0),
		m_isLoop(false),
		m_isPlayAnimation(false)
	{

	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void SkinningKeyFrame::LoadKeyFrame(Serializer& file)
	{
		file>>m_FrameBging>>m_FrameEnd>>m_Rate;//��ȡ��ʼ֡������֡��֡��
		m_PlayBegin = m_FrameBging;
		m_PlayEnd = m_FrameEnd;
		int BoneCount = 0;
		file>>BoneCount;
		m_BoneKeyFrames.resize(BoneCount);
		for (int i = 0 ; i < BoneCount; i ++ )
		{
			std::string Name;
			int frameCount = 0 ;
			file>>m_BoneKeyFrames[i].m_Name>>frameCount;//��ȡ��ǰ����������key����
			m_BoneKeyFrames[i].m_QuaternionArray.resize(frameCount);
			for (int j = 0 ; j < frameCount ; j ++)
			{
				Quaternionf real,dual;
				file>>real.x() >>real.y() >>real.z() >>real.w();
				file>>dual.x() >>dual.y() >>dual.z() >>dual.w();
				m_BoneKeyFrames[i].m_QuaternionArray[j].first = real;
				m_BoneKeyFrames[i].m_QuaternionArray[j].second = dual;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	int SkinningKeyFrame::GetIndex(const std::string& jointName)
	{
		for ( int i = 0; i < m_BoneKeyFrames.size(); i ++)
		{
			if( jointName == m_BoneKeyFrames[i].m_Name )
			{
				return i;
			}
		}
		ASSERT(false);//���ܴ��ڹ����ڶ����ؼ����в�����
		return -1;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void SkinningKeyFrame::SetTime(uint timespan)
	{
		if( m_isLoop )
		{
			m_LastFrame = m_CurrentFrame;
			float sec = TO_SEC(timespan);
			float IncreaseFrame = sec * m_Rate;
			m_CurrentFrame += IncreaseFrame;
			if( m_CurrentFrame >= static_cast<float>( m_PlayEnd - m_FrameBging ) ) //����������ֵ
			{
				m_CurrentFrame = m_PlayBegin 
					+ static_cast<int>( m_CurrentFrame ) % ( m_PlayEnd  - m_FrameBging )
					+ m_CurrentFrame - static_cast<int>( m_CurrentFrame );
			}
		}
		else
		{
			if( m_CurrentFrame < static_cast<float>( m_PlayEnd  - m_FrameBging - 1 ) )
			{
				m_LastFrame = m_CurrentFrame;
				float sec = TO_SEC(timespan);
				float IncreaseFrame = sec * m_Rate;
				m_CurrentFrame += IncreaseFrame;
			}
			if( m_CurrentFrame >= static_cast<float>( m_PlayEnd - m_FrameBging - 1 ) ) //����������ֵ
			{
				m_CurrentFrame = m_PlayEnd  - m_FrameBging - 1;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	std::pair<Quaternionf, Quaternionf> SkinningKeyFrame::GetFrame(int jointIndex)
	{
		int floor = static_cast<int>(m_CurrentFrame);
		float weight = m_CurrentFrame - floor;
		int ceili = floor + 1;
		//if ( ceili >= m_FrameEnd )
		//{
		//	ceili = m_FrameBging;
		//}
		if ( ceili >= m_PlayEnd - m_FrameBging )
		{
			ceili = m_PlayBegin;
		}
		std::pair<Quaternionf, Quaternionf>& FloorQuat = m_BoneKeyFrames[jointIndex].m_QuaternionArray[floor];
		std::pair<Quaternionf, Quaternionf>&  CeilQuate = m_BoneKeyFrames[jointIndex].m_QuaternionArray[ceili];
		return Math::Sclerp(FloorQuat.first, FloorQuat.second, CeilQuate.first, CeilQuate.second, weight);
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	std::vector< std::string > SkinningKeyFrame::GetJointNameList()
	{
		vector< string > nameVer;
		for ( int i = 0; i < m_BoneKeyFrames.size(); i ++)
		{
			nameVer.push_back( m_BoneKeyFrames[i].m_Name );
		}
		return nameVer;
	}

	//--------------------------------------------------------------------------------------------------------------------------------------













	//--------------------------------------------------------------------------------------------------------------------------------------
	ModelData::ModelData()
		:m_MaxBoneIndexCount(0),
		m_isBone(false)
	{

	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	ModelData::~ModelData()
	{
		for ( int i = 0 ; i < m_MaterialVector.size(); i ++ )
		{
			SAFE_DELETE( m_MaterialVector[i] );
		}
		for ( int i = 0 ; i < m_RendMeshVector.size(); i ++ )
		{
			SAFE_DELETE( m_RendMeshVector[i] );
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void ModelData::_LoadBones(Serializer& file)
	{
		m_isBone = true ;//��������
		int size;
		file>>m_MaxBoneIndexCount>>size;//�õ���������������
		m_JointVector.resize(size);
		for( int i = 0; i < size ; i ++ )
		{
			file>>m_JointVector[i].m_Name>>m_JointVector[i].m_Parent;//��ȡjoint���ƺ͸�ID
			file>>m_JointVector[i].m_RealBind.x()
				>>m_JointVector[i].m_RealBind.y()
				>>m_JointVector[i].m_RealBind.z()
				>>m_JointVector[i].m_RealBind.w();
			file>>m_JointVector[i].m_DaulBind.x()
				>>m_JointVector[i].m_DaulBind.y()
				>>m_JointVector[i].m_DaulBind.z()
				>>m_JointVector[i].m_DaulBind.w();
			m_JointVector[i].m_InversPair = Math::Inverse(m_JointVector[i].m_RealBind, m_JointVector[i].m_DaulBind);
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void ModelData::_LoadMaterials(Serializer& file, const std::string& folderpath)
	{
		int MaterialCount = 0;
		file>> MaterialCount;
		for ( int i = 0 ; i < MaterialCount ; i ++)
		{
			Material* m = NEW Material();
			m_MaterialVector.push_back(m);

			for ( bool isDone = false; !isDone; )
			{
				int att = 0;
				file>> att;
				switch( att )
				{
				case static_cast<int>(Material::ATT_AMBIENT) : file>>m->Ambient().m_x >>m->Ambient().m_y >>m->Ambient().m_z; break;
				case static_cast<int>(Material::ATT_DIFFUSE) : file>>m->Diffuse().m_x >>m->Diffuse().m_y >>m->Diffuse().m_z; break;
				case static_cast<int>(Material::ATT_SPECULAR) : file>>m->Specular().m_x >>m->Specular().m_y >>m->Specular().m_z; break;
				case static_cast<int>(Material::ATT_EMIT) : file>>m->Emit().m_x >>m->Emit().m_y >>m->Emit().m_z; break;
				case static_cast<int>(Material::ATT_OPCITY) : file>>m->Opacity(); break;
				case static_cast<int>(Material::ATT_SPECULAR_LEVEL) : file>>m->SpeLevel(); break;
				case static_cast<int>(Material::ATT_SHININESS) : file>>m->Shininess(); break;
				case static_cast<int>(Material::ATT_TEXTURES) :
					{
						int texCount = 0;
						file>> texCount;
						for ( int j = 0; j < texCount; j ++)
						{
							std::string useage;
							std::string name;
							int ID;
							file>>useage>>ID>>name;
							m->LoadTexture( static_cast<Material::Material_Attribute>(ID), folderpath + name );
						}
					}break;
				case static_cast<int>(Material::ATTRIBUTE_END) : isDone = true; break;
				}
			}
			if( m_isBone ) //������ж�����
			{
				//���ڶ��ڳ�ʼ�׶�
				switch( m_MaxBoneIndexCount )
				{
				case 1:
					{
						ShaderManager::ShaderName sn;
						sn = NULL == m->Attribute( Material::ATT_TEX_BUMP ) ? ShaderManager::SkeletonModel1B : ShaderManager::SkeletonModelBump1B;
						m->SetShader( Pipeline::PT_DEPTH, ShaderManager::SkeletonModelDepth1B );
						m->SetShader( Pipeline::PT_NORMAL, ShaderManager::SkeletonModelNormal1B );
						m->SetShader( Pipeline::PT_LIGHTING, sn );
					}break;
				case 2:
					{
						ShaderManager::ShaderName sn;
						sn = NULL == m->Attribute( Material::ATT_TEX_BUMP ) ? ShaderManager::SkeletonModel2B : ShaderManager::SkeletonModelBump2B;
						m->SetShader( Pipeline::PT_DEPTH, ShaderManager::SkeletonModelDepth2B );
						m->SetShader( Pipeline::PT_NORMAL, ShaderManager::SkeletonModelNormal2B );
						m->SetShader( Pipeline::PT_LIGHTING, sn );
					}break;
				case 3:
					{
						ShaderManager::ShaderName sn;
						sn = NULL == m->Attribute( Material::ATT_TEX_BUMP ) ? ShaderManager::SkeletonModel3B : ShaderManager::SkeletonModelBump3B;
						m->SetShader( Pipeline::PT_DEPTH, ShaderManager::SkeletonModelDepth3B );
						m->SetShader( Pipeline::PT_NORMAL, ShaderManager::SkeletonModelNormal3B );
						m->SetShader( Pipeline::PT_LIGHTING, sn );
					}break;
				case 4:
					{
						ShaderManager::ShaderName sn;
						sn = NULL == m->Attribute( Material::ATT_TEX_BUMP ) ? ShaderManager::SkeletonModel4B : ShaderManager::SkeletonModelBump4B;
						m->SetShader( Pipeline::PT_DEPTH, ShaderManager::SkeletonModelDepth4B );
						m->SetShader( Pipeline::PT_NORMAL, ShaderManager::SkeletonModelNormal4B );
						m->SetShader( Pipeline::PT_LIGHTING, sn );
					}break;
				default: ASSERT(false);
				}
			}
			else
			{
				m->SetShader( Pipeline::PT_DEPTH, ShaderManager::ModelDepth );
				m->SetShader( Pipeline::PT_NORMAL, ShaderManager::ModelNormal );
				if( NULL == m->Attribute( Material::ATT_TEX_BUMP ) )
				{
					m->SetShader( Pipeline::PT_LIGHTING, ShaderManager::ModelPrePixelLight );
				}
				else
				{
					m->SetShader( Pipeline::PT_LIGHTING, ShaderManager::ModelBump );
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void ModelData::_LoadMeshesData(Serializer& file, const Vertex_Attrs& att, vector2f& tex, vector3f& pos, vector3f& nor, vector3f& bin, vector4f& tat, vector4f& bone, vector4f& weight )
	{
		if( att.vertex )
		{
			file>>pos.m_x >>pos.m_y >>pos.m_z;
		}
		if( att.normal )
		{
			file>>nor.m_x >>nor.m_y >>nor.m_z;
		}
		if( att.tangent )
		{
			file>>tat.m_x >>tat.m_y >>tat.m_z>>tat.m_w;
		}
		if( att.binormal )
		{
			file>>bin.m_x >>bin.m_y >>bin.m_z;
		}
		if( att.tex )
		{
			int size = 0 ;
			file>>size;
			//ASSERT( size == 1 );//��ʱֻ�ܴ���һ�����������ģ��
			for( int i = 0 ; i < size ;  i++)
			{
				if ( i == 0 )
				{
					file>>tex.m_x >>tex.m_y;
				}
				else
				{
					vector2f local;
					file>>local.m_x >>local.m_y;
				}
			}
		}
		if( att.bone )
		{
			int bpv = 0;//ÿ������Ĺ�������
			file>>bpv;
			for ( int i = 0 ; i < bpv ; i ++ )
			{
				file>>bone.data[i]
				>>weight.data[i];
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void ModelData::_LoadMeshes(Serializer& file)
	{
		const static int IA_VERTEX = 1 << 0;
		const static int IA_NORMAL = 1 << 1;
		const static int IA_BINORMAL = 1 << 2;
		const static int IA_TANGENT = 1 << 3;
		const static int IA_TEX_COORD = 1 << 4;
		const static int IA_BONEWEIGHT = 1 << 5;


		vector<RendBuffer*> meshes;//�Ѿ���ȡ����mesh
		int meshSize = 0;
		file>> meshSize;
		for ( int i = 0; i < meshSize ; i ++ )
		{
			RendMesh* rm = NEW RendMesh();
			rm->m_pBuffer = NEW RendBuffer(	Device::RM_TRIANGLES );//����һ����Ⱦbuffer
			rm->m_pVertexBuffer = NEW VertexBuffer( Device::MU_STATIC );
			rm->m_pIndicesBuffer = NEW IndicesBuffer( Device::MU_STATIC );
			rm->m_pBuffer->SetVertexBuffer( rm->m_pVertexBuffer );
			rm->m_pBuffer->SetIndicesBuffer( rm->m_pIndicesBuffer );
			meshes.push_back( rm->m_pBuffer );
			file>>rm->m_Name>>rm->m_Materialindex;//����mesh�����ƺͲ�������
			for ( bool isDone = false; !isDone; )
			{
				int dataType = 0;
				file>> dataType;//mesh�������ͣ��Ƕ��㣬��������
				switch( dataType )
				{
				case static_cast<int>(MESHES_DATA) :
					{
						Vertex_Attrs VertexElemnts;//�������������
						for ( bool isDone = false; !isDone; )
						{
							int element = 0;
							file>> element;
							switch( element )
							{
							case static_cast<int>(VE_VERTEX): VertexElemnts.vertex = true; VertexElemnts.Attrs |= IA_VERTEX; break;
							case static_cast<int>(VE_NORMAL): VertexElemnts.normal = true; VertexElemnts.Attrs |= IA_NORMAL; break;
							case static_cast<int>(VE_BINORMAL): VertexElemnts.binormal = true; VertexElemnts.Attrs |= IA_BINORMAL; break;
							case static_cast<int>(VE_TANGENT): VertexElemnts.tangent = true; VertexElemnts.Attrs |= IA_TANGENT; break;
							case static_cast<int>(VE_TEX_COORD): VertexElemnts.tex = true; VertexElemnts.Attrs |= IA_TEX_COORD; break;
							case static_cast<int>(VE_BONEWEIGHT): VertexElemnts.bone = true; VertexElemnts.Attrs |= IA_BONEWEIGHT; break;
							case static_cast<int>(VE_END): isDone = true; break;
							}
						}
						//ASSERT( true == VertexElemnts.vertex == VertexElemnts.normal == VertexElemnts.tex == VertexElemnts.tangent );//����Ҫ��binormal��ȫ�����Զ���
						int meshSize = 0 ;
						file>>meshSize;//��ȡ���������

						//��ʼ��ȡ����
						{
							vector4f weight, index, tat;
							vector3f pos, nor, bin;
							vector2f tex;
							switch ( VertexElemnts.Attrs )
							{
							case IA_VERTEX | IA_TEX_COORD | IA_NORMAL | IA_BONEWEIGHT :
								{
									VertexTextureNormalBoneWeight* vertex = NEW VertexTextureNormalBoneWeight[meshSize];
									for (int i = 0 ; i < meshSize ; i ++)
									{
										this->_LoadMeshesData(file, VertexElemnts, tex, pos, nor, bin, tat, index, weight);
										vertex[i] = VertexTextureNormalBoneWeight(vector4f(pos), tex, nor, index, weight);
									}
									rm->m_pVertexBuffer->FlushVertexBuffer( meshSize, vertex );
									SAFE_DELETE_ARRAY( vertex );
								}break;
							case IA_VERTEX | IA_TEX_COORD | IA_NORMAL | IA_TANGENT | IA_BONEWEIGHT :
								{
									VertexTextureNormalTangentBoneWeight* vertex = NEW VertexTextureNormalTangentBoneWeight[meshSize];
									for (int i = 0 ; i < meshSize ; i ++)
									{
										this->_LoadMeshesData(file, VertexElemnts, tex, pos, nor, bin, tat, index, weight);
										vertex[i] = VertexTextureNormalTangentBoneWeight(vector4f(pos), tex, nor, tat, index, weight);
									}
									rm->m_pVertexBuffer->FlushVertexBuffer( meshSize, vertex );
									SAFE_DELETE_ARRAY( vertex );
								}break;
							case IA_VERTEX | IA_TEX_COORD | IA_NORMAL | IA_TANGENT | IA_BONEWEIGHT | IA_BINORMAL:
								{
									VertexTextureNormalTangentBinormalBoneWeight* vertex = NEW VertexTextureNormalTangentBinormalBoneWeight[meshSize];
									for (int i = 0 ; i < meshSize ; i ++)
									{
										this->_LoadMeshesData(file, VertexElemnts, tex, pos, nor, bin, tat, index, weight);
										vertex[i] = VertexTextureNormalTangentBinormalBoneWeight(vector4f(pos), tex, nor, tat, bin, index, weight);
									}
									rm->m_pVertexBuffer->FlushVertexBuffer( meshSize, vertex );
									SAFE_DELETE_ARRAY( vertex );
								}break;
							default:
								{
									ASSERT(false);//��֧�ֵĸ�ʽ
								}break;
							}
						}
					}break;
				case static_cast<int>(MESHES_INDEX) :
					{
						int indexSize = 0 ;
						file>> indexSize;
						ASSERT(indexSize <= ushort(-1));
						ushort *index = NEW ushort[ 3 * indexSize ];//�����ʱ����һ�α���3����һ�� triangle list
						uint a,b,c;
						for( int i = 0 ; i < indexSize ; i ++ )
						{
							file>> a >> b >> c;
							index[ 3 * i ] = static_cast<ushort>( a );
							index[ 3 * i + 1] = static_cast<ushort>( b );
							index[ 3 * i + 2] = static_cast<ushort>( c );
						}
						rm->m_pIndicesBuffer->FlushIndiesBuffer( indexSize*3, index );
						SAFE_DELETE_ARRAY(index);
					}break;
				case static_cast<int>(MESHES_BINDBOX) :
					{
						vector3f min,max;
						file>>min.m_x
							>>min.m_y
							>>min.m_z

							>>max.m_x
							>>max.m_y
							>>max.m_z;
						rm->m_BindBox = aabbox3df( min, max );
					}break;
				case static_cast<int>(MESHES_END) :
					{
						m_RendMeshVector.push_back(rm);
						isDone = true;
					}break;
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void ModelData::_LoadKeyFrame(Serializer& file)
	{
		m_KeyFrame.LoadKeyFrame(file);
	}
	//--------------------------------------------------------------------------------------------------------------------------------------
	void ModelData::LoadModel(const std::string& name)
	{
		m_FileName = name.substr( name.find_last_of("/\\")+1);
		string path = name + PLANTFORMDELIMITER;
		Serializer file( path + m_FileName + ModelFileExpand );//���ļ���
		for ( bool isDone = false; !isDone ; )
		{
			int chunk = -1;
			file>>chunk;//��ȡchunk
			switch ( chunk )
			{
			case static_cast<int>(CHUNK_BONES) : _LoadBones(file); break;
			case static_cast<int>(CHUNK_MATERIALS) : _LoadMaterials(file, path); break;
			case static_cast<int>(CHUNK_MESHES) : _LoadMeshes(file); break;
			case static_cast<int>(CHUNK_KEYFRAMES) : _LoadKeyFrame(file); break;
			case static_cast<int>(MODEL_END) : isDone = true; break;
			}
		}
		//����ȡ��ϵ�ʱ��m_RendMeshVector�еĲ���ָ��ָ��ֵ
		if ( 0 < m_MaterialVector.size() )
		{
			for (int i = 0 ; i < m_RendMeshVector.size(); i ++ )
			{
				m_RendMeshVector[i]->m_pMaterial = m_MaterialVector[m_RendMeshVector[i]->m_Materialindex];
				m_BindBox.Contains( m_RendMeshVector[i]->m_BindBox.GetMax() );
				m_BindBox.Contains( m_RendMeshVector[i]->m_BindBox.GetMin() );
			}
		}
		if( 0 < m_JointVector.size() )
		{
			for ( int i = 0 ; i < m_JointVector.size(); i++ )//��ʼ���ؼ�֡����
			{
				m_JointVector[i].m_KeyFrameIndex = m_KeyFrame.GetIndex(m_JointVector[i].m_Name);
			}
		}
	}

}