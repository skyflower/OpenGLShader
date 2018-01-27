#pragma once

#include "../common/shader.h"
#include "../common/common.h"
#include "../common/UnCopyable.h"

#include <iostream>
#include <unordered_map>

class CImageShaderManager : public UnCopyable
{
	static CImageShaderManager* GetInstance();
	CShader* GetShaderByName(char *name);
	~CImageShaderManager();
private:
	CImageShaderManager();
	void init();
	CImageShaderManager(const CImageShaderManager&) = delete;
	CImageShaderManager& operator=(const CImageShaderManager&) = delete;

	static CImageShaderManager* m_pInstance;
	std::unordered_map<std::string, CShader*> *m_pShader;
};