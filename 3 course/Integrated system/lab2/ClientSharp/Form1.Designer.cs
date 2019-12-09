namespace ClientSharp
{
    partial class Form1
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.label1 = new System.Windows.Forms.Label();
            this.Exit_btn = new System.Windows.Forms.Button();
            this.Send_btn = new System.Windows.Forms.Button();
            this.box_msg = new System.Windows.Forms.TextBox();
            this.Logon_btn = new System.Windows.Forms.Button();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.lbl_msg = new System.Windows.Forms.Label();
            this.box_name = new System.Windows.Forms.TextBox();
            this.lbl_name = new System.Windows.Forms.Label();
            this.lbl_to = new System.Windows.Forms.Label();
            this.box_to = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(64, 62);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(46, 17);
            this.label1.TabIndex = 0;
            this.label1.Text = "label1";
            // 
            // Exit_btn
            // 
            this.Exit_btn.Location = new System.Drawing.Point(321, 397);
            this.Exit_btn.Name = "Exit_btn";
            this.Exit_btn.Size = new System.Drawing.Size(75, 29);
            this.Exit_btn.TabIndex = 2;
            this.Exit_btn.Text = "Sign out";
            this.Exit_btn.UseVisualStyleBackColor = true;
            this.Exit_btn.Visible = false;
            this.Exit_btn.Click += new System.EventHandler(this.Exit_btn_Click);
            // 
            // Send_btn
            // 
            this.Send_btn.Location = new System.Drawing.Point(321, 345);
            this.Send_btn.Name = "Send_btn";
            this.Send_btn.Size = new System.Drawing.Size(75, 29);
            this.Send_btn.TabIndex = 3;
            this.Send_btn.Text = "Send";
            this.Send_btn.UseVisualStyleBackColor = true;
            this.Send_btn.Visible = false;
            this.Send_btn.Click += new System.EventHandler(this.Send_btn_Click);
            // 
            // box_msg
            // 
            this.box_msg.Location = new System.Drawing.Point(276, 294);
            this.box_msg.Name = "box_msg";
            this.box_msg.Size = new System.Drawing.Size(162, 22);
            this.box_msg.TabIndex = 4;
            this.box_msg.Visible = false;
            // 
            // Logon_btn
            // 
            this.Logon_btn.Location = new System.Drawing.Point(321, 62);
            this.Logon_btn.Name = "Logon_btn";
            this.Logon_btn.Size = new System.Drawing.Size(75, 29);
            this.Logon_btn.TabIndex = 1;
            this.Logon_btn.Text = "Sign in";
            this.Logon_btn.UseVisualStyleBackColor = true;
            this.Logon_btn.Click += new System.EventHandler(this.Logon_btn_Click);
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 6000;
            this.timer1.Tick += new System.EventHandler(this.Timer1_Tick);
            // 
            // lbl_msg
            // 
            this.lbl_msg.AutoSize = true;
            this.lbl_msg.Location = new System.Drawing.Point(273, 272);
            this.lbl_msg.Name = "lbl_msg";
            this.lbl_msg.Size = new System.Drawing.Size(84, 17);
            this.lbl_msg.TabIndex = 5;
            this.lbl_msg.Text = "Сообщение";
            this.lbl_msg.Visible = false;
            // 
            // box_name
            // 
            this.box_name.Location = new System.Drawing.Point(276, 109);
            this.box_name.Name = "box_name";
            this.box_name.Size = new System.Drawing.Size(162, 22);
            this.box_name.TabIndex = 6;
            // 
            // lbl_name
            // 
            this.lbl_name.AutoSize = true;
            this.lbl_name.Location = new System.Drawing.Point(273, 136);
            this.lbl_name.Name = "lbl_name";
            this.lbl_name.Size = new System.Drawing.Size(92, 17);
            this.lbl_name.TabIndex = 7;
            this.lbl_name.Text = "Введите имя";
            // 
            // lbl_to
            // 
            this.lbl_to.AutoSize = true;
            this.lbl_to.Location = new System.Drawing.Point(273, 211);
            this.lbl_to.Name = "lbl_to";
            this.lbl_to.Size = new System.Drawing.Size(49, 17);
            this.lbl_to.TabIndex = 9;
            this.lbl_to.Text = "Кому?";
            this.lbl_to.Visible = false;
            // 
            // box_to
            // 
            this.box_to.Location = new System.Drawing.Point(276, 233);
            this.box_to.Name = "box_to";
            this.box_to.Size = new System.Drawing.Size(162, 22);
            this.box_to.TabIndex = 8;
            this.box_to.Visible = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(461, 453);
            this.Controls.Add(this.lbl_to);
            this.Controls.Add(this.box_to);
            this.Controls.Add(this.lbl_name);
            this.Controls.Add(this.box_name);
            this.Controls.Add(this.lbl_msg);
            this.Controls.Add(this.box_msg);
            this.Controls.Add(this.Send_btn);
            this.Controls.Add(this.Exit_btn);
            this.Controls.Add(this.Logon_btn);
            this.Controls.Add(this.label1);
            this.Name = "Form1";
            this.Text = "Lab2_client";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button Exit_btn;
        private System.Windows.Forms.Button Send_btn;
        private System.Windows.Forms.TextBox box_msg;
        private System.Windows.Forms.Button Logon_btn;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Label lbl_msg;
        private System.Windows.Forms.TextBox box_name;
        private System.Windows.Forms.Label lbl_name;
        private System.Windows.Forms.Label lbl_to;
        private System.Windows.Forms.TextBox box_to;
    }
}

