#pragma once

#include <iostream>
#include "common.h"

#define STATIC_GLEW
#include <GL/glew.h>

#define GLUT_BUILDING_LIB
#include <GL/glut.h>

class Camera
{
public:
	vec3f m_fPos;
	vec3f m_fCenter;
	vec3f m_fUpDir;
	float m_fMoveSpeed;
	bool m_bMoveLeft;
	bool m_bMoveRight;
	bool m_bMoveForward;
	bool m_bMoveBackward;
	bool m_bRotateY;
	bool m_bNegRotateY;
	bool m_bRotateView;
	int m_nViewportWidth;
	int m_nViewportHeight;

	Camera() : m_fPos(CVector<3, float>(0, 3))
	{
		//m_fPos[0] = m_fPos[1] = m_fPos = CVector<3, float>(0, 3);
		m_fCenter[0] = m_fCenter[1] = 0;
		m_fCenter[2] = -1;
		m_fUpDir[0] = m_fUpDir[2] = 0;
		m_fUpDir[1] = 1;
		m_bMoveLeft = false;
		m_bMoveRight = false;
		m_fMoveSpeed = 5.0;
		m_bMoveBackward = false;
		m_bMoveForward = false;
		m_bNegRotateY = false;
		m_bRotateView = false;
		m_nViewportWidth = 0;
		m_nViewportHeight = 0;
	}
	void SetMoveSpeed(float speed)
	{
		m_fMoveSpeed = speed;
	}
	void Pitch(float angle)
	{
		vec3f viewDirector = m_fCenter - m_fPos;
		vec3f NormalViewDirector = viewDirector.Normalize();
		vec3f rightDirector = CVector<3, float>::CrossProduct(NormalViewDirector, m_fUpDir);
		vec3f NormalRightDirector = rightDirector.Normalize();
		RotateView(angle, NormalRightDirector[0], NormalRightDirector[1], NormalRightDirector[2]);
	}
	void Yaw(float angle)
	{
		RotateView(angle, m_fUpDir[0], m_fUpDir[1], m_fUpDir[2]);
	}
	void RotateView(float angle, float x, float y, float z)
	{
		vec3f tmpLabel;
		tmpLabel[0] = x;
		tmpLabel[1] = y;
		tmpLabel[2] = z;
		vec3f viewDirector = m_fCenter - m_fPos;
		vec3f newDirector;
		float C = cosf(angle);
		float S = sinf(angle);

		vec3f tmpX;// (C + x * x * (1 - C), )
		tmpX[0] = C + x * x * (1 - C);
		tmpX[1] = x * y * (1 - C) - z * S;
		tmpX[2] = x * z * (1 - C) + y * S;
		newDirector[0] = tmpX.dotMultiply(viewDirector);
		vec3f tmpY;
		tmpY[0] = x * y * (1 - C) + z * S;
		tmpY[1] = C + y * y * (1 - C);
		tmpY[2] = y * z * (1 - C) - x * S;
		newDirector[1] = tmpY.dotMultiply(viewDirector);

		vec3f tmpZ;
		tmpZ[0] = x * z * (1 - C) - y * S;
		tmpZ[1] = y * z * (1 - C) + x * S;
		tmpZ[2] = C + z * z * (1 - C);
		newDirector[2] = tmpZ.dotMultiply(viewDirector);


		m_fCenter = m_fPos + newDirector;
	}

	void Update(float deltaTime)
	{
		//float moveSpeed = 10.0f;
		float tmpRotateSpeed = 1.0f;

		if (m_bMoveLeft)
		{
			vec3f viewDirection = m_fCenter - m_fPos;
			vec3d NormalViewDirection = viewDirection.Normalize();
			vec3f LeftDirection = CVector<3, float>::CrossProduct(NormalViewDirection, m_fUpDir);
			vec3f NormalLeftDirection = LeftDirection.Normalize();
			m_fPos = m_fPos - NormalLeftDirection * m_fMoveSpeed * deltaTime;
			m_fCenter = m_fCenter - NormalLeftDirection * m_fMoveSpeed * deltaTime;
		}
		if (m_bMoveRight)
		{
			vec3f viewDirection = m_fCenter - m_fPos;
			vec3d NormalViewDirection = viewDirection.Normalize();
			vec3f RightDirection = CVector<3, float>::CrossProduct(NormalViewDirection, m_fUpDir);
			vec3f NormalRightDirection = RightDirection.Normalize();
			m_fPos = m_fPos + NormalRightDirection * m_fMoveSpeed * deltaTime;
			m_fCenter = m_fCenter + NormalRightDirection * m_fMoveSpeed * deltaTime;

		}
		if (m_bMoveBackward)
		{
			vec3f BackPos = m_fPos - m_fCenter;
			vec3d NormalBackPos = BackPos.Normalize();
			m_fPos += NormalBackPos * m_fMoveSpeed * deltaTime;
			m_fCenter += NormalBackPos * m_fMoveSpeed * deltaTime;
		}
		if (m_bMoveForward)
		{
			vec3f ForwardPos = m_fCenter - m_fPos;
			
			vec3d NormalForwardPos = ForwardPos.Normalize();
			m_fPos = m_fPos + NormalForwardPos * m_fMoveSpeed * deltaTime;
			m_fCenter = m_fCenter + NormalForwardPos * m_fMoveSpeed * deltaTime;
		}
		if (m_bRotateY)
		{
			this->RotateView(tmpRotateSpeed * deltaTime, m_fUpDir[0], m_fUpDir[1], m_fUpDir[2]);
		}
		if (m_bNegRotateY)
		{
			this->RotateView(-tmpRotateSpeed * deltaTime, m_fUpDir[0], m_fUpDir[1], m_fUpDir[2]);
		}

		gluLookAt(m_fPos[0], m_fPos[1], m_fPos[2], \
			m_fCenter[0], m_fCenter[1], m_fCenter[2], \
			m_fUpDir[0], m_fUpDir[1], m_fUpDir[2]);
	}
	void SwitchTo2D()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(-m_nViewportWidth / 2, m_nViewportWidth/2, -m_nViewportHeight / 2, m_nViewportHeight / 2);

		glMatrixMode(GL_MODELVIEW);
	}
	void SwitchTo3D()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(50, (float)m_nViewportWidth / (float)m_nViewportHeight, 0.1, 1000);

		glMatrixMode(GL_MODELVIEW);
		
	}
};