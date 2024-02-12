#include "Thread.h"

DWORD WINAPI Thread::Static_ThreadProc(LPVOID lpParameter)
{
    Thread* pThread = static_cast<Thread*>(lpParameter);

    // Call the non-static member function
    pThread->OnRun();

    // Clean up if necessary, avoiding memory leaks
    // delete pThread; // Uncomment this line if Thread objects should be deleted after OnRun

    return 0;
}

Thread::Thread()
{
	m_sSize = 0;
	m_hThreadHandler = 0;
	m_iFlags = 0;
}

void Thread::Run()
{
	m_hThreadHandler = CreateThread(NULL, m_sSize, Static_ThreadProc, this, m_iFlags, NULL);
}

void Thread::OnRun()
{
	std::cout << "Thread is running..." << std::endl;
}

void Thread::HandleError(LPCTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code.

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

    // Display the error message.

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR), TEXT("%s failed with error %d: %s"), lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

    // Free error-handling buffer allocations.

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);

	return;
}