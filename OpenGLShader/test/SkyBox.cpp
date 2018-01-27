#include "SkyBox.h"



CSkyBox::CSkyBox()
{
	for (size_t i = 0; i < TEXTURE_COUNT; ++i)
	{
		m_pTexture[i] = nullptr;
	}

	m_pShader = nullptr;
	m_pSphereData = nullptr;
	m_pSphereIndex = nullptr;
	m_nSphereBuffer = -1;
	m_nSphereIndex = -1;
	m_pVertex = nullptr;
	m_nVertexBuffer = -1;
	Init();
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
	if (m_pSphereIndex != nullptr)
	{
		delete m_pSphereIndex;
		m_pSphereIndex = nullptr;
	}
	if (m_pVertex != nullptr)
	{
		delete[]m_pVertex;
		m_pVertex = nullptr;
	}
	if (m_pSphereData != nullptr)
	{
		delete m_pSphereData;
		m_pSphereData = nullptr;
	}
	if (glIsBuffer(m_nSphereBuffer))
	{
		glDeleteBuffers(1, &m_nSphereBuffer);
		m_nSphereBuffer = -1;
	}
	if (glIsBuffer(m_nSphereIndex))
	{
		glDeleteBuffers(1, &m_nSphereIndex);
		m_nSphereIndex = -1;
	}

	if (glIsBuffer(m_nVertexBuffer))
	{
		glDeleteBuffers(1, &m_nVertexBuffer);
		m_nVertexBuffer = -1;
	}
}

void CSkyBox::InitTexture(const char * fileDir)
{
	char fileName[TEXTURE_COUNT][50] = { "right.tga", "left.tga", "up.tga", "down.tga", "forward.tga", "backward.tga" };
	for (size_t i = 0; i < TEXTURE_COUNT; ++i)
	{
		char tmpFilePath[255] = { 0 };
		memcpy_s(tmpFilePath, sizeof(tmpFilePath), fileDir, strlen(fileDir));
		strcat_s(tmpFilePath, fileName[i]);
		m_pTexture[i] = CTexture::LoadTexture(tmpFilePath);
	}
	//SetVertexAttrib(CDrawable::AttribType::VERTEXATTRIB);
}

void CSkyBox::Draw()
{
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(m_pShader->GetProgram());
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	//glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, m_nSphereBuffer);
	SetTransformMatrix();
	SetSphereVertexAttrib(CDrawable::AttribType::ATTRIBPOINTER);
	
	/*for (size_t i = 0; i < TEXTURE_COUNT; ++i)
	{
		if (m_pTexture[i])
		{
			glActiveTexture(GL_TEXTURE0 + i);
			m_pTexture[i]->Bind();
			glUniform1i(glGetUniformLocation(m_pShader->GetProgram(), "texSampler"), i);
		}
		glDrawArrays(GL_QUADS, 4 * i, 4);

		if (m_pTexture[i])
		{
			
			m_pTexture[i]->Bind(false);
		}
	}*/
	char *tmpFix[] = { "One", "Two", "Thr", "Fou", "Fiv", "Six" };
	for (size_t i = 0; i < TEXTURE_COUNT; ++i)
	{
		if (m_pTexture[i])
		{
			glActiveTexture(GL_TEXTURE0 + i);
			m_pTexture[i]->Bind();
			char tmpName[20] = "tex";
			strcat_s(tmpName, tmpFix[i]);
			glUniform1i(glGetUniformLocation(m_pShader->GetProgram(), tmpName), i);
		}
	}
	//glDrawArrays(GL_QUADS, 0, POINT_COUNT);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	
	//GLuint tmpPosLocation = glGetAttribLocation(m_pShader->GetProgram(), "pos");
	//glEnableVertexAttribArray(tmpPosLocation);
	//glVertexAttribPointer(tmpPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(CVertex), (GLvoid*)0);

	//GLuint tmpNormalLocation = glGetAttribLocation(m_pShader->GetProgram(), "normal");
	//glEnableVertexAttribArray(tmpNormalLocation);
	//glVertexAttribPointer(tmpNormalLocation, 4, GL_FLOAT, GL_FALSE, sizeof(CVertex), (GLvoid*)(3 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nSphereIndex);

	glDrawElements(GL_TRIANGLES, m_pSphereIndex->size(), GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	glUseProgram(0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE);
	glDisable(GL_DEPTH_TEST);
}

void CSkyBox::Update(float Duration)
{
	static float speed = 0.0005;
	double zTranslate = Duration * speed;
	static bool initFlag = true;
	if (initFlag)
	{
		//SetScale(2, 2, 2);
		SetTranslate(0, 0, -1);
		initFlag = false;
	}
	AddRotate(0, zTranslate, 0);
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
	m_pShader = new CShader("./shader/skybox.vs", "./shader/skybox.fs");
	SetShaderProgram(m_pShader->GetProgram());
	size_t Length = 0;
	m_pVertex = utils::CreateCubicData(0.6, Length);
	
	InitSphere(2.8, 50, 50);

	m_nVertexBuffer = utils::CreateBufferObject(GL_ARRAY_BUFFER, sizeof(CVertexData) * POINT_COUNT, m_pVertex);
	SetSphereVertexAttrib(CDrawable::AttribType::VERTEXATTRIB);

	m_nSphereBuffer = utils::CreateBufferObject(GL_ARRAY_BUFFER, sizeof(CVertex) * m_pSphereData->size(), m_pSphereData->data());

	m_nSphereIndex = utils::CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_pSphereIndex->size(), m_pSphereIndex->data());

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

void CSkyBox::SetSphereVertexAttrib(AttribType type)
{
	if (type == CDrawable::AttribType::VERTEXATTRIB)
	{
		if (m_pShader)
		{
			const char *tmpName[] = { "pos", "normal" };
			m_pShader->SetShaderAttrib(2, tmpName);
		}
	}
	else if (type == CDrawable::AttribType::ATTRIBPOINTER)
	{
		if (m_pShader)
		{
			size_t count[] = { 3, 3 };
			GLuint offset[] = { 0, sizeof(float) * 3 };
			m_pShader->SetAttribPointer(count, GL_FLOAT, GL_FALSE, sizeof(CVertex), offset);
		}
	}
}


void CSkyBox::InitSphere(float radis, int slide, int stacks)
{
	utils::CreateSphereData(radis, slide, stacks, m_pSphereData, m_pSphereIndex);

	if ((m_pSphereData == nullptr) || (m_pSphereIndex == nullptr))
	{
		return;
	}
	
	
	CVertex *pVertex = m_pSphereData->data();

	unsigned int tmpSize = m_pSphereData->size();
	for (int i = 0; i < tmpSize; ++i)
	{
		memcpy(pVertex[i].mColor, pVertex[i].mPos, 3 * sizeof(float));
		pVertex[i].mPos[2] -= 5;
	}
}

