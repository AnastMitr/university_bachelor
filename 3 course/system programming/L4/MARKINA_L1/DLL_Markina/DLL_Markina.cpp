// DLL_Markina.cpp: определяет процедуры инициализации для библиотеки DLL.
//

#include "stdafx.h"
#include "DLL_Markina.h"
#include <thread>
#include <fstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDLLMarkinaApp

BEGIN_MESSAGE_MAP(CDLLMarkinaApp, CWinApp)
END_MESSAGE_MAP()


// Создание CDLLMarkinaApp

CDLLMarkinaApp::CDLLMarkinaApp()
{
	// TODO: добавьте код создания,
	// Размещает весь важный код инициализации в InitInstance
}


// Единственный объект CDLLMarkinaApp

CDLLMarkinaApp theApp;

using namespace std;

ofstream f("logdll.txt", ios::app);

// Инициализация CDLLMarkinaApp

BOOL CDLLMarkinaApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

struct Header
{
	int Adress;
	int Size;
};

int static count_thread = 0;

extern "C" _declspec(dllexport) int __stdcall GetCountThread(int id_event) {
	if (WaitNamedPipe("\\\\.\\pipe\\MyPipe", 5000))
	{

		HANDLE hPipe = CreateFile("\\\\.\\pipe\\MyPipe", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		DWORD dwRead, dwWrite;
		WriteFile(hPipe, LPCVOID(&id_event), sizeof(id_event), &dwWrite, NULL);

		f << "Отправили событие " << id_event << endl;
		ReadFile(hPipe, LPVOID(&count_thread), sizeof(count_thread), &dwRead, NULL); // получить количество потоков

		CloseHandle(hPipe);

		return count_thread;
	} else return -1;
}

extern "C" _declspec(dllexport) int __stdcall Start(int id_event) {
	f<<"Пришло событие    " << id_event << endl;
	if (WaitNamedPipe("\\\\.\\pipe\\MyPipe", 5000))
	{
		f << "Подключились к каналу" << endl;
		HANDLE hPipe = CreateFile("\\\\.\\pipe\\MyPipe", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		DWORD dwRead, dwWrite;
		WriteFile(hPipe, LPCVOID(&id_event), sizeof(id_event), &dwWrite, NULL);

		f << "Отправили событие " << id_event << endl;
		ReadFile(hPipe, LPVOID(&count_thread), sizeof(count_thread), &dwRead, NULL); // получить количество потоков

		f << "Пришло потоков " << count_thread << endl;

		CloseHandle(hPipe);
	}

	return count_thread;
}

extern "C" _declspec(dllexport) int __stdcall Stop(int id_event) {
	if (WaitNamedPipe("\\\\.\\pipe\\MyPipe", 5000)) {

		HANDLE hPipe = CreateFile("\\\\.\\pipe\\MyPipe", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		DWORD dwRead, dwWrite;

		if (count_thread != 0) {
			f << "Отправили событие " << id_event << endl;
			WriteFile(hPipe, LPCVOID(&id_event), sizeof(id_event), &dwWrite, NULL);
			ReadFile(hPipe, LPVOID(&count_thread), sizeof(count_thread), &dwRead, NULL); // получить количество потоков
			f << "Пришло потоков " << count_thread << endl;
		}

		CloseHandle(hPipe);
	}
	return count_thread;
}

extern "C" _declspec(dllexport) bool __stdcall Send(int id_event, int adr, char* msg ) {
	if (WaitNamedPipe("\\\\.\\pipe\\MyPipe", 5000)) {
		
		HANDLE hPipe = CreateFile("\\\\.\\pipe\\MyPipe", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		DWORD dwRead, dwWrite;

		if (count_thread != 0) {
			Header h;
			h.Adress = adr;
			h.Size = strlen(msg) + 1;

			DWORD dwWrite;
			WriteFile(hPipe, LPCVOID(&id_event), sizeof(id_event), &dwWrite, NULL);
			WriteFile(hPipe, LPCVOID(&h.Adress), sizeof(h.Adress), &dwWrite, NULL); // отправить адрес
			WriteFile(hPipe, LPCVOID(&h.Size), sizeof(h.Size), &dwWrite, NULL); // отправить размер
			WriteFile(hPipe, LPCVOID(msg), sizeof(h.Size), &dwWrite, NULL); // отправить сообщение
		}

		CloseHandle(hPipe);
	}
	return true;
}


