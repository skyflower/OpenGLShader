#pragma once
#include <iostream>


class CVertex
{
public:
	float mPos[3];
	float mColor[4];
};



class CVertexData
{
public:
	float m_fPos[3];
	float m_fNormal[3];
	float m_fTex[2];
};

class CVertex_SSBO
{
public:
	float m_fPos[4];
	float m_fNormal[4];
	float m_fTex[4];
};

class CVertex_SSBO_Two
{
public:
	float m_fPos[4];
};

class CParticleVertex
{
public:
	union 
	{
		float v[8];
		struct
		{
			float pos[4];
			float mesh[4];
		};
	};
};


