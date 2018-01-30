#ifndef FRAMEBUFFERTEST_H_INCLUDED
#define FRAMEBUFFERTEST_H_INCLUDED

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
#include "../render/FrameBuffer.h"
#include "FullScreen.h"


class CFrameBufferTest
{
public:
	CFrameBufferTest(size_t width, size_t height);
	~CFrameBufferTest();

	void SetTexture(GLuint textureID);

	void Draw(int nCount);
	
	void SetShader(CShader* pShader, size_t index);

	GLuint GetColorBuffer();

	GLuint GetDepthBuffer();

private:


	CFrameBuffer m_nFrameBuffer[2];

	CShader* m_pFirstShader;
	CShader* m_pSecondShader;

	CFullScreen m_pFullScreen;
	GLuint m_nTextureID;
	
};


#endif // FRAMEBUFFERTEST_H_INCLUDED
