using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace SGP1___Particle_Editor
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
           // Application.Run(new Form1());

            //Make the form
            Form1 theform = new Form1();

            //display the form
            theform.Show();

            while( theform.bLooping )
            {
                //Call the update functions
                theform.Update();
                theform.TimeUpdate();

                //Call the render function
                theform.Render();

                //Handle Events
                Application.DoEvents();
            }
        }
    }
}
