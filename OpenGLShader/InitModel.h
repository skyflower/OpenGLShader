#pragma once

#define STATIC_GLEW
#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include "utils.h"
#include "VertexData.h"
#include "common.h"

#include <GL/wglew.h>

#include <fstream>
#include <ctime>
#include "ObjModel.h"
#include "Polygon.h"
#include "Timer.h"
#include "Frustum.h"
#include <SOIL\SOIL.h>
#include "Triangles.h"
#include "PointSprite.h"
#include "ParticleCube.h"
#include "ParticleGS.h"
#include "ParticleTFO.h"
#include "ParticleEmit.h"
#include "CPolygonTesslation.h"
#include "LightModelTest.h"
#include "Sphere.h"
#include "WaterWave.h"
#include "TextureTest.h"


bool InitModel(std::vector<C3DModel*> *&pModelVec, CVector<4, float> &Perspective)
{
	if (pModelVec == nullptr)
	{
		pModelVec = new std::vector<C3DModel*>;
	}

	CObjModel *pObjModel = nullptr;
	CPolygon *pPolygon = nullptr;
	CFrustum *pFrustum = nullptr;
	CTriangle *pTriangle = nullptr;
	CPointSprite *pPointSprite = nullptr;
	CParticleCube *pParticleCube = nullptr;
	CParticleGS *pParticleGS = nullptr;
	CParticleTFO *pParticleTFO = nullptr;
	CParticleEmit *pParticleEmit = nullptr;
	CPolygonTess *pPolygonTess = nullptr;
	CLightTest *pLightTest = nullptr;
	CSphere *pSphere = nullptr;
	CWaterWave *pWaterWave = nullptr;
	


	//pObjModel = new CObjModel;
	//pObjModel->InitTexture("./res/model/Arcticcondor/ArcticCondorGold.tga");
	//pObjModel->Init("./res/model/Arcticcondor/Arcticcondor.obj");
	//pObjModel->InitTexture("./res/model/Dragon/n901.tga");
	//pObjModel->Init("./res/model/Dragon/n901.obj");
	if (pModelVec && pObjModel)
	{
		pModelVec->push_back(pObjModel);
	}

	//pPolygon = new CPolygon;
	//pPolygon->InitTexture("./res/image/xiongxiang.jpg");
	if (pModelVec && pPolygon)
	{
		pModelVec->push_back(pPolygon);
	}


	//pPointSprite = new CPointSprite;
	//pPointSprite->InitTexture("./res/image/xiongxiang.jpg");
	if (pModelVec && pPointSprite)
	{
		pModelVec->push_back(pPointSprite);
	}

	//pParticleCube = new CParticleCube;
	if (pModelVec && pParticleCube)
	{
		pModelVec->push_back(pParticleCube);
	}

	//pParticleGS = new CParticleGS;
	if (pModelVec && pParticleGS)
	{
		pModelVec->push_back(pParticleGS);
	}

	
	//pPolygonTess = new CPolygonTess;
	if (pModelVec && pPolygonTess)
	{
		pModelVec->push_back(pPolygonTess);
	}

	//pParticleTFO
	//pParticleTFO = new CParticleTFO;
	if (pModelVec && pParticleTFO)
	{
		pModelVec->push_back(pParticleTFO);
	}

	//pParticleEmit = new CParticleEmit;
	if (pModelVec && pParticleEmit)
	{
		pModelVec->push_back(pParticleEmit);
	}


	//pFrustum = new CFrustum();
	//pFrustum->Init(Perspective);
	if (pModelVec && pFrustum)
	{
		pModelVec->push_back(pFrustum);
	}


	//pTriangle = new CTriangle;
	//pTriangle->InitTexture("./res/image/xiongxiang.jpg");
	if (pModelVec && pTriangle)
	{
		pModelVec->push_back(pTriangle);
	}

	//pLightTest = new CLightTest;
	//pLightTest->InitTexture("./res/image/xiongxiang.jpg");
	if (pModelVec && pLightTest)
	{
		pModelVec->push_back(pLightTest);
	}

	//pSphere = new CSphere(2.5, 30, 30);
	//pSphere->InitTexture("./res/image/xiongxiang.jpg");
	if (pSphere && pModelVec)
	{
		pModelVec->push_back(pSphere);
	}

	pWaterWave = new CWaterWave;
	pWaterWave->InitTexture("./res/image/xiongxiang.jpg");
	if (pWaterWave && pModelVec)
	{
		pModelVec->push_back(pWaterWave);
	}

	CTextureTest *pTextureTest = nullptr;
	//pTextureTest = new CTextureTest;
	//pTextureTest->InitTexture("./res/image/xiongxiang.jpg");
	if (pTextureTest && pModelVec)
	{
		pModelVec->push_back(pTextureTest);
	}


	return true;
}


LRESULT CALLBACK GLWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
