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

namespace servoControlArduino
{
    public partial class Form1 : Form
    {
        public Stopwatch watch { get; set; }

        public int down = 0;
            
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            watch = Stopwatch.StartNew();
            port.Open();
        }

        private void Form1_MouseMove(object sender, MouseEventArgs e)
        {
            writeToPort(new Point(e.X, e.Y));
        }

        public void writeToPort(Point coords)
        {
            if(watch.ElapsedMilliseconds > 15)
            {
                watch = Stopwatch.StartNew();

                port.Write(String.Format("X{0}Y{1}D{2}",
                (180 - coords.X / (Size.Width / 180)),
                (180 - coords.Y / (Size.Height / 180)),
                //(coords.X),
                //(coords.Y),
                (down)));

                Console.WriteLine(String.Format("X{0}Y{1}D{2}",
                (180 - coords.X / (Size.Width / 180)),
                (180 - coords.Y / (Size.Height / 180)),
                //(coords.X),
                //(coords.Y),
                (down)));
            }
        }

        private void Form1_MouseDown(object sender, MouseEventArgs e)
        {
            down = 1;
            writeToPort(new Point(e.X, e.Y));
        }

        private void Form1_MouseUp(object sender, MouseEventArgs e)
        {
            down = 0;
        }
    }
}
