#pragma once

#include <iostream>

#define STATIC_GLEW
#include <GL/glew.h>

#define GLUT_BUILDING_LIB
#include <GL/glut.h>

#include "VertexData.h"
#include <vector>
#include "common.h"
#include "Texure.h"
#include "Drawable.h"
#include "shader.h"
#include "C3DModel.h"

class CObjModel : public C3DModel
{
public:
	CObjModel();
	~CObjModel();
	// ./res/model/arcticcondor/Creature/Arcticcondor/arcticcondor.obj
	void Init(const char* filepath);
	void InitTexture(const char*pTextureFile);
	void InitDisplayBuffer();
	virtual void Draw();
	virtual void Update(float duration);
	
	GLuint GetShaderProgram();
	void ResetState();
	void TestShader(mat4f model);
private:

	void SetVertexAttrib(AttribType type);
	void InitVerexArrayBuffer();
	void InitByFileContent(char *content, size_t wLength);

	std::vector<CVertexData> *m_pVertex;
	std::vector<unsigned int> *m_pIndex;

	
	CShader *m_pShader;

	GLuint m_nNormalMatrixLocation;
	GLuint m_nVertexBufferObj;
	GLuint m_nIndexBufferObj;
	
	GLuint m_nVertexArrayID;

	CTexture *m_pTexture;
};

