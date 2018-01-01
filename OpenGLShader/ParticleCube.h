#pragma once
#include "Drawable.h"
#include "common.h"
#include "Texure.h"
#include "shader.h"
#include "VertexData.h"
#include "C3DModel.h"

class CParticleCube :
	public C3DModel
{
public:
	CParticleCube();
	virtual ~CParticleCube();
	void InitTexture(const char *filepath);
	virtual void Draw();
	virtual void Update(float Duration);
	
	void TestShader(mat4f& model);
	GLuint GetShaderProgram();

private:
	void Init();
	void SetVertexAttrib(AttribType type);
	void ResetState();
	
	CShader* m_pShader;

	CVertex_SSBO_Two *m_pPoints;
	GLuint m_nSSBOProgram;

	CVertex_SSBO_Two *m_pVel;
	GLuint m_nVelProgram;

	GLuint m_nTextureID;
	GLuint m_nTexLocation;

	GLuint m_nCSShader;

	size_t m_nPointCount;

	GLuint *m_pIndex;
	GLuint m_nIndexCount;
};

