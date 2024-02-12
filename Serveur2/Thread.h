#pragma once

#include <strsafe.h>
#include <iostream>
#define NOMINMAX
#include <winsock2.h>

class Thread
{
private:
	SIZE_T m_sSize;

	HANDLE m_hThreadHandler;

	static DWORD WINAPI Static_ThreadProc(LPVOID lpParameter);

public:
	int m_iFlags;

	Thread();

	void Run();

	virtual void OnRun();

	virtual void HandleError(LPCTSTR lpszFunction);
};

