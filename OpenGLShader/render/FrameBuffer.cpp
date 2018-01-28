#include "FrameBuffer.h"



CFrameBuffer::CFrameBuffer()
{
	glGenFramebuffers(1, &m_nFbo);
	m_nDepthBuffer = -1;
}


CFrameBuffer::~CFrameBuffer()
{
	if (m_pBuffer.size() != 0)
	{
		for (auto iter : m_pBuffer)
		{
			if (glIsBuffer(iter.second))
			{
				glDeleteBuffers(1, &iter.second);
				iter.second = 0;
			}
		}
	}
	if (glIsFramebuffer(m_nFbo))
	{
		glDeleteFramebuffers(1, &m_nFbo);
		m_nFbo = -1;
	}
}

void CFrameBuffer::Finish()
{
	int nCount = m_pDrawBuffer.size();
	if (nCount > 0)
	{
		GLenum *buffer = new GLenum[nCount];
		int i = 0;
		while (i < nCount)
		{
			buffer[i] = m_pDrawBuffer[i];
			i++;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, m_nFbo);

		glDrawBuffers(nCount, buffer);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		delete[]buffer;
		buffer = nullptr;
	}
}

void CFrameBuffer::AttachColorBuffer(const char * bufferName, GLenum attachment, GLenum dataType, int width, int height)
{
	GLuint colorBuffer;
	glBindFramebuffer(GL_FRAMEBUFFER, m_nFbo);

	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if ((dataType == GL_RGB) || (dataType == GL_RGBA))
	{
		glTexImage2D(GL_TEXTURE_2D, 0, dataType, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	}
	else if (dataType == GL_RGBA16F)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, dataType, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT, nullptr);
	}
	else if (dataType == GL_RGBA32F)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, dataType, width, height, 0, GL_RGBA, GL_UNSIGNED_INT, nullptr);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, colorBuffer, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_pDrawBuffer.push_back(attachment);
	m_pBuffer.insert(std::make_pair(bufferName, colorBuffer));
}

void CFrameBuffer::AttachDepthBuffer(const char * bufferName, int width, int height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_nFbo);
	//GLuint depthBuffer;
	glGenTextures(1, &m_nDepthBuffer);
	glBindTexture(GL_TEXTURE_2D, m_nDepthBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_nDepthBuffer, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//m_pDrawBuffer.push_back(GL_DEPTH_ATTACHMENT);
	m_pBuffer.insert(std::make_pair(bufferName, m_nDepthBuffer));
}

GLuint CFrameBuffer::GetColorBuffer(const char * name)
{
	if (m_pBuffer.size() > 0)
	{
		//size_t nSize = m_pBuffer.size();
		std::string tmpStr(name);
		auto k = m_pBuffer.find(tmpStr);
		if (k != m_pBuffer.end())
		{
			return k->second;
		}
	}
	return 0;
}

void CFrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_nFbo);
}

void CFrameBuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
