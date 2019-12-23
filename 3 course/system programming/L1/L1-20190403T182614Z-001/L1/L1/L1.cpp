// L1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "L1.h"

#include <thread>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

CRITICAL_SECTION cs;
HANDLE hMutex;
HANDLE hEventStart;
HANDLE hEventConfirm;

void start3()
{
	struct Header
	{
		int Size;
		int Address;
	};

	Header h;
	char* s;
	HANDLE hFile = CreateFile("file.dat", GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, 0, OPEN_ALWAYS, 0, NULL);
	HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, sizeof(Header), NULL);
	BYTE* pBuf = (BYTE*)MapViewOfFile(hFileMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, sizeof(Header));
	memcpy(&h, pBuf, sizeof(h));
	UnmapViewOfFile(pBuf);
	CloseHandle(hFileMap);

	hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, sizeof(Header) + h.Size, NULL);
	pBuf = (BYTE*)MapViewOfFile(hFileMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, sizeof(Header) + h.Size);
	s = new char[h.Size];
	memcpy(s, pBuf+sizeof(h), h.Size);

	cout << s << endl;

	UnmapViewOfFile(pBuf);
	CloseHandle(hFileMap);
	CloseHandle(hFile);
	delete[]s;
}

void start4()
{
	struct Header
	{
		int Size;
		int Address;
	};
	Header h;
	CString s("12345");
	h.Size = s.GetLength() + 1;

	int N = 100;
	//	int* a = new int[N];
	//	for (int i = 0; i < N; ++i)
	//		a[i] = i;
	HANDLE hFile = CreateFile("file.dat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_ALWAYS, 0, NULL);
	HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, sizeof(Header) + h.Size, NULL);
	BYTE* pBuf = (BYTE*)MapViewOfFile(hFileMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, sizeof(Header) + h.Size);
	memcpy(pBuf, &h, sizeof(h));
	memcpy(pBuf + sizeof(h), s.GetString(), h.Size);


	UnmapViewOfFile(pBuf);
	CloseHandle(hFileMap);
	CloseHandle(hFile);
	//	delete[]a;
}

__declspec(thread) LONG gCount = 0;

DWORD WINAPI MyThread(LPVOID lpParam)
{
	int i = 0;
	int id = (int)lpParam;
//	EnterCriticalSection(&cs);
	WaitForSingleObject(hMutex, INFINITE);
	cout << "Thread id: " << id << endl;
	cout << "gCount: " << ++gCount << endl << endl;
//	LeaveCriticalSection(&cs);
	ReleaseMutex(hMutex);
	while (1)
	{
//		EnterCriticalSection(&cs);
//		Sleep(100);

		WaitForSingleObject(hEventStart, INFINITE);
		ResetEvent(hEventStart);
		SetEvent(hEventConfirm);

		WaitForSingleObject(hMutex, INFINITE);
		cout << "Thread " << id << ": " << ++i << endl;
//		LeaveCriticalSection(&cs);
		ReleaseMutex(hMutex);
		if (i == 100)
			break;
	}
	return 0;
}

void start2()
{
//	InitializeCriticalSection(&cs);
	hMutex = CreateMutex(NULL, FALSE, "MyMutex");
	hEventStart = CreateEvent(NULL, TRUE, FALSE, "MyEventStart");
	hEventConfirm = CreateEvent(NULL, TRUE, FALSE, "MyEventConfirm");
	HANDLE hThreads[10];
	for (int i = 0; i < 10; ++i)
	{
//		hThreads[i] = CreateThread(NULL, 0, MyThread, (LPVOID)i, 0, NULL);
		thread t(MyThread, (LPVOID)i);
		hThreads[i] = t.native_handle();
		t.detach();
	}

	while (1)
	{
		int i;
		cin >> i;
		SetEvent(hEventStart);
//		ResetEvent(hEvent);
//		PulseEvent(hEvent);
	}

	WaitForMultipleObjects(10, hThreads, TRUE, INFINITE);

	cout << "All threads done" << endl;

	int i;
	cin >> i;

//	DeleteCriticalSection(&cs);
	CloseHandle(hMutex);
	CloseHandle(hEventStart);
	CloseHandle(hEventConfirm);
}


void start1()
{
	HANDLE hMutex = CreateMutex(NULL, FALSE, "MyMutex");

	STARTUPINFO si = {0};
	//ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi;
	if (CreateProcess(NULL, "notepad.exe", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		DWORD dwCode;
		
		GetExitCodeProcess(pi.hProcess, &dwCode);
		cout << "Notepad started: " << dwCode << endl;
		WaitForSingleObject(pi.hProcess, INFINITE);
		GetExitCodeProcess(pi.hProcess, &dwCode);
		cout << "Notepad done: " << dwCode << endl;
	}
}

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // initialize MFC and print and error on failure
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: change error code to suit your needs
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
        {
			start3();
//			thread t(start2);
//			t.join();
        }
    }
    else
    {
        // TODO: change error code to suit your needs
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}
