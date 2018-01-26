#include "WaterWave.h"

#include <fstream>
#include <iostream>


CWaterWave::CWaterWave() :C3DModel()
{
	m_data = nullptr;
	m_pIndex = nullptr;
	m_pShader = nullptr;
	
	m_nVertexBufferObj = -1;
	
	m_pTexture = nullptr;
	
	m_nIndexBufferObj = -1;
	
	m_nParamLocation = -1;
	m_fParam[0] = 0;
	m_fParam[1] = 0.2;
	m_fParam[2] = 0.1;
	m_fParam[3] = -6;

	Init();
}

CWaterWave::~CWaterWave()
{
	if (m_data != nullptr)
	{
		m_data->clear();
		delete m_data;
		m_data = nullptr;
	}
	if (m_pIndex != nullptr)
	{
		m_pIndex->clear();
		delete m_pIndex;
		m_pIndex = nullptr;
	}
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
	
	
	if (m_pTexture)
	{
		CTexture::UnLoadTexture(m_pTexture);
		m_pTexture = nullptr;
	}
}

void CWaterWave::Init()
{
	//  0 1, 0.01
	// 0, 0.01, 0.02, 0.03 ,...,1
	// 1  2
	float yBegin = 0;
	float yEnd = 5.0;
	float radius = yEnd - yBegin;
	float step = 0.04;
	float zBase = -10;
	//float zWave = 0.5;
	size_t nCount = (radius) / step;
	m_fParam[3] = zBase;
	size_t nLineLoop = 40;

	size_t Length = nCount * nLineLoop + 1;
	if (m_data == nullptr)
	{
		m_data = new std::vector<CVertexData>(Length);
		memset(m_data->data(), 0, sizeof(CVertexData) * Length);
	}
	if (m_pIndex == nullptr)
	{
		size_t tmpCount = nLineLoop + (nCount - 1) * 2 * nLineLoop;
		m_pIndex = new std::vector<unsigned int>(tmpCount * 3);
		memset(m_pIndex->data(), 0, sizeof(unsigned int) * tmpCount * 3);
	}
	CVertexData *pVertex = m_data->data();
	size_t dataIndex = 0;
	pVertex[dataIndex].m_fPos[0] = 0;
	pVertex[dataIndex].m_fPos[1] = 0;
	pVertex[dataIndex].m_fPos[2] = zBase;
	dataIndex++;

	size_t indexIndex = 0;
	unsigned int *pIndex = m_pIndex->data();

	for (size_t i = 0; i < nCount; ++i)
	{
		double tmpRadius = (i + 1) * step;
		double tmpStepAlpha = (2 * 3.1415926) / nLineLoop;
		size_t tmpBeginIndex = dataIndex;
		for (size_t j = 0; j < nLineLoop; ++j)
		{
			pVertex[dataIndex].m_fPos[0] = tmpRadius * std::sin(j * tmpStepAlpha);
			pVertex[dataIndex].m_fPos[1] = tmpRadius * std::cos(j * tmpStepAlpha);
			pVertex[dataIndex].m_fPos[2] = zBase;
			pVertex[dataIndex].m_fTex[0] = (pVertex[dataIndex].m_fPos[0] + radius) / (2 * radius);
			pVertex[dataIndex].m_fTex[1] = (pVertex[dataIndex].m_fPos[1] + radius) / (2 * radius);

			dataIndex++;
		}
		if (i == 0)
		{
			for (size_t j = 0; j < nLineLoop; ++j)
			{
				pIndex[indexIndex++] = 0;
				pIndex[indexIndex++] = j + 1;
				pIndex[indexIndex++] = (j + 2) % nLineLoop;
			}
		}
		if ((i != 0) && (i > 0))
		{
			unsigned int preBeginIndex = tmpBeginIndex - nLineLoop;
			for (int j = 0; j < nLineLoop; ++j)
			{
				pIndex[indexIndex++] = preBeginIndex + j;
				pIndex[indexIndex++] = preBeginIndex + (j + 1) % nLineLoop;
				pIndex[indexIndex++] = tmpBeginIndex + j;

				pIndex[indexIndex++] = tmpBeginIndex + j;
				pIndex[indexIndex++] = preBeginIndex + (j + 1) % nLineLoop;
				pIndex[indexIndex++] = tmpBeginIndex + (j + 1) % nLineLoop;

			}
		}
	}

	InitDisplayBuffer();
}

void CWaterWave::InitTexture(const char * pTextureFile)
{
	m_pTexture = CTexture::LoadTexture(pTextureFile);
	SetVertexAttrib(VERTEXATTRIB);
	
}

void CWaterWave::InitDisplayBuffer()
{
	m_nVertexBufferObj = utils::CreateBufferObject(GL_ARRAY_BUFFER, sizeof(CVertexData) * m_data->size(), m_data->data(), GL_STATIC_DRAW);

	m_nIndexBufferObj = utils::CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_pIndex->size(), m_pIndex->data());

	m_pShader = new CShader("./shader/waterwaveTwo.vs", "./shader/waterwaveTwo.fs");
	SetVertexAttrib(VERTEXATTRIB);

	m_nParamLocation = glGetUniformLocation(m_pShader->GetProgram(), "param");
	
	SetShaderProgram(m_pShader->GetProgram());
}

void CWaterWave::Draw()
{
	if (m_pShader)
	{
		m_pShader->Bind();
	}
	if (m_pTexture)
	{
		m_pTexture->Bind();
	}
	//TestShader(tmp);
	glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObj);

	SetTransformMatrix();
	
	glUniform4fv(m_nParamLocation, 1, &m_fParam[0]);

	SetVertexAttrib(ATTRIBPOINTER);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIndexBufferObj);
	
	glDrawElements(GL_TRIANGLES, m_pIndex->size(), GL_UNSIGNED_INT, 0);
	
	if (m_pTexture)
	{
		m_pTexture->Bind(false);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	glUseProgram(0);
}

void CWaterWave::Update(float duration)
{
	static float speed = 0.001;
	double zTranslate = duration * speed;
	static bool initFlag = true;
	if (initFlag)
	{
		//SetScale(4.414, 3.3118, 1);
		initFlag = false;
	}
	if (zTranslate > 2 * 3.1415026)
	{
		zTranslate = 0;
	}
	m_fParam[0] += zTranslate;
	//AddRotate(0, 0, zTranslate);
	
}

GLuint CWaterWave::GetShaderProgram()
{
	if (m_pShader)
	{
		return m_pShader->GetProgram();
	}
	return -1;
	
}


void CWaterWave::TestShader(mat4f model)
{
	static bool flag = false;
	if (flag == true)
	{
		return;
	}
	//Log << "CWaterWave EndMatrix\n" << model << "\n";
	//mat4f tmpModel = model.Transposition();
	for (size_t i = 0; i < m_data->size(); ++i)
	{
		CVertexData *p = m_data->data() + i;
		vec3f tmp(p->m_fPos, 3);
		vec4f tmpTwo(tmp, 1.0);
		tmpTwo = tmpTwo * model;
		//Log << tmpTwo << "\n";
	}
	flag = true;
}

void CWaterWave::ResetState()
{
}

void CWaterWave::SetVertexAttrib(AttribType type)
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
