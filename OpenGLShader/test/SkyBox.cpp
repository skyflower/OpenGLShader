#include "SkyBox.h"



CSkyBox::CSkyBox()
{
	m_pShader = nullptr;
	m_pSphereData = nullptr;
	m_pSphereIndex = nullptr;
	m_nSphereBuffer = -1;
	m_nSphereIndex = -1;
	m_pVertex = nullptr;
	m_pCubeTexture = nullptr;
	m_pBoxShader = nullptr;
	m_nVertexBuffer = -1;
	m_nCubicVertexCount = 0;
	Init();
}


CSkyBox::~CSkyBox()
{
	if (m_pBoxShader != nullptr)
	{
		delete m_pBoxShader;
		m_pBoxShader = nullptr;
	}
	if (m_pCubeTexture != nullptr)
	{
		CTexture::UnLoadTexture(m_pCubeTexture);
		m_pCubeTexture = nullptr;
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
	char fileName[6][50] = { "right.tga", "left.tga", "up.tga", "down.tga", "forward.tga", "backward.tga" };
	
	std::string strFileDir(fileDir);
	if (strFileDir.find('/') != std::string::npos)
	{
		if (strFileDir.back() != '/')
		{
			strFileDir = strFileDir + '/';
		}
	}
	else
	{
		if (strFileDir.back() != '\\')
		{
			strFileDir = strFileDir + '\\';
		}
	}
	std::string strPosX = strFileDir + fileName[0];
	std::string strNegX = strFileDir + fileName[1];
	std::string strPosY = strFileDir + fileName[2];
	std::string strNegY = strFileDir + fileName[3];
	std::string strPosZ = strFileDir + fileName[4];
	std::string strNegZ = strFileDir + fileName[5];
	m_pCubeTexture = CTexture::LoadCubeMapTexture(strPosX.c_str(), 
		strNegX.c_str(), strPosY.c_str(), strNegY.c_str(), 
		strPosZ.c_str(), strNegZ.c_str());

	//SetVertexAttrib(CDrawable::AttribType::VERTEXATTRIB);
}

void CSkyBox::Draw()
{
	//glDisable(GL_TEXTURE);
	//glDisable(GL_DEPTH_TEST);
	DrawSkyBox();
	DrawSphere();
}

void CSkyBox::Update(float Duration)
{
	static float speed = 0.0002;
	double zTranslate = Duration * speed;
	static bool initFlag = true;
	if (initFlag)
	{
		//SetScale(2, 2, 2);
		//SetTranslate(0, 0, -1);
		initFlag = false;
	}
	//mat4f modelMatrix = GetModel();
	//modelMatrix = modelMatrix * CMatrix<4, 4, float>::GetRotate<float>(zTranslate, 0.3, 1, 0);
	//SetModelMatrix(modelMatrix);
	//AddRotate(0, zTranslate, 0);
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
	m_pVertex = utils::CreateCubicData(0.8, m_nCubicVertexCount);
	m_nVertexBuffer = utils::CreateBufferObject(GL_ARRAY_BUFFER, sizeof(CVertexData) * m_nCubicVertexCount, m_pVertex);

	m_pBoxShader = new CShader("./shader/SkyBox/cubic.vs", "./shader/SkyBox/cubic.fs");
	SetVertexAttrib(CDrawable::AttribType::VERTEXATTRIB);


	m_pShader = new CShader("./shader/SkyBox/skybox.vs", "./shader/SkyBox/skybox.fs");
	SetSphereVertexAttrib(CDrawable::AttribType::VERTEXATTRIB);

	InitSphere(0.3, 50, 50);
	m_nSphereBuffer = utils::CreateBufferObject(GL_ARRAY_BUFFER, sizeof(CVertex) * m_pSphereData->size(), m_pSphereData->data());
	m_nSphereIndex = utils::CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_pSphereIndex->size(), m_pSphereIndex->data());

}

void CSkyBox::SetVertexAttrib(AttribType type)
{
	if (type == CDrawable::AttribType::VERTEXATTRIB)
	{
		if (m_pBoxShader)
		{
			const char *tmpName[] = { "pos", "texCoord", "normal" };
			m_pBoxShader->SetShaderAttrib(3, tmpName);
		}
	}
	else if (type == CDrawable::AttribType::ATTRIBPOINTER)
	{
		if (m_pBoxShader)
		{
			size_t count[] = { 3, 2, 3 };
			GLuint offset[] = { 0, sizeof(float) * 6, sizeof(float) * 3 };
			m_pBoxShader->SetAttribPointer(count, GL_FLOAT, GL_FALSE, sizeof(CVertexData), offset);
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
			size_t count[] = { 3, 4 };
			GLuint offset[] = { 0, sizeof(float) * 3 };
			m_pShader->SetAttribPointer(count, GL_FLOAT, GL_FALSE, sizeof(CVertex), offset);
		}
	}
}

void CSkyBox::DrawSkyBox()
{
	mat4f model = CMatrix<4, 4, float>::GetUnit();
	SetModelMatrix(model);
	glEnable(GL_BLEND);
	
	glDisable(GL_DEPTH_TEST);
	SetShaderProgram(m_pBoxShader->GetProgram());
	glUseProgram(m_pBoxShader->GetProgram());
	glEnable(GL_TEXTURE_CUBE_MAP);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_pCubeTexture->GetTextureID());
	glUniform1i(m_pBoxShader->GetLocation("texSampler"), 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBuffer);
	SetTransformMatrix();
	SetVertexAttrib(CDrawable::AttribType::ATTRIBPOINTER);

	glDrawArrays(GL_QUADS, 0, m_nCubicVertexCount);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
	glDisable(GL_TEXTURE_CUBE_MAP);
	glDisable(GL_BLEND);
}

void CSkyBox::DrawSphere()
{
	mat4f model = CMatrix<4, 4, float>::GetTranslate<float>(0.0f, 0.0f, -1.0f);
	SetModelMatrix(model);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_DEPTH_TEST);
	SetShaderProgram(m_pShader->GetProgram());
	glUseProgram(m_pShader->GetProgram());

	glBindBuffer(GL_ARRAY_BUFFER, m_nSphereBuffer);
	SetTransformMatrix();
	SetSphereVertexAttrib(CDrawable::AttribType::ATTRIBPOINTER);

	glActiveTexture(GL_TEXTURE0);
	m_pCubeTexture->Bind();
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_pCubeTexture->GetTextureID());
	glUniform1i(glGetUniformLocation(m_pShader->GetProgram(), "texSampler"), 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nSphereIndex);

	glDrawElements(GL_TRIANGLES, m_pSphereIndex->size(), GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	
	m_pCubeTexture->Bind(false);
	glUseProgram(0);
	
	glDisable(GL_TEXTURE_CUBE_MAP);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

}


void CSkyBox::InitSphere(float radis, int slide, int stacks)
{
	utils::CreateSphereData(radis, slide, stacks, m_pSphereData, m_pSphereIndex);

	if ((m_pSphereData == nullptr) || (m_pSphereIndex == nullptr))
	{
		WriteInfo("Create sphere Failed");
		return;
	}
}

