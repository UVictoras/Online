#include "Thread.h"

Thread::Thread()
{
	m_sSize = 0;
	m_hThreadHandler = 0;
	m_iFlags = 0;
}

void Thread::Run()
{
	CreateThread(NULL, m_sSize, Static_ThreadProc, NULL, m_iFlags, NULL);
}

DWORD WINAPI Thread::Static_ThreadProc(LPVOID lpParameter)
{
	std::cout << "yes sir";
	return 0;
}