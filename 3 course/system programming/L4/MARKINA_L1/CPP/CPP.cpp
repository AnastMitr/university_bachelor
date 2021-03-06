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

int count_thread = 0;


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
			FileOut(to_string(id+1));
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
	cout << "Start" << endl;

	//while (1)
	//{
		HANDLE hPipe = CreateNamedPipe("\\\\.\\pipe\\MyPipe", PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 1024, 1024, 0, NULL);
		cout << hPipe << " hPipe" << endl;

		while (1) {
			DWORD dwRead;
			int id_event = -1;
			ConnectNamedPipe(hPipe, NULL);
			ReadFile(hPipe, LPVOID(&id_event), sizeof(id_event), &dwRead, NULL);

			switch (id_event)
			{ //Start
			case 0: {
				cout << id_event << " Start" << endl;
				CloseHandle(CreateThread(NULL, FALSE, NewThread, (LPVOID)count_thread++, 0, NULL)); //связываем поток с событием
				DWORD dwWrite;
				WriteFile(hPipe, LPCVOID(&count_thread), sizeof(count_thread), &dwWrite, NULL);
				cout << count_thread << " Treads" << endl;
				FlushFileBuffers(hPipe);
				DisconnectNamedPipe(hPipe);

				break;
			}
					//Stop
			case 1: {
				cout << id_event << " Stop"<< endl;
				SetEvent(vEventClose.back());
				WaitForSingleObject(hLocalConf, INFINITE);
				count_thread--;

				DWORD dwWrite;
				WriteFile(hPipe, LPCVOID(&count_thread), sizeof(count_thread), &dwWrite, NULL);
				FlushFileBuffers(hPipe);
				DisconnectNamedPipe(hPipe);

				break;
			}
					//Message
			case 3: {
				ReadFile(hPipe, LPVOID(&ThreadAdr), sizeof(ThreadAdr), &dwRead, NULL);
				int size = 0;
				ReadFile(hPipe, LPVOID(&size), sizeof(size), &dwRead, NULL);
				msg = new char[size];
				ReadFile(hPipe, LPVOID(msg), size, &dwRead, NULL);
				FlushFileBuffers(hPipe);
				DisconnectNamedPipe(hPipe);

				f << "Receive message:  " << msg << "  for thread   " << ThreadAdr + 2 << endl;

				if (ThreadAdr == -2) {
					f << "All Thread" << endl;
					for (auto it : vEventMsg) {
						SetEvent(it);
						WaitForSingleObject(hLocalConf, INFINITE);
					}

				}
				else if (ThreadAdr == -1) {
					cout << "Main Thread: " << msg << endl;
				}
				else if (ThreadAdr >= 0) {
					SetEvent(vEventMsg[ThreadAdr]);
				}
				break;


			}
			
			// Return count Pipe
			case 4: {
				DWORD dwWrite;
				WriteFile(hPipe, LPCVOID(&count_thread), sizeof(count_thread), &dwWrite, NULL);
				FlushFileBuffers(hPipe);
				DisconnectNamedPipe(hPipe);

				break;
			}
			default: {
				break;
			}
			}
		}
	/*}*/

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