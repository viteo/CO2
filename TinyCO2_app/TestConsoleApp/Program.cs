using System;
using System.ComponentModel;
using System.IO.Ports;
using System.Text;
using System.Threading;
using MHZ19uart;

namespace TestConsoleApp
{
    class Program
    {
        private static void PropertyChangedHandler(object sender, PropertyChangedEventArgs args)
        {
            if (args.PropertyName == "Data")
                Console.WriteLine(((MHZ19)sender).GasConcn + " | " + 
                    ((MHZ19)sender).Temperature + " | " +
                    DateTime.Now.ToLongTimeString());
        }
        static void Main(string[] args)
        {
            using (var sensor = new MHZ19("COM5"))
            {
                sensor.PropertyChanged += PropertyChangedHandler;
                //sensor.StartPolling();

                Console.WriteLine("Press any key to stop . . .");
                Console.ReadLine();
            }
        }
    }
}
