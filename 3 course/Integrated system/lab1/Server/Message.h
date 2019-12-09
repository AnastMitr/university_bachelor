#pragma once

enum Addresses {
	A_BROCKER = 0,
	A_ALL = 1,
	A_USER = 100
};

enum Messages {
	M_INIT,
	M_EXIT,
	M_GETDATA,
	M_NODATA,
	M_TEXT,
	M_CONFIRM
};

struct Header
{
	int m_To;
	int m_From;
	int m_Type;
	int m_Size;
};

struct Message
{
	Header m_Header;
	string m_Data;
	Message()
	{
		m_Header = { 0 };
	}
	Message(int To, int From, int Type = M_TEXT, const string& Data = "")
		:m_Data(Data)
	{
		m_Header.m_To = To;
		m_Header.m_From = From;
		m_Header.m_Type = Type;
		m_Header.m_Size = Data.length();
	}
	void Send(CSocket& s)
	{
		s.Send(&m_Header, sizeof(Header));
		if (m_Header.m_Size)
		{
			s.Send(m_Data.c_str(), m_Header.m_Size+1);
		}
	}
	int Receive(CSocket& s)
	{
		s.Receive(&m_Header, sizeof(Header));
		if (m_Header.m_Size)
		{
			char* pBuff = new char[m_Header.m_Size + 1];
			s.Receive(pBuff, m_Header.m_Size + 1);
			m_Data = pBuff;
			delete[] pBuff;
		}
		return m_Header.m_Type;
	}
	static void SendMessage(CSocket& s, int To, int From, int Type = M_TEXT, const string& Data = "")
	{
		Message msg(To, From, Type, Data);
		msg.Send(s);
	}
};
