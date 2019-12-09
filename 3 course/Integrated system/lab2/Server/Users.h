#pragma once

struct Users
{
	int m_ID;
	string m_Name;
	int time;
	queue<Message> m_Messages;
	CRITICAL_SECTION m_CS;

	Users(int ID, string Name)
		:m_ID(ID), m_Name(Name)
	{
		InitializeCriticalSection(&m_CS);
	}

	~Users()
	{
		DeleteCriticalSection(&m_CS);
	}

	void Add(Message& msg)
	{
		EnterCriticalSection(&m_CS);
		m_Messages.push(msg);
		LeaveCriticalSection(&m_CS);
	}

	void Send(CSocket& s)
	{
		EnterCriticalSection(&m_CS);
		if (m_Messages.empty())
		{
			Message::SendMessage(s, m_ID, A_BROCKER, M_NODATA);
		}
		else
		{
			m_Messages.front().Send(s);
			m_Messages.pop();
		}
		LeaveCriticalSection(&m_CS);
	}
};

