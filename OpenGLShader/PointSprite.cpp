#include "PointSprite.h"
#include <fstream>

extern std::fstream Log;


CPointSprite::CPointSprite():C3DModel()
{
	m_pTexture = nullptr;
	m_pShader = nullptr;
	
	m_nSSBOProgram = -1;
	//m_point = nullptr;
	
	//m_nPointCount = 20000;

	Init();
}


CPointSprite::~CPointSprite()
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
	if (glIsBuffer(m_nSSBOProgram))
	{
		glDeleteBuffers(1, &m_nSSBOProgram);
		m_nSSBOProgram = -1;
	}
}

void CPointSprite::InitTexture(const char * filepath)
{
	m_pTexture = CTexture::LoadTexture(filepath);
}

void CPointSprite::Draw()
{
	glUseProgram(m_pShader->GetProgram());
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_nSSBOProgram);
	if (m_pTexture)
	{
		m_pTexture->Bind();
	}

	SetTransformMatrix();

	//glDrawArrays(GL_QUADS, 0, m_nPointCount);
	glDrawArrays(GL_QUADS, 0, 4);
	
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	if (m_pTexture)
	{
		m_pTexture->Bind(false);
	}
	glUseProgram(0);
	glDisable(GL_PROGRAM_POINT_SIZE);
	glDisable(GL_POINT_SPRITE);
	glDisable(GL_BLEND);
}

void CPointSprite::Update(float duration)
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

GLuint CPointSprite::GetShaderProgram()
{
	if (m_pShader)
	{
		return m_pShader->GetProgram();
	}
	return -1;
}


void CPointSprite::ResetState()
{
}

void CPointSprite::Init()
{
	
	m_point.m_fPos[0] = 0;
	m_point.m_fPos[1] = 0;
	m_point.m_fPos[2] = -3;
	m_point.m_fPos[3] = 1;

	m_nSSBOProgram = utils::CreateBufferObject(GL_SHADER_STORAGE_BUFFER, sizeof(CVertex_SSBO_Two), &m_point);
	
	
	m_pShader = new CShader("./shader/PointSpriteSSBO.vs", "./shader/PointSpriteSSBO.fs");

	SetShaderProgram(m_pShader->GetProgram());
	
	//delete[]Array;
	//Array = nullptr;
}

void CPointSprite::SetVertexAttrib(AttribType type)
{
	if (type == CDrawable::AttribType::VERTEXATTRIB)
	{
		if (m_pShader)
		{
			const char *tmpName[] = { "pos", "texCoord", "normal" };
			m_pShader->SetShaderAttrib(3, tmpName);
		}
	}
	else if (type == CDrawable::AttribType::ATTRIBPOINTER)
	{
		if (m_pShader)
		{
			size_t count[] = { 3, 2, 3 };
			GLuint offset[] = { 0, sizeof(float) * 6, sizeof(float) * 3 };
			m_pShader->SetAttribPointer(count, GL_FLOAT, GL_FALSE, sizeof(CVertexData), offset);
		}
	}
}