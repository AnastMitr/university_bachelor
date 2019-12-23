using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MARKINA_L1
{
    public partial class Form1 : Form
    {
        EventWaitHandle evStart = new EventWaitHandle(false, EventResetMode.AutoReset, "Start_event");
        EventWaitHandle evConfirm = new EventWaitHandle(false, EventResetMode.AutoReset, "Confirm_event");
        EventWaitHandle evStop = new EventWaitHandle(false, EventResetMode.AutoReset, "Stop_event");
        EventWaitHandle evQuit = new EventWaitHandle(false, EventResetMode.AutoReset, "Quit_event");
        EventWaitHandle Ev = new EventWaitHandle(false, EventResetMode.ManualReset, "MyEvent");
        EventWaitHandle evSend = new EventWaitHandle(false, EventResetMode.AutoReset, "Send_event");

        int count_process = 0; // нет ни одного процесса
        Process CPP_Process = null;  // 

        // Импорт функционала из DLL
        [DllImport("DLL_Markina.dll", CharSet = CharSet.Ansi)]
        private static extern void Send(int adr, StringBuilder msg);

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {   //HasExited - завершен, если true
            if (CPP_Process == null || CPP_Process.HasExited)
            {
                label2.Text = "";
                comboBox1.Items.Clear();
                CPP_Process = Process.Start("CPP.exe");
                comboBox1.Items.AddRange(new string[] { "ALL", "MAIN"});
                comboBox1.SelectedIndex = 0;
                count_process = 1;
                comboBox1.Enabled = true;
            }
            else
            {   int n;
                if (int.TryParse(textBox1.Text, out n))
                {
                    for(int i = 0; i < n; i++)
                    {
                        evStart.Set();
                        comboBox1.Items.Add(count_process);
                        evConfirm.WaitOne();
                        count_process++;
                        label2.Text = label2.Text + "\n Open thread number " + (count_process-1).ToString();
                    }
                }
                else
                    MessageBox.Show("N может быть только числом!");
            }

        }

        private void button2_Click(object sender, EventArgs e)
        {
            int back_index = comboBox1.Items.Count;
            if (CPP_Process != null && !CPP_Process.HasExited)
            {
                if (count_process > 1)
                {
                    evStop.Set();
                    count_process--;
                    comboBox1.Items.RemoveAt(back_index - 1);
                    evConfirm.WaitOne();
                    label2.Text = label2.Text + "\n Deleted thread number " + count_process.ToString();
                }

                if (count_process == 1)
                {
                    evQuit.Set();
                    comboBox1.Enabled = false;
                    evConfirm.WaitOne();
                }
                
            }
            else
            {
                //CPP_Process.Close();
                MessageBox.Show("There are no threads!");
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
         
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            evQuit.Set();
            CPP_Process.Close();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            int index = comboBox1.SelectedIndex;
            string msg = textBox2.Text;
            StringBuilder str = new StringBuilder(msg);
            Send(index - 2, str);
            evSend.Set();
            evConfirm.WaitOne();
        }
    }
}
