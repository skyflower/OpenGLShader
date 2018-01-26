#pragma once
#include "../common/common.h"
#include "../graphics/CNode.h"
class CCEvent
{
public:
	/** Type Event type.*/
	enum class EventType
	{
		TOUCH,
		KEYBOARD,
		ACCELERATION,
		MOUSE,
		FOCUS,
		GAME_CONTROLLER,
		CUSTOM
	};

public:
	CCEvent(CCEvent::EventType type);
	~CCEvent();
	inline CCNode* GetTargetNode() { return m_pTargetNode; }
	void SetTargetNode(CCNode *pNode);
	void SetStopStatus(bool flag);
	inline bool GetStopStatus() { return m_bStopped; }

protected:
	CCNode *m_pTargetNode;
	bool m_bStopped;
	CCEvent::EventType m_nEventType;
};

