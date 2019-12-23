// Kamshilov_cpl.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Kamshilov_cpl.h"
#include "../MFCLibrary1/MFCLibrary1.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef void(*PGET)(int &n, char *&str);

// Единственный объект приложения

CWinApp theApp;
CRITICAL_SECTION sa;
using namespace std;
int c = 1;
int num;
char *str;

HANDLE hThread;
HANDLE hEventsThread[2];
HANDLE hEvConf = CreateEvent(NULL, FALSE, FALSE, NULL);

CString message;

UINT __cdecl ProcessClient(LPVOID pParam)
{
	setlocale(LC_ALL, "Russian");
	int id = (int)pParam;
	EnterCriticalSection(&sa);
	std::cout << "Start - " << id << endl;
	LeaveCriticalSection(&sa);
	while (1)
	{
		DWORD dwResult = WaitForMultipleObjects(2, hEventsThread, FALSE, INFINITE) - WAIT_OBJECT_0;
		switch (dwResult)
		{
		case 0:
		{
			if (id == c)
			{
				ResetEvent(hEventsThread[0]);
				std::cout << id << " stopped" << endl;
				SetEvent(hEvConf);
				return 0;
			}
			break;
		}

		case 1:
		{
			if (id == num + 1 || num == -1) {
				ResetEvent(hEventsThread[1]);
				ofstream f;
				CString namefile;
				namefile.Format("%d.txt", id);
				f.open(namefile, ios_base::app);
				f << str << endl;
				f.close();
			}
			break;
		}
		}

	}
}

void start()
{
	setlocale(LC_ALL, "Russian");
	hEventsThread[0] = CreateEvent(NULL, TRUE, FALSE, "EventDelete");
	hEventsThread[1] = CreateEvent(NULL, TRUE, FALSE, "EvSend");
	bool b = false;
	InitializeCriticalSection(&sa);
	while (1)
	{
		HANDLE hPipe = CreateNamedPipe("\\\\.\\pipe\\MyPipe", PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 1024, 1024, 0, NULL);
		while (1)
		{
			ConnectNamedPipe(hPipe, NULL);
			DWORD dwRead, dwWrite;
			int n = -1;
			ReadFile(hPipe, LPVOID(&n), sizeof(n), &dwRead, NULL);
			switch (n) {
			case 0:
			{
				hThread = AfxBeginThread(ProcessClient, LPVOID(c++), 0, 0, 0, NULL);
				CloseHandle(hThread);
				WriteFile(hPipe, LPCVOID(&c), sizeof(c), &dwWrite, NULL);
				FlushFileBuffers(hPipe);
				break;
			}
			case 1:
			{
				if (c != 1)
				{
					c--;

					WriteFile(hPipe, LPCVOID(&c), sizeof(c), &dwWrite, NULL);
					FlushFileBuffers(hPipe);
					SetEvent(hEventsThread[0]);
					WaitForSingleObject(hEvConf, INFINITE);
				}
				break;
			}
			case 2:
			{
				if (c != 1)
				{
					int size = 0;
					ReadFile(hPipe, LPVOID(&num), sizeof(num), &dwRead, NULL);
					ReadFile(hPipe, LPVOID(&size), sizeof(size), &dwRead, NULL);
					str = new char[size];
					ReadFile(hPipe, LPVOID(str), size, &dwRead, NULL);
					if (num == -2)
					{
						std::cout << "message main: " << str << endl;
					}
					if (num == -1)
					{
						std::cout << "message for all: " << str << endl;
					}
					if (num >= -1)
					{
						SetEvent(hEventsThread[1]);
					}
				}
				break;
			}
			case 3:
			{
				std::cout << "Quit" << endl;

				b = true;
				break;
			}
			if (b == true) {
				break;
			}

			}
			DisconnectNamedPipe(hPipe);
		}
		DeleteCriticalSection(&sa);
		CloseHandle(hPipe);
		CloseHandle(hEventsThread[0]);
		CloseHandle(hEventsThread[1]);
	}
}



int main()
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(nullptr);

	if (hModule != nullptr)
	{
		// инициализировать MFC, а также печать и сообщения об ошибках про сбое
		if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
		{
			// TODO: измените код ошибки соответственно своим потребностям
			wprintf(L"Критическая ошибка: сбой при инициализации MFC\n");
			nRetCode = 1;
		}
		else
		{
			start();
			// TODO: Вставьте сюда код для приложения.
		}
	}
	else
	{
		// TODO: Измените код ошибки соответственно своим потребностям
		wprintf(L"Критическая ошибка: неудачное завершение GetModuleHandle\n");
		nRetCode = 1;
	}

	return nRetCode;
}
