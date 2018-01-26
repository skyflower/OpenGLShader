#include "Camera.h"


CCamera* CCamera::m_pInstance = nullptr;

CCamera::CCamera():m_bMoveBackward(false),
	m_bMoveForward(false),
	m_bMoveLeft(false),
	m_bMoveRight(false),
	m_bRotateY(false),
	m_bNegRotateY(false),
	m_bRotateView(false)
{
	m_fPosition = vec3f(0, 3);
	m_fDestination[0] = m_fDestination[2] = 0;
	m_fDestination[2] = -1;
	m_fUpward[0] = m_fUpward[2] = 0;
	m_fUpward[1] = 1;

	m_fRotateSpeed = 1;
	m_fMoveSpeed = 5;

}


CCamera * CCamera::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CCamera();
	}
	return m_pInstance;
}

CCamera::~CCamera()
{
}

void CCamera::Update(float deltaTime)
{
	if (m_bMoveLeft)
	{
		vec3f viewDirection = m_fDestination - m_fPosition;
		vec3d NormalViewDirection = viewDirection.Normalize();
		vec3f LeftDirection = CVector<3, float>::CrossProduct(NormalViewDirection, m_fUpward);
		vec3f NormalLeftDirection = LeftDirection.Normalize();
		WriteInfo("OnePosition = %s", m_fPosition.FormatToString().c_str());
		m_fPosition = m_fPosition - NormalLeftDirection * m_fMoveSpeed * deltaTime;
		WriteInfo("TwoPosition = %s", m_fPosition.FormatToString().c_str());

		m_fDestination = m_fDestination - NormalLeftDirection * m_fMoveSpeed * deltaTime;
	}
	if (m_bMoveRight)
	{
		vec3f viewDirection = m_fDestination - m_fPosition;
		vec3d NormalViewDirection = viewDirection.Normalize();
		vec3f RightDirection = CVector<3, float>::CrossProduct(NormalViewDirection, m_fUpward);
		vec3f NormalRightDirection = RightDirection.Normalize();
		m_fPosition = m_fPosition + NormalRightDirection * m_fMoveSpeed * deltaTime;
		m_fDestination = m_fDestination + NormalRightDirection * m_fMoveSpeed * deltaTime;

	}
	if (m_bMoveBackward)
	{
		vec3f BackPos = m_fPosition - m_fDestination;
		vec3d NormalBackPos = BackPos.Normalize();
		m_fPosition += NormalBackPos * m_fMoveSpeed * deltaTime;
		m_fDestination += NormalBackPos * m_fMoveSpeed * deltaTime;
	}
	if (m_bMoveForward)
	{
		vec3f ForwardPos = m_fDestination - m_fPosition;

		vec3d NormalForwardPos = ForwardPos.Normalize();
		m_fPosition = m_fPosition + NormalForwardPos * m_fMoveSpeed * deltaTime;
		m_fDestination = m_fDestination + NormalForwardPos * m_fMoveSpeed * deltaTime;
	}
	if (m_bRotateY)
	{
		this->RotateView(m_fRotateSpeed * deltaTime, m_fUpward[0], m_fUpward[1], m_fUpward[2]);
	}
	if (m_bNegRotateY)
	{
		this->RotateView(-m_fRotateSpeed * deltaTime, m_fUpward[0], m_fUpward[1], m_fUpward[2]);
	}

	//gluLookAt(m_fPos[0], m_fPos[1], m_fPos[2], \
	//	m_fCenter[0], m_fCenter[1], m_fCenter[2], \
	//	m_fUpDir[0], m_fUpDir[1], m_fUpDir[2]);

}

void CCamera::Pitch(float angle)
{
	vec3f viewDirector = m_fDestination - m_fPosition;
	vec3f NormalViewDirector = viewDirector.Normalize();
	vec3f rightDirector = CVector<3, float>::CrossProduct(NormalViewDirector, m_fUpward);
	vec3f NormalRightDirector = rightDirector.Normalize();
	RotateView(angle, NormalRightDirector[0], NormalRightDirector[1], NormalRightDirector[2]);
}

void CCamera::SetRotateSpeed(float a)
{
	m_fRotateSpeed = a;
}

void CCamera::SetMoveSpeed(float a)
{
	m_fMoveSpeed = a;
}

void CCamera::SetMoveLeft(bool flag)
{
	m_bMoveLeft = flag;
}

void CCamera::SetMoveRight(bool flag)
{
	m_bMoveRight = flag;
}

void CCamera::SetMoveForward(bool flag)
{
	m_bMoveForward = flag;
}

void CCamera::SetMoveBackward(bool flag)
{
	m_bMoveBackward = flag;
}

void CCamera::SetRotateRight(bool flag)
{
	m_bRotateY = flag;
}

void CCamera::SetRotateLeft(bool flag)
{
	m_bNegRotateY = flag;
}

void CCamera::SetRotateView(bool flag)
{
	m_bRotateView = flag;
}

mat4f CCamera::GetViewMatrix()
{
	CMatrix<4, 4, float> result = CMatrix<4, 4, float>::GetLookAt(m_fPosition, m_fDestination, m_fUpward);
	return result;
}

void CCamera::SetWindowSize(size_t width, size_t height)
{
	m_nWindowSize.cx = width;
	m_nWindowSize.cy = height;
	
}

SIZE CCamera::GetWindowSize()
{
	return m_nWindowSize;
}

void CCamera::Yaw(float angle)
{
	RotateView(angle, m_fUpward[0], m_fUpward[1], m_fUpward[2]);
}
//void

void CCamera::RotateView(float angle, float x, float y, float z)
{
	vec3f tmpLabel;
	tmpLabel[0] = x;
	tmpLabel[1] = y;
	tmpLabel[2] = z;
	vec3f viewDirector = m_fDestination - m_fPosition;
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


	m_fDestination = m_fPosition + newDirector;
}

bool CCamera::GetMoveLeft()
{
	return m_bMoveLeft;
}

bool CCamera::GetMoveRight()
{
	return m_bMoveRight;
}

bool CCamera::GetMoveForward()
{
	return m_bMoveForward;
}

bool CCamera::GetMoveBackward()
{
	return m_bMoveBackward;
}

bool CCamera::GetRotateRight()
{
	return m_bRotateY;
}

bool CCamera::GetRotateLeft()
{
	return m_bNegRotateY;
}

bool CCamera::GetRotateView()
{
	return m_bRotateView;
}
