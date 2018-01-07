#pragma once
#include <iostream>
#include "fbxsdk.h"

#include "../common/Texure.h"
#include "../common/common.h"

#define STATIC_GLEW
#include <GL/glew.h>

#define GLUT_BUILDING_LIB
#include <GL/glut.h>
#include <vector>
#include <unordered_map>
#include "../common/VertexData.h"



struct Material
{
	int mType;	// 0 lambert 1 phong other : defalt lambert
	char mDiffuseColorTexture[256];
	CTexture *m_pTexture;
};


class CFbxModel
{
public:
	CFbxModel();
	~CFbxModel();
	void InitFromFile(const char *filePath);
	void Draw();
private:
	void ImportNode(FbxNode *pNode);
	void ImportMaterial(FbxNode *pNode);
	void ImportMesh(FbxMesh *pMesh);
	void ImportPosition(FbxGeometryBase*pGeo, std::vector<float> &pos);
private:
	void GetMaterialPath(char*filepath);

	std::vector<std::vector<int>> *m_pIndex;
	std::unordered_map<size_t, size_t> *m_pMaterialMap;
	std::string m_strModelPath;
	std::vector<Material*> *m_pMaterial;
	std::vector<CVertexData> *m_pVertexInfo;
};

