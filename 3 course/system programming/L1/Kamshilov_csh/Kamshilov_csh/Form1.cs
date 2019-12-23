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

namespace Kamshilov_csh
{
    public partial class Form1 : Form
    {
        int c = 2;
        int cc = 0;

        public Form1()
        {
            InitializeComponent();
        }

        [DllImport("MFCLibrary1.dll", CharSet = CharSet.Ansi)]
        private static extern int Start(int n);

        [DllImport("MFCLibrary1.dll", CharSet = CharSet.Ansi)]
        private static extern int Stop(int n);

        [DllImport("MFCLibrary1.dll", CharSet = CharSet.Ansi)]
        private static extern int Send(int nn, int n, int s, StringBuilder message);

        [DllImport("MFCLibrary1.dll", CharSet = CharSet.Ansi)]
        private static extern void Quit(int n);

        private void button1_Click(object sender, EventArgs e)
        {
            string edit_string = textBox1.Text;
            int edit_int;
            edit_int = Convert.ToInt32(edit_string);
            listBox1.Items.Clear();
            listBox1.Items.Add("main");
            listBox1.Items.Add("all");

            for (int i = 0; i < edit_int; i++)
            {
                cc = Start(0);
                c = cc;
                // listBox1.Items.Add(c - 2);

            }
            if (c > 2)
            {
                for (int i = 1; i <= c - 2; i++)
                {
                    listBox1.Items.Add(i);
                }
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (c > 2)
            {
                cc = Stop(1);
                c = cc;
                listBox1.Items.RemoveAt(c);
            }
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            Quit(3);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            int y;
            int index = listBox1.SelectedIndex;
            if (index >= 0)
            {
                string message = textBox2.Text;
                StringBuilder arr = new StringBuilder(message);
                y = Send(2, index - 2, message.Length, arr);
            }
            else
            {
                MessageBox.Show("Not selected");
            }
        }
    }
}
