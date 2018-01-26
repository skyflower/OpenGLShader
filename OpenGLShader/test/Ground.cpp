#include "Ground.h"



CGround::CGround()
{
	m_pTexture = nullptr;
	m_pShader = nullptr;
	memset(&m_pVertex, 0, sizeof(CVertexData));
	m_nVertexBuffer = -1;
	Init();
}


CGround::~CGround()
{
	if (m_pTexture)
	{
		CTexture::UnLoadTexture(m_pTexture);
		m_pTexture = nullptr;
	}
	if (m_pShader)
	{
		delete m_pShader;
		m_pShader = nullptr;
	}
	if (glIsBuffer(m_nVertexBuffer))
	{
		glDeleteBuffers(1, &m_nVertexBuffer);
		m_nVertexBuffer = -1;
	}
}

void CGround::InitTexture(const char * filepath)
{
	if (filepath != nullptr)
	{
		m_pTexture = CTexture::LoadTexture(filepath);
	}
}

void CGround::Draw()
{
	glUseProgram(m_pShader->GetProgram());
	if (m_pTexture)
	{
		m_pTexture->Bind();
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBuffer);
	SetVertexAttrib(CDrawable::AttribType::ATTRIBPOINTER);
	SetTransformMatrix();
	glDrawArrays(GL_QUADS, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (m_pTexture)
	{
		m_pTexture->Bind(false);
	}
	glUseProgram(0);
}

void CGround::Update(float duration)
{
	static float speed = 0.001;
	double zTranslate = duration * speed;
	static bool initFlag = true;
	if (initFlag)
	{
		//SetScale(4.41828, 3.3137, 1);
		SetRotate(10 * 3.1516926 / 180, 0, 0);
		initFlag = false;
	}
	//AddRotate(0, zTranslate, zTranslate);
}

GLuint CGround::GetShaderProgram()
{
	if (m_pShader != nullptr)
	{
		return m_pShader->GetProgram();
	}
	return 0;
}

void CGround::Init()
{
	m_pVertex.m_fPos[0] = m_pVertex.m_fPos[2] = 0;
	m_pVertex.m_fPos[1] = 0;
	m_pVertex.m_fNormal[0] = m_pVertex.m_fNormal[2] = 0;
	m_pVertex.m_fNormal[1] = 1;
	m_pVertex.m_fTex[0] = m_pVertex.m_fTex[1] = 0;
	m_nVertexBuffer = utils::CreateBufferObject(GL_ARRAY_BUFFER, sizeof(CVertexData), &m_pVertex);
	m_pShader = new CShader("./shader/ground.vs", "./shader/ground.fs");
	SetShaderProgram(m_pShader->GetProgram());
	SetVertexAttrib(CDrawable::AttribType::VERTEXATTRIB);
}

void CGround::SetVertexAttrib(AttribType type)
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
