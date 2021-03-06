#include "FullScreen.h"

CFullScreen::CFullScreen()
{
	float points[] = {
	-0.5, -0.5, -1,
	0.5, -0.5, -1.0,
	0.5, 0.5,	-1.0,
	-0.5, 0.5, -1.0
	};
	
	m_fParam[0] = m_fParam[1] = 0;
	m_fParam[2] = 2.0;
	m_fParam[3] = 0;
	m_nVertexNum = 4;
	m_pVertex = new CVertexData[m_nVertexNum];
	memset(m_pVertex, 0, sizeof(CVertexData) * m_nVertexNum);
	for (size_t i = 0; i < m_nVertexNum; ++i)
	{
		memcpy(m_pVertex[i].m_fPos, &points[3 * i], 3 * sizeof(GLfloat));
	}
	m_nVertexBuffObj = utils::CreateBufferObject(GL_ARRAY_BUFFER, m_nVertexNum * sizeof(CVertexData), m_pVertex);
	
	m_nAuxTexID = -1;
	m_nTexLocation = -1;
	m_nAuxTexLocation = -1;
	m_nTextureID = -1;
	//m_nParamLocation = glGetUniformLocation(m_pShader->GetProgram(), "param");

	//SetShaderProgram(m_pShader->GetProgram());
	//SetVertexAttrib(CDrawable::AttribType::VERTEXATTRIB);
}

CFullScreen::~CFullScreen()
{
	if (m_pVertex != nullptr)
	{
		delete[]m_pVertex;
		m_pVertex = nullptr;
	}
	if (glIsBuffer(m_nVertexBuffObj))
	{
		glDeleteBuffers(1, &m_nVertexBuffObj);
		m_nVertexBuffObj = -1;
	}
	if (m_pDefaultShader != nullptr)
	{
		delete m_pDefaultShader;
		m_pDefaultShader = nullptr;
	}
}

void CFullScreen::initTexture(GLuint textureID)
{
	if (glIsTexture(textureID))
	{
		m_nTextureID = textureID;
		if (m_pShader)
		{
			m_nTexLocation = glGetUniformLocation(m_pShader->GetProgram(), "texSampler");
		}
	}
	else
	{
		m_nTexLocation = m_nTextureID = -1;
	}
}

void CFullScreen::SetAuxTexture(GLuint textureID)
{
	if (glIsTexture(textureID))
	{
		m_nAuxTexID = textureID;
		if (m_pShader)
		{
			m_nAuxTexLocation = glGetUniformLocation(m_pShader->GetProgram(), "texSamplerAux");
		}
	}
	else
	{
		m_nAuxTexID = m_nAuxTexLocation = -1;
	}
}

void CFullScreen::Draw()
{
	//glEnable(GL_DEPTH_TEST);
	//glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

	if (m_pShader != nullptr)
	{
		glUseProgram(m_pShader->GetProgram());
	}
	if (glIsTexture(m_nTextureID))
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_nTextureID);
		glUniform1i(m_nTexLocation, 0);
	}
	if (glIsTexture(m_nAuxTexID))
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_nAuxTexID);
		glUniform1i(m_nAuxTexLocation, 1);
	}
	
	SetTransformMatrix();
	
	glUniform4fv(m_nParamLocation, 1, &m_fParam[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBuffObj);
	SetVertexAttrib(CDrawable::AttribType::ATTRIBPOINTER);

	//glEnableVertexAttribArray(m_nPosLocation);
	//glVertexAttribPointer(m_nPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(CVertexData), 0);
	glDrawArrays(GL_QUADS, 0, m_nVertexNum);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(m_nPosLocation);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void CFullScreen::Update(float Duration)
{
}

void CFullScreen::DrawLeftTop()
{
	vec4f tmpParam = m_fParam;
	m_fParam[0] = -0.5;
	m_fParam[1] = 0.5;
	m_fParam[2] = 1;
	m_fParam[3] = 0.0;
	Draw();
	m_fParam = tmpParam;
}

void CFullScreen::DrawLeftBottom()
{
	vec4f tmpParam = m_fParam;
	m_fParam[0] = -0.5;
	m_fParam[1] = -0.5;
	m_fParam[2] = 1;
	m_fParam[3] = 0.0;
	Draw();
	m_fParam = tmpParam;
}

void CFullScreen::DrawRightTop()
{
	vec4f tmpParam = m_fParam;
	m_fParam[0] = 0.5;
	m_fParam[1] = 0.5;
	m_fParam[2] = 1;
	m_fParam[3] = 0.0;
	Draw();
	m_fParam = tmpParam;
}

void CFullScreen::DrawRightBottom()
{
	vec4f tmpParam = m_fParam;
	m_fParam[0] = 0.5;
	m_fParam[1] = -0.5;
	m_fParam[2] = 1;
	m_fParam[3] = 0.0;
	Draw();
	m_fParam = tmpParam;
}

void CFullScreen::SetPosLocation(GLuint PosLoc)
{
	m_nPosLocation = PosLoc;
}

void CFullScreen::SetShader(CShader * pShader)
{
	m_pShader = pShader;
	m_nParamLocation = glGetUniformLocation(m_pShader->GetProgram(), "param");
	m_nTexLocation = glGetUniformLocation(m_pShader->GetProgram(), "texSampler");

	m_nAuxTexLocation = glGetUniformLocation(m_pShader->GetProgram(), "texSamplerAux");
	
	SetShaderProgram(m_pShader->GetProgram());
	SetVertexAttrib(CDrawable::AttribType::VERTEXATTRIB);
}

void CFullScreen::SetScaleAndTranslate(float scaleFactor, float translateX, float translateY)
{
	m_fParam[0] = translateX;
	m_fParam[1] = translateY;
	m_fParam[2] = scaleFactor;
	m_fParam[3] = 0.0;
}


void CFullScreen::SetVertexAttrib(AttribType type)
{
	if (type == AttribType::VERTEXATTRIB)
	{
		if (m_pShader)
		{
			const char *tmpName[] = { "pos"};
			m_pShader->SetShaderAttrib(1, tmpName);
		}
	}
	else if (type == AttribType::ATTRIBPOINTER)
	{
		if (m_pShader)
		{
			size_t count[] = { 3};
			GLuint offset[] = { 0};
			m_pShader->SetAttribPointer(count, GL_FLOAT, GL_FALSE, sizeof(CVertexData), offset);
		}
	}
}