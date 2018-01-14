#pragma once
class UnCopyable
{
public:
	UnCopyable();
	~UnCopyable();
private:
	UnCopyable(const UnCopyable &) = delete;
	UnCopyable& operator=(const UnCopyable &) = delete;
	UnCopyable(UnCopyable &) = delete;
	UnCopyable& operator=(UnCopyable &) = delete;
};

