#pragma once
#include "Drawable.h"
#include "./common/common.h"
#include "./common/Texure.h"
#include "./common/shader.h"
#include "./common/VertexData.h"
#include "C3DModel.h"

class CPointSprite :
	public C3DModel
{
public:
	CPointSprite();
	virtual ~CPointSprite();
	void InitTexture(const char * filepath, const char*skyFile = nullptr);
	virtual void Draw();
	virtual void Update(float Duration);
	
	GLuint GetShaderProgram();
	
private:
	void Init();
	void SetVertexAttrib(AttribType type);
	void ResetState();

	CTexture* m_pTexture;
	CTexture *m_pSkyTexture;
	CShader* m_pShader;

	GLuint m_nSSBOProgram;
	CVertex_SSBO_Two m_point;
};

