namespace Animation
{
    partial class Preview
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.Panel1 = new System.Windows.Forms.Panel();
            this.StatusStrip1 = new System.Windows.Forms.StatusStrip();
            this.ToolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.ToolStripStatusLabel2 = new System.Windows.Forms.ToolStripStatusLabel();
            this.Button3 = new System.Windows.Forms.Button();
            this.Button1 = new System.Windows.Forms.Button();
            this.MenuStrip1 = new System.Windows.Forms.MenuStrip();
            this.MenuStrip2 = new System.Windows.Forms.MenuStrip();
            this.StatusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // Panel1
            // 
            this.Panel1.BackColor = System.Drawing.SystemColors.Desktop;
            this.Panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.Panel1.Location = new System.Drawing.Point(0, 48);
            this.Panel1.Name = "Panel1";
            this.Panel1.Size = new System.Drawing.Size(507, 353);
            this.Panel1.TabIndex = 10;
            // 
            // StatusStrip1
            // 
            this.StatusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolStripStatusLabel1,
            this.ToolStripStatusLabel2});
            this.StatusStrip1.Location = new System.Drawing.Point(0, 401);
            this.StatusStrip1.Name = "StatusStrip1";
            this.StatusStrip1.Size = new System.Drawing.Size(507, 25);
            this.StatusStrip1.TabIndex = 9;
            this.StatusStrip1.Text = "StatusStrip1";
            // 
            // ToolStripStatusLabel1
            // 
            this.ToolStripStatusLabel1.Name = "ToolStripStatusLabel1";
            this.ToolStripStatusLabel1.Size = new System.Drawing.Size(125, 20);
            this.ToolStripStatusLabel1.Text = "Animation Name:";
            // 
            // ToolStripStatusLabel2
            // 
            this.ToolStripStatusLabel2.Name = "ToolStripStatusLabel2";
            this.ToolStripStatusLabel2.Size = new System.Drawing.Size(66, 20);
            this.ToolStripStatusLabel2.Text = "Example";
            // 
            // Button3
            // 
            this.Button3.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.Button3.Location = new System.Drawing.Point(266, 1);
            this.Button3.Name = "Button3";
            this.Button3.Size = new System.Drawing.Size(229, 23);
            this.Button3.TabIndex = 8;
            this.Button3.Text = "Single Play";
            this.Button3.UseVisualStyleBackColor = true;
            this.Button3.Click += new System.EventHandler(this.Button3_Click);
            // 
            // Button1
            // 
            this.Button1.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.Button1.Location = new System.Drawing.Point(12, 1);
            this.Button1.Name = "Button1";
            this.Button1.Size = new System.Drawing.Size(240, 23);
            this.Button1.TabIndex = 6;
            this.Button1.Text = "Play Looping";
            this.Button1.UseVisualStyleBackColor = true;
            this.Button1.Click += new System.EventHandler(this.Button1_Click);
            // 
            // MenuStrip1
            // 
            this.MenuStrip1.BackColor = System.Drawing.SystemColors.Desktop;
            this.MenuStrip1.Location = new System.Drawing.Point(0, 24);
            this.MenuStrip1.Name = "MenuStrip1";
            this.MenuStrip1.Size = new System.Drawing.Size(507, 24);
            this.MenuStrip1.TabIndex = 5;
            this.MenuStrip1.Text = "MenuStrip1";
            // 
            // MenuStrip2
            // 
            this.MenuStrip2.Location = new System.Drawing.Point(0, 0);
            this.MenuStrip2.Name = "MenuStrip2";
            this.MenuStrip2.Size = new System.Drawing.Size(507, 24);
            this.MenuStrip2.TabIndex = 11;
            this.MenuStrip2.Text = "MenuStrip2";
            // 
            // Preview
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(507, 426);
            this.ControlBox = false;
            this.Controls.Add(this.Panel1);
            this.Controls.Add(this.StatusStrip1);
            this.Controls.Add(this.Button3);
            this.Controls.Add(this.Button1);
            this.Controls.Add(this.MenuStrip1);
            this.Controls.Add(this.MenuStrip2);
            this.Name = "Preview";
            this.Text = "Preview";
            this.StatusStrip1.ResumeLayout(false);
            this.StatusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        internal System.Windows.Forms.Panel Panel1;
        internal System.Windows.Forms.StatusStrip StatusStrip1;
        internal System.Windows.Forms.ToolStripStatusLabel ToolStripStatusLabel1;
        internal System.Windows.Forms.ToolStripStatusLabel ToolStripStatusLabel2;
        internal System.Windows.Forms.Button Button3;
        internal System.Windows.Forms.Button Button1;
        internal System.Windows.Forms.MenuStrip MenuStrip1;
        internal System.Windows.Forms.MenuStrip MenuStrip2;

    }
}