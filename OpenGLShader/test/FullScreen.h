#pragma once
#include "..\render\C3DModel.h"

#include "../common/common.h"
#include "../common/Log.h"

class CFullScreen :
	public C3DModel
{
public:
	CFullScreen();
	~CFullScreen();
	void initTexture(GLuint textureID);
	void SetAuxTexture(GLuint textureID);
	virtual void Draw();
	virtual void Update(float Duration);
	void DrawLeftTop();
	void DrawLeftBottom();
	void DrawRightTop();
	void DrawRightBottom();
	void SetPosLocation(GLuint PosLoc);
	void SetShader(CShader* pShader);
	void SetScaleAndTranslate(float scaleFactor, float translateX, float translateY);
	
private:

	void SetVertexAttrib(AttribType type);

	GLuint m_nTextureID;
	GLuint m_nTexLocation;

	GLuint m_nAuxTexID;
	GLuint m_nAuxTexLocation;



	GLuint m_nVertexBuffObj;
	//GLuint 
	CVertexData *m_pVertex;
	size_t m_nVertexNum;
	GLuint m_nPosLocation;
	CShader *m_pShader;
	GLuint m_nParamLocation;
	vec4f m_fParam;

	CShader* m_pDefaultShader;
};

