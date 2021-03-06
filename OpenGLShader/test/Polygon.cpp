#include "Polygon.h"

#include <iostream>


CPolygon::CPolygon():C3DModel()
{
	m_data = nullptr;
	m_pShader = nullptr;
	m_nVertexBufferObj = -1;
	m_pTexture = nullptr;

	Init();
}

CPolygon::~CPolygon()
{
	if (m_data != nullptr)
	{
		m_data->clear();
		delete m_data;
		m_data = nullptr;
	}
	if (glIsBuffer(m_nVertexBufferObj))
	{
		glDeleteBuffers(1, &m_nVertexBufferObj);
		m_nVertexBufferObj = -1;
	}
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
}

void CPolygon::Init()
{
	size_t Length = 4;
	if (m_data == nullptr)
	{
		m_data = new std::vector<CVertexData>(Length);
	}
	CVertexData tmpData;
	//float tmpY = 1.66;
	//float tmpX = 2.21;//0.5
	tmpData.m_fPos[0] = -0.5;
	tmpData.m_fPos[1] = -0.5;
	tmpData.m_fPos[2] = -4.0f;

	tmpData.m_fTex[0] = 0.0f;
	tmpData.m_fTex[1] = 0.0f;
	(*m_data)[0] = tmpData;

	tmpData.m_fPos[0] = 0.5;
	tmpData.m_fTex[0] = 1.0f;
	(*m_data)[1] = tmpData;

	tmpData.m_fPos[1] = 0.5;
	tmpData.m_fTex[1] = 1.0f;
	(*m_data)[2] = tmpData;

	tmpData.m_fPos[0] = -0.5;
	tmpData.m_fTex[0] = 0.0f;
	(*m_data)[3] = tmpData;
	//m_nCount = Length;
	InitDisplayBuffer();
}

void CPolygon::InitTexture(const char * pTextureFile)
{
	m_pTexture = CTexture::LoadTexture(pTextureFile);
	SetVertexAttrib(VERTEXATTRIB);	
}

void CPolygon::InitDisplayBuffer()
{
	m_nVertexBufferObj = utils::CreateBufferObject(GL_ARRAY_BUFFER, sizeof(CVertexData) * m_data->size(), m_data->data(), GL_STATIC_DRAW);
	//m_nSSBOProgram = utils::CreateBufferObject(GL_SHADER_STORAGE_BUFFER, sizeof(CVertexData) * m_data->size(), m_data->data());
	//m_nProgram = utils::CreateGPUProgram("./shader/depthBuffer.vs", "./shader/depthBuffer.fs");
	m_pShader = new CShader("./shader/UI.vs", "./shader/UI.fs");
	
	SetVertexAttrib(VERTEXATTRIB);
	SetShaderProgram(m_pShader->GetProgram());
	
	//m_nMVPLocation = glGetUniformLocation(m_pShader->GetProgram(), "MVP");
}

void CPolygon::Draw()
{
	glUseProgram(m_pShader->GetProgram());

	glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObj);
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_nSSBOProgram);
	if (m_pTexture)
	{
		m_pTexture->Bind();
	}
	
	SetTransformMatrix();

	SetVertexAttrib(ATTRIBPOINTER);
	
	glDrawArrays(GL_QUADS, 0, m_data->size());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	if (m_pTexture)
	{
		m_pTexture->Bind(false);
	}
	glUseProgram(0);
}

void CPolygon::Update(float duration)
{
	static float speed = 0.001;
	double zTranslate = duration * speed;
	static bool initFlag = true;
	if (initFlag)
	{
		SetScale(4.41828, 3.3137, 1);
		//SetTranslate(0, 0, -2);
		initFlag = false;
	}
	AddRotate(0, zTranslate, 0);
	
}

GLuint CPolygon::GetShaderProgram()
{
	if (m_pShader)
	{
		return m_pShader->GetProgram();
	}
	return -1;
}

void CPolygon::TestShader(mat4f model)
{
	static bool bFlag = true;
	if (bFlag == false)
	{
		return;
	}
	//Log << "CPolygon EndMatrix\n" << model << "\n";
	//Log << "Scale\t" << GetScale() << "\n";

	for (size_t i = 0; i < m_data->size() && i < 10; ++i)
	{
		CVertexData *p = m_data->data() + i;
		vec3f tmp(p->m_fPos, 3);
		vec4f tmpTwo(tmp, 1.0);
		tmpTwo = model * tmpTwo;
		//Log << "OrgVertex: " << tmpTwo << "\n";
	}
	bFlag = false;
}

void CPolygon::SetVertexAttrib(AttribType type)
{
	if (type == AttribType::VERTEXATTRIB)
	{
		if (m_pShader)
		{
			const char *tmpName[] = { "pos", "texCoord" };
			m_pShader->SetShaderAttrib(2, tmpName);
		}
	}
	else if (type == AttribType::ATTRIBPOINTER)
	{
		if (m_pShader)
		{
			size_t count[] = { 3, 2 };
			GLuint offset[] = { 0, sizeof(float) * 6 };
			m_pShader->SetAttribPointer(count, GL_FLOAT, GL_FALSE, sizeof(CVertexData), offset);
		}
	}
}
