#include "FBXModel.h"
#include <fstream>
#include "../common/Log.h"

using namespace fbxsdk;

CFbxModel::CFbxModel():m_pMaterial(nullptr),m_pMaterialMap(nullptr),	\
m_pVertexInfo(nullptr),m_strModelPath("")
{
	m_pIndex = nullptr;
}


CFbxModel::~CFbxModel()
{
	if (m_pVertexInfo != nullptr)
	{
		m_pVertexInfo->clear();
		delete m_pVertexInfo;
		m_pVertexInfo = nullptr;
	}
	if (m_pMaterial == nullptr)
	{
		for (size_t i = 0; i < m_pMaterial->size(); ++i)
		{
			Material *mat = *(m_pMaterial->data() + i);
			if ((mat != nullptr) && (mat->m_pTexture != nullptr))
			{
				CTexture::UnLoadTexture(mat->m_pTexture);
				mat->m_pTexture = nullptr;
			}
		}
		m_pMaterial->clear();
		delete m_pMaterial;
		m_pMaterial = nullptr;
	}
	if (m_pMaterialMap == nullptr)
	{
		m_pMaterialMap->clear();
		delete m_pMaterialMap;
		m_pMaterialMap = nullptr;
	}
	if(m_pIndex != nullptr)
	{
		m_pIndex->clear();
		delete m_pIndex;
		m_pIndex = nullptr;
	}
}

void CFbxModel::InitFromFile(const char * filePath)
{
	m_strModelPath = std::string(filePath);

	fbxsdk::FbxManager *manager = fbxsdk::FbxManager::Create();
	FbxIOSettings *pFbxIOSetting = FbxIOSettings::Create(manager, IOSROOT);
	FbxImporter *pFbxImporter = FbxImporter::Create(manager, "");
	manager->SetIOSettings(pFbxIOSetting);
	// ./res/model/girl.FBX

	if (pFbxImporter->Initialize(filePath, -1, manager->GetIOSettings()))
	{
		FbxScene *pScene = FbxScene::Create(manager, "");
		pFbxImporter->Import(pScene);
		FbxNode *rootNode = pScene->GetRootNode();
		ImportNode(rootNode);
		pScene->Destroy();
	}
	pFbxIOSetting->Destroy();
	pFbxImporter->Destroy();
	manager->Destroy();
}

void CFbxModel::Draw()
{
	glPushMatrix();
	
	glScalef(0.2, 0.2, 0.2);
	glColor4ub(255, 255, 40, 255);
	for (size_t i = 0; i < m_pMaterial->size(); ++i)
	{
		glBegin(GL_TRIANGLES);
		for (size_t j = 0; j < m_pIndex->at(i).size(); ++j)
		{
			glVertex3fv(m_pVertexInfo->at(m_pIndex->at(i)[j]).m_fPos);
		}
		glEnd();
	}
	glPopMatrix();
}

void CFbxModel::ImportNode(FbxNode *node)
{
	size_t nChild = node->GetChildCount();
	if (node->GetMesh())
	{
		ImportMaterial(node);
		ImportMesh(node->GetMesh());
	}
	for (size_t i = 0; i < nChild; ++i)
	{
		ImportNode(node->GetChild(i));
	}

}

void CFbxModel::ImportMaterial(FbxNode * pNode)
{
	char szFbxTextureDir[256] = { 0 };
	strcpy_s(szFbxTextureDir, m_strModelPath.c_str());
	GetMaterialPath(szFbxTextureDir);

	size_t materialCount = pNode->GetMaterialCount();
	FbxProperty property;
	size_t nType = 0;
	for (size_t i = 0; i < materialCount; ++i)
	{
		FbxSurfaceMaterial *pSurfaceMat = pNode->GetMaterial(i);
		//Log << "Surface Material Name \t" << pSurfaceMat->GetName() <<"\n";
		if (FbxSurfaceLambert::ClassId == pSurfaceMat->GetClassId())
		{
			nType = 0;
			//Log << "Material FbxSurfaceLamber \n";
		}
		else if (FbxSurfacePhong::ClassId == pSurfaceMat->GetClassId())
		{
			nType = 1;
			//Log << "Material FbxSurfacePhong \n";
		}
		else
		{
			WriteError("unknown material");
		}
		property = pSurfaceMat->FindProperty(FbxLayerElement::sTextureChannelNames[0]);
		if (property.IsValid())
		{
			size_t nCurrentMaterialIndex = -1;
			size_t textureCount = property.GetSrcObjectCount<FbxTexture>();
			FbxTexture *texture = property.GetSrcObject<FbxTexture>(0);
			if (texture)
			{
				if (m_pMaterial == nullptr)
				{
					m_pMaterial = new std::vector<Material*>();
				}
				size_t currentMatCount = m_pMaterial->size();
				for (size_t k = 0; k < currentMatCount; ++k)
				{
					if (nType == (*m_pMaterial)[k]->mType)
					{
						if (strcmp(texture->GetName(), (*m_pMaterial)[k]->mDiffuseColorTexture))
						{
							nCurrentMaterialIndex = k;
							break;
						}
					}
					else if (nType == (*m_pMaterial)[k]->mType)
					{
						if (strcmp(texture->GetName(), (*m_pMaterial)[k]->mDiffuseColorTexture))
						{
							nCurrentMaterialIndex = k;
							break;
						}
					}
				}
				if (nCurrentMaterialIndex == -1)
				{
					Material *mat = new Material;
					mat->mType = nType;
					memset(mat->mDiffuseColorTexture, 0, sizeof(char) * 256);
					
					strcpy_s(mat->mDiffuseColorTexture, texture->GetName());
					
					std::string texturePath(szFbxTextureDir);
					
					texturePath += texture->GetName();
					
					WriteInfo("TexturePath = : %s", texturePath.c_str());
					
					nCurrentMaterialIndex = m_pMaterial->size();

					mat->m_pTexture = CTexture::LoadTexture(mat->mDiffuseColorTexture);
					m_pMaterial->push_back(mat);
				}
				if (m_pMaterialMap == nullptr)
				{
					m_pMaterialMap = new std::unordered_map<size_t, size_t>();
				}
				m_pMaterialMap->insert(std::make_pair(i, nCurrentMaterialIndex));
			}
		}
	}
}

void CFbxModel::ImportMesh(FbxMesh * pMesh)
{
	std::vector<float> pos;
	ImportPosition(pMesh, pos);

	FbxStringList uvSetNames;
	pMesh->GetUVSetNames(uvSetNames);
	if (uvSetNames.GetCount() == 0)
	{
		WriteError("ImportMesh GetUV Names Failed");
	}
	
	size_t polygonCount = pMesh->GetPolygonCount();
	size_t polygonIndexCount = 0;
	for (size_t i = 0; i < polygonCount; ++i)
	{
		polygonIndexCount += pMesh->GetPolygonSize(i);
	}
	/*if (polygonIndexCount == pMesh->GetPolygonVertexCount())
	{
		Log << __FUNCTION__ << "\t" << __LINE__ << "\tGetPolygonVertexCount return all points count\t" << "\n";
	}*/
	if (m_pVertexInfo == nullptr)
	{
		m_pVertexInfo = new std::vector<CVertexData>(polygonCount * 3);
	}

	FbxLayerElementArrayTemplate<int> *pMaterialIndex = nullptr;
	pMesh->GetMaterialIndices(&pMaterialIndex);
	FbxGeometryElement::EMappingMode mode = pMesh->GetElementMaterial()->GetMappingMode();
	if (m_pIndex == nullptr)
	{
		m_pIndex = new std::vector<std::vector<int>>;
	}
	m_pIndex->resize(m_pMaterial->size());
	switch (mode)
	{
	case fbxsdk::FbxLayerElement::eByPolygon:
	case fbxsdk::FbxLayerElement::eAllSame:
		{
			for (size_t i = 0; i < polygonCount; ++i)
			{
				int materialIndex = pMaterialIndex->GetAt(i);
				int materialRef = m_pMaterialMap->at(materialIndex);
				m_pIndex->at(materialRef).push_back(i * 3);
				m_pIndex->at(materialRef).push_back(i * 3 + 1);
				m_pIndex->at(materialRef).push_back(i * 3 + 2);


			}
		}
		break;

	default:
		break;
	}
	CVertexData *pVertex = m_pVertexInfo->data();
	for (size_t i = 0; i < polygonCount; ++i)
	{
		size_t currentIndex = pMesh->GetPolygonSize(i);
		for (size_t j = 0; j < currentIndex; ++j)
		{
			int vertexIndex = pMesh->GetPolygonVertex(i, j);

			//Log << "Vertex  " << pos[vertexIndex * 3] << "  " << pos[vertexIndex * 3 + 1] << "  " << pos[vertexIndex * 3 + 2] << "\n";
			
			memcpy(pVertex->m_fPos, &pos[3 * vertexIndex], sizeof(float) * 3);


			FbxVector4 normal;
			if (!pMesh->GetPolygonVertexNormal(i, j, normal))
			{
				WriteError("GetPolygonVertex Normal Failed");
			}
			//memcpy(pVertex->m_fNormal, normal.mData)
			for (size_t k = 0; k < 3; ++k)
			{
				pVertex->m_fNormal[k] = normal.mData[k];
			}


			FbxVector2 uv;
			bool pUnmapped = true;
			if (!pMesh->GetPolygonVertexUV(i, j, uvSetNames.GetItemAt(0)->mString.Buffer(), uv, pUnmapped))
			{
				WriteError("GetPolygonVertex UV Failed");
			}
			for (size_t k = 0; k < 2; ++k)
			{
				pVertex->m_fTex[k] = uv.mData[k];
			}
			pVertex++;
		}
	}

}

void CFbxModel::ImportPosition(FbxGeometryBase * pGeo, std::vector<float>& pos)
{
	size_t CtrlPointCount = pGeo->GetControlPointsCount();
	pos.resize(CtrlPointCount * 3);
	FbxVector4 *CtrlPoint = pGeo->GetControlPoints();
	for (size_t i = 0; i < CtrlPointCount; ++i)
	{
		pos[3 * i] = CtrlPoint[i].mData[0];
		pos[3 * i + 1] = CtrlPoint[i].mData[1];
		pos[3 * i + 2] = CtrlPoint[i].mData[2];
	}
}

void CFbxModel::GetMaterialPath(char * filepath)
{
	//char *path = filepath;
	memset((void*)(filepath + strlen(filepath) - 4), 0, 4);
	//strcat(filepath, ".fbm/");

	strcat_s(filepath, 256, ".fbm/");
}
