#pragma once
#include "Drawable.h"
#include "./common/common.h"
#include "./common/Texure.h"
#include "./common/shader.h"
#include "./common/VertexData.h"
#include "C3DModel.h"

class CParticleGS :
	public C3DModel
{
public:
	CParticleGS();
	virtual ~CParticleGS();
	void InitTexture(const char *filepath);
	virtual void Draw();
	virtual void Update(float Duration);
	
	GLuint GetShaderProgram();
	
private:
	void Init();
	void SetVertexAttrib(AttribType type);
	void ResetState();

	CTexture* m_pTexture;
	CShader* m_pShader;

	
	//GLuint m_nPosLocation;
	CVertex_SSBO_Two m_point;
};

