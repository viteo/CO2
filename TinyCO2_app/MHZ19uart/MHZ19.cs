using System;
using System.ComponentModel;
using System.IO.Ports;
using System.Runtime.CompilerServices;
using System.Threading;

namespace MHZ19uart
{
    public class MHZ19 : INotifyPropertyChanged, IDisposable
    {
        private SerialPort _serialPort;
        private Timer _pollingTimer;
        private int _gasConcn;
        private int _temperature;
        private static readonly byte[] dataRequest = { 0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79 };
        //public static readonly byte[] calibZero = { 0xff, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78 };
        //public static readonly byte[] calibSpan = { 0xff, 0x01, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77 };
        //public static readonly byte[] calibAuto_on = { 0xff, 0x01, 0x79, 0xA0, 0x00, 0x00, 0x00, 0x00, 0xe6 };
        //public static readonly byte[] calibAuto_off = { 0xff, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86 };

        /// <summary>Gas Concentration</summary>
        /// <value>PPM - particles per million</value>
        public int GasConcn { get => _gasConcn; private set { _gasConcn = value; OnPropertyChanged(); } }
        /// <summary>Sensor temperature</summary>
        /// <value>Celsius</value>
        public int Temperature { get => _temperature; private set { _temperature = value; OnPropertyChanged(); } }

        public MHZ19(string portName)
        {
            _serialPort = new SerialPort(portName);
            _serialPort.BaudRate = 9600;
            _serialPort.DataBits = 8;
            _serialPort.StopBits = StopBits.One;
            _serialPort.Open();
            _serialPort.DataReceived += DataReceivedHandler;
        }

        public void RequestNewData()
        {
            Write(dataRequest);
        }

        public void StartPolling([System.ComponentModel.DataAnnotations.Range(6, int.MaxValue)] int timeout = 8)
        {
            _pollingTimer = new Timer(e => { this.RequestNewData(); }, null, TimeSpan.Zero, TimeSpan.FromSeconds(timeout));
        }
        
        public void StopPolling()
        {
            _pollingTimer.Dispose();
        }

        public void Dispose()
        {
            _pollingTimer?.Dispose();
            _serialPort.Close();
        }

        private void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort sp = (SerialPort)sender;
            byte[] response = new byte[sp.BytesToRead];
            sp.Read(response, 0, response.Length);
            if (response.Length == 9 && response[8] == GetCheckSum(response))
            {
                Parse(response);
            }
        }

        private void Parse(byte[] packet)
        {
            switch (packet[1])
            {
                case 0x86: // gas concentration and temperature
                    GasConcn = btoi(packet[2], packet[3]);
                    Temperature = packet[4] - 40;
                    OnPropertyChanged("Data");
                    break;
                default: return;
            }
        }

        private int btoi(byte high, byte low)
        {
            return (256 * high) + low;
        }

        private byte GetCheckSum(byte[] packet)
        {
            byte checkSum = 0;
            for (int i = 1; i < 8; i++)
                checkSum += packet[i];
            checkSum = (byte)(0xff - checkSum);
            checkSum++;
            return checkSum;
        }

        private void Write(byte[] command)
        {
            _serialPort.Write(command, 0, command.Length);
        }

        #region INotifyPropertyChanged
        public event PropertyChangedEventHandler PropertyChanged;
        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion
    }
}
