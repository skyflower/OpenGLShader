#ifndef CFOGTEST_H_INCLUDED
#define CFOGTEST_H_INCLUDED

#include <iostream>

#define STATIC_GLEW
#include <GL/glew.h>

#define GLUT_BUILDING_LIB
#include <GL/glut.h>

#include "../common/shader.h"
#include "../common/VertexData.h"
#include <vector>
#include "../common/common.h"
#include "../common/utils.h"
#include "../common/Texure.h"

#include "../render/C3DModel.h"


class CFogTest : public C3DModel
{
public:
	CFogTest(float radis = 0.4, int slide = 50, int stacks = 50);
	~CFogTest();
	
	void Init(float radis, int slide, int stacks);
	void InitTexture(const char*pTextureFile);
	void InitDisplayBuffer();
	virtual void Draw();
	virtual void Update(float);
	
	GLuint GetShaderProgram();
	void TestShader();
private:
	
	void SetVertexAttrib(AttribType type);
	
	std::vector<CVertex> *m_data;
	std::vector<unsigned int> *m_pIndex;

	CTexture *m_pTexture;
	CShader *m_pShader;

	GLuint m_nVertexBufferObj;
	GLuint m_nIndexBufObj;
	
};


#endif // CFOGTEST_H_INCLUDED
