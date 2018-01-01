#include <Windows.h>

#define STATIC_GLEW
#include <GL/glew.h>

//#define GLUT_BUILDING_LIB
//#include <GL/glut.h>

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
#include "InitModel.h"

std::fstream Log("log.txt", std::ios::trunc | std::ios::in | std::ios::out);


extern LRESULT CALLBACK GLWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

INT __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR argv, int showCmd)
{
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
		Log << __FILE__ << "  " << __FUNCTION__ << "  " << __LINE__ << " RegisterClassEx " << error << "\n";
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
	Log << "ScreenWidth = " << viewWidth << "\tScreenHeight = " << viewHeight << "\n";

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


	//Log << "sizeof(CParticleVertex) = " << sizeof(CParticleVertex) << "\n";
	if (GLEW_OK != glewInit())
	{
		Log << __FUNCTION__ << "  " << __LINE__ << "glewInit Failed\n";
		return -1;
	}
	Log << "OpenGL Version:\t" << glGetString(GL_VERSION) << "\n";
	Log << "GLSL Version:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
	
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
	for (size_t i = 0; i < pModelVec->size(); ++i)
	{
		C3DModel *&p = *(pModelVec->data() + i);
		p->SetMatrix(IdentityMatrix, ProjectionMatrix);
	}

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
		
		glFlush();
		SwapBuffers(dc);
		
		//Log << "Timer : " << duration << "\n";
	}
	Log.close();

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