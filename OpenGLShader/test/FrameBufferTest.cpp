#include "FrameBufferTest.h"
#include "../common/Log.h"
#include <iostream>


CFrameBufferTest::CFrameBufferTest(size_t width, size_t height) :
	m_pFirstShader(nullptr),
	m_pSecondShader(nullptr)
{
	for (size_t i = 0; i < 2; ++i)
	{
		m_nFrameBuffer[i].AttachColorBuffer("color", GL_COLOR_ATTACHMENT0, GL_RGBA, width, height);
		m_nFrameBuffer[i].AttachDepthBuffer("depth", width, height);
		m_nFrameBuffer[i].Finish();
	}
}

CFrameBufferTest::~CFrameBufferTest()
{
	
	
}

void CFrameBufferTest::SetTexture(GLuint textureID)
{
	m_nTextureID = textureID;
}


void CFrameBufferTest::Draw(int nCount)
{
	if ((m_pFirstShader == nullptr) || (m_pSecondShader == nullptr))
	{
		return;
	}
	m_nFrameBuffer[0].Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_pFirstShader->GetProgram());
	glBindTexture(GL_TEXTURE_2D, m_nTextureID);
	glUniform1i(m_pFirstShader->GetLocation(FS_SAMPLER2D), 0);
	m_pFullScreen.initTexture(m_nTextureID);
	m_pFullScreen.SetShader(m_pFirstShader);
	m_pFullScreen.Draw();
	m_nFrameBuffer[0].UnBind();

	m_nFrameBuffer[1].Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_pSecondShader->GetProgram());
	glBindTexture(GL_TEXTURE_2D, m_nFrameBuffer[0].GetColorBuffer("color"));
	glUniform1i(m_pSecondShader->GetLocation(FS_SAMPLER2D), 0);
	m_pFullScreen.initTexture(m_nFrameBuffer[0].GetColorBuffer("color"));
	m_pFullScreen.SetShader(m_pSecondShader);
	m_pFullScreen.Draw();
	m_nFrameBuffer[1].UnBind();
	for (size_t i = 1; i < nCount; ++i)
	{
		m_nFrameBuffer[0].Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(m_pFirstShader->GetProgram());
		glBindTexture(GL_TEXTURE_2D, m_nFrameBuffer[1].GetColorBuffer("color"));
		glUniform1i(m_pFirstShader->GetLocation(FS_SAMPLER2D), 0);
		m_pFullScreen.initTexture(m_nFrameBuffer[1].GetColorBuffer("color"));
		m_pFullScreen.SetShader(m_pFirstShader);
		m_pFullScreen.Draw();
		m_nFrameBuffer[0].UnBind();

		m_nFrameBuffer[1].Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(m_pSecondShader->GetProgram());
		glBindTexture(GL_TEXTURE_2D, m_nFrameBuffer[0].GetColorBuffer("color"));
		glUniform1i(m_pSecondShader->GetLocation(FS_SAMPLER2D), 0);
		m_pFullScreen.initTexture(m_nFrameBuffer[0].GetColorBuffer("color"));
		m_pFullScreen.SetShader(m_pSecondShader);
		m_pFullScreen.Draw();
		m_nFrameBuffer[1].UnBind();
	}
}

void CFrameBufferTest::SetShader(CShader * pShader, size_t index)
{
	if (index == 0)
	{
		m_pFirstShader = pShader;
	}
	else if (index == 1)
	{
		m_pSecondShader = pShader;
	}
}

GLuint CFrameBufferTest::GetColorBuffer()
{
	return m_nFrameBuffer[1].GetColorBuffer("color");
}

GLuint CFrameBufferTest::GetDepthBuffer()
{
	return m_nFrameBuffer[1].GetDepthBuffer();
}
