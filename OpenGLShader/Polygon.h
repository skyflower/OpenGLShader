#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

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


class CPolygon : public C3DModel
{
public:
	CPolygon();
	~CPolygon();
	
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

	//GLuint m_nSSBOProgram;

	CTexture *m_pTexture;
	CShader *m_pShader;

	GLuint m_nVertexBufferObj;
	
	//GLuint m_nMVPLocation;
	//mat4f m_fMVPMatrix;
	//vec3f m_fTrans[3];
};


#endif // SHADER_H_INCLUDED
