#include "ParticleTFO.h"
#include <fstream>

extern std::fstream Log;


CParticleTFO::CParticleTFO():C3DModel()
{
	m_pTexture = nullptr;
	m_pShader = nullptr;
	m_pTFOShader = nullptr;
	m_nPointCount = 3;
	m_pPoints = nullptr;
	m_nVertexBufferObj = -1;
	m_nTfoID = -1;
	m_nTfoBuffer = -1;
	m_nPosTfoLocation = -1;

	Init();
}


CParticleTFO::~CParticleTFO()
{
	if (m_pShader)
	{
		delete m_pShader;
		m_pShader = nullptr;
	}
	if (m_pTFOShader)
	{
		delete m_pTFOShader;
		m_pTFOShader = nullptr;
	}
	if (m_pPoints)
	{
		delete []m_pPoints;
		m_pPoints = nullptr;
		m_nPointCount = 0;
	}
	if (m_pTexture)
	{
		CTexture::UnLoadTexture(m_pTexture);
		m_pTexture = nullptr;
	}
	if (glIsBuffer(m_nVertexBufferObj))
	{
		glDeleteBuffers(1, &m_nVertexBufferObj);
		m_nVertexBufferObj = -1;
	}
	if (glIsBuffer(m_nTfoBuffer))
	{
		glDeleteBuffers(1, &m_nTfoBuffer);
		m_nTfoBuffer = -1;
	}

	
}

void CParticleTFO::InitTexture(const char * filepath)
{
	m_pTexture = CTexture::LoadTexture(filepath);
}

void CParticleTFO::Draw()
{
	glUseProgram(m_pShader->GetProgram());
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (m_pTexture)
	{
		m_pTexture->Bind();
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObj);

	SetVertexAttrib(CDrawable::AttribType::ATTRIBPOINTER);

	mat4f tmp = C3DModel::GetModel();// *m_fMVPMatrix;

	glUniformMatrix4fv(m_nMVPLocation, 1, GL_FALSE, &tmp[0][0]);

	//glDrawArrays(GL_QUADS, 0, m_nPointCount);
	glDrawArrays(GL_TRIANGLES, 0, m_nPointCount);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (m_pTexture)
	{
		m_pTexture->Bind(false);
	}
	glUseProgram(0);

	glDisable(GL_BLEND);
}

void CParticleTFO::Update(float duration)
{
	static float speed = 0.001;
	double zTranslate = duration * speed;
	static bool initFlag = true;
	if (initFlag)
	{
		//SetScale(4.41828, 3.3137, 1);
		initFlag = false;
	}
	//AddRotate(0, zTranslate, zTranslate);
}

GLuint CParticleTFO::GetShaderProgram()
{
	if (m_pShader)
	{
		return m_pShader->GetProgram();
	}
	return -1;
}


void CParticleTFO::ResetState()
{
}

void CParticleTFO::Init()
{
	m_pPoints = new CVertex_SSBO_Two[m_nPointCount];
	m_pPoints[0].m_fPos[0] = -0.5;
	m_pPoints[0].m_fPos[1] = -0.5;
	m_pPoints[0].m_fPos[2] = -1;
	m_pPoints[0].m_fPos[3] = 1;

	m_pPoints[1].m_fPos[0] = 0.5;
	m_pPoints[1].m_fPos[1] = -0.5;
	m_pPoints[1].m_fPos[2] = -1;
	m_pPoints[1].m_fPos[3] = 1;

	m_pPoints[2].m_fPos[0] = 0.5;
	m_pPoints[2].m_fPos[1] = 0.5;
	m_pPoints[2].m_fPos[2] = -1;
	m_pPoints[2].m_fPos[3] = 1;


	m_pShader = new CShader("./shader/TFO/Simple.vs", "./shader/TFO/Simple.fs");

	m_nMVPLocation = glGetUniformLocation(m_pShader->GetProgram(), "MVP");

	const char *attrib[] = { "gl_Position" };
	m_nTfoProgram = utils::CreateTFOProgram("./shader/TFO/SimpleTFO.vs", attrib, 1, GL_INTERLEAVED_ATTRIBS);

	m_nPosTfoLocation = glGetAttribLocation(m_nTfoProgram, "pos");

	m_nVertexBufferObj = utils::CreateBufferObject(GL_ARRAY_BUFFER, sizeof(CVertex_SSBO_Two) * m_nPointCount, m_pPoints);

	SetVertexAttrib(CDrawable::AttribType::VERTEXATTRIB);
	
	m_nTfoID = utils::CreateTransformFeedBack(m_nTfoBuffer, sizeof(CVertex_SSBO_Two) * m_nPointCount, nullptr, GL_STATIC_DRAW);
	

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_nTfoID);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_nTfoBuffer);

	glUseProgram(m_nTfoProgram);
	glBeginTransformFeedback(GL_TRIANGLES);
	glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObj);
	glEnableVertexAttribArray(m_nPosTfoLocation);
	glVertexAttribPointer(m_nPosTfoLocation, 4, GL_FLOAT, GL_FALSE, sizeof(CVertex_SSBO_Two), 0);
	

	glDrawArrays(GL_TRIANGLES, 0, m_nPointCount);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEndTransformFeedback();
	glUseProgram(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_nTfoBuffer);
	CVertex_SSBO_Two *p = (CVertex_SSBO_Two*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
	for (size_t i = 0; i < 3; ++i)
	{
		Log << p->m_fPos[0] << "  " << p->m_fPos[1] << "  " << p->m_fPos[2] << "  " << p->m_fPos[3] << "\n";
		++p;
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void CParticleTFO::SetVertexAttrib(AttribType type)
{
	if (type == CDrawable::AttribType::VERTEXATTRIB)
	{
		if (m_pShader)
		{
			const char *tmpName[] = { "pos"};
			m_pShader->SetShaderAttrib(1, tmpName);
		}
	}
	else if (type == CDrawable::AttribType::ATTRIBPOINTER)
	{
		if (m_pShader)
		{
			size_t count[] = { 4};
			GLuint offset[] = { 0};
			m_pShader->SetAttribPointer(count, GL_FLOAT, GL_FALSE, sizeof(CVertex_SSBO_Two), offset);
		}
	}
}