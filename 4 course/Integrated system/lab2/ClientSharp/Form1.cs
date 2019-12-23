using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ClientSharp
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();




        }

        int myID = -1;
        string myName = "";

        public Socket Connect()
        {
            IPEndPoint endPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 1214);
            Socket s = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            s.Connect(endPoint);
            return s;

        }

        private void Log_on()
        {

        }

        private void Send_btn_Click(object sender, EventArgs e)
        {
            int to = Convert.ToInt32(box_to.Text);
            string msg = box_msg.Text;

            using (Socket s = Connect())
            {
                Message.msgSend(s, (Address)to, (Address)myID, Messages.M_TEXT, msg.Length, msg);

                label1.Text = label1.Text + "\n";
                label1.Text = label1.Text + "Send";
                s.Close();
            }

        }

        private void Logon_btn_Click(object sender, EventArgs e)
        {
            myName = box_name.Text;
            using (Socket s = Connect())
            {
                Message.msgSend(s, 0, 0, Messages.M_INIT, myName.Length, myName);
                myID = (int)Message.msgReceive(s).m_header.m_to;

                label1.Text = String.Format("Hello, {0}! Your ID is: {1}", myName, myID);

                lbl_name.Visible = false;
                box_name.Visible = false;
                Logon_btn.Visible = false;

                lbl_msg.Visible = true;
                box_msg.Visible = true;
                Send_btn.Visible = true;
                Exit_btn.Visible = true;
                lbl_to.Visible = true;
                box_to.Visible = true;

                s.Close();
            }

        }

        private void Exit_btn_Click(object sender, EventArgs e)
        {

        }


        private void Timer1_Tick(object sender, EventArgs e)
        {
            using(Socket s = Connect())
            {
                Message.msgSend(s, Address.A_BROCKER, (Address)myID, Messages.M_GETDATA);
                Message m = Message.msgReceive(s);
                if (((int)m.m_header.m_to == myID || m.m_header.m_to == Address.A_ALL) && m.m_data != "")
                {
                    label1.Text = label1.Text + "\n";
                    label1.Text = label1.Text + String.Format("Message: {0} From: {1}", m.m_data, (int)m.m_header.m_from);
                }

                s.Close();
            }

        }
    }
}
