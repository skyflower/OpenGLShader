#pragma once

#include <iostream>

#define STATIC_GLEW
#include <GL/glew.h>

#define GLUT_BUILDING_LIB
#include <GL/glut.h>

#include "../common/VertexData.h"
#include <vector>
#include "../common/common.h"
#include "../common/utils.h"
#include "../common/Texure.h"
#include "../common/shader.h"
#include "../graphics/Drawable.h"
#include "../render/C3DModel.h"


class CLightTest : public C3DModel
{
public:
	CLightTest();
	~CLightTest();
	
	void Init();
	void InitTexture(const char*pTextureFile);
	void InitDisplayBuffer();
	virtual void Draw();
	virtual void Update(float duration);
	
	GLuint GetShaderProgram();
	void TestShader(mat4f model);
	void TestFragmentShader();
	void SetLightType(GLuint lightType);
	
private:
	void ResetState();
	void SetVertexAttrib(AttribType type);

	CVertexData *m_pVertexData;
	size_t m_nDataLength;

	CShader* m_pShader;

	CTexture *m_pTexture;
	
	GLuint m_nLightTypeLocation;
	GLuint m_nLightType;
	
	GLuint m_nVertexBufferObj;
	//GLuint m_nIndexBufferObj;
};

