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

namespace L1Sharp
{
    public partial class Form1 : Form
    {
        Process ChildProcess = null;
        EventWaitHandle evStart = new EventWaitHandle(false, EventResetMode.AutoReset, "MyEventStart");
        EventWaitHandle evConfirm = new EventWaitHandle(false, EventResetMode.AutoReset, "MyEventConfirm");
        public Form1()
        {
            InitializeComponent();
        }

        private void Start_Click(object sender, EventArgs e)
        {
            if (ChildProcess == null || ChildProcess.HasExited)
            {
                ChildProcess = Process.Start("L1.exe");
            }
            else
            {
                evStart.Set();
                evConfirm.WaitOne();
            }
        }
    }
}
