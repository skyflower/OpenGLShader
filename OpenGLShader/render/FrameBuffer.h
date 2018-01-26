#pragma once
#include <iostream>
#include <unordered_map>

#define STATIC_GLEW
#include <GL/glew.h>

#define GLUT_BUILDING_LIB
#include <GL/glut.h>

#include "../common/utils.h"

class CFrameBuffer
{
public:
	CFrameBuffer();
	~CFrameBuffer();
	void Finish();
	void AttachColorBuffer(const char *bufferName, GLenum attachment, GLenum dataType, int width, int height);
	void AttachDepthBuffer(const char *bufferName, int width, int height);

	inline GLuint GetFrameBuffer() { return m_nFbo; }
	inline GLuint GetDepthBuffer() { return m_nDepthBuffer; }
	GLuint GetColorBuffer(const char *name);
private:
	GLuint m_nFbo;
	GLuint m_nDepthBuffer;
	std::unordered_map<std::string, GLuint> m_pBuffer;
	std::vector<GLenum> m_pDrawBuffer;

};

