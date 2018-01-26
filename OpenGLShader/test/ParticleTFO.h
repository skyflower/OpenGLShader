#pragma once
#include "../graphics/Drawable.h"
#include "../common/common.h"
#include "../common/Texure.h"
#include "../common/shader.h"
#include "../common/VertexData.h"
#include "../render/C3DModel.h"

class CParticleTFO :
	public C3DModel
{
public:
	CParticleTFO();
	virtual ~CParticleTFO();
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

	CShader *m_pTFOShader;
	GLuint m_nTfoProgram;
	GLuint m_nPosTfoLocation;

	GLuint m_nTfoID;
	GLuint m_nTfoBuffer;

	GLuint m_nVertexBufferObj;
	
	GLuint m_nPointCount;
	CVertex_SSBO_Two *m_pPoints;
};

