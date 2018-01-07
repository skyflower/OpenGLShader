#include "SkyBox.h"



CSkyBox::CSkyBox()
{
	for (size_t i = 0; i < TEXTURE_COUNT; ++i)
	{
		m_pTexture[i] = nullptr;
	}

	m_pShader = nullptr;
	m_pSphere = nullptr;
	memset(m_pVertex, 0, sizeof(CVertexData) * POINT_COUNT);
	m_nVertexBuffer = -1;
}


CSkyBox::~CSkyBox()
{
	for (size_t i = 0; i < TEXTURE_COUNT; ++i)
	{
		if (m_pTexture[i] != nullptr)
		{
			CTexture::UnLoadTexture(m_pTexture[i]);
			m_pTexture[i] = nullptr;
		}
	}
	if (m_pShader != nullptr)
	{
		delete m_pShader;
		m_pShader = nullptr;
	}
	if (m_pSphere != nullptr)
	{
		delete m_pSphere;
		m_pSphere = nullptr;
	}
	if (glIsBuffer(m_nVertexBuffer))
	{
		glDeleteBuffers(1, &m_nVertexBuffer);
		m_nVertexBuffer = -1;
	}
}

void CSkyBox::InitTexture(const char * fileDir)
{
	char fileName[TEXTURE_COUNT][50] = { "right.tag", "left.tag", "up.tag", "down.tag", "forward.tag", "backward.tag" };
	for (size_t i = 0; i < TEXTURE_COUNT; ++i)
	{
		char tmpFilePath[255] = { 0 };
		memcpy_s(tmpFilePath, sizeof(tmpFilePath), fileDir, strlen(fileDir));
		strcat_s(tmpFilePath, fileName[i]);
		m_pTexture[i] = CTexture::LoadTexture(tmpFilePath);
	}
}

void CSkyBox::Draw()
{
}

void CSkyBox::Update(float Duration)
{
}

GLuint CSkyBox::GetShaderProgram()
{
	if (m_pShader != nullptr)
	{
		return m_pShader->GetProgram();
	}
	return 0;
}

void CSkyBox::Init()
{
}

void CSkyBox::SetVertexAttrib(AttribType type)
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
