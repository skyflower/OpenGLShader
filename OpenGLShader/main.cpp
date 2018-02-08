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
#include "./test/FrameBufferTest.h"
#include "./test/Fog.h"

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
	Perspective[0] = 45 * 3.1415926 / 180;
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

	//mat4f IdentityMatrix = CMatrix<4, 4, float>::GetUnit();
	//mat4f ViewMatrix = CMatrix<4, 4, float>::GetUnit();
	// Perspective glm  transpose
	mat4f ProjectionMatrix = CMatrix<4, 4, float>::GetPerspective(Perspective);
	//mat4f Trianslate = CMatrix<4, 4, float>::GetTranslate(0, 0, -10);
	//ProjectionMatrix = ProjectionMatrix;// *Trianslate;
	//mat4f UIProjectionMatrix = CMatrix<4, 4, float>::GetOrtho(OrthoVec);
	glm::mat4 glmProjectionMatrix = glm::perspective(45 * 3.1415926 / 180, viewWidth / (double)viewHeight, 0.01, 1000.0);
	//glm::mat4 glmOrthoMatrix = glm::ortho(OrthoVec[0], OrthoVec[1], OrthoVec[2], OrthoVec[3], OrthoVec[4], OrthoVec[5]);
	//mat4f tmpTest;
	/*glm::tvec3<float> glmTranslateVec = glm::tvec3<float>(1.3f, 4.32f, -1.0f);
	glm::mat4 glmTranslate = glm::translate(glmTranslateVec);
	mat4f Translate = CMatrix<4, 4, float>::GetTranslate(1.3, 4.32, -1.0);*/
	//glm::mat4 glmNormalMatrix = glm::inverseTranspose(glmProjectionMatrix);
	//mat4f NormalMatrix = ProjectionMatrix.inverse().Transposition();
	//utils::OutputToFile(ProjectionMatrix, glmProjectionMatrix);
	//utils::OutputToFile(NormalMatrix, glmNormalMatrix);
	//mat4f inverseTranslate = Translate.inverse();// Transposition();
	//mat4f LeftUnit = inverseTranslate * Translate;
	//mat4f RightUnit = Translate * inverseTranslate;
	//glmTranslate = glm::inverse(glmTranslate);
	//glmTranslate = glm::transpose(glmTranslate);
	
	//glm::vec4 glmPerspective = glm::vec4(45 * 3.1415926 / 180, viewWidth / (double)viewHeight, 0.01, 1000);
	//glm::mat4 glmMatrix = glm::perspective(45 * 3.1415926f / 180, viewWidth / (float)viewHeight, 0.01f, 1000.0f);
	//utils::OutputToFile(ProjectionMatrix, glmMatrix);


	/*vec4f tmpTestPoint;
	tmpTestPoint[0] = 8.7;
	tmpTestPoint[1] = -2.57;
	tmpTestPoint[2] = 1.7;
	tmpTestPoint[3] = 1.0;

	vec4f LeftMultiply = Translate * tmpTestPoint;
	vec4f RightMultiply = tmpTestPoint * Translate;

	WriteInfo("LeftMultiply = %s", LeftMultiply.FormatToString().c_str());
	WriteInfo("RightMultiply = %s", RightMultiply.FormatToString().c_str());*/

	//double radia = glm::radians(45.0);

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
	
	//glClearColor(0.16, 0.16, 0.16, 1);
	glClearColor(0.5, 0.5, 0.5, 1);
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
	CFogTest *pFogTest = new CFogTest(0.5, 40, 40);
	pFogTest->SetMatrix(viewMatrix, ProjectionMatrix);
	//pFogTest->SetMatrix(viewMatrix, utils::glmToMatrix(glmProjectionMatrix, false));
	mat4f modelMatrix = CMatrix<4, 4, float>::GetTranslate<float>(-1.0f, 0.0f, -3.0f);
	//glm::vec3 glmTranslate(0, 0, -10.8);
	//glm::mat4 glmModel = glm::translate(glmTranslate);
	pFogTest->SetModelMatrix(modelMatrix);
	//pFogTest->SetModelMatrix(utils::glmToMatrix(glmModel, false));
	CFogTest *pFogTestTwo = new CFogTest(0.6, 40, 40);
	mat4f modelMatrixTwo = CMatrix<4, 4, float>::GetTranslate<float>(-0.0f, 0.0f, -10.0f);
	pFogTestTwo->SetModelMatrix(modelMatrixTwo);
	pFogTestTwo->SetMatrix(viewMatrix, ProjectionMatrix);

	CFogTest *pFogTestThree = new CFogTest(0.6, 40, 40);
	mat4f modelMatrixThree = CMatrix<4, 4, float>::GetTranslate<float>(1.9f, 0.0f, -15.0f);
	pFogTestThree->SetModelMatrix(modelMatrixThree);
	pFogTestThree->SetMatrix(viewMatrix, ProjectionMatrix);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	
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
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		for (size_t i = 0; i < nSize; ++i)
		{
			C3DModel *&p = *(pModelVec->data() + i);
			p->Update(duration);
			p->Draw();
		}

		//pFogTest->ResetStatus();
		
		//pFogTest->SetTranslate(-0.6, 0, -20.5);
		//pFogTest->Update(duration);
		pFogTest->Draw();

		pFogTestTwo->Draw();
		pFogTestThree->Draw();
		/*pFogTest->ResetStatus();
		pFogTest->SetTranslate(-0.1, 0, -0.6);
		pFogTest->Draw();


		pFogTest->ResetStatus();
		pFogTest->SetTranslate(0.4, 0, -0.1);
		pFogTest->Draw();*/

		/*pFogTest->ResetStatus();
		pFogTest->SetTranslate(0, 0, -1.5);
		pFogTest->Draw();
		
		
		pFogTest->ResetStatus();
		pFogTest->SetTranslate(0.5, 0, -2);
		pFogTest->Draw();*/

		
		//vec3f rotate = pFogTest->GetRotate();
		//vec3f tranf = pFogTest->GetTranslate();
		//WriteInfo("rotate = %s", rotate.FormatToString().c_str());
		//WriteInfo("tranf = %s", tranf.FormatToString().c_str());
		//pFogTest->ResetStatus();
		//pFogTest->SetTranslate(0, 0, -8);
		//pFogTest->Draw();

		//pFogTest->ResetStatus();
		//pFogTest->SetTranslate(0, 0, -16);
		//pFogTest->Draw();


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