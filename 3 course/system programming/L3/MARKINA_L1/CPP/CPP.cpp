// CPP.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include "CPP.h"
#include <vector>
#include <fstream>
#include <thread>
#include <string>
#include "..//DLL_Markina/DLL_Markina.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Единственный объект приложения

CWinApp theApp;

int ThreadAdr = -2; //Адрес
char* msg;

HANDLE mMutex = CreateMutex(NULL, FALSE, NULL); // синхронизация
HANDLE hLocalConf = CreateEvent(NULL, FALSE, FALSE, NULL); //локальный флажок

int count_thread = 1;


using namespace std;

vector <HANDLE> vEventMsg; //события прихода сообщений для переключения между потоками
vector <HANDLE> vEventClose; //события прихода сообщений для переключения между потоками

ofstream f("log.txt");

void FileOut(string name)
{
	ofstream f(name + ".txt");
	f << msg;
	cout << "File  " << name << ".txt " << "  is create" << endl;
	f.close();

}

DWORD WINAPI NewThread(LPVOID lpParam)
{
	HANDLE EventTread[2]; //два события для каждого потока. Закрытие и приход сообщения
	int id = (int)lpParam;

	EventTread[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
	EventTread[1] = CreateEvent(NULL, FALSE, FALSE, NULL);

	vEventClose.push_back(EventTread[0]); //связь закрытия сообщения с конкретным потоком
	vEventMsg.push_back(EventTread[1]); //связь прихода сообщения с конкретным потоком

	f << "Start thread " << id << endl;
	while (true)
	{
		DWORD dwResult = WaitForMultipleObjects(2, EventTread, FALSE, INFINITE) - WAIT_OBJECT_0;

		switch (dwResult)
		{
		case 0:
		{
			vEventClose.pop_back();
			vEventMsg.pop_back();
			f << "Close thread " << id << endl;
			SetEvent(hLocalConf);
			return 0;
		}
		case 1:
		{
			ResetEvent(EventTread[1]);
			WaitForSingleObject(mMutex, INFINITE);
			FileOut(to_string(id));
			f << "Message was print. Thread " << id << endl;
			ReleaseMutex(mMutex);
			SetEvent(hLocalConf);

			break;
		}
		}
	}

	return 0;
}

void start()
{
	while (1)
	{
		HANDLE hRead = GetStdHandle(STD_INPUT_HANDLE);
		cout << "Start" << endl;
		DWORD dwRead;
		int id_event;
		ReadFile(hRead, LPVOID(&id_event), sizeof(id_event), &dwRead, NULL);

		switch (id_event)
		{ //Start
		case 0: {
			CloseHandle(CreateThread(NULL, FALSE, NewThread, (LPVOID)count_thread++, 0, NULL)); //связываем поток с событием
			break;
		}
		 //Stop
		case 1: {
			SetEvent(vEventClose.back());
			WaitForSingleObject(hLocalConf, INFINITE);
			count_thread--;
			break;
		}
		// Quit
		case 2: {
			for (auto it : vEventClose)
				SetEvent(it);
			CloseHandle(hLocalConf);
			CloseHandle(mMutex);
			CloseHandle(hRead);
			count_thread = 0;
			exit(0);
		}
		 //Message
		case 3: {
			ReadFile(hRead, LPVOID(&ThreadAdr), sizeof(ThreadAdr), &dwRead, NULL);
			int size = 0;
			ReadFile(hRead, LPVOID(&size), sizeof(size), &dwRead, NULL);
			msg = new char[size];
			ReadFile(hRead, LPVOID(msg), size, &dwRead, NULL);

			f << "Receive message:  " << msg << "  for thread   " << ThreadAdr + 1 << endl;

			if (ThreadAdr == -2) {
				f << "All Thread" << endl;
				for (auto it : vEventMsg) {
					SetEvent(it);
					WaitForSingleObject(hLocalConf, INFINITE);
				}

			}
			else if (ThreadAdr == -1) {
				f << "Main Thread: " << msg << endl;
			}
			else if (ThreadAdr >= 0) {
				SetEvent(vEventMsg[ThreadAdr]);
			}
			break;


		}
		default: {
			break;
		}
		}
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
			// TODO: вставьте сюда код для приложения.
			wprintf(L"Критическая ошибка: сбой при инициализации MFC\n");
			nRetCode = 1;
		}
		else
		{
			start();
			// TODO: вставьте сюда код для приложения.
		}
	}
	else
	{
		// TODO: измените код ошибки в соответствии с потребностями
		wprintf(L"Критическая ошибка: сбой GetModuleHandle\n");
		nRetCode = 1;
	}

	return nRetCode;
}