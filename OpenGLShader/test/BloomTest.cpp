#include "BloomTest.h"
#include "../common/Log.h"
#include <iostream>


CBloomTest::CBloomTest(float radis, int slide, int stacks) :
	m_pSphereShader(nullptr),
	m_pCubicShader(nullptr),
	m_pGroundShader(nullptr),
	m_pTexture(nullptr),
	m_pGroundVertex(nullptr),
	m_nGroundVertexBuffer(-1),
	m_pSphereData(nullptr),
	m_pSphereIndex(nullptr),
	m_pCubicVertex(nullptr),
	m_nCubicVertexBuffer(-1),
	C3DModel()
{
	Init(radis, slide, stacks);
}

CBloomTest::~CBloomTest()
{
	if (m_pTexture)
	{
		CTexture::UnLoadTexture(m_pTexture);
		m_pTexture = nullptr;
	}
	if (m_pSphereShader)
	{
		delete m_pSphereShader;
		m_pSphereShader = nullptr;
	}
	if (m_pCubicShader)
	{
		delete m_pCubicShader;
		m_pCubicShader = nullptr;
	}
	if (m_pGroundShader)
	{
		delete m_pGroundShader;
		m_pGroundShader = nullptr;
	}

	if (m_pGroundVertex != nullptr)
	{
		m_pGroundVertex->clear();
		delete m_pGroundVertex;
		m_pGroundVertex = nullptr;
	}
	if (glIsBuffer(m_nGroundVertexBuffer))
	{
		glDeleteBuffers(1, &m_nGroundVertexBuffer);
		m_nGroundVertexBuffer = -1;
	}

	if (m_pSphereData != nullptr)
	{
		m_pSphereData->clear();
		delete m_pSphereData;
		m_pSphereData = nullptr;
	}
	if (m_pSphereIndex != nullptr)
	{
		m_pSphereIndex->clear();
		delete m_pSphereIndex;
		m_pSphereIndex = nullptr;
	}
	if (glIsBuffer(m_nSphereVertexBuffer))
	{
		glDeleteBuffers(1, &m_nSphereVertexBuffer);
		m_nSphereVertexBuffer = -1;
	}
	if (glIsBuffer(m_nSphereIndexBuffer))
	{
		glDeleteBuffers(1, &m_nSphereIndexBuffer);
		m_nSphereIndexBuffer = -1;
	}
	if (glIsBuffer(m_nCubicVertexBuffer))
	{
		glDeleteBuffers(1, &m_nCubicVertexBuffer);
		m_nCubicVertexBuffer = -1;
	}
	if (m_pCubicVertex != nullptr)
	{
		m_pCubicVertex->clear();
		delete m_pCubicVertex;
		m_pCubicVertex = nullptr;
	}
}

void CBloomTest::Init(float radis, int slide, int stacks)
{
	if ((radis < 0.00001) && (slide < 2))
	{
		return;
	}
	utils::CreateSphereData(radis, slide, stacks, m_pSphereData, m_pSphereIndex);
	CVertex *pVertex = m_pSphereData->data();

	unsigned int tmpSize = m_pSphereData->size();
	for (int i = 0; i < tmpSize; ++i)
	{
		memcpy(pVertex[i].mColor, pVertex[i].mPos, 3 * sizeof(float));
		pVertex[i].mPos[2] -= 3;
	}

	size_t nCubicLength = 0;
	CVertexData *pCubicVertexData = utils::CreateCubicData(0.1, nCubicLength);
	if (m_pCubicVertex == nullptr)
	{
		m_pCubicVertex = new std::vector<CVertexData>(nCubicLength);
		memcpy(m_pCubicVertex->data(), pCubicVertexData, sizeof(CVertexData) * nCubicLength);
	}
	
	utils::CreateGroundData(5, m_pGroundVertex);

	InitDisplayBuffer();
}

void CBloomTest::InitTexture(const char * pTextureFile)
{
	m_pTexture = CTexture::LoadTexture(pTextureFile);
	//SetVertexAttrib(VERTEXATTRIB);
}

void CBloomTest::InitDisplayBuffer()
{
	m_nGroundVertexBuffer = utils::CreateBufferObject(GL_ARRAY_BUFFER, sizeof(CVertexData) * m_pGroundVertex->size(), m_pGroundVertex->data());
	m_nSphereVertexBuffer = utils::CreateBufferObject(GL_ARRAY_BUFFER, sizeof(CVertex) * m_pSphereData->size(), m_pSphereData->data());
	m_nSphereIndexBuffer = utils::CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_pSphereIndex->size(), m_pSphereIndex->data());
	m_nCubicVertexBuffer = utils::CreateBufferObject(GL_ARRAY_BUFFER, sizeof(CVertexData) * m_pCubicVertex->size(), m_pCubicVertex->data());
	
	m_pGroundShader = new CShader("./shader/Bloom/ground.vs", "./shader/Bloom/bloomtest.fs");
	
	const char *name[] = { "pos", "normal", "texCoord" };
	m_pGroundShader->SetShaderAttrib(3, name);

	
	m_pCubicShader = new CShader("./shader/Bloom/cubic.vs", "./shader/Bloom/bloomtest.fs");
	m_pCubicShader->SetShaderAttrib(3, name);
	
	m_pSphereShader = new CShader("./shader/Bloom/sphere.vs", "./shader/Bloom/bloomtest.fs");
	m_pSphereShader->SetShaderAttrib(2, name);
}

void CBloomTest::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawCubic();
	DrawGround();
	DrawSphere();
}

void CBloomTest::Update(float duration)
{
	static float speed = 0.0003;
	double zTranslate = duration * speed;
	static bool initFlag = true;
	if (initFlag)
	{
		//SetScale(2, 2, 2);
		//SetTranslate(0, 0, -1);
		initFlag = false;
	}
	//AddRotate(0, zTranslate, 0);
	
}

GLuint CBloomTest::GetShaderProgram()
{
	return -1;
}

void CBloomTest::TestShader(mat4f model)
{
	static bool initFlag = true;
	if (initFlag == false)
	{
		return;
	}
	WriteInfo("modelMatrix = \n%s", model.FormatToString().c_str());
	
	initFlag = false;
}

void CBloomTest::DrawGround()
{
	glEnable(GL_DEPTH_TEST);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ResetStatus();
	
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	SetShaderProgram(m_pGroundShader->GetProgram());
	float Angle = 15 * 3.1415926 / 180;
	SetRotate(Angle, 0, 0);
	glUseProgram(m_pGroundShader->GetProgram());
	glBindBuffer(GL_ARRAY_BUFFER, m_nGroundVertexBuffer);

	SetTransformMatrix();
	int posLocation = m_pGroundShader->GetLocation("pos");
	int normalLocation = m_pGroundShader->GetLocation("normal");
	int texLocation = m_pGroundShader->GetLocation("texCoord");
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(CVertexData), 0);
	glEnableVertexAttribArray(normalLocation);
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(CVertexData), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(texLocation);
	glVertexAttribPointer(texLocation, 2, GL_FLOAT, GL_FALSE, sizeof(CVertexData), (void*)(6 * sizeof(float)));

	glDrawArrays(GL_QUADS, 0, m_pGroundVertex->size());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
	glDisable(GL_DEPTH_TEST);
	//glDisable(GL_BLEND);
}

void CBloomTest::DrawCubic()
{
	glEnable(GL_DEPTH_TEST);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ResetStatus();
	
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	SetShaderProgram(m_pCubicShader->GetProgram());
	float Angle = 15 * 3.1415926 / 180;
	SetRotate(Angle, Angle, Angle);
	SetTranslate(-0.7, 0, 0);
	glUseProgram(m_pCubicShader->GetProgram());
	glBindBuffer(GL_ARRAY_BUFFER, m_nCubicVertexBuffer);

	SetTransformMatrix();
	int posLocation = m_pCubicShader->GetLocation("pos");
	int normalLocation = m_pCubicShader->GetLocation("normal");
	int texLocation = m_pCubicShader->GetLocation("texCoord");
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(CVertexData), 0);
	glEnableVertexAttribArray(normalLocation);
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(CVertexData), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(texLocation);
	glVertexAttribPointer(texLocation, 2, GL_FLOAT, GL_FALSE, sizeof(CVertexData), (void*)(6 * sizeof(float)));

	glDrawArrays(GL_QUADS, 0, m_pCubicVertex->size());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
	glDisable(GL_DEPTH_TEST);
	//glDisable(GL_BLEND);
}

void CBloomTest::DrawSphere()
{
	glEnable(GL_DEPTH_TEST);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ResetStatus();
	
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	SetShaderProgram(m_pSphereShader->GetProgram());
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//static float Angle = 90 * 3.1415926 / 180;
	//Angle += 0.0002;
	SetTranslate(0, 0.4, -4);
	SetRotate(0, 3.340890, 0);
	//WriteInfo("Sphere Angle = %f", Angle);
	glUseProgram(m_pSphereShader->GetProgram());
	glBindBuffer(GL_ARRAY_BUFFER, m_nSphereVertexBuffer);

	SetTransformMatrix();

	int posLocation = m_pSphereShader->GetLocation("pos");
	int normalLocation = m_pSphereShader->GetLocation("normal");
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(CVertex), 0);
	glEnableVertexAttribArray(normalLocation);
	glVertexAttribPointer(normalLocation, 4, GL_FLOAT, GL_FALSE, sizeof(CVertex), (void*)(3 * sizeof(float)));
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nSphereIndexBuffer);

	glDrawElements(GL_TRIANGLES, m_pSphereIndex->size(), GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glUseProgram(0);
	glDisable(GL_DEPTH_TEST);
	//glDisable(GL_BLEND);
}
