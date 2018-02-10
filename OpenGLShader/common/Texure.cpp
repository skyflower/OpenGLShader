#include "Texure.h"
#include "SOIL/SOIL.h"


std::unordered_map<std::string, std::pair<unsigned int, CTexture*>> *CTexture::m_pCacheTexture = nullptr;

CTexture::CTexture()
{
	m_TextureID = -1;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nTexType = -1;
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
		glBindTexture(m_nTexType, m_TextureID);
	}
	else
	{
		glBindTexture(m_nTexType, 0);
	}
}

void CTexture::UnBind()
{
	glBindTexture(m_nTexType, 0);
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
	pTexture->m_nTexType = GL_TEXTURE_2D;
	if (pTexture->InitByFilePath(filepath))
	{
		return pTexture;
	}
	return nullptr;
}

CTexture * CTexture::LoadCubeMapTexture(const char * PosX, const char * NegX, const char * PosY, const char * NegY, const char * PosZ, const char * NegZ)
{
	CTexture* pTexture = new CTexture();
	pTexture->m_nTexType = GL_TEXTURE_CUBE_MAP;
	glGenTextures(1, &pTexture->m_TextureID);

	glBindTexture(GL_TEXTURE_CUBE_MAP, pTexture->m_TextureID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);

	const char *tmpDir[] = { PosX, NegX, PosY, NegY, PosZ, NegZ };
	GLenum tmpTexture[] = { GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };
	std::string tmpName;
	for (size_t i = 0; i < 6; ++i)
	{
		tmpName = tmpName + tmpDir[i];
		int width = 0;
		int height = 0;
		int iChannel = 0;
		unsigned char *content = SOIL_load_image(tmpDir[i], &width, &height, &iChannel, 4);
		if (content == nullptr)
		{
			continue;
		}
		if (iChannel == 3)
		{
			glTexImage2D(tmpTexture[i], 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, content);
		}
		else if (iChannel == 4)
		{
			glTexImage2D(tmpTexture[i], 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, content);
		}
		if (pTexture->m_nWidth == 0)
		{
			pTexture->m_nWidth = width;
		}
		if (pTexture->m_nHeight == 0)
		{
			pTexture->m_nHeight = height;
		}
		SOIL_free_image_data(content);
		content = nullptr;
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	if (m_pCacheTexture == nullptr)
	{
		m_pCacheTexture = new std::unordered_map<std::string, std::pair<unsigned int, CTexture*>>;
	}
	pTexture->m_strPath = tmpName;
	m_pCacheTexture->insert(std::make_pair(tmpName, std::make_pair(1, pTexture)));

	return pTexture;
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
	pTexture->m_nTexType = GL_TEXTURE_2D;
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
