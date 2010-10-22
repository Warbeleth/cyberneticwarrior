using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace SGP_Map_Editor
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
            //Application.Run(new Map_Editor());

            //Make the form
            Map_Editor Editor = new Map_Editor();

            //Display the form
            Editor.Show();

            //Start the main program loop
            while (Editor.Looping)
            {
                //Call the forms update
                Editor.Update();

                //Call the forms render
                Editor.Render();
    
                //Handle events
                Application.DoEvents();
            }

            Editor.Close();
        }
    }
}
