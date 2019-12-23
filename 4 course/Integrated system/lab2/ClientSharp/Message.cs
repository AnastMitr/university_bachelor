using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace ClientSharp
{
    public enum Address
    {
        A_NULL = -1,
        A_BROCKER,
        A_ALL,
        A_USER = 100
    };

    public enum Messages
    {
        M_INIT,
        M_EXIT,
        M_GETDATA,
        M_NODATA,
        M_TEXT,
        M_CONFIRM,

    };
    struct Header
    {
        public Address m_to;
        public Address m_from;
        public Messages m_type;
        public int m_size;
    };

    class Message
    {
        public Header m_header;
        public string m_data;

        static private Encoding cp866 = Encoding.GetEncoding("CP866");
        const int SIZE_INT = sizeof(int);
        const int SIZE_HEADER = SIZE_INT * 4;

        public Message()
        {
        }

        public Message(Address to, Address from, Messages type, int size = 0, string data = "")
        {
            m_header.m_to = to;
            m_header.m_from = from;
            m_header.m_type = type;
            m_header.m_size = size;
            m_data = data;
        }
        public void Send(Socket s)
        {

            byte[] headerBytes = new byte[SIZE_HEADER];
            int shift = 0;
            Array.Copy(BitConverter.GetBytes((int)m_header.m_to), 0, headerBytes, shift++ * SIZE_INT, SIZE_INT);
            Array.Copy(BitConverter.GetBytes((int)m_header.m_from), 0, headerBytes, shift++ * SIZE_INT, SIZE_INT);
            Array.Copy(BitConverter.GetBytes((int)m_header.m_type), 0, headerBytes, shift++ * SIZE_INT, SIZE_INT);
            Array.Copy(BitConverter.GetBytes(m_header.m_size), 0, headerBytes, shift++ * SIZE_INT, SIZE_INT);

            s.Send(headerBytes, SIZE_HEADER, SocketFlags.None);
            if (m_data.Length > 0)
            {
                s.Send(cp866.GetBytes(m_data), m_data.Length, SocketFlags.None);
            }
        }
        public int Receive(Socket s)
        {
            byte[] bytes = new byte[SIZE_HEADER];
            s.Receive(bytes, SIZE_HEADER, SocketFlags.None);

            int shift = 0;
            m_header.m_to = (Address)BitConverter.ToInt32(bytes, shift++ * SIZE_INT);
            m_header.m_from = (Address)BitConverter.ToInt32(bytes, shift++ * SIZE_INT);
            m_header.m_type = (Messages)BitConverter.ToInt32(bytes, shift++ * SIZE_INT);
            m_header.m_size = BitConverter.ToInt32(bytes, shift++ * SIZE_INT);

            if (m_header.m_size >= 0)
            {
                bytes = new byte[m_header.m_size + 1];
                s.Receive(bytes, m_header.m_size + 1, SocketFlags.None);
                m_data = cp866.GetString(bytes, 0, m_header.m_size);
            }

            return (int)m_header.m_type;
        }
        static public void msgSend(Socket s, Address to, Address from, Messages type, int size = 0, string data = "")
        {
            Message mes = new Message(to, from, type, size, data);
            mes.Send(s);
        }
        static public Message msgReceive(Socket s)
        {
            Message mes = new Message();
            mes.Receive(s);
            return mes;
        }

    }
}
