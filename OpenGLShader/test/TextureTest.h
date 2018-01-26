#ifndef TEXTURETEST_H_INCLUDED
#define TEXTURETEST_H_INCLUDED

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


class CTextureTest : public C3DModel
{
public:
	CTextureTest();
	~CTextureTest();
	
	void Init();
	void InitTexture(const char*pTextureFile);
	void InitDisplayBuffer();
	virtual void Draw();
	virtual void Update(float);
	
	GLuint GetShaderProgram();
	void TestShader(mat4f model);
private:
	
	void SetVertexAttrib(AttribType type);

	std::vector<CVertexData> *m_data;

	CTexture *m_pTexture;
	CShader *m_pShader;
	//GLuint m_nModelMatrixLocation;

	GLuint m_nVertexBufferObj;
	
};


#endif // TEXTURETEST_H_INCLUDED
