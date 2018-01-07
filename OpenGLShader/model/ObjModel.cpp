#include "ObjModel.h"
#include <cstring>
#include <sstream>
#include "../common/utils.h"

#include <vector>

extern std::fstream Log;


CObjModel::CObjModel() :C3DModel()
{
	m_pVertex = nullptr;
	
	m_nVertexBufferObj = -1;
	
	m_nVertexArrayID = -1;
	m_nIndexBufferObj = -1;

	m_pTexture = nullptr;
	m_pShader = nullptr;
}


CObjModel::~CObjModel()
{
	
	if (m_pVertex != nullptr)
	{
		delete m_pVertex;
		m_pVertex = nullptr;
	}
	
	if (m_pShader)
	{
		delete m_pShader;
		m_pShader = nullptr;
	}
	if (glIsBuffer(m_nVertexBufferObj))
	{
		glDeleteBuffers(1, &m_nVertexBufferObj);
		m_nVertexBufferObj = -1;
	}
	if (m_pTexture != nullptr)
	{
		CTexture::UnLoadTexture(m_pTexture);
		m_pTexture = nullptr;
	}
}

void CObjModel::Init(const char* filepath)
{
	size_t wLength = 0;
	char *content = utils::LoadFileContent(filepath, wLength);
	if (content == nullptr)
	{
		return;
	}
	InitByFileContent(content, wLength);
	return;

	std::vector<vec3i> *tmpIndexNode = new std::vector<vec3i>;
	tmpIndexNode->reserve(2000);
	
	std::stringstream ssContent(content);
	char szLine[1024];
	std::string tmp;
	if (m_pVertex == nullptr)
	{
		m_pVertex = new std::vector<CVertexData>;
		m_pVertex->reserve(2000);
	}
	if (m_pIndex == nullptr)
	{
		m_pIndex = new std::vector<unsigned int>;
		m_pIndex->reserve(2000);
	}
	
	size_t vtPos = 0;
	size_t vnPos = 0;
	size_t vpPos = 0;
	size_t tmpLineCount = 0;
	while (!ssContent.eof())
	{
		Log << "LineNumber = " << tmpLineCount << "\n";
		++tmpLineCount;
		memset(szLine, 0, sizeof(char) * 1024);
		ssContent.getline(szLine, 1024);
		if (strlen(szLine) > 0)
		{
			std::stringstream ssline(szLine);
			if (szLine[0] == 'v')
			{
				
				ssline >> tmp;
				CVertexData info;
				size_t tmpLength = m_pVertex->size();
				if (szLine[1] == 't')
				{
					ssline >> info.m_fTex[0] >> info.m_fTex[1];// >> info.m_fTex[2];
					vtPos += 1;
					if (vtPos <= tmpLength)
					{
						CVertexData &tmpPointer = *(m_pVertex->data() + vtPos - 1);
						memcpy(tmpPointer.m_fTex, info.m_fTex, sizeof(float) * 2);
					}
				}
				else if (szLine[1] == 'n')
				{
					ssline >> info.m_fNormal[0] >> info.m_fNormal[1] >> info.m_fNormal[2];
					vnPos += 1;
					if (vnPos <= tmpLength)
					{
						CVertexData &tmpPointer = *(m_pVertex->data() + vnPos - 1);
						memcpy(tmpPointer.m_fNormal, info.m_fNormal, sizeof(float) * 3);
					}
				}
				else // position
				{
					ssline >> info.m_fPos[0] >> info.m_fPos[1] >> info.m_fPos[2];
					//Log << info.m_fPos[0] << "\t" << info.m_fPos[1] << "\t" << info.m_fPos[2] << "\n";
					vpPos += 1;
					if (vpPos <= tmpLength)
					{
						CVertexData &tmpPointer = *(m_pVertex->data() + vpPos - 1);
						memcpy(tmpPointer.m_fPos, info.m_fPos, sizeof(float) * 3);
					}
				}
				
				if ((vtPos > tmpLength) || (vpPos > tmpLength) || (vnPos > tmpLength))
				{
					m_pVertex->push_back(info);
				}
			}
			else if (szLine[0] == 'f')
			{
				ssline >> tmp;
				std::string vertexIndex;
				vec3i curIndex;
				for (size_t i = 0; i < 3; ++i)
				{
					ssline >> vertexIndex;
					size_t tmpIndex = vertexIndex.find_first_of('/');
					if (tmpIndex == -1)
					{
						//Log << "i = " << i << "\tPos\n";
						continue;
					}
					std::string posIndexStr(vertexIndex.c_str(), tmpIndex);
					size_t tmpIndexTwo = vertexIndex.find('/', tmpIndex + 1);
					if (tmpIndexTwo == -1)
					{
						//Log << "i = " << i << "\tTexCoord\n";
						continue;
					}
					std::string texIndexStr(&vertexIndex[tmpIndex + 1], tmpIndexTwo - tmpIndex - 1);
					std::string norIndexStr(&vertexIndex[tmpIndexTwo + 1]);
					curIndex[0] = atoi(posIndexStr.c_str()) - 1;
					curIndex[1] = atoi(texIndexStr.c_str()) - 1;
					curIndex[2] = atoi(norIndexStr.c_str()) - 1;
					/*if ((curIndex[0] != curIndex[2]) && (curIndex[1] != curIndex[0]))
					{
						Log << "Index\t" << curIndex << "\n";
					}*/
					size_t tmpIndexIndex = -1;
					size_t tmpIndexSize = tmpIndexNode->size();
					for (size_t j = 0; j < tmpIndexSize; ++j)
					{
						vec3i *tmpRef = tmpIndexNode->data() + j;
						if (*tmpRef == curIndex)
						{
							tmpIndexIndex = j;
							break;
						}
					}
					if (tmpIndexIndex == -1)
					{
						tmpIndexNode->push_back(curIndex);
						tmpIndexIndex = tmpIndexSize;
						//m_pIndex->push_back(tmpIndexNode->size() - 1);
					}
					m_pIndex->push_back(tmpIndexIndex);

				}
				//Log << "\n";
			}
		}
	}
	size_t tmpSize = tmpIndexNode->size();
	std::vector<CVertexData> *tmpVertexIndex = new std::vector<CVertexData>(tmpSize);
	for (size_t i = 0; i < tmpSize; ++i)
	{
		CVertexData *pVertexData = tmpVertexIndex->data() + i;
		vec3i *pIndex = tmpIndexNode->data() + i;
		CVertexData *pSrcVertex = m_pVertex->data();
		memcpy(pVertexData->m_fNormal, (pSrcVertex + (*pIndex)[2])->m_fNormal, sizeof(float) * 3);
		memcpy(pVertexData->m_fTex, (pSrcVertex + (*pIndex)[1])->m_fTex, sizeof(float) * 2);
		memcpy(pVertexData->m_fPos, (pSrcVertex + (*pIndex)[0])->m_fPos, sizeof(float) * 3);
	}
	if (m_pVertex != nullptr)
	{
		m_pVertex->clear();
		delete m_pVertex;
		m_pVertex = tmpVertexIndex;
	}
	if (tmpIndexNode != nullptr)
	{
		tmpIndexNode->clear();
		delete tmpIndexNode;
		tmpIndexNode = nullptr;
	}
	InitDisplayBuffer();

}

void CObjModel::InitTexture(const char * pTextureFile)
{
	m_pTexture = CTexture::LoadTexture(pTextureFile);
	
	//SetVertexAttrib(VERTEXATTRIB);
}

void CObjModel::InitDisplayBuffer()
{
	
	size_t tmpStep = sizeof(CVertexData) / sizeof(float);
	
	m_nVertexBufferObj = utils::CreateBufferObject(GL_ARRAY_BUFFER, sizeof(CVertexData) * m_pVertex->size(), m_pVertex->data(), GL_STATIC_DRAW);
	m_nIndexBufferObj = utils::CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_pIndex->size(), m_pIndex->data());

	m_pShader = new CShader("./shader/shader.vs", "./shader/shader.fs");
	SetVertexAttrib(VERTEXATTRIB);
	
	InitVerexArrayBuffer();
	
	SetShaderProgram(m_pShader->GetProgram());

}

void CObjModel::Draw()
{
	if (m_pShader)
	{
		m_pShader->Bind();
	}
	if (m_pTexture)
	{
		m_pTexture->Bind();
	}

	SetTransformMatrix();

	//glUniform1i(m_nTexCoordLocation, 0);

	glBindVertexArray(m_nVertexArrayID);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_nIndexBufferObj);
	glDrawElements(GL_TRIANGLES, m_pIndex->size(), GL_UNSIGNED_INT, 0);
	ResetState();
}

void CObjModel::Update(float duration)
{
	static float speed = 0.001;
	double zTranslate = duration * speed;
	static bool initFlag = true;
	if (initFlag)
	{
		//SetTranslate(0, 0, -100);
		SetScale(0.1, 0.1, 0.1);
		initFlag = false;
	}
	AddRotate(0, zTranslate, 0);
	//vec3f rotate = GetRotate();
	//Log << "CObjModel zTranslate = " << zTranslate << "\t" << rotate[1] << "\n";
}

GLuint CObjModel::GetShaderProgram()
{
	if (m_pShader)
	{
		return m_pShader->GetProgram();
	}
	return -1;
}

void CObjModel::ResetState()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	if (m_pShader)
	{
		m_pShader->Bind(false);
	}
	if (m_pTexture)
	{
		m_pTexture->Bind(false);

	}
	glBindVertexArray(0);
	glUseProgram(0);
}

void CObjModel::TestShader(mat4f model)
{
	Log << "CObjModel TestShader Matrix\n" << model << "\n";
	//mat4f tmpModel = model.Transposition();
	size_t tmpSize = m_pVertex->size();
	/*for (size_t i = 0; i < tmpSize && i < 10; ++i)
	{
		CVertexData *p = m_pVertex->data() + i;
		vec3f tmp(p->m_fPos, 3);
		vec4f tmpTwo(tmp, 1.0);
		tmpTwo = tmpTwo * model;
		Log << tmpTwo << "\n";
	}*/
}

void CObjModel::InitVerexArrayBuffer()
{
	m_nVertexArrayID = utils::CreateVertexArrayObj([=]()->void
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_nVertexBufferObj);
		SetVertexAttrib(ATTRIBPOINTER);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	});
}

void CObjModel::InitByFileContent(char *content, size_t wLength)
{
	size_t nPointCount = 0;
	size_t nFaceCount = 0;
	size_t nTexCount = 0;
	size_t nNorCount = 0;
	//size_t wLength = 0;
	for (char *p = content; p < content + wLength - 1; )
	{
		//size_t nNextSpace = i;
		char *Pointer = strchr(p, '\n');
		if (Pointer == NULL)
		{
			break;
		}
		
		if ((p[0] == 'v') && (p[1] == ' '))
		{
			nPointCount += 1;
		}
		else if ((p[0] == 'f') && (p[1] == ' '))
		{
			nFaceCount += 1;
		}
		else if ((p[0] == 'v') && (p[1] == 't'))
		{
			nTexCount += 1;
		}
		else if ((p[0] == 'v') && (p[1] == 'n'))
		{
			nNorCount += 1;
		}
		p = Pointer + 1;
	}

	//std::vector<CVertexData> *m_pVertex;
	//std::vector<unsigned int> *m_pIndex;
	std::vector<vec3i> *tmpIndexNode = new std::vector<vec3i>(nFaceCount * 3);
	//tmpIndexNode->reserve(2000);
	size_t nMaxCount = nNorCount > nTexCount ? nNorCount : nTexCount;
	nMaxCount = nMaxCount > nPointCount ? nMaxCount : nPointCount;

	m_pVertex = new std::vector<CVertexData>(nMaxCount);
	m_pIndex = new std::vector<unsigned int>();
	m_pIndex->reserve(nMaxCount / 2);
	//size_t tmpVertexCount = 0;
	size_t tmpTexCount = 0;
	size_t tmpPosCount = 0;
	size_t tmpNorCount = 0;
	size_t tmpFaceCount = 0;
	int lineCutIndex[20];
	int faceCutIndex[10];
	char *Pointer = nullptr;
	for (char *p = content; p < content + wLength - 1; p = Pointer + 1)
	{
		//size_t nNextSpace = i;
		Pointer = strchr(p, '\n');
		if (Pointer == NULL)
		{
			break;
		}
		if ((p[0] == 'v') && (p[1] == ' '))
		{
			size_t tmpCount = 0;
			utils::CutLogBySpace(p + 2, Pointer, lineCutIndex, tmpCount, ' ');
			CVertexData *tmp = m_pVertex->data() + tmpPosCount;
			for (size_t i = 0; i < tmpCount && i < 3; ++i)
			{
				tmp->m_fPos[i] = atof(lineCutIndex[i] + p + 2);
			}
			tmpPosCount += 1;
		}
		else if ((p[0] == 'v') && (p[1] == 'n'))
		{
			size_t tmpCount = 0;
			utils::CutLogBySpace(p + 2, Pointer, lineCutIndex, tmpCount, ' ');
			CVertexData *tmp = m_pVertex->data() + tmpNorCount;
			for (size_t i = 0; i < tmpCount && i < 3; ++i)
			{
				float tmpFloat = atof(lineCutIndex[i] + p + 2);
				tmp->m_fNormal[i] = tmpFloat;
			}
			tmpNorCount += 1;
		}
		else if ((p[0] == 'v') && (p[1] == 't'))
		{
			size_t tmpCount = 0;
			utils::CutLogBySpace(p + 2, Pointer, lineCutIndex, tmpCount, ' ');
			CVertexData *tmp = m_pVertex->data() + tmpTexCount;
			for (size_t i = 0; i < tmpCount && i < 2; ++i)
			{
				tmp->m_fTex[i] = atof(lineCutIndex[i] + p + 2);
			}
			tmpTexCount += 1;
		}
		else if ((p[0] == 'f') && (p[1] == ' '))
		{
			size_t tmpCount = 0;
			size_t tmpCutFaceCount = 0;
			
			utils::CutLogBySpace(p + 2, Pointer, lineCutIndex, tmpCount, ' ');
			//vec3i &tmp = *(tmpIndexNode->data() + tmpFaceCount);
			for (size_t i = 0; i < tmpCount && i < 3; ++i)
			{
				//
				utils::CutLogBySpace(lineCutIndex[i] + p + 2, lineCutIndex[i + 1] + p + 2, faceCutIndex, tmpCutFaceCount, '/');
				if (tmpCutFaceCount != 3)
				{
					break;
				}
				//vec3i tmpVertex;
				vec3i tmp;
				tmp[0] = atoi(lineCutIndex[i] + p + 2 + faceCutIndex[0]) - 1;
				tmp[1] = atoi(lineCutIndex[i] + p + 2 + faceCutIndex[1]) - 1;
				tmp[2] = atoi(lineCutIndex[i] + p + 2 + faceCutIndex[2]) - 1;

				size_t tmpIndexIndex = -1;
				//size_t tmpIndexSize = tmpIndexNode->size();
				for (size_t j = 0; j < tmpFaceCount; ++j)
				{
					vec3i *tmpRef = tmpIndexNode->data() + j;
					if (*tmpRef == tmp)
					{
						tmpIndexIndex = j;
						break;
					}
				}
				if (tmpIndexIndex == -1)
				{
					*(tmpIndexNode->data() + tmpFaceCount) = tmp;
					tmpIndexIndex = tmpFaceCount;
					tmpFaceCount += 1;
				}
				m_pIndex->push_back(tmpIndexIndex);
			}
		}
	}

	size_t tmpSize = tmpIndexNode->size();
	std::vector<CVertexData> *tmpVertexIndex = new std::vector<CVertexData>(tmpSize);
	for (size_t i = 0; i < tmpSize; ++i)
	{
		CVertexData *pVertexData = tmpVertexIndex->data() + i;
		vec3i *pIndex = tmpIndexNode->data() + i;
		//Log << *pIndex << "\n";
		CVertexData *pSrcVertex = m_pVertex->data();
		memcpy(pVertexData->m_fNormal, (pSrcVertex + (*pIndex)[2])->m_fNormal, sizeof(float) * 3);
		memcpy(pVertexData->m_fTex, (pSrcVertex + (*pIndex)[1])->m_fTex, sizeof(float) * 2);
		memcpy(pVertexData->m_fPos, (pSrcVertex + (*pIndex)[0])->m_fPos, sizeof(float) * 3);
	}
	if (m_pVertex != nullptr)
	{
		m_pVertex->clear();
		delete m_pVertex;
		m_pVertex = tmpVertexIndex;
	}
	if (tmpIndexNode != nullptr)
	{
		tmpIndexNode->clear();
		delete tmpIndexNode;
		tmpIndexNode = nullptr;
	}
	InitDisplayBuffer();
}

void CObjModel::SetVertexAttrib(AttribType type)
{
	if (type == CDrawable::AttribType::VERTEXATTRIB)
	{
		if (m_pShader)
		{
			const char *tmpName[] = { "pos", "texCoord", "normal" };
			m_pShader->SetShaderAttrib(3, tmpName);
		}
	}
	else if (type == CDrawable::AttribType::ATTRIBPOINTER)
	{
		if (m_pShader)
		{
			size_t count[] = { 3, 2, 3};
			GLuint offset[] = { 0, sizeof(float) * 6, sizeof(float) * 3 };
			m_pShader->SetAttribPointer(count, GL_FLOAT, GL_FALSE, sizeof(CVertexData), offset);
		}
	}
}