#include "Fog.h"
#include "../common/Log.h"
#include <iostream>


CFogTest::CFogTest(float radis, int slide, int stacks):C3DModel()
{
	m_data = nullptr;
	m_pShader = nullptr;
	m_nVertexBufferObj = -1;
	m_nIndexBufObj = -1;
	m_pTexture = nullptr;
	m_pIndex = nullptr;

	Init(radis, slide, stacks);
}

CFogTest::~CFogTest()
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
	
	if (glIsBuffer(m_nIndexBufObj))
	{
		glDeleteBuffers(1, &m_nIndexBufObj);
		m_nIndexBufObj = -1;
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

void CFogTest::Init(float radis, int slide, int stacks)
{
	if ((radis < 0.00001) && (slide < 2))
	{
		return;
	}
	utils::CreateSphereData(radis, slide, stacks, m_data, m_pIndex);
	InitDisplayBuffer();
}

void CFogTest::InitTexture(const char * pTextureFile)
{
	m_pTexture = CTexture::LoadTexture(pTextureFile);
	SetVertexAttrib(VERTEXATTRIB);
}

void CFogTest::InitDisplayBuffer()
{
	m_nVertexBufferObj = utils::CreateBufferObject(GL_ARRAY_BUFFER, sizeof(CVertex) * m_data->size(), m_data->data(), GL_STATIC_DRAW);
	m_nIndexBufObj = utils::CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, m_pIndex->size() * sizeof(unsigned int), m_pIndex->data());
	
	m_pShader = new CShader("./shader/Fog/fog.vs", "./shader/Fog/fog.fs");
	
	SetVertexAttrib(VERTEXATTRIB);
	
	SetShaderProgram(m_pShader->GetProgram());
}

void CFogTest::Draw()
{
	//glClearColor(0.1, 0.4, 0.7, 1.0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	//glDisable(GL_TEXTURE_2D);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	glUseProgram(m_pShader->GetProgram());

	glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObj);
	
	SetTransformMatrix();

	SetVertexAttrib(ATTRIBPOINTER);
	//TestShader();
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIndexBufObj);

	//glDrawArrays(GL_POINTS, 0, m_data->size());
	glDrawElements(GL_TRIANGLES, m_pIndex->size(), GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glUseProgram(0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	//glDisable(GL_TEXTURE_2D);
}

void CFogTest::Update(float duration)
{
	static float speed = 0.0003;
	double zTranslate = duration * speed;
	static bool initFlag = true;
	if (initFlag)
	{
		//SetScale(2, 2, 2);
		//SetTranslate(-0.5, 0, -1);
		initFlag = false;
	}
	AddRotate(0, zTranslate, 0);
}

GLuint CFogTest::GetShaderProgram()
{
	if (m_pShader)
	{
		return m_pShader->GetProgram();
	}
	return -1;
}

void CFogTest::TestShader()
{
	static bool initFlag = true;
	if (initFlag == false)
	{
		return;
	}
	//WriteInfo("modelMatrix = \n%s", model.FormatToString().c_str());
	//mat3f normalMatrix = model.reduceDimension();
	//normalMatrix = normalMatrix.inverse().Transposition();
	mat4f model = GetModel().Transposition();
	mat4f mvp = GetMVPMatrix().Transposition();
	mat4f InverseModel = model.inverse();
	size_t nSize = m_data->size();
	CVertex *pVertex = m_data->data();
	size_t i = 0;
	//for (size_t i = 0; i < m_data->size(); ++i)
	{
		vec3f tmp(pVertex[i].mPos, 3);
		vec4f tmpPos(tmp, 1.0);
		WriteInfo("tmpPos = %s", tmpPos.FormatToString().c_str());

		vec4f tmpData = tmpPos * model;
		//vec4f tmpTranf = model * tmpPos;
		WriteInfo("tmpData = %s", tmpData.FormatToString().c_str());
		vec4f tmpMvpData = tmpPos * mvp;
		WriteInfo("tmpMvpData = %s", tmpMvpData.FormatToString().c_str());
		
		tmpMvpData = tmpPos * InverseModel * mvp;
		WriteInfo("tmpInverseModel = %s", tmpMvpData.FormatToString().c_str());
	}
	//initFlag = false;
}

void CFogTest::SetVertexAttrib(AttribType type)
{
	if (type == AttribType::VERTEXATTRIB)
	{
		if (m_pShader)
		{
			const char *tmpName[] = { "pos", "normal" };
			m_pShader->SetShaderAttrib(2, tmpName);
		}
	}
	else if (type == AttribType::ATTRIBPOINTER)
	{
		if (m_pShader)
		{
			size_t count[] = { 3, 4 };
			GLuint offset[] = { 0, sizeof(float) * 3 };
			m_pShader->SetAttribPointer(count, GL_FLOAT, GL_FALSE, sizeof(CVertex), offset);
		}
	}
}
