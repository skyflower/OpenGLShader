#include "CPolygonTesslation.h"
#include <fstream>
#include "./common/Log.h"



CPolygonTess::CPolygonTess():C3DModel()
{
	//m_pTexture = nullptr;
	m_pShader = nullptr;
	m_nPointCount = 3;
	m_pPoints = nullptr;

	Init();
}


CPolygonTess::~CPolygonTess()
{
	if (m_pShader)
	{
		delete m_pShader;
		m_pShader = nullptr;
	}
	if (m_pPoints)
	{
		delete[] m_pPoints;
		m_pPoints = nullptr;
	}
	if (glIsBuffer(m_nVertexBuffer))
	{
		glDeleteBuffers(1, &m_nVertexBuffer);
		m_nVertexBuffer = -1;
	}
}

void CPolygonTess::InitTexture(const char * filepath)
{
	//m_pTexture = CTexture::LoadTexture(filepath);
}

void CPolygonTess::Draw()
{
	glUseProgram(m_pShader->GetProgram());
	glDisable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
	SetTransformMatrix();
	glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBuffer);

	SetVertexAttrib(CDrawable::AttribType::ATTRIBPOINTER);
	//glDrawArrays(GL_QUADS, 0, m_nPointCount);
	//glPatchParameteri(GL_PATCH_VERTICES, 3);
	//glDrawArrays(GL_PATCHES, 0, m_nPointCount);
	glDrawArrays(GL_TRIANGLES, 0, m_nPointCount);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
	
	//glDisable(GL_BLEND);
}

void CPolygonTess::Update(float duration)
{
	static float speed = 0.001;
	double zTranslate = duration * speed;
	static bool initFlag = true;
	if (initFlag)
	{
		//SetScale(4.41828, 3.3137, 1);
		//SetTranslate(0, 0, -1);
		initFlag = false;
	}
	AddRotate(0, speed, 0);
	
}

GLuint CPolygonTess::GetShaderProgram()
{
	if (m_pShader)
	{
		return m_pShader->GetProgram();
	}
	return -1;
}


void CPolygonTess::TestShader(mat4f &matrix)
{
	//Log << "CPolygonTess::TestShader Matrix\n" << matrix << "\n";
	//int i = 1;
	//double Interval = 0.004;
	//static mat4f PreMatrix = matrix;
	vec4d tmpPoint[3];
	for (size_t i = 0; i < m_nPointCount; ++i)
	{
		tmpPoint[i] = CVector<4, float>(m_pPoints[i].m_fPos, 4);
		tmpPoint[i] = tmpPoint[i] * matrix;
		//vec4f PrePoint = tmpPoint * PreMatrix;
		////Log << i << "  Cur " << shaderPoint << "\n";
		////Log << i << "  Pre " << PrePoint << "\n";
		////Log << PrePoint - shaderPoint << "\n";
		//vec4f subPoint = PrePoint - shaderPoint;
		//subPoint.Abs();
		//if (subPoint[0] > Interval || subPoint[1] > Interval || \
		//	subPoint[2] > Interval || subPoint[3] > Interval)
		//{
			
		//	Log << "Rotate \t" << tmpRotate << "\n";
		//	Log << subPoint << "\n";
		//}
		//PreMatrix = matrix;
	}
	vec3f tmpRotate = GetRotate();
	vec4d labelVec[3];
	labelVec[0] = tmpPoint[1] - tmpPoint[0];
	labelVec[1] = tmpPoint[2] - tmpPoint[1];
	labelVec[2] = tmpPoint[2] - tmpPoint[0];
	labelVec[0][3] = 0;
	labelVec[1][3] = 0;
	labelVec[2][3] = 0;

	long double array[4];
	array[0] = std::sqrtl(labelVec[0].Magnitude());
	array[1] = std::sqrtl(labelVec[1].Magnitude());
	array[2] = std::sqrtl(labelVec[2].Magnitude());
	array[3] = (array[0] + array[1] + array[2]) / 2;
	double AreaTri = std::sqrtl(array[3] * (array[3] - array[1]) * (array[3] - array[2]) * (array[3] - array[0]));
	WriteInfo("Area Triangles = %lf, tmpRotate = %lf", AreaTri, tmpRotate);
	//Log << "============================End TestShader " << __FUNCTION__ << "\n";
}

void CPolygonTess::Init()
{
	m_pPoints = new CVertex_SSBO_Two[m_nPointCount];
	memset(m_pPoints, 0, sizeof(CVertex_SSBO_Two) * m_nPointCount);
	m_pPoints[0].m_fPos[0] = 1;
	m_pPoints[0].m_fPos[1] = 0;
	m_pPoints[0].m_fPos[2] = -4;
	m_pPoints[0].m_fPos[3] = 1;

	m_pPoints[1].m_fPos[0] = 0;
	m_pPoints[1].m_fPos[1] = 1;
	m_pPoints[1].m_fPos[2] = -4;
	m_pPoints[1].m_fPos[3] = 1;

	m_pPoints[2].m_fPos[0] = 1;
	m_pPoints[2].m_fPos[1] = 1;
	m_pPoints[2].m_fPos[2] = -4;
	m_pPoints[2].m_fPos[3] = 1;

	
	m_nVertexBuffer = utils::CreateBufferObject(GL_ARRAY_BUFFER, sizeof(CVertex_SSBO_Two) * m_nPointCount, m_pPoints);
	
	char *path[] = { "./shader/Tesslation/Simple.vs" , "./shader/Tesslation/Simple.fs" , nullptr, nullptr, "./shader/Tesslation/Simple.tes" };

	//m_pShader = new CShader(5, path);
	m_pShader = new CShader("./shader/Tesslation/Simple.vs", "./shader/Tesslation/Simple.fs");
	SetVertexAttrib(CDrawable::AttribType::VERTEXATTRIB);

	SetShaderProgram(m_pShader->GetProgram());
}

void CPolygonTess::SetVertexAttrib(AttribType type)
{
	if (type == CDrawable::AttribType::VERTEXATTRIB)
	{
		if (m_pShader)
		{
			const char *tmpName[] = { "pos"};
			m_pShader->SetShaderAttrib(1, tmpName);
		}
	}
	else if (type == CDrawable::AttribType::ATTRIBPOINTER)
	{
		if (m_pShader)
		{
			size_t count[] = { 4 };
			GLuint offset[] = { 0};
			m_pShader->SetAttribPointer(count, GL_FLOAT, GL_FALSE, sizeof(CVertex_SSBO_Two), offset);
		}
	}
}