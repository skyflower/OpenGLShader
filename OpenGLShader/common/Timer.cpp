#include "Timer.h"



CTimer::CTimer()
{
	m_nCurrentThreadID = std::this_thread::get_id();// std::thread::get_id();
	m_nCurrentThread = GetCurrentThread();
	DWORD_PTR systemAffinityMask;
	GetProcessAffinityMask(GetCurrentProcess(), &mProcessAffinityMask, &systemAffinityMask);
	SetThreadAffinityMask(m_nCurrentThread, 1);
	QueryPerformanceFrequency(&mCPUFrequency);
	SetThreadAffinityMask(m_nCurrentThread, mProcessAffinityMask);
	
}


CTimer::~CTimer()
{
}

void CTimer::Start()
{
	SetThreadAffinityMask(m_nCurrentThread, 1);
	QueryPerformanceCounter(&mStart);
	SetThreadAffinityMask(m_nCurrentThread, mProcessAffinityMask);
}

LONGLONG CTimer::GetPassedTicks()
{
	SetThreadAffinityMask(m_nCurrentThread, 1);
	QueryPerformanceCounter(&mEnd);
	SetThreadAffinityMask(m_nCurrentThread, mProcessAffinityMask);
	return mEnd.QuadPart - mStart.QuadPart;
}

double CTimer::GetPassedTimeInMs()
{
	return 1000 * (double)GetPassedTicks() / (double)mCPUFrequency.QuadPart;
}

