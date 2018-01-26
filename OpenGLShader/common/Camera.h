#pragma once
#include "UnCopyable.h"
#include "common.h"
#include <Windows.h>

class CCamera :
	public UnCopyable
{
public:
	static CCamera* GetInstance();
	~CCamera();
	void Update(float duration);
	void RotateView(float angle, float x, float y, float z);
	void Yaw(float angle);
	void Pitch(float angle);

	void SetRotateSpeed(float a);
	void SetMoveSpeed(float a);

	void SetMoveLeft(bool flag);
	void SetMoveRight(bool flag);
	void SetMoveForward(bool flag);
	void SetMoveBackward(bool flag);
	void SetRotateRight(bool flag);
	void SetRotateLeft(bool flag);
	void SetRotateView(bool flag);

	bool GetMoveLeft();
	bool GetMoveRight();
	bool GetMoveForward();
	bool GetMoveBackward();
	bool GetRotateRight();
	bool GetRotateLeft();
	bool GetRotateView();


	mat4f GetViewMatrix();
	void SetWindowSize(size_t width, size_t height);
	SIZE GetWindowSize();
private:
	static CCamera* m_pInstance;
	vec3f m_fPosition;
	vec3f m_fDestination;
	vec3f m_fUpward;
	float m_fRotateSpeed;
	float m_fMoveSpeed;
	bool m_bMoveBackward;
	bool m_bMoveForward;
	bool m_bMoveLeft;
	bool m_bMoveRight;
	bool m_bRotateY;
	bool m_bNegRotateY;
	bool m_bRotateView;

	SIZE m_nWindowSize;

	CCamera();
};

