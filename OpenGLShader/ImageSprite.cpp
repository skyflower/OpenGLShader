#include "ImageSprite.h"



CImageSprite::CImageSprite()
{
	m_pTexture = nullptr;
	//m_ubColor[0] = m_ubColor[1] = m_ubColor[2] = m_ubColor[3] = 
	//m_nAlpha = 255;
	//m_bFadeIn = false;
	//m_bFadeOut = false;
	m_AnimationType = Renderable::NONE;
	m_ubColor[0] = m_ubColor[1] = m_ubColor[2] = m_ubColor[3] = 255;
}


CImageSprite::~CImageSprite()
{
}

void CImageSprite::SetTexture(CTexture * pTexture)
{
	m_pTexture = pTexture;
}

void CImageSprite::Draw()
{
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glColor4ubv(&m_ubColor[0]);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBindTexture(GL_TEXTURE_2D, m_pTexture->m_TextureID);
	m_pTexture->Bind();
	glTranslatef(m_fOffset[0], m_fOffset[1], m_fOffset[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3fv(&m_fPos[0][0]);

	glTexCoord2f(0, 1);
	glVertex3fv(&m_fPos[1][0]);

	glTexCoord2f(1, 1);
	glVertex3fv(&m_fPos[2][0]);

	glTexCoord2f(1, 0);
	glVertex3fv(&m_fPos[3][0]);

	glEnd();

	glPopMatrix();
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	//glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void CImageSprite::SetPosition(float x, float y, float width, float height)
{
	float halfWidth = width / 2;
	float halfHeight = height / 2;
	m_fPos[0][0] = x - halfWidth;
	m_fPos[0][1] = y - halfHeight;

	m_fPos[1][0] = x - halfWidth;
	m_fPos[1][1] = y + halfHeight;

	m_fPos[2][0] = x + halfWidth;
	m_fPos[2][1] = y + halfHeight;

	m_fPos[3][0] = x + halfWidth;
	m_fPos[3][1] = y - halfHeight;



}

void CImageSprite::FadeIn(float duration)
{
	if (m_AnimationType == Renderable::FADE_IN)
	{
		double tmpAlpha = m_ubColor[3] + duration * GetSpeed(2);
		m_ubColor[3] = tmpAlpha > 255 ? 255 : tmpAlpha;
		if (m_ubColor[3] == 255)
		{
			//m_bFadeIn = false;
			m_AnimationType = Renderable::NONE;
		}
	}
}

void CImageSprite::FadeOut(float duration)
{
	if (m_AnimationType == Renderable::FADE_OUT)
	{
		double tmpAlpha = m_ubColor[3] - duration * GetSpeed(2);
		m_ubColor[3] = tmpAlpha < 0 ? 0 : tmpAlpha;
		if (m_ubColor[3] == 0)
		{
			m_AnimationType = Renderable::NONE;
		}
	}
}

float CImageSprite::GetSpeed(float time)
{
	if (time < 0.1 || time > 10)
	{
		time = 2;
	}
	float tmp = 255 / (double)time;
	return tmp;
}

void CImageSprite::Update(float duration)
{
	if(m_AnimationType = Renderable::FADE_IN)
	{
		FadeIn(duration);
	}
	else if (m_AnimationType = Renderable::FADE_OUT)
	{
		FadeOut(duration);
	}
}

void CImageSprite::SetColor(unsigned char R, unsigned char G, unsigned char B, unsigned char Alpha)
{
	m_ubColor[0] = R;
	m_ubColor[1] = G;
	m_ubColor[2] = B;
	m_ubColor[3] = Alpha;
}

void CImageSprite::SetColor(vec4b color)
{
	m_ubColor = color;
}

vec4b CImageSprite::GetColor()
{
	return m_ubColor;
}

void CImageSprite::SetOffset(float x, float y, float z)
{
	m_fOffset[0] = x;
	m_fOffset[1] = y;
	m_fOffset[2] = z;
}

void CImageSprite::SetOffset(vec3f offset)
{
	m_fOffset = offset;
}

