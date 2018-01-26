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
	virtual void Draw();
	virtual void Update(float Duration);
	void DrawLeftTop();
	void DrawLeftBottom();
	void DrawRightTop();
	void DrawRightBottom();
};

