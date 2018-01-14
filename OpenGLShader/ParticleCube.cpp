#include "ParticleCube.h"




CParticleCube::CParticleCube():C3DModel()
{
	m_pShader = nullptr;
	m_pVel = nullptr;
	
	m_nSSBOProgram = -1;
	
	m_pIndex = nullptr;
	m_nIndexCount = 0;
	m_pPoints = nullptr;

	m_nPointCount = 200000;

	m_nVelProgram = -1;
	m_nTextureID = -1;
	m_nCSShader = -1;
	
	m_nTextureID = -1;
	m_nCSShader = -1;
	m_nTexLocation = -1;

	Init();
}


CParticleCube::~CParticleCube()
{
	if (glIsTexture(m_nTextureID))
	{
		glDeleteTextures(1, &m_nTextureID);
		m_nTextureID = -1;
	}

	if (glIsShader(m_nCSShader))
	{
		glDeleteShader(m_nCSShader);
		m_nCSShader = -1;
	}
	if (m_pShader)
	{
		delete m_pShader;
		m_pShader = nullptr;
	}
	if (m_pVel)
	{
		delete[] m_pVel;
		m_pVel = nullptr;
	}
	if (m_pPoints)
	{
		delete[] m_pPoints;
		m_pPoints = nullptr;
	}
	if (m_pIndex)
	{
		delete[] m_pIndex;
		m_pIndex = nullptr;
	}
	
	if (glIsBuffer(m_nSSBOProgram))
	{
		glDeleteBuffers(1, &m_nSSBOProgram);
		m_nSSBOProgram = -1;
	}
	if (glIsBuffer(m_nVelProgram))
	{
		glDeleteBuffers(1, &m_nVelProgram);
		m_nVelProgram = -1;
	}
}

void CParticleCube::InitTexture(const char * filepath)
{
	//m_pTexture = CTexture::LoadTexture(filepath);
}

void CParticleCube::Draw()
{
	glUseProgram(m_nCSShader);
	glBindTexture(GL_TEXTURE_3D, m_nTextureID);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_nSSBOProgram);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_nVelProgram);
	glDispatchCompute(m_nPointCount / 128, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
	glBindTexture(GL_TEXTURE_3D, 0);
	glUseProgram(0);

	glUseProgram(m_pShader->GetProgram());
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// GL_ONE_MINUS_SRC_ALPHA);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_nSSBOProgram);
	
	SetTransformMatrix();
	
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIndexBufferObj);
	//glDrawElements(GL_QUADS, m_nIndexCount, GL_UNSIGNED_INT, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDrawArrays(GL_QUADS, 0, m_nIndexCount);
	
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	
	glUseProgram(0);
	glDisable(GL_BLEND);
}

void CParticleCube::Update(float duration)
{
	static float speed = 0.001;
	double zTranslate = duration * speed;
	static bool initFlag = true;
	if (initFlag)
	{
		//Set(4.41828, 3.3137, 1);
		initFlag = false;
	}
	if (zTranslate > 2 * 3.1415026)
	{
		zTranslate = 0;
	}
	SetRotate(30 * 3.1415926 / 180, 30 * 3.1415926 / 180, 0);
}

GLuint CParticleCube::GetShaderProgram()
{
	if (m_pShader)
	{
		return m_pShader->GetProgram();
	}
	return -1;
}


void CParticleCube::ResetState()
{
}

void CParticleCube::Init()
{
	m_nIndexCount = 4 * m_nPointCount;
	m_pIndex = new GLuint[m_nIndexCount];
	memset(m_pIndex, 0, sizeof(GLuint) * m_nIndexCount);

	GLuint *tmpPointer = m_pIndex;

	//Log << "sizeof(GLuint) == sizeof(unsigned int) " << sizeof(GLuint) << "\t" << sizeof(unsigned int) << "\n";

	m_pVel = new CVertex_SSBO_Two[m_nPointCount];
	memset(m_pVel, 0, sizeof(CVertex_SSBO_Two) * m_nPointCount);

	m_pPoints = new CVertex_SSBO_Two[m_nPointCount];
	memset(m_pPoints, 0, sizeof(CVertex_SSBO_Two) * m_nPointCount);

	GLfloat tmpScale = 0.5;

	for (size_t i = 0; i < m_nPointCount; i++)
	{
		m_pVel[i].m_fPos[0] = 0;
		m_pVel[i].m_fPos[1] = 0;
		m_pVel[i].m_fPos[2] = 0;
		m_pVel[i].m_fPos[3] = 0;

		m_pPoints[i].m_fPos[0] = tmpScale * utils::srandf();
		m_pPoints[i].m_fPos[1] = tmpScale * utils::srandf();
		m_pPoints[i].m_fPos[2] = tmpScale * utils::srandf();
		m_pPoints[i].m_fPos[3] = 1.0;

		GLuint index = (i << 2);
		*tmpPointer++ = index;
		*tmpPointer++ = index + 1;
		*tmpPointer++ = index + 2;
		*tmpPointer++ = index + 3;
	}

	m_pShader = new CShader("./shader/ParticleCube.vs", "./shader/ParticleCube.fs");

	m_nSSBOProgram = utils::CreateBufferObject(GL_SHADER_STORAGE_BUFFER, sizeof(CVertex_SSBO_Two) * m_nPointCount, m_pPoints);
	
	m_nVelProgram = utils::CreateBufferObject(GL_SHADER_STORAGE_BUFFER, sizeof(CVertex_SSBO_Two) * m_nPointCount, m_pVel);

	m_nTextureID = utils::CreateTexture3D(16, 16, 16);

	m_nCSShader = utils::CreateComputeProgram("./shader/updateParticle.shader");

	m_nTexLocation = glGetUniformLocation(m_nCSShader, "U_MainTexture");

	SetShaderProgram(m_pShader->GetProgram());
	
}

void CParticleCube::SetVertexAttrib(AttribType type)
{
	if (type == CDrawable::AttribType::VERTEXATTRIB)
	{
		if (m_pShader)
		{
			const char *tmpName[] = { "PosCount"};
			m_pShader->SetShaderAttrib(1, tmpName);
		}
	}
	else if (type == CDrawable::AttribType::ATTRIBPOINTER)
	{
		if (m_pShader)
		{
			size_t count[] = {1};
			GLuint offset[] = { 0 };
			m_pShader->SetAttribPointer(count, GL_FLOAT, GL_FALSE, 0, offset);
		}
	}
}


void CParticleCube::TestShader(mat4f& model)
{
	static bool bFlag = true;
	if (bFlag == false)
	{
		return;
	}
	//Log << "CParticleCube EndMatrix:\n" << model << "\n";

	for (size_t i = 0; i < m_nIndexCount; ++i)
	{
		int spriteID = (m_pIndex[i] >> 2);
		vec4f spritePos;
		for (size_t j = 0; j < 4; ++j)
		{
			spritePos[j] = m_pPoints[spriteID].m_fPos[j];
		}
		vec4f spritePosInView = spritePos;
		//Log << m_pIndex[i] << "\tpritePosInView\t" << spritePosInView << "\n";
		vec2i texcoord;
		texcoord[0] = ((m_pIndex[i] - 1) & 2) >> 1;
		texcoord[1] = (m_pIndex[i] & 2) >> 1;
		float spriteSize = 1.4;
		vec4f tmpPos;
		tmpPos[0] = (texcoord[0] * 2.0 - 1.0) * spriteSize;
		tmpPos[1] = (texcoord[1] * 2.0 - 1.0) * spriteSize;
		tmpPos[2] = 0;
		tmpPos[3] = 1.0;

		//Log << m_pIndex[i] << "\ttmpPos\t" << tmpPos << "\n";

		vec4f fixPos = spritePosInView +2 * tmpPos;
		//Log << m_pIndex[i] << "\tfixPos\t" << fixPos << "\n";
		fixPos = fixPos * model;
		//Log << m_pIndex[i] << "\tfixPos\t" << fixPos << "\n";
		//Log << "========================================\n";
	}
	bFlag = false;
}