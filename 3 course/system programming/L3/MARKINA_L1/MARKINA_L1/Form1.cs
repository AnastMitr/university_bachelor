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
        int count_process = 0; // нет ни одного процесса
        Process CPP_Process = null;  // 

        // Импорт функционала из DLL
        [DllImport("DLL_Markina.dll", CharSet = CharSet.Ansi)]
        private static extern void Send(int evID,int adr, StringBuilder msg);

        [DllImport("DLL_Markina.dll", CharSet = CharSet.Ansi)]
        private static extern void Start(int evID);

        [DllImport("DLL_Markina.dll", CharSet = CharSet.Ansi)]
        private static extern void Stop(int evID);

        [DllImport("DLL_Markina.dll", CharSet = CharSet.Ansi)]
        private static extern void Quit(int evID);

    public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {  
            if (count_process == 0)
            {
                Start(0);
                comboBox1.Items.AddRange(new string[] { "ALL", "MAIN" });
                comboBox1.SelectedIndex = 0;
                count_process = 1;
                comboBox1.Enabled = true;
            }
            else
            {
                int n;
                if (int.TryParse(textBox1.Text, out n))
                {
                    for (int i = 0; i < n; i++)
                    {
                        Start(0);
                        comboBox1.Items.Add(count_process);
                        count_process++;
                        label2.Text = label2.Text + "\n Open thread number " + (count_process - 1).ToString();
                    }
                }
                else
                    MessageBox.Show("N может быть только числом!");
            }
            //}

        }

        private void button2_Click(object sender, EventArgs e)
        {
            int back_index = comboBox1.Items.Count;

                if (count_process >1)
                {   
                    count_process--;
                    comboBox1.Items.RemoveAt(back_index - 1);
                    Stop(1);
                    label2.Text = label2.Text + "\n Deleted thread number " + count_process.ToString();
                }

                if (count_process == 1)
                {
                    comboBox1.Enabled = false;
                    Quit(2);
                    count_process = 0;
                }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
         
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            Quit(2);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            int index = comboBox1.SelectedIndex;
            string msg = textBox2.Text;
            StringBuilder str = new StringBuilder(msg);
            Send(3,index - 2, str);
        }
    }
}
