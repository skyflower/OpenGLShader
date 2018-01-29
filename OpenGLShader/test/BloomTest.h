#ifndef BLOOMTEST_H_INCLUDED
#define BLOOMTEST_H_INCLUDED

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


class CBloomTest : public C3DModel
{
public:
	CBloomTest(float radis = 0.1, int slide = 50, int stacks = 50);
	~CBloomTest();
	
	void Init(float radis, int slide, int stacks);
	void InitTexture(const char*pTextureFile);
	void InitDisplayBuffer();
	virtual void Draw();
	virtual void Update(float);
	
	GLuint GetShaderProgram();
	void TestShader(mat4f model);
private:
	
	void DrawGround();
	void DrawCubic();
	void DrawSphere();


	CTexture *m_pTexture;
	CShader *m_pGroundShader;
	CShader *m_pCubicShader;
	CShader *m_pSphereShader;

	//ground
	std::vector<CVertexData> *m_pGroundVertex;
	GLuint m_nGroundVertexBuffer;

	//light


	// sphere
	std::vector<CVertex>* m_pSphereData;
	std::vector<unsigned int>* m_pSphereIndex;
	GLuint m_nSphereVertexBuffer;
	GLuint m_nSphereIndexBuffer;

	// cubic
	//CVertexData *m_pVertex;
	std::vector<CVertexData> *m_pCubicVertex;
	GLuint m_nCubicVertexBuffer;
	
};


#endif // BLOOMTEST_H_INCLUDED
