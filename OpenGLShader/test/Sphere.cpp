#include "Sphere.h"
#include "../common/Log.h"
#include <iostream>


CSphere::CSphere(float radis, int slide, int stacks):C3DModel()
{
	m_data = nullptr;
	m_pShader = nullptr;
	m_nVertexBufferObj = -1;
	m_nIndexBufObj = -1;
	m_pTexture = nullptr;
	m_pIndex = nullptr;

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
	utils::CreateSphereData(radis, slide, stacks, m_data, m_pIndex);


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
	
	//m_pShader = new CShader("./shader/sphere.vs", "./shader/sphere.fs");
	m_pShader = new CShader("./shader/sphereTwo.vs", "./shader/sphereTwo.fs");
	
	SetVertexAttrib(VERTEXATTRIB);
	
	SetShaderProgram(m_pShader->GetProgram());
}

void CSphere::Draw()
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	
	glUseProgram(m_pShader->GetProgram());
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObj);
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_nSSBOProgram);
	if (m_pTexture)
	{
		m_pTexture->Bind();
	}

	SetTransformMatrix();
	//mat4f modelMatrix = GetModel();
	//TestShader(modelMatrix);
	
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
	glDisable(GL_TEXTURE_2D);
}

void CSphere::Update(float duration)
{
	static float speed = 0.0002;
	double zTranslate = duration * speed;
	static bool initFlag = true;
	if (initFlag)
	{
		//SetScale(2, 2, 2);
		SetTranslate(-0.5, 0, -1);
		initFlag = false;
		//SetRotate(0, 3.1415926, 0);
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
	//Log << "TestShader Matrix...\n" << model << "\n";
	//WriteInfo("modelMatrix = \n%s", model.FormatToString().c_str());
	//mat3f normalMatrix = model.reduceDimension();
	//normalMatrix = normalMatrix.inverse().Transposition();
	//model = model.Transposition();
	/*mat4f viewMatrix = GetViewMatrix();
	mat4f projectionMatrix = GetProjectionMatrix();
	WriteInfo("viewMatrix = \n%s", viewMatrix.FormatToString().c_str());
	WriteInfo("projectionMatrix = \n%s", projectionMatrix.FormatToString().c_str());
	*/
	WriteInfo("modelMatrix = \n%s", model.FormatToString().c_str());

	//projectionMatrix = projectionMatrix.Transposition();
	size_t nSize = m_data->size();
	CVertex *pVertex = m_data->data();
	for (size_t i = 0; i < m_data->size() && i < 10; ++i)
	{
		vec3f tmp(pVertex[i].mColor, 3);
		vec4f tmpData(tmp, 1.0);
		WriteInfo("pos vector = %s", tmpData.FormatToString().c_str());
		//tmpData = tmpData * model;
		//WriteInfo("pos * model vector = %s", tmpData.FormatToString().c_str());
		//Log << tmpData << "\n";
		//Log << (tmpData * model) << "\n";
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
			size_t count[] = { 3, 4 };
			GLuint offset[] = { 0, sizeof(float) * 3 };
			m_pShader->SetAttribPointer(count, GL_FLOAT, GL_FALSE, sizeof(CVertex), offset);
		}
	}
}
