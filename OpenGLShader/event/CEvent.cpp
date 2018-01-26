#include "CEvent.h"





CCEvent::CCEvent(CCEvent::EventType type):
	m_nEventType(type),
	m_bStopped(false),
	m_pTargetNode(nullptr)
{
}

CCEvent::~CCEvent()
{
}

void CCEvent::SetTargetNode(CCNode * pNode)
{
	m_pTargetNode = pNode;
}

void CCEvent::SetStopStatus(bool flag)
{
	m_bStopped = flag;
}
