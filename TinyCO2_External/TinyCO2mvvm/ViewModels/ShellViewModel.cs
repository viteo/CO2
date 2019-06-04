using Caliburn.Micro;
using MHZ19uart;
using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Threading;
using TinyCO2mvvm.Models;

namespace TinyCO2mvvm.ViewModels
{
    public class ShellViewModel : PropertyChangedBase
    {
        private List<string> _serialPorts;
        public List<string> SerialPorts
        {
            get => _serialPorts;
            set
            {
                _serialPorts = value;
                NotifyOfPropertyChange();
            }
        }

        private string _selectedSerialPort;
        public string SelectedSerialPort
        {
            get { return _selectedSerialPort; }
            set
            {
                _selectedSerialPort = value;
                NotifyOfPropertyChange();
                NotifyOfPropertyChange(() => CanConnect);
            }
        }

        public bool CanConnect
        {
            get { return !string.IsNullOrEmpty(SelectedSerialPort); }
        }

        private Timer _progressTimer;
        int _timeInterval = 8;
        public void Connect()
        {
            Sensor = new MHZ19(SelectedSerialPort);
            Sensor.StartPolling(_timeInterval);
            _progressTimer = new Timer(e => { Progress++; }, null, TimeSpan.Zero, TimeSpan.FromMilliseconds(50));
            Sensor.PropertyChanged += (o, a) =>
            {
                Plot.Series.Points.Add(new OxyPlot.DataPoint(OxyPlot.Axes.DateTimeAxis.ToDouble(DateTime.Now), Sensor.GasConcn));
                Plot.Plot.InvalidatePlot(true);
                if (a.PropertyName == "Data")
                    Progress = 0;
            };
        }

        private MHZ19 _sensor;
        public MHZ19 Sensor
        {
            get => _sensor;
            private set
            {
                _sensor = value;
                NotifyOfPropertyChange();
            }
        }
        private int _progress;
        public int Progress
        {
            get => _progress;
            set
            {
                _progress = value;
                NotifyOfPropertyChange();
            }
        }

        public void UpdatePortList()
        {
            SerialPorts = SerialPort.GetPortNames().ToList();
        }

        public PlotModel Plot { get; } = new PlotModel();

        public void OnClose()
        {
            _progressTimer?.Dispose();
            Sensor?.Dispose();
        }
    }
}
