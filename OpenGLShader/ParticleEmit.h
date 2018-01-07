#pragma once
#include "Drawable.h"
#include "./common/common.h"
#include "./common/Texure.h"
#include "./common/shader.h"
#include "./common/VertexData.h"
#include "C3DModel.h"

class CParticleEmit :
	public C3DModel
{
public:
	CParticleEmit();
	virtual ~CParticleEmit();
	void InitTexture(const char *filepath);
	virtual void Draw();
	virtual void Update(float Duration);

	
	GLuint GetShaderProgram();
	
	void EmitParticle();

private:
	void Init();
	void SetVertexAttrib(CShader *pShader, AttribType type);
	void ResetState();

	CTexture* m_pTexture;
	GLuint m_nTexLocation;

	CShader* m_pShader;

	CShader *m_pTFOShader;
	//GLuint m_nTfoProgram;
	//GLuint m_nPosTfoLocation;

	CShader *m_pUpdateShader;
	//GLuint m_nUpdateProgram;
	//GLuint m_nUpdatePosLocation;

	bool m_bEmitNewParticle;
	GLuint m_nUpdateTfo[2];
	GLuint m_nUpdateTfoBuffer[2];
	GLuint m_nCurUpdateTfo;
	GLuint m_nCurDrawTfo;
	
	GLint m_nPointCountInBuffer;
	GLuint m_nQureyObject;
	

	GLuint m_nTfoID;
	GLuint m_nTfoBuffer;

	
	GLuint m_nVertexBufferObj;
	
	GLuint m_nPointCount;
	CParticleVertex *m_pPoints;
};

