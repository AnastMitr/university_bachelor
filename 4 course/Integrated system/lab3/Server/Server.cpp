// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "Server.h"
#include "Message.h"
#include "Users.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

int gMaxID = A_USER;

map<int, shared_ptr<Users>> gUsers; //массив с пользователями

void checkConnection(SOCKET hSocket) {

	CSocket Socket;
	Socket.Attach(hSocket);

	while (1) {
		cout << "Check Connection" << endl;
		int nowTime = clock();

			for (auto it = gUsers.begin(); it != gUsers.end(); ) //пустой третий блок заголовка for – это принципиально
			{
				if (abs(nowTime - it->second->time) / CLOCKS_PER_SEC > 15)
				{
					cout << it->second->m_Name << " was disconected to the server" << endl;
					gUsers.erase(it++); // трюк с удалением элемента уже после увеличения итератора
				
				}
				else
				{
					++it; // обычное увеличение итератора
				}
			}
		Sleep(1600000);
	}
}

int WEB_IP;

void ProcessClient(SOCKET hSocket)
{
	CSocket Client;
	Client.Attach(hSocket);
	Message msg;

		switch (msg.Receive(Client))
		{
			
			case M_INIT:
			{	
				auto pUsers = make_shared<Users>(++gMaxID, msg.m_Data);
				gUsers[pUsers->m_ID] = pUsers;
				cout << "INIT " << pUsers->m_ID << endl;
				gUsers[pUsers->m_ID]->time = clock(); //отметка времени
				Message::SendMessage(Client, pUsers->m_ID, A_BROCKER, M_INIT);
				cout << pUsers->m_Name << " joined the server" << endl;
				break;
			}
			case M_EXIT:
			{	
				gUsers.erase(msg.m_Header.m_From);
				cout << msg.m_Header.m_From << " was disconnected" << endl;
				Message::SendMessage(Client, msg.m_Header.m_From, A_BROCKER, M_CONFIRM);
				break;
			}
			case M_GETDATA:
			{	
				if (gUsers.find(msg.m_Header.m_From) != gUsers.end())
				{
					gUsers[msg.m_Header.m_From]->time = clock(); //отметка времени
					gUsers[msg.m_Header.m_From]->Send(Client);
				}
				break;
			}

			default:
			{	
				if (gUsers.find(msg.m_Header.m_From) != gUsers.end())
				{
					gUsers[msg.m_Header.m_From]->time = clock(); //отметка времени

					if (gUsers.find(msg.m_Header.m_To) != gUsers.end())
					{
						gUsers[msg.m_Header.m_To]->Add(msg);
					}
					else if (msg.m_Header.m_To == A_ALL)
					{
						for (auto it : gUsers)
						{
							if (it.second->m_ID != msg.m_Header.m_From)
							{
								it.second->Add(msg);
							}
						}
					}
					Message::SendMessage(Client, msg.m_Header.m_From, A_BROCKER, M_CONFIRM);
				}
				break;
			}
		}
	}

void start()
{
	AfxSocketInit();
	CSocket Server;
	Server.Create(1214);

	
	thread time(checkConnection, Server.m_hSocket);
	time.detach();

	while (Server.Listen())
	{
		CSocket s;
		Server.Accept(s);
		thread t(ProcessClient, s.Detach());
		t.detach();
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
            // TODO: code your application's behavior here.
            wprintf(L"Fatal Error: MFC initialization failed\n");
            nRetCode = 1;
        }
        else
		{
			start();
			
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
