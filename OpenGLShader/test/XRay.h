#ifndef CXRAY_H_INCLUDED
#define CXRAY_H_INCLUDED

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


class CXRay : public C3DModel
{
public:
	CXRay(float radis = 1.0, int slide = 30, int stacks = 10);
	~CXRay();
	
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
	
};


#endif // CXRAY_H_INCLUDED
