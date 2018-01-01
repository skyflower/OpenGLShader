#ifndef CSPHERE_H_INCLUDED
#define CSPHERE_H_INCLUDED

#include <iostream>

#define STATIC_GLEW
#include <GL/glew.h>

#define GLUT_BUILDING_LIB
#include <GL/glut.h>

#include "shader.h"
#include "VertexData.h"
#include <vector>
#include "common.h"
#include "utils.h"
#include "Texure.h"
#include "Drawable.h"
#include "C3DModel.h"


class CSphere : public C3DModel
{
public:
	CSphere(float radis = 1.0, int slide = 30, int stacks = 10);
	~CSphere();
	
	void Init(float radis, int slide, int stacks);
	void InitTexture(const char*pTextureFile);
	void InitDisplayBuffer();
	virtual void Draw();
	virtual void Update(float);
	
	GLuint GetShaderProgram();
	void TestShader(mat4f model);
private:
	
	void SetVertexAttrib(AttribType type);
	
	std::vector<CVertex> *m_data;
	std::vector<unsigned int> *m_pIndex;

	CTexture *m_pTexture;
	CShader *m_pShader;

	GLuint m_nVertexBufferObj;
	GLuint m_nIndexBufObj;
	//GLuint m_nNormalMatrixIndex;
	//GLuint m_nModelViewMatrixIndex;
};


#endif // CSPHERE_H_INCLUDED
