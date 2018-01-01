#pragma once

#include <iostream>

#define STATIC_GLEW
#include <GL/glew.h>

#define GLUT_BUILDING_LIB
#include <GL/glut.h>

#include "utils.h"


class CShader
{
public:
	
	CShader(const char*vsfileName, const char*fsfileName, const char *gsfile = nullptr);
	
	CShader(const char*vsFile, const char*attrib[], int nCount, GLenum memoryFormat, const char *gsFile = nullptr);

	CShader(int nCount, char*path[]);

	void Bind(bool flag = true);
	void SetShaderAttrib(int count, const char*value[]);
	void SetAttribPointer(size_t* count, GLenum type, GLboolean flag, GLsizei stride, GLuint* offset);

	inline GLuint GetProgram() { return m_nProgram; }
    ~CShader();
protected:

private:

	void CreateProgram(const char*vsfileName, const char*fsfileName, const char *gsfile = nullptr);

	GLuint CreateShader(const char*filepath, GLenum ShaderType);

	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, std::string Msg);

    static const size_t NUM_SHADERS = 5;
    GLuint m_nProgram;

	// 0 vsShader
	// 1 fsShader
	// 2 gsShader
	// 3 tcsShader
	// 4 tesShader
    GLuint m_shaders[NUM_SHADERS];

	GLint m_nValueNum;
	GLuint *m_pLocation;
	char **m_pName;
};

