#include "Triangles.h"

#include <fstream>
#include <iostream>

extern std::fstream Log;

CTriangle::CTriangle() :C3DModel()
{
	m_data = nullptr;
	//m_pIndex = nullptr;
	m_pShader = nullptr;
	
	m_nVertexBufferObj = -1;
	
	m_pTexture = nullptr;
	
	m_nIndexBufferObj = -1;
	m_nLightModel = -1;

	m_nOffsetBufferObj = -1;
	m_nOffsetLocation = -1;
	m_nSurfaceLocation = -1;
	
	m_bInstanced = false;

	Init();
}

CTriangle::~CTriangle()
{
	if (m_data != nullptr)
	{
		m_data->clear();
		delete m_data;
		m_data = nullptr;
	}
	/*if (m_pIndex != nullptr)
	{
		m_pIndex->clear();
		delete m_pIndex;
		m_pIndex = nullptr;
	}*/
	if (m_pShader != nullptr)
	{
		delete m_pShader;
		m_pShader = nullptr;
	}
	if (glIsBuffer(m_nVertexBufferObj))
	{
		glDeleteBuffers(1, &m_nVertexBufferObj);
		m_nVertexBufferObj = -1;
	}
	
	if (glIsBuffer(m_nOffsetBufferObj))
	{
		glDeleteBuffers(1, &m_nOffsetBufferObj);
		m_nOffsetBufferObj = -1;
	}
	if (m_pTexture)
	{
		CTexture::UnLoadTexture(m_pTexture);
		m_pTexture = nullptr;
	}
}

void CTriangle::Init()
{
	size_t Length = 3;
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
	tmpData.m_fNormal[0] = 0;
	tmpData.m_fNormal[1] = 0;
	tmpData.m_fNormal[2] = 1;

	tmpData.m_fTex[0] = 0.0f;
	tmpData.m_fTex[1] = 0.0f;
	(*m_data)[0] = tmpData;

	tmpData.m_fPos[0] = 0.5;
	tmpData.m_fTex[0] = 1.0f;
	(*m_data)[1] = tmpData;

	tmpData.m_fPos[1] = 0.5;
	tmpData.m_fTex[1] = 1.0f;
	(*m_data)[2] = tmpData;

	InitDisplayBuffer();
}

void CTriangle::InitTexture(const char * pTextureFile)
{
	m_pTexture = CTexture::LoadTexture(pTextureFile);
	SetVertexAttrib(VERTEXATTRIB);
	
}

void CTriangle::InitDisplayBuffer()
{
	GLubyte index[] = { 0, 1, 2 };

	m_nIndexBufferObj = utils::CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index);

	m_nVertexBufferObj = utils::CreateBufferObject(GL_ARRAY_BUFFER, sizeof(CVertexData) * m_data->size(), m_data->data(), GL_STATIC_DRAW);

	m_pShader = new CShader("./shader/MixLight.vs", "./shader/MixLight.fs");
	SetVertexAttrib(VERTEXATTRIB);
	
	if (m_bInstanced == true)
	{
		GLfloat offset[] = {
			-1, 0, 0,
			0, 0, 0,
			1, 0, 0
		};
		m_nOffsetBufferObj = utils::CreateBufferObject(GL_ARRAY_BUFFER, sizeof(offset), offset);
		m_nOffsetLocation = glGetAttribLocation(m_pShader->GetProgram(), "offset");
		if (m_nOffsetLocation == -1)
		{
			Log << __FUNCTION__ << "  " << __LINE__ << " glGetError: " << glGetError() << "\n";
		}
	}
	
	m_nMVPLocation = glGetUniformLocation(m_pShader->GetProgram(), "MVP");
	m_nSurfaceLocation = glGetSubroutineUniformLocation(m_pShader->GetProgram(), GL_FRAGMENT_SHADER, "U_SurfaceColor");

	GLuint AmbientIndex = glGetSubroutineIndex(m_pShader->GetProgram(), GL_FRAGMENT_SHADER, "Ambient");
	GLuint DiffuseIndex = glGetSubroutineIndex(m_pShader->GetProgram(), GL_FRAGMENT_SHADER, "diffuse");
	GLuint SpecularIndex = glGetSubroutineIndex(m_pShader->GetProgram(), GL_FRAGMENT_SHADER, "specular");

	m_nLightModel = DiffuseIndex;
	//glBindVertexArray(0);
}

void CTriangle::Draw()
{
	mat4f tmp = GetModel() * m_fMVPMatrix;
	
	if (m_pShader)
	{
		m_pShader->Bind();
	}
	if (m_pTexture)
	{
		m_pTexture->Bind();
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObj);
	glUniformMatrix4fv(m_nMVPLocation, 1, GL_FALSE, &tmp[0][0]);
	SetVertexAttrib(ATTRIBPOINTER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (m_bInstanced == true)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_nOffsetBufferObj);
		glEnableVertexAttribArray(m_nOffsetLocation);
		glVertexAttribPointer(m_nOffsetLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
		glVertexAttribDivisor(m_nOffsetLocation, 1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIndexBufferObj);
	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &m_nLightModel);
	
	if (m_bInstanced == true)
	{
		//glDrawArraysInstanced(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, 3);
		glDrawElementsInstanced(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, 0, 3);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, 0);
	}
	if (m_pTexture)
	{
		m_pTexture->Bind(false);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	glUseProgram(0);
}

void CTriangle::Update(float duration)
{
	static float speed = 0.001;
	double zTranslate = duration * speed;
	static bool initFlag = true;
	if (initFlag)
	{
		SetScale(4.414, 3.3118, 1);
		initFlag = false;
	}
	if (zTranslate > 2 * 3.1415026)
	{
		zTranslate = 0;
	}
	AddRotate(0, 0, zTranslate);
	
}

GLuint CTriangle::GetShaderProgram()
{
	if (m_pShader)
	{
		return m_pShader->GetProgram();
	}
	return -1;
	
}


void CTriangle::TestShader(mat4f model)
{
	Log << "CTriangle EndMatrix\n" << model << "\n";
	//mat4f tmpModel = model.Transposition();
	/*for (size_t i = 0; i < m_data->size() && i < 10; ++i)
	{
		CVertexData *p = m_data->data() + i;
		vec3f tmp(p->m_fPos, 3);
		vec4f tmpTwo(tmp, 1.0);
		tmpTwo = tmpTwo * model;
		Log << tmpTwo << "\n";
	}*/
}

void CTriangle::ResetState()
{
}

void CTriangle::SetVertexAttrib(AttribType type)
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
