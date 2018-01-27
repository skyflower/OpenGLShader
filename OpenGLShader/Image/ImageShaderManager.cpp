#include "ImageShaderManager.h"


CImageShaderManager* CImageShaderManager::m_pInstance = nullptr;

CImageShaderManager * CImageShaderManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CImageShaderManager();
	}
	return m_pInstance;
}

CShader * CImageShaderManager::GetShaderByName(char * name)
{
	if (name == nullptr)
	{
		return nullptr;
	}
	if ((m_pShader != nullptr) && (!m_pShader->empty()))
	{
		size_t nSize = m_pShader->size();
		for (auto iter = m_pShader->begin(); iter != m_pShader->end(); ++iter)
		{
			if (strcmp(name, iter->first.c_str()) == 0)
			{
				return iter->second;
			}
		}
	}
	return nullptr;
}

CImageShaderManager::~CImageShaderManager()
{

}

void CImageShaderManager::init()
{

}

CImageShaderManager::CImageShaderManager()
{
	m_pShader = new std::unordered_map<std::string, CShader*>;
	init();
}