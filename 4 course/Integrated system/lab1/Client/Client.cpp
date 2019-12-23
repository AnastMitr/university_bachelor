// Client.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "Client.h"
#include "..//Server/framework.h"
#include "pch.h"
#include "..//Server/Message.h"
#include "c_person.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Единственный объект приложения

CWinApp theApp;

using namespace std;


void Menu() {

	cout << "1: Send message" << endl;
	cout << "2: Sign out" << endl;

}

void MyThread(int ID)
{
	while (1) {

		CSocket t_client;
		t_client.Create();

		Message m;
		if (t_client.Connect("127.0.0.1", 1214)) {

			m.SendMessageA(t_client, A_BROCKER, ID, M_GETDATA);
			m.Receive(t_client);

			if ((m.m_Header.m_To == ID || m.m_Header.m_To == A_ALL) && m.m_Data != "") {
				cout << "You received message from: "<< m.m_Header.m_From << " Message: " << m.m_Data << endl;
			}

			t_client.Close();
		}
		else
			cout << "Error connetction" << endl;

		Sleep(7000);

	}
}

char reg() {
	cout << "You must register to continue y//n" << endl;
	char a = ' ';
	while (1) {
		cin >> a;
		if ((a != 'y' && a != 'Y' && a != 'n' && a != 'N'))
		{
			cout << "Repeat, please!" << endl;

		} else	
			return a;
	}
}

int start() {

	char a = reg();
	if (a == 'y' || a == 'Y') {
		AfxSocketInit();
		CSocket client;
		client.Create();

		if (!client.Connect("127.0.0.1", 1214))
			return 0;

		Message msg;
		string name;
		cout << "What's your name?" << endl;
		cin >> name;

		msg.SendMessageA(client, 0, 0, M_INIT, name);
		msg.Receive(client);

		c_person iPerson = c_person(msg.m_Header.m_To, name);

		cout << "Your name: " << iPerson.p_name << endl;
		cout << "Your ID: " << iPerson.p_ID << endl;

		thread t(MyThread, iPerson.p_ID);
		t.detach();

		client.Close();

			while (1) {

				CSocket client;
				client.Create();

				Menu();
				int a;
				while (!(cin >> a)) {
					cout << "Only number" << endl;
					cin.clear();
					cin.ignore();
				}


				switch (a)
				{
				case 1: {
					cout << "To? If you enter 100 then your message was sent to all person" << endl;
					int To;
					cin >> To;

					if (To == 100)
						To = A_ALL;

					cout << "What?" << endl;
					string data;
					cin.ignore();
					getline(cin,data);
	
					if (!client.Connect("127.0.0.1", 1214))
						return 0;

					 msg.SendMessageA(client, To, iPerson.p_ID, M_TEXT, data);
						client.Close();
					break;
				}
				case 2: {
					if (!client.Connect("127.0.0.1", 1214))
						return 0;

					msg.SendMessageA(client, A_BROCKER, iPerson.p_ID, M_EXIT);
					msg.Receive(client);
					if (msg.m_Header.m_Type == M_CONFIRM)
						cout << "You was disconnected to the server" << endl;
					client.Close();
					return 0;
	
				}

				default:
					cout << "Error enter. You must enter number between 1 and 2 only" << endl;
					break;
				}
			}
	}
	else return 0;

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
			system("pause");
			
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
