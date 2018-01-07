#pragma once
#include <iostream>
#include <thread>
#include <Windows.h>

class CTimer
{
public:
	CTimer();
	~CTimer();
	void Start();
	LONGLONG GetPassedTicks();
	double GetPassedTimeInMs();
private:
	HANDLE m_nCurrentThread;
	std::thread::id m_nCurrentThreadID;
	DWORD_PTR mProcessAffinityMask;
	LARGE_INTEGER mCPUFrequency;
	LARGE_INTEGER mStart;
	LARGE_INTEGER mEnd;
};

