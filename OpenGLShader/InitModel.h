#pragma once

#define STATIC_GLEW
#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include "./common/utils.h"
#include "./common/VertexData.h"
#include "./common/common.h"

#include <GL/wglew.h>

#include <fstream>
#include <ctime>
#include "./model/ObjModel.h"
#include "./test/Polygon.h"
#include "./common/Timer.h"
#include "./test/Frustum.h"
#include <SOIL\SOIL.h>
#include "./test/Triangles.h"
#include "./test/PointSprite.h"
#include "./test/ParticleCube.h"
#include "./test/ParticleGS.h"
#include "./test/ParticleEmit.h"
#include "./test/CPolygonTesslation.h"
#include "./test/LightModelTest.h"
#include "./test/Sphere.h"
#include "./test/WaterWave.h"
#include "./test/TextureTest.h"
#include "./test/Ground.h"
#include "./test/SkyBox.h"
#include "./common/Camera.h"
#include "./test/XRay.h"
#include "./test/BloomTest.h"

extern CCamera *director;
extern POINT orgPoint;


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
	
	CParticleEmit *pParticleEmit = nullptr;
	CPolygonTess *pPolygonTess = nullptr;
	CLightTest *pLightTest = nullptr;
	CSphere *pSphere = nullptr;
	CWaterWave *pWaterWave = nullptr;
	CGround *pGround = nullptr;
	CSkyBox *pSkyBox = nullptr;
	CXRay *pRay = nullptr;
	CBloomTest *pBloomTest = nullptr;
	
	


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
	//pPointSprite->InitTexture("./res/image/xiongxiang.jpg", "./res/image/3.bmp");
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

	//pSphere = new CSphere(0.4, 50, 50);
	//pSphere->InitTexture("./res/image/xiongxiang.jpg");
	if (pSphere && pModelVec)
	{
		pModelVec->push_back(pSphere);
	}

	//pWaterWave = new CWaterWave;
	//pWaterWave->InitTexture("./res/image/xiongxiang.jpg");
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

	//pGround
	//pGround = new CGround;
	//pGround->InitTexture("./res/image/water.jpg");
	if (pGround && pModelVec)
	{
		pModelVec->push_back(pGround);
	}

	pSkyBox = new CSkyBox;
	pSkyBox->InitTexture("./res/image/skybox/");
	if (pSkyBox && pModelVec)
	{
		//mat4f model = CMatrix<4, 4, float>::GetTranslate<float>(0.0f, 0.0f, -3.0f);
		//pSkyBox->SetModelMatrix(model);
		pModelVec->push_back(pSkyBox);
	}

	//pRay = new CXRay(0.8, 50, 50);
	if (pRay && pModelVec)
	{
		pModelVec->push_back(pRay);
	}

	//pBloomTest = new CBloomTest;
	if (pBloomTest && pModelVec)
	{
		pModelVec->push_back(pBloomTest);
	}

	return true;
}


LRESULT CALLBACK GLWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'A':
		case 'a':
			director->SetMoveLeft(true);
			//director->m_bMoveLeft = true;
			break;
		case 'D':
		case 'd':
			director->SetMoveRight(true);
			//director->m_bMoveRight = true;
			break;
		case 'W':
		case 'w':
			director->SetMoveForward(true);
			//director->m_bMoveForward = true;
			break;
		case 'S':
		case 's':
			director->SetMoveBackward(true);
			//director->m_bMoveBackward = true;
			break;
		case 'Q':
		case 'q':
			director->SetRotateRight(true);
			//director->m_bRotateY = true;
			break;
		case 'E':
		case 'e':
			director->SetRotateLeft(true);
			//director->m_bNegRotateY = true;
			break;
		}
		break;
	case WM_KEYUP:
		switch (wParam)
		{
		case 'A':
		case 'a':
			director->SetMoveLeft(false);
			//director->m_bMoveLeft = false;
			break;
		case 'D':
		case 'd':
			director->SetMoveRight(false);
			//director->m_bMoveRight = false;
			break;
		case 'W':
		case 'w':
			director->SetMoveForward(false);
			//director->m_bMoveForward = false;
			break;
		case 'S':
		case 's':
			director->SetMoveBackward(false);
			//director->m_bMoveBackward = false;
			break;
		case 'Q':
		case 'q':
			director->SetRotateRight(false);
			//director->m_bRotateY = false;
			break;
		case 'E':
		case 'e':
			director->SetRotateLeft(false);
			//director->m_bNegRotateY = false;
			break;
		}
		break;
	case WM_LBUTTONDOWN:
	{
		SIZE tmpWinSize = director->GetWindowSize();
		int x = LOWORD(lParam) - tmpWinSize.cx / 2;
		int y = tmpWinSize.cy / 2 - HIWORD(lParam);
		/*if (x <0 && x > -director->m_nViewportWidth / 2)
		{
			if (y <0 && y > -director->m_nViewportHeight / 2)
			{
				bPushOnMe = true;
			}
		}*/
	}
		break;
		
	case WM_LBUTTONUP:
		
		break;
	case WM_RBUTTONDOWN:
		orgPoint.x = LOWORD(lParam);
		orgPoint.y = HIWORD(lParam);
		ClientToScreen(hwnd, &orgPoint);
		//SetCapture(hwnd);
		//ShowCursor(false);
		director->SetRotateView(true);
		//director->m_bRotateView = true;
		break;
	case WM_RBUTTONUP:
		//SetCursorPos(orgPoint.x, orgPoint.y);
		//ReleaseCapture();
		//ShowCursor(true);
		director->SetRotateView(false);
		//director->m_bRotateView = false;
		break;
	case WM_MOUSEMOVE:
		if (director->GetRotateView())// director->m_bRotateView)
		{
			POINT curPos;
			curPos.x = LOWORD(lParam);
			curPos.y = HIWORD(lParam);
			ClientToScreen(hwnd, &curPos);
			int deltaX = curPos.x - orgPoint.x;
			int deltaY = curPos.y - orgPoint.y;
			float angleY = (float)deltaY / 1000;
			float angleX = (float)deltaX / 1000;
			director->Pitch(angleY);
			director->Yaw(angleX);
			SetCursorPos(orgPoint.x, orgPoint.y);
		}
		break;
	case WM_SIZE:
	{
		size_t nWidth = LOWORD(lParam); // width of client area
		size_t nHeight = HIWORD(lParam); // height of client area
		glViewport(0, 0, nWidth, nHeight);
		director->SetWindowSize(nWidth, nHeight);
		//director->m_nViewportWidth = nWidth;
		//director->m_nViewportHeight = nHeight;
	}
		
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}
