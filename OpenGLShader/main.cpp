#include <Windows.h>

#define STATIC_GLEW
#include <GL/glew.h>

//#define GLUT_BUILDING_LIB
//#include <GL/glut.h>

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
#include "InitModel.h"
#include "./common/Log.h"
#include <thread>
#include "./common/Camera.h"
#include "./render/FrameBuffer.h"

#include "./test/FullScreen.h"

//std::fstream Log("log.txt", std::ios::trunc | std::ios::in | std::ios::out);


CCamera *director = CCamera::GetInstance();
POINT orgPoint;


extern LRESULT CALLBACK GLWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

INT __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR argv, int showCmd)
{
	std::unique_ptr<CLog> pLog(CLog::GetInstance());

	WNDCLASSEX wndclass;
	wndclass.cbClsExtra = 0;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = NULL;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIcon = NULL;
	wndclass.hInstance = hInstance;
	wndclass.hIconSm = NULL;
	wndclass.lpszClassName = L"Win32OpenGL";
	wndclass.lpfnWndProc = GLWindowProc;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	ATOM atom = RegisterClassEx(&wndclass);
	if (!atom)
	{
		DWORD error = GetLastError();
		WriteError(" RegisterClassEx erro = %u", error);
		return 0;
	}
	RECT rect;
	rect.left = 0;
	rect.right = 800;
	rect.top = 0;
	rect.bottom = 600;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, NULL);
	HWND hwnd = CreateWindowEx(NULL, L"Win32OpenGL", L"Win32OpenGL", WS_OVERLAPPEDWINDOW,// WS_OVERLAP, 
		100, 100, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);

	HDC dc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nVersion = 1;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.cStencilBits = 8;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_TYPE_RGBA;

	int pixelFormat = ChoosePixelFormat(dc, &pfd);
	SetPixelFormat(dc, pixelFormat, &pfd);
	HGLRC rc = wglCreateContext(dc);
	wglMakeCurrent(dc, rc);

	GetClientRect(hwnd, &rect);
	int viewWidth = rect.right - rect.left;
	int viewHeight = rect.bottom - rect.top;
	director->SetWindowSize(viewWidth, viewHeight);
	WriteInfo("ScreenWidth = %d, ScreenHeight = %d", viewWidth, viewHeight);
	
	vec4f Perspective;
	Perspective[0] = 45 * 3.1415927 / 180;
	Perspective[1] = (double)viewWidth / (double)viewHeight;
	Perspective[2] = 0.01f;
	Perspective[3] = 1000.0f;

	CVector<6, float> OrthoVec;
	OrthoVec[0] = -viewWidth /2.0f;
	OrthoVec[1] = viewWidth / 2.0f;
	OrthoVec[2] = -viewHeight / 2.0f;
	OrthoVec[3] = viewHeight / 2.0f;
	OrthoVec[4] = 0.01f;
	OrthoVec[5] = 100.0f;

	mat4f IdentityMatrix = CMatrix<4, 4, float>::GetUnit();
	//mat4f ViewMatrix = CMatrix<4, 4, float>::GetUnit();
	// Perspective glm  transpose
	mat4f ProjectionMatrix = CMatrix<4, 4, float>::GetPerspective(Perspective);
	//mat4f Trianslate = CMatrix<4, 4, float>::GetTranslate(0, 0, -10);
	//ProjectionMatrix = ProjectionMatrix;// *Trianslate;
	//mat4f UIProjectionMatrix = CMatrix<4, 4, float>::GetOrtho(OrthoVec);
	//glm::mat4 glmOrthoMatrix = glm::ortho(OrthoVec[0], OrthoVec[1], OrthoVec[2], OrthoVec[3], OrthoVec[4], OrthoVec[5]);
	
	//glm::tvec3<float> glmTranslateVec = glm::tvec3<float>(0.0f, 0.0f, -1.0f);
	//glm::mat4 glmTranslate = glm::translate(glmTranslateVec);
	//mat4f Translate = CMatrix<4, 4, float>::GetTranslate(0, 0, -1.0);
	//utils::OutputToFile(Log, Translate, glmTranslate);

	//mat4f inverse = ProjectionMatrix.inverse();
	//mat4f tmpMultiply = inverse * ProjectionMatrix;
	//Log << "Projection Inverse\n" << inverse;
	//Log << "Multiply \n" << tmpMultiply;

	//utils::TestMatrixGLM();
	//Log << "sizeof(CParticleVertex) = " << sizeof(CParticleVertex) << "\n";
	if (GLEW_OK != glewInit())
	{
		WriteError("glewInit Failed");
		return -1;
	}
	WriteInfo("OpenGL Version : %s", glGetString(GL_VERSION));
	WriteInfo("GLSL Version : %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
	
	//glClearColor(0.2, 0.2, 0.5, 1);
	glClearColor(0, 0, 0, 1);
	glClearDepth(1.0);
	glViewport(0, 0, viewWidth, viewHeight);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	std::vector<C3DModel*> *pModelVec = nullptr;
	if (InitModel(pModelVec, Perspective) == false)
	{
		return -1;
	}

	mat4f viewMatrix = director->GetViewMatrix();
	for (size_t i = 0; i < pModelVec->size(); ++i)
	{
		C3DModel *&p = *(pModelVec->data() + i);
		p->SetMatrix(viewMatrix, ProjectionMatrix);
	}
	//CLightTest *pLightTest = new CLightTest;
	//pLightTest->SetMatrix(viewMatrix, ProjectionMatrix);


	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	//CShader *pErosionShader = new CShader("./shader/ImageProcess/FullScreen.vs", "./shader/ImageProcess/erosion.fs");
	//CShader *pDilationShader = new CShader("./shader/ImageProcess/FullScreen.vs", "./shader/ImageProcess/dilation.fs");
	CShader *pHdrShader = new CShader("./shader/HDR/hdrLight.vs", "./shader/HDR/hdrLight.fs");
	//CShader *pGussianShader = new CShader("./shader/ImageProcess/FullScreen.vs", "./shader/ImageProcess/gussian.fs");
	CShader *pOriginShader = new CShader("./shader/ImageProcess/FullScreen.vs", "./shader/ImageProcess/FullScreen.fs");
	//CShader *pGussianShader = new CShader("./shader/ImageProcess/FullScreen.vs", "./shader/ImageProcess/gaussianVert.fs");
	//CShader *pGussianShader = new CShader("./shader/ImageProcess/FullScreen.vs", "./shader/ImageProcess/gaussianVert.fs");
	CShader *pHdrProcessShader = new CShader("./shader/HDR/hdrProcess.vs", "./shader/HDR/hdrProcess.fs");
	CShader *pHdrMergeShader = new CShader("./shader/HDR/hdrCombination.vs", "./shader/HDR/hdrCombination.fs");
	

	SIZE viewWinSize = director->GetWindowSize();
	CFrameBuffer framebufferLeftTop;
	framebufferLeftTop.AttachColorBuffer("color", GL_COLOR_ATTACHMENT0, GL_RGBA, viewWinSize.cx, viewWinSize.cy);
	framebufferLeftTop.AttachDepthBuffer("depth", viewWinSize.cx, viewWinSize.cy);
	framebufferLeftTop.Finish();

	CFrameBuffer framebufferRightTop;
	framebufferRightTop.AttachColorBuffer("color", GL_COLOR_ATTACHMENT0, GL_RGBA, viewWinSize.cx, viewWinSize.cy);
	framebufferRightTop.AttachDepthBuffer("depth", viewWinSize.cx, viewWinSize.cy);
	framebufferRightTop.Finish();

	CFrameBuffer framebufferLeftBottom;
	framebufferLeftBottom.AttachColorBuffer("color", GL_COLOR_ATTACHMENT0, GL_RGBA, viewWinSize.cx, viewWinSize.cy);
	framebufferLeftBottom.AttachDepthBuffer("depth", viewWinSize.cx, viewWinSize.cy);
	framebufferLeftBottom.Finish();

	CFrameBuffer framebufferRightBottom;
	framebufferRightBottom.AttachColorBuffer("color", GL_COLOR_ATTACHMENT0, GL_RGBA, viewWinSize.cx, viewWinSize.cy);
	framebufferRightBottom.AttachDepthBuffer("depth", viewWinSize.cx, viewWinSize.cy);
	framebufferRightBottom.Finish();

	CFrameBuffer framebufferHDR;
	framebufferHDR.AttachColorBuffer("color", GL_COLOR_ATTACHMENT0, GL_RGBA, viewWinSize.cx, viewWinSize.cy);
	framebufferHDR.AttachColorBuffer("hdrBuffer", GL_COLOR_ATTACHMENT1, GL_RGBA32F, viewWinSize.cx, viewWinSize.cy);
	framebufferHDR.AttachDepthBuffer("depth", viewWinSize.cx, viewWinSize.cy);
	framebufferHDR.Finish();



	CFullScreen *pFullScreen = new CFullScreen();
	//pFullScreen->SetShader(pOriginShader);
	
	MSG msg;
	
	CTimer uTimer;
	uTimer.Start();
	size_t nSize = pModelVec->size();
	while (true)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				//DestroyWindow(hwnd);
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		glEnable(GL_TEXTURE_2D);
		double duration = uTimer.GetPassedTimeInMs();
		uTimer.Start();
		
		framebufferLeftTop.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		for (size_t i = 0; i < nSize; ++i)
		{
			C3DModel *&p = *(pModelVec->data() + i);
			p->Update(duration);
			p->Draw();
		}
		framebufferLeftTop.UnBind();


		framebufferHDR.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		pFullScreen->initTexture(framebufferLeftTop.GetColorBuffer("color"));
		pFullScreen->SetShader(pHdrShader);
		pFullScreen->Draw();
		framebufferHDR.UnBind();
		//WriteInfo("framebufferLeftTop.GetColorBuffer(color) = %u", framebufferLeftTop.GetColorBuffer("color"));

		framebufferRightTop.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		pFullScreen->initTexture(framebufferHDR.GetColorBuffer("color"));
		pFullScreen->SetShader(pHdrProcessShader);
		pFullScreen->Draw();
		framebufferRightTop.UnBind();


		//glClearColor(0.1, 0.4, 0.7, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		pFullScreen->initTexture(framebufferLeftTop.GetColorBuffer("color"));
		pFullScreen->SetShader(pOriginShader);
		pFullScreen->DrawLeftTop();


		pFullScreen->initTexture(framebufferHDR.GetColorBuffer("hdrBuffer"));
		pFullScreen->SetShader(pOriginShader);
		pFullScreen->DrawRightTop();

		pFullScreen->initTexture(framebufferRightTop.GetColorBuffer("color"));
		pFullScreen->SetShader(pOriginShader);
		pFullScreen->DrawLeftBottom();


		pFullScreen->initTexture(framebufferHDR.GetColorBuffer("hdrBuffer"));
		pFullScreen->SetAuxTexture(framebufferRightTop.GetColorBuffer("color"));
		pFullScreen->SetShader(pHdrMergeShader);
		pFullScreen->DrawRightBottom();

		pFullScreen->SetAuxTexture(-1);

		director->Update(duration / CLOCKS_PER_SEC);
		glFlush();
		SwapBuffers(dc);
		
	}

	for (size_t i = 0; i < nSize; ++i)
	{
		C3DModel *&p = *(pModelVec->data() + i);
		delete p;
		p = nullptr;
	}
	delete pModelVec;
	pModelVec = nullptr;
	
	return 0;
}