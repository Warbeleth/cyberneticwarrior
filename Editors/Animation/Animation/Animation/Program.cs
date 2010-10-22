using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace Animation
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
            //Application.Run(new AnimationEditor());

            //Make the form
            AnimationEditor Editor = new AnimationEditor();

            // Display the form
            Editor.Show();

            //Start the main program loop
            while (Editor.Looping)
            {
                //Update
                Editor.Update();

                //Render
                Editor.Render();

                //Events
                Application.DoEvents();
            }
        }
    }
}
