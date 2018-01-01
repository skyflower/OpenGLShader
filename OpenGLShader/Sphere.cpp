#include "Sphere.h"

#include <fstream>
#include <iostream>

extern std::fstream Log;

CSphere::CSphere(float radis, int slide, int stacks):C3DModel()
{
	m_data = nullptr;
	m_pShader = nullptr;
	m_nVertexBufferObj = -1;
	m_nIndexBufObj = -1;
	m_pTexture = nullptr;
	m_pIndex = nullptr;
	//m_nNormalMatrixIndex = -1;
	//m_nModelViewMatrixIndex = -1;

	Init(radis, slide, stacks);
}

CSphere::~CSphere()
{
	if (m_data != nullptr)
	{
		//m_data->clear();
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

void CSphere::Init(float radis, int slide, int stacks)
{
	if ((radis < 0.00001) && (slide < 2))
	{
		return;
	}
	size_t Length = slide * stacks + 2;
	if (m_data == nullptr)
	{
		m_data = new std::vector<CVertex>(Length);
	}
	if (m_pIndex == nullptr)
	{
		//(slide - 1) * 2 * stacks 
		m_pIndex = new std::vector<unsigned int>(slide * stacks * 6);
	}
	double radisStep = 2 * radis / (slide + 1);
	unsigned int dataIndex = 0;
	unsigned int indexIndex = 0;
	CVertex *pVertex = m_data->data();
	pVertex[0].mPos[0] = -radis;
	pVertex[0].mPos[1] = 0;
	pVertex[0].mPos[2] = 0;
	dataIndex++;

	unsigned int *pIndex = m_pIndex->data();
	for (int i = 0; i < slide; ++i)
	{
		double distanceX = (i + 1) * radisStep;
		
		double tmpCos = sqrt(distanceX * 2 * radis);
		double tmp = sqrt(tmpCos * tmpCos - distanceX * distanceX);
		double nextX = -radis + distanceX;
		//double tmp = sqrt(radis * radis - nextX * nextX);
		double alphaStep = (2 * 3.1415926) / (stacks);
		unsigned int tmpBeginIndex = dataIndex;
		for (int j = 0; j < stacks; ++j)
		{
			pVertex[dataIndex].mPos[0] = nextX;
			pVertex[dataIndex].mPos[1] = tmp * std::sin(alphaStep * j);
			pVertex[dataIndex].mPos[2] = tmp * std::cos(alphaStep * j);
			
			dataIndex++;
			
		}
		if(i == 0)
		{
			for (int j = 0; j < stacks; ++j)
			{
				pIndex[indexIndex++] = 0;
				pIndex[indexIndex++] = j + 1;
				pIndex[indexIndex++] = (j + 2) % stacks;
			}
		}
		if ((i != 0) && (i > 0))
		{
			unsigned int preBeginIndex = tmpBeginIndex - stacks;
			for (int j = 0; j < stacks; ++j)
			{
				pIndex[indexIndex++] = preBeginIndex + j;
				pIndex[indexIndex++] = preBeginIndex + (j + 1) % stacks;
				pIndex[indexIndex++] = tmpBeginIndex + j;

				pIndex[indexIndex++] = tmpBeginIndex + j;
				pIndex[indexIndex++] = preBeginIndex + (j + 1) % stacks;
				pIndex[indexIndex++] = tmpBeginIndex + (j + 1) % stacks;

			}
		}
		if (i == slide - 1)
		{
			pVertex[dataIndex].mPos[0] = radis;
			pVertex[dataIndex].mPos[1] = 0;
			pVertex[dataIndex].mPos[2] = 0;
			for (int j = 0; j < stacks; ++j)
			{
				pIndex[indexIndex++] = dataIndex;
				pIndex[indexIndex++] = j + tmpBeginIndex;
				pIndex[indexIndex++] = (j + 1) % stacks + tmpBeginIndex;
			}
			dataIndex++;
		}
	}

	unsigned int tmpSize = m_data->size();
	for (int i = 0; i < tmpSize; ++i)
	{
		memcpy(pVertex[i].mColor, pVertex[i].mPos, 3 * sizeof(float));
		pVertex[i].mPos[2] -= 8;
	}

	InitDisplayBuffer();
}

void CSphere::InitTexture(const char * pTextureFile)
{
	m_pTexture = CTexture::LoadTexture(pTextureFile);
	SetVertexAttrib(VERTEXATTRIB);	
}

void CSphere::InitDisplayBuffer()
{
	m_nVertexBufferObj = utils::CreateBufferObject(GL_ARRAY_BUFFER, sizeof(CVertex) * m_data->size(), m_data->data(), GL_STATIC_DRAW);
	m_nIndexBufObj = utils::CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, m_pIndex->size() * sizeof(unsigned int), m_pIndex->data());
	
	m_pShader = new CShader("./shader/sphere.vs", "./shader/sphere.fs");
	
	SetVertexAttrib(VERTEXATTRIB);
	
	SetShaderProgram(m_pShader->GetProgram());
}

void CSphere::Draw()
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE);
	glEnable(GL_TEXTURE_2D);
	glUseProgram(m_pShader->GetProgram());

	glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObj);
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_nSSBOProgram);
	if (m_pTexture)
	{
		m_pTexture->Bind();
	}

	SetTransformMatrix();

	SetVertexAttrib(ATTRIBPOINTER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIndexBufObj);

	//glDrawArrays(GL_POINTS, 0, m_data->size());
	glDrawElements(GL_TRIANGLES, m_pIndex->size(), GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	if (m_pTexture)
	{
		m_pTexture->Bind(false);
	}
	glUseProgram(0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE);
	glDisable(GL_TEXTURE_2D);
}

void CSphere::Update(float duration)
{
	static float speed = 0.001;
	double zTranslate = duration * speed;
	static bool initFlag = true;
	if (initFlag)
	{
		//SetScale(2, 2, 2);
		SetTranslate(0, 0, -1);
		initFlag = false;
	}
	AddRotate(0, zTranslate, 0);
	
}

GLuint CSphere::GetShaderProgram()
{
	if (m_pShader)
	{
		return m_pShader->GetProgram();
	}
	return -1;
}

void CSphere::TestShader(mat4f model)
{
	static bool initFlag = true;
	if (initFlag == false)
	{
		return;
	}
	Log << "TestShader Matrix...\n" << model << "\n";
	size_t nSize = m_data->size();
	CVertex *pVertex = m_data->data();
	for (size_t i = 0; i < m_data->size(); ++i)
	{
		vec3f tmp(pVertex[i].mPos, 3);
		vec4f tmpData(tmp, 1.0);
		Log << tmpData << "\n";
		Log << (tmpData * model) << "\n";
	}
	initFlag = false;
}

void CSphere::SetVertexAttrib(AttribType type)
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
			size_t count[] = { 3, 3 };
			GLuint offset[] = { 0, sizeof(float) * 3 };
			m_pShader->SetAttribPointer(count, GL_FLOAT, GL_FALSE, sizeof(CVertex), offset);
		}
	}
}
