using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;
using System.Threading;
using System.Runtime.InteropServices;

namespace Lisina_SP1
{
    public partial class Form1 : Form
    {
        [DllImport("MFCLib_Lab2.dll")]
        public static extern void Send(int adr, int size, StringBuilder message);


        Process ChildProcess = null;
        EventWaitHandle evStart = new EventWaitHandle(false, EventResetMode.AutoReset, "EventStart");
        EventWaitHandle evSend = new EventWaitHandle(false, EventResetMode.AutoReset, "EventSend");
        EventWaitHandle evStop = new EventWaitHandle(false, EventResetMode.AutoReset, "EventStop");
        EventWaitHandle evQuit = new EventWaitHandle(false, EventResetMode.AutoReset, "EventQuit");
        EventWaitHandle evConfirm = new EventWaitHandle(false, EventResetMode.AutoReset, "EventConfirm");
        EventWaitHandle evDelete = new EventWaitHandle(false, EventResetMode.AutoReset, "EventDelete");


        int i = 0;

        public Form1()
        {

            InitializeComponent();

        }

      


        private void button1_Click(object sender, EventArgs e)
        {
            if (ChildProcess == null || ChildProcess.HasExited)
            {
                i = 0;
                listBox1.Items.Clear();
                ChildProcess = Process.Start("MFCApplication.exe");
                listBox1.Items.Add("Главный поток");
                i++;
            }
            else
            {
                int a;
                if (!Int32.TryParse(textBox1.Text, out a))
                {
                    MessageBox.Show("Enter an integer value!!!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                else
                {
                    a = Convert.ToInt32(textBox1.Text);
                }
                for (int j = 0; j < a; j++)
                {
                    evStart.Set();
                    evConfirm.WaitOne();
                    listBox1.Items.Add("Поток " + i.ToString());
                    i++;
                    label2.Text = i.ToString();
                }

            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (ChildProcess != null && !ChildProcess.HasExited)
            {
                
                evStop.Set();

                evConfirm.WaitOne();
                
                if (i > 1)
                {
                    listBox1.Items.RemoveAt(i-1);
                    i--;
                    label2.Text = i.ToString();

                }
                else
                {
                    listBox1.Items.Clear();
                    ChildProcess.Kill();
                }

                


            }
            else
            {
                i = 0;
                listBox1.Items.Clear();
            }
        }


        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            evQuit.Set();
            ChildProcess.Close();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            
            label2.Text = i.ToString();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (ChildProcess != null && !ChildProcess.HasExited)
            {
                //int inx = listBox1.SelectedIndex;
                                          
                int inx = listBox1.SelectedIndex;
                

                if (inx >-2)
                {
                    string message = textBox2.Text;
                    StringBuilder msg = new StringBuilder(message);
                    Send(inx, message.Length, msg);
                    evSend.Set();
                    evConfirm.WaitOne();
                }
                else
                {
                    MessageBox.Show("None of the recipients is selected", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }

            }
        }

    }

}
