#include "ParticleEmit.h"
#include <fstream>

extern std::fstream Log;


CParticleEmit::CParticleEmit():C3DModel()
{
	
	m_pShader = nullptr;
	m_nPointCount = 3;
	m_pPoints = nullptr;
	m_nVertexBufferObj = -1;
	m_nTfoID = -1;
	m_nTfoBuffer = -1;
	//m_nPosTfoLocation = -1;
	m_pTexture = nullptr;
	m_nTexLocation = -1;
	m_bEmitNewParticle = true;
	m_pTFOShader = nullptr;
	m_pUpdateShader = nullptr;
	m_nPointCountInBuffer = 0;
	
	

	Init();
}


CParticleEmit::~CParticleEmit()
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
	if (m_pUpdateShader)
	{
		delete m_pUpdateShader;
		m_pUpdateShader = nullptr;
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
	if (m_pPoints)
	{
		delete[]m_pPoints;
		m_pPoints = nullptr;
		m_nPointCount = 0;
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

void CParticleEmit::InitTexture(const char * filepath)
{
	if (filepath == nullptr)
	{
		const size_t defaultSize = 256;
		int defaultWidth = defaultSize;
		int defaultHeight = defaultSize;
		unsigned char *pData = new unsigned char[defaultWidth * defaultHeight];
		double distanceMax = sqrt(pow(defaultWidth, 2) + pow(defaultHeight, 2)) / 2;
		//Log << "distanceMax\t" << distanceMax << "\n";
		for (int i = 0; i < defaultWidth; ++i)
		{
			for (int j = 0; j < defaultHeight; ++j)
			{
				double distanceX = i - defaultWidth / 2;
				double distanceY = j - defaultHeight / 2;

				double distance = sqrt(distanceX * distanceX + distanceY * distanceY);
				double alpha = pow((distanceMax - distance) / distanceMax, 8.0);
				alpha = alpha > 1.0f ? 1.0f : alpha;
				pData[i * defaultHeight + j] = alpha > 0.0f ? (alpha * 255) : 0;
			}
		}
		m_pTexture = CTexture::CreateTextureFromData(pData, defaultWidth, defaultHeight, GL_ALPHA);
		delete[]pData;
		pData = nullptr;
	}
	else
	{
		m_pTexture = CTexture::LoadTexture(filepath);
	}
}

void CParticleEmit::Draw()
{
	glUseProgram(m_pShader->GetProgram());
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	if (m_pTexture != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(m_nTexLocation, 0);
		m_pTexture->Bind();
	}

	//glEnableVertexAttribArray(m_nTexLocation);
	//glVertexAttribPointer(m_nTexLocation, )
	
	//glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObj);
	glBindBuffer(GL_ARRAY_BUFFER, m_nUpdateTfoBuffer[m_nCurDrawTfo]);

	SetVertexAttrib(m_pShader, CDrawable::AttribType::ATTRIBPOINTER);

	mat4f tmp = C3DModel::GetModel();// *m_fMVPMatrix;

	glUniformMatrix4fv(m_nMVPLocation, 1, GL_FALSE, &tmp[0][0]);

	//glDrawArrays(GL_QUADS, 0, m_nPointCount);
	//glDrawArrays(GL_POINTS, 0, m_nPointCountInBuffer);// m_nPointCount);
	//
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawTransformFeedback(GL_POINTS, m_nUpdateTfo[m_nCurDrawTfo]);

	glUseProgram(0);
	if (m_pTexture != nullptr)
	{
		m_pTexture->Bind(false);
		glDisable(GL_TEXTURE_2D);
	}
	glDisable(GL_BLEND);
}

void CParticleEmit::Update(float duration)
{
	static float speed = 0.001;
	double zTranslate = duration * speed;
	static bool initFlag = true;
	if (initFlag)
	{
		//SetScale(4.41828, 3.3137, 1);
		initFlag = false;
	}
	//if (m_nPointCountInBuffer != 0)
	{
		glGetQueryObjectiv(m_nQureyObject, GL_QUERY_RESULT, &m_nPointCountInBuffer);
		//Log << __LINE__ << "\tm_nPointCountInBuffer = " << m_nPointCountInBuffer << "\n";

		if (m_nPointCountInBuffer < 2000)
		{
			EmitParticle();
		}
	}
	//Log << __LINE__ << "\tm_nPointCountInBuffer = " << m_nPointCountInBuffer << "\n";
	glEnable(GL_RASTERIZER_DISCARD);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_nUpdateTfo[m_nCurUpdateTfo]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_nUpdateTfoBuffer[m_nCurUpdateTfo]);
	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, m_nQureyObject);
	glUseProgram(m_pUpdateShader->GetProgram());
	glBeginTransformFeedback(GL_POINTS);
	
	if (m_bEmitNewParticle)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_nTfoBuffer);
		SetVertexAttrib(m_pUpdateShader, CDrawable::AttribType::ATTRIBPOINTER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawTransformFeedback(GL_POINTS, m_nTfoID);
		m_bEmitNewParticle = false;
	}
	
	if (m_nPointCountInBuffer > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_nUpdateTfoBuffer[m_nCurDrawTfo]);
		SetVertexAttrib(m_pUpdateShader, CDrawable::AttribType::ATTRIBPOINTER);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawTransformFeedback(GL_POINTS, m_nUpdateTfo[m_nCurDrawTfo]);
	}

	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
	glUseProgram(0);
	glEndTransformFeedback();
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glDisable(GL_RASTERIZER_DISCARD);

	/*glBindBuffer(GL_ARRAY_BUFFER, m_nUpdateTfoBuffer[m_nCurDrawTfo]);
	CParticleVertex *p = (CParticleVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
	for (size_t i = 0; i < m_nPointCount; ++i)
	{
		Log << p->mesh[0] << "\n";
		Log << p->pos[0] << "  " << p->pos[1] << "  " << p->pos[2] << "  " << p->pos[3] << "\n";
		++p;
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/

	
	m_nCurDrawTfo = m_nCurUpdateTfo;
	m_nCurUpdateTfo = (m_nCurUpdateTfo + 1) % 2;
}

GLuint CParticleEmit::GetShaderProgram()
{
	if (m_pShader)
	{
		return m_pShader->GetProgram();
	}
	return -1;
}

void CParticleEmit::EmitParticle()
{
	static bool initShader = true;
	if (initShader == true)
	{
		const char *Attribs[] = { "gl_Position", "mess" };

		m_pTFOShader = new CShader("./shader/TFO/SimpleTFO.vs", Attribs, 2, GL_INTERLEAVED_ATTRIBS);
		//utils::CheckError(__FILE__, __FUNCTION__, __LINE__);
		SetVertexAttrib(m_pTFOShader, CDrawable::AttribType::VERTEXATTRIB);

		m_nTfoID = utils::CreateTransformFeedBack(m_nTfoBuffer, sizeof(CParticleVertex) * m_nPointCount, nullptr, GL_STATIC_DRAW);

		m_pUpdateShader = new CShader("./shader/TFO/updateParticle.vs", Attribs, 2, GL_INTERLEAVED_ATTRIBS, "./shader/TFO/updateParticle.gs");

		SetVertexAttrib(m_pUpdateShader, CDrawable::AttribType::VERTEXATTRIB);
		initShader = false;
	}
	

	m_bEmitNewParticle = true;
	glEnable(GL_RASTERIZER_DISCARD);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_nTfoID);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_nTfoBuffer);

	glUseProgram(m_pTFOShader->GetProgram());
	glBeginTransformFeedback(GL_POINTS);
	glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObj);
	
	SetVertexAttrib(m_pTFOShader, CDrawable::AttribType::ATTRIBPOINTER);
	glDrawArrays(GL_POINTS, 0, m_nPointCount);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEndTransformFeedback();
	glUseProgram(0);
	glDisable(GL_RASTERIZER_DISCARD);

	/*glBindBuffer(GL_ARRAY_BUFFER, m_nTfoBuffer);
	CParticleVertex *p = (CParticleVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
	for (size_t i = 0; i < 3; ++i)
	{
		Log << p->pos[0] << "  " << p->pos[1] << "  " << p->pos[2] << "  " << p->pos[3] << "\n";
		++p;
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/
}


void CParticleEmit::ResetState()
{
}

void CParticleEmit::Init()
{
	//glEnable(GL_POINT_SPRITE);
	//glEnable(GL_PROGRAM_POINT_SIZE);
	m_pPoints = new CParticleVertex[m_nPointCount];
	memset(m_pPoints, 0, sizeof(CParticleVertex) * m_nPointCount);
	m_pPoints[0].pos[0] = -0.5;
	m_pPoints[0].pos[1] = -0.5;
	m_pPoints[0].pos[2] = -1;
	m_pPoints[0].pos[3] = 1;
				 
	m_pPoints[1].pos[0] = 0.5;
	m_pPoints[1].pos[1] = -0.5;
	m_pPoints[1].pos[2] = -1;
	m_pPoints[1].pos[3] = 1;
				 
	m_pPoints[2].pos[0] = 0.5;
	m_pPoints[2].pos[1] = 0.5;
	m_pPoints[2].pos[2] = -1;
	m_pPoints[2].pos[3] = 1;

	glGenQueries(1, &m_nQureyObject);


	m_pShader = new CShader("./shader/TFO/Simple.vs", "./shader/TFO/Simple.fs", "./shader/TFO/Simple.gs");

	m_nMVPLocation = glGetUniformLocation(m_pShader->GetProgram(), "MVP");

	m_nTexLocation = glGetUniformLocation(m_pShader->GetProgram(), "U_MainTexture");

	m_nVertexBufferObj = utils::CreateBufferObject(GL_ARRAY_BUFFER, sizeof(CParticleVertex) * m_nPointCount, m_pPoints);

	SetVertexAttrib(m_pShader, CDrawable::AttribType::VERTEXATTRIB);

	for (size_t i = 0; i < 2; ++i)
	{
		m_nUpdateTfo[i] = utils::CreateTransformFeedBack(m_nUpdateTfoBuffer[i], sizeof(CParticleVertex) * m_nPointCount * 10000, nullptr);
	}

	InitTexture(nullptr);

	EmitParticle();

}

void CParticleEmit::SetVertexAttrib(CShader *pShader, AttribType type)
{
	if (type == CDrawable::AttribType::VERTEXATTRIB)
	{
		if (pShader)
		{
			const char *tmpName[] = { "pos", "life"};
			pShader->SetShaderAttrib(2, tmpName);
		}
	}
	else if (type == CDrawable::AttribType::ATTRIBPOINTER)
	{
		if (pShader)
		{
			size_t count[] = { 4, 4};
			GLuint offset[] = { 0, sizeof(float) * 4};
			pShader->SetAttribPointer(count, GL_FLOAT, GL_FALSE, sizeof(CParticleVertex), offset);
		}
	}
}