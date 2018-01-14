#pragma once
class Runable
{
public:
	Runable();
	~Runable();
	virtual void run() = 0;
};

