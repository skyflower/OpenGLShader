#include "Texure.h"
#include "SOIL/SOIL.h"


std::unordered_map<std::string, std::pair<unsigned int, CTexture*>> *CTexture::m_pCacheTexture = nullptr;

CTexture::CTexture()
{
	m_TextureID = -1;
}


CTexture::~CTexture()
{
	if ((m_TextureID != -1) && (glIsTexture(m_TextureID)))
	{
		glDeleteTextures(1, &m_TextureID);
		m_TextureID = -1;
	}
}



GLuint CTexture::InitTextureByData(const unsigned char * pData, size_t width, size_t height, GLenum colorType)
{
	GLuint textureID = -1;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	if (colorType == GL_RGB)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pData);
	}
	else if (colorType == GL_RGBA)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData);

	}
	else if (colorType == GL_ALPHA)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pData);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}

void CTexture::Bind(bool flag)
{
	if ((m_TextureID != -1) && glIsTexture(m_TextureID) && (flag == true))
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

//void CTexture::SetShaderProgram(GLuint program, const char * name)
//{
//	m_nShaderProgram = program;
//	m_nTexLocation = glGetAttribLocation(m_nShaderProgram, name);
//	if (m_nTexLocation == -1)
//	{
//		Log << __FUNCTION__ << "  " << __LINE__ << " glGetError: " << glGetError() << "\n";
//	}
//}

//void CTexture::SetTexPointer(size_t count, GLenum type, GLboolean flag, GLsizei stride, GLvoid* offset)
//{
//	glEnableVertexAttribArray(m_nTexLocation);
//	glVertexAttribPointer(m_nTexLocation, count, type, flag, stride, offset);
//}

CTexture * CTexture::LoadTexture(const char * filepath)
{
	std::string tmp(filepath);
	if (m_pCacheTexture && !m_pCacheTexture->empty())
	{
		std::unordered_map<std::string, std::pair<unsigned int, CTexture*>>::iterator iter = m_pCacheTexture->begin();
		if ((iter = m_pCacheTexture->find(tmp)) != m_pCacheTexture->end())
		{
			iter->second.first += 1;
			return iter->second.second;
		}
	}

	CTexture *pTexture = new CTexture;
	if (pTexture->InitByFilePath(filepath))
	{
		return pTexture;
	}
	return nullptr;
}

void CTexture::UnLoadTexture(const CTexture * pTexture)
{
	if (pTexture == nullptr)
	{
		return;
	}
	if (m_pCacheTexture && !m_pCacheTexture->empty())
	{
		std::unordered_map<std::string, std::pair<unsigned int, CTexture*>>::iterator iter = m_pCacheTexture->begin();
		if ((iter = m_pCacheTexture->find(pTexture->m_strPath)) != m_pCacheTexture->end())
		{
			//glDeleteTextures(1, &pTexture->m_TextureID);
			iter->second.first -= 1;
			if (iter->second.first == 0)
			{
				delete pTexture;
				pTexture = nullptr;
				m_pCacheTexture->erase(iter);
			}
			
		}
		if (m_pCacheTexture->empty())
		{
			delete m_pCacheTexture;
			m_pCacheTexture = nullptr;
		}
	}
}

CTexture* CTexture::CreateTextureFromData(const unsigned char * pData, size_t width, size_t height, GLenum colorType)
{
	char tmpChar[20] = { 0 };
	sprintf_s(tmpChar, "%uX%u_%u", width, height, colorType);
	std::string tmp(tmpChar);
	if (m_pCacheTexture && !m_pCacheTexture->empty())
	{
		std::unordered_map<std::string, std::pair<unsigned int, CTexture*>>::iterator iter = m_pCacheTexture->begin();
		if ((iter = m_pCacheTexture->find(tmp)) != m_pCacheTexture->end())
		{
			iter->second.first += 1;
			return iter->second.second;
		}
	}
	CTexture *pTexture = new CTexture();
	pTexture->m_TextureID = pTexture->InitTextureByData(pData, width, height, colorType);
	pTexture->m_nWidth = width;
	pTexture->m_nHeight = height;
	pTexture->m_strPath = tmp;

	if (m_pCacheTexture == nullptr)
	{
		m_pCacheTexture = new std::unordered_map<std::string, std::pair<unsigned int, CTexture*>>;
	}
	m_pCacheTexture->insert(std::make_pair(tmp, std::make_pair(1, pTexture)));
	
	return pTexture;
}

bool CTexture::InitByFilePath(const char * filepath)
{
	if (filepath == nullptr)
	{
		return false;
	}
	if (m_pCacheTexture == nullptr)
	{
		m_pCacheTexture = new std::unordered_map<std::string, std::pair<unsigned int, CTexture*>>;
	}
	std::string tmp(filepath);
	
	//CTexture *pTexture = new CTexture;
	int iBits;
	unsigned char *data = SOIL_load_image(filepath, &m_nWidth, &m_nHeight, &iBits, 4);
	if (data == NULL)
	{
		return false;
	}
	m_TextureID = InitTextureByData(data, m_nWidth, m_nHeight, GL_RGBA);

	SOIL_free_image_data(data);
	data = nullptr;

	m_strPath = tmp;
	m_pCacheTexture->insert(std::make_pair(tmp, std::make_pair(1, this)));
	return true;
}
