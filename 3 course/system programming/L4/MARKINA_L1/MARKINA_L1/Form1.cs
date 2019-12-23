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

        // Импорт функционала из DLL
        [DllImport("DLL_Markina.dll", CharSet = CharSet.Ansi)]
        private static extern bool Send(int evID,int adr, StringBuilder msg);

        [DllImport("DLL_Markina.dll", CharSet = CharSet.Ansi)]
        private static extern int Start(int evID);

        [DllImport("DLL_Markina.dll", CharSet = CharSet.Ansi)]
        private static extern int Stop(int evID);

        [DllImport("DLL_Markina.dll", CharSet = CharSet.Ansi)]
        private static extern int Quit(int evID);

        [DllImport("DLL_Markina.dll", CharSet = CharSet.Ansi)]
        private static extern int GetCountThread(int evID);

        void ReloadCComboBox()
        {
            comboBox1.Items.Clear();
            comboBox1.Items.AddRange(new string[] { "ALL", "MAIN" });
            comboBox1.SelectedIndex = 0;

            count_process = GetCountThread(4);
            if (count_process != 0)
            {
                button2.Enabled = true;
                for (int i = 1; i <= count_process; i++)
                    comboBox1.Items.Add(i);

            }
        }


        public Form1()
        {
            InitializeComponent();

            button2.Enabled = false;
            ReloadCComboBox();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            ReloadCComboBox();
            int n;
                if (int.TryParse(textBox1.Text, out n))
                {
                    
                for (int i = 0; i < n; i++)
                    {
                        count_process = Start(0);
                        comboBox1.Items.Add(count_process);
                        label2.Text = label2.Text + "\n Open thread number " + count_process.ToString();
                        button2.Enabled = true;
                    }

                }
                else
                    MessageBox.Show("N может быть только числом!");
             
        }

        private void button2_Click(object sender, EventArgs e)
        {
            ReloadCComboBox();

            int back_index = comboBox1.Items.Count;
            comboBox1.Items.RemoveAt(back_index - 1);
            count_process = Stop(1);
            label2.Text = label2.Text + "\n Deleted thread number " + (count_process+1).ToString();

            if (count_process == 0)
            {
                button2.Enabled = false;
                return;
            }
            
        }

        private void Form1_Load(object sender, EventArgs e)
        {
         
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
         
        }

        private void button3_Click(object sender, EventArgs e)
        {
            ReloadCComboBox();
            if (count_process != 0)
            {
                int index = comboBox1.SelectedIndex;
                string msg = textBox2.Text;
                StringBuilder str = new StringBuilder(msg);
                bool flag = Send(3, index - 2, str);

                if (flag)
                  label2.Text = label2.Text + "\n Message send thread " + (index-2).ToString();
            }
        }
    }
}
