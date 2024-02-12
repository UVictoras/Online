#pragma once

#include <iostream>
#define NOMINMAX
#include <winsock2.h>

class Thread
{
private:
	SIZE_T m_sSize;

	HANDLE m_hThreadHandler;

public:
	int m_iFlags;

	Thread();

	virtual void Run();

	static DWORD WINAPI Static_ThreadProc(LPVOID lpParameter);
};

