#include "Frustum.h"
#include <fstream>

extern std::fstream Log;

CFrustum::CFrustum() :C3DModel()
{
	//m_nProgram = -1;
	m_nVertexBufferID = -1;
	m_nIndexBufferID = -1;
	m_pShader = nullptr;
}


CFrustum::~CFrustum()
{
	if (glIsBuffer(m_nVertexBufferID))
	{
		glDeleteBuffers(1, &m_nVertexBufferID);
		m_nVertexBufferID = -1;
	}
	if (glIsBuffer(m_nIndexBufferID))
	{
		glDeleteBuffers(1, &m_nIndexBufferID);
		m_nIndexBufferID = -1;
	}
	if (m_pShader)
	{
		delete m_pShader;
		m_pShader = nullptr;
	}
}

void CFrustum::Init(double alpha, double ratio, double zNear, double zFar)
{
	double tan_halfAlpha = std::tan(alpha / 2.0f);// *3.1415926 / 360);
	double heightNear = zNear * tan_halfAlpha;
	double heightFar = zFar * tan_halfAlpha;
	double widthNear = heightNear * ratio;
	double widthFar = heightFar * ratio;

	float points[] = {
	-widthNear, -heightNear, -zNear,
	widthNear, -heightNear, -zNear,
	widthNear, heightNear, -zNear,
	-widthNear, heightNear, -zNear,

	-widthFar, -heightFar, -zFar,
	widthFar, -heightFar, -zFar,
	widthFar, heightFar, -zFar,
	-widthFar, heightFar, -zFar
	};

	GLubyte index[] = { 0, 1, 1, 2, 2, 3, 3, 0,
		0, 4, 1, 5, 2, 6, 3, 7,
		4, 5, 5, 6, 6, 7, 7, 4 };
	m_nCount = sizeof(index) / sizeof(GLubyte);
	InitShaderProgram(points, sizeof(points) / sizeof(float), index, sizeof(index)/ sizeof(GLubyte));
}

void CFrustum::Init(vec4f & param)
{
	Init(param[0], param[1], param[2], param[3]);
}

void CFrustum::Init(float left, float right, float bottom, float top, float zNear, float zFar)
{
	float points[] = {
		left, bottom, -zNear,
		right, bottom, -zNear,
		right, top, -zNear,
		left, top, -zNear,

		left, bottom, -zFar,
		right, bottom, -zFar,
		right, top, -zFar,
		left, top, -zFar
	};

	GLubyte index[] = { 0, 1, 1, 2, 2, 3, 3, 0,
		0, 4, 1, 5, 2, 6, 3, 7,
		4, 5, 5, 6, 6, 7, 7, 4 };
	m_nCount = sizeof(index) / sizeof(GLubyte);
	InitShaderProgram(points, sizeof(points) / sizeof(float), index, sizeof(index) / sizeof(GLubyte));
}

void CFrustum::Init(CVector<6, float>& rhs)
{
	Init(rhs[0], rhs[1], rhs[2], rhs[3], rhs[4], rhs[5]);
}


void CFrustum::Draw()
{
	glUseProgram(m_pShader->GetProgram());
	
	//TestShader(tmp);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIndexBufferID);
	SetTransformMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferID);
	if (m_pShader)
	{
		size_t count = 3;
		GLuint offset = 0;
		m_pShader->SetAttribPointer(&count, GL_FLOAT, GL_FALSE, sizeof(float) * 3 , &offset);
	}
	//glEnableVertexAttribArray(m_nPosLocation);
	//glVertexAttribPointer(m_nPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	//glDrawArrays(GL_LINES, 0, m_nCount);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIndexBufferID);
	
	glDrawElements(GL_LINES, m_nCount, GL_UNSIGNED_BYTE, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glUseProgram(0);
}

void CFrustum::Update(float duration)
{
	static float speed = 0.001;
	double zTranslate = duration * speed;
	static bool initFlag = true;
	if (initFlag)
	{
		SetScale(0.1, 0.1, 0.3);
		initFlag = false;
	}
	if (zTranslate > 2 * 3.1415026)
	{
		zTranslate = 0;
	}
	vec3f rotate = GetRotate();
	SetRotate(rotate[0] + zTranslate, 30 * 3.1415926 / 180, 0);
	//m_fTrans[1][1] = 30 * 3.1415926 / 180;
	//AddRotate(zTranslate, 0, 0);
	
}

void CFrustum::TestShader(mat4f model)
{
	static bool bFlag = true;
	if (bFlag == false)
	{
		return;
	}
	Log << "CFrustum EndMatrix:\n" << model << "\n";
	bFlag = false;
}

void CFrustum::InitShaderProgram(float *points,size_t PointCount, GLubyte *index, size_t indexLength)
{

	m_nVertexBufferID = utils::CreateBufferObject(GL_ARRAY_BUFFER, sizeof(float) * PointCount, points, GL_STATIC_DRAW);
	m_nIndexBufferID = utils::CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, indexLength * sizeof(GLubyte), index, GL_STATIC_DRAW);

	m_pShader = new CShader("./shader/Frustum.vs", "./shader/Frustum.fs");
	const char *tmpName = "pos";
	m_pShader->SetShaderAttrib(1, &tmpName);
	//m_nProgram = utils::CreateGPUProgram("./shader/Frustum.vs", "./shader/Frustum.fs");
	/*
	m_nPosLocation = glGetAttribLocation(m_nProgram, "pos");
	if (m_nPosLocation == -1)
	{
		Log << __FUNCTION__ << "  " << __LINE__ << " glGetError: " << glGetError() << "\n";
	}*/

	SetShaderProgram(m_pShader->GetProgram());
}

