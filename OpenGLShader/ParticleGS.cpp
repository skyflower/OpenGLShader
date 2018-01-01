#include "ParticleGS.h"
#include <fstream>

extern std::fstream Log;


CParticleGS::CParticleGS():C3DModel()
{
	m_pTexture = nullptr;
	m_pShader = nullptr;
	

	Init();
}


CParticleGS::~CParticleGS()
{
	if (m_pShader)
	{
		delete m_pShader;
		m_pShader = nullptr;
	}
	if (m_pTexture)
	{
		CTexture::UnLoadTexture(m_pTexture);
		m_pTexture = nullptr;
	}
	
}

void CParticleGS::InitTexture(const char * filepath)
{
	m_pTexture = CTexture::LoadTexture(filepath);
}

void CParticleGS::Draw()
{
	glUseProgram(m_pShader->GetProgram());
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (m_pTexture)
	{
		m_pTexture->Bind();
	}

	SetVertexAttrib(CDrawable::AttribType::ATTRIBPOINTER);

	mat4f tmp = C3DModel::GetModel();// *m_fMVPMatrix;

	glUniformMatrix4fv(m_nMVPLocation, 1, GL_FALSE, &tmp[0][0]);

	//glDrawArrays(GL_QUADS, 0, m_nPointCount);
	glDrawArrays(GL_POINTS, 0, 1);
	
	
	if (m_pTexture)
	{
		m_pTexture->Bind(false);
	}
	glUseProgram(0);
	glDisable(GL_PROGRAM_POINT_SIZE);
	glDisable(GL_POINT_SPRITE);
	glDisable(GL_BLEND);
}

void CParticleGS::Update(float duration)
{
	static float speed = 0.001;
	double zTranslate = duration * speed;
	static bool initFlag = true;
	if (initFlag)
	{
		//SetScale(4.41828, 3.3137, 1);
		initFlag = false;
	}
	//AddRotate(0, zTranslate, zTranslate);
}

GLuint CParticleGS::GetShaderProgram()
{
	if (m_pShader)
	{
		return m_pShader->GetProgram();
	}
	return -1;
}


void CParticleGS::ResetState()
{
}

void CParticleGS::Init()
{
	m_point.m_fPos[0] = 0;
	m_point.m_fPos[1] = 0;
	m_point.m_fPos[2] = -1;
	m_point.m_fPos[3] = 1;

	m_pShader = new CShader("./shader/SimpleGS.vs", "./shader/SimpleGS.fs", "./shader/SimpleGS.gs");

	m_nMVPLocation = glGetUniformLocation(m_pShader->GetProgram(), "MVP");

	SetVertexAttrib(CDrawable::AttribType::VERTEXATTRIB);
	
}

void CParticleGS::SetVertexAttrib(AttribType type)
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
			size_t count[] = { 4};
			GLuint offset[] = { 0};
			m_pShader->SetAttribPointer(count, GL_FLOAT, GL_FALSE, sizeof(CVertex_SSBO_Two), offset);
		}
	}
}