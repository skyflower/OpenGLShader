#pragma once
#include "CNode.h"
#include "Drawable.h"
#include "../common/common.h"
#include "../event/CEvent.h"

class CCLayer :
	public CCNode, public CDrawable
{
public:
	CCLayer();
	~CCLayer();
};

