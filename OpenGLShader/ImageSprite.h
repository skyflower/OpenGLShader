#pragma once

#define STATIC_GLEW
#include <GL/glew.h>

#define GLUT_BUILDING_LIB
#include <GL/glut.h>

#include "common.h"
#include "Texure.h"
#include "CCList.h"


class CImageSprite : public Renderable
{
public:
	CImageSprite();
	virtual ~CImageSprite();
	void SetTexture(CTexture *pTexture);
	virtual void Draw();
	void SetPosition(float x, float y, float width, float height);
	void FadeIn(float duration);
	void FadeOut(float duration);
	float GetSpeed(float time);
	void Update(float duration);
	void SetColor(unsigned char R, unsigned char G, unsigned char B, unsigned char Alpha);
	void SetColor(vec4b color);
	vec4b GetColor();
	void SetOffset(float x, float y, float z);
	void SetOffset(vec3f offset);
	inline vec3f GetOffset() { return m_fOffset; }
	inline const vec3f & GetOffset()const { return m_fOffset; }
private:
	vec3f m_fPos[4]; // display screen coords
	CTexture *m_pTexture;
	vec3f m_fOffset;
	vec4b m_ubColor;
	Renderable::AnimationType m_AnimationType;
};

