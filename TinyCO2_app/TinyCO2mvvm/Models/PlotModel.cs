using Caliburn.Micro;
using OxyPlot;
using OxyPlot.Axes;
using OxyPlot.Series;
using System;
using System.Collections.Generic;
using System.Text;

namespace TinyCO2mvvm.Models
{
    public class PlotModel : PropertyChangedBase
    {
        private OxyPlot.PlotModel _model;
        public OxyPlot.PlotModel Plot { get => _model; }

        private LineSeries _series;

        public PlotModel()
        {
            _model = new OxyPlot.PlotModel();
            _series = new LineSeries();
            _model.Series.Add(_series);

            _model.Title = "Gas Concentration";
            _series.Title = "PPM";

            var xAxis = new DateTimeAxis
            {
                Position = AxisPosition.Bottom,
                MinorGridlineStyle = LineStyle.Solid,
                MinorGridlineColor = OxyColors.LightGray,
                TickStyle = TickStyle.Outside,
                MajorGridlineStyle = LineStyle.Solid,
                MajorGridlineColor = OxyColors.LightSlateGray,
                Minimum = DateTimeAxis.ToDouble(DateTime.Now.AddSeconds(-10)),
                Maximum = DateTimeAxis.ToDouble(DateTime.Now.AddHours(1)),
                IntervalType = DateTimeIntervalType.Minutes,
                StringFormat = "HH:mm\ndd MMM"
            };

            var yAxis = new RangeColorAxis
            {
                Position = AxisPosition.Left,
                TickStyle = TickStyle.Outside,
                MinorGridlineStyle = LineStyle.Solid,
                MinorGridlineColor = OxyColors.LightGray,
                MajorGridlineStyle = LineStyle.Solid,
                MajorGridlineColor = OxyColors.LightSlateGray,
                AbsoluteMinimum = 400,
                AbsoluteMaximum = 2000,
                Minimum = 400,
                Maximum = 2000,
                MajorStep = 400,
                MinorStep = 100,
                IsZoomEnabled = false,
                IsPanEnabled = false
            };
            
            for (int i = 400; i < 1000; i += 5)
                yAxis.AddRange(i, i + 5, OxyColor.Interpolate(OxyColors.ForestGreen, OxyColors.Yellow, (i - 400) / 600d));
            for (int i = 1000; i < 1600; i += 5)
                yAxis.AddRange(i, i + 5, OxyColor.Interpolate(OxyColors.Yellow, OxyColors.Orange, (i - 1000) / 600d));
            for (int i = 1600; i < 2000; i += 5)
                yAxis.AddRange(i, i + 5, OxyColor.Interpolate(OxyColors.Orange, OxyColors.Red, (i - 1600) / 400d));
            
            _model.Axes.Add(xAxis);
            _model.Axes.Add(yAxis);

            _model.LegendOrientation = LegendOrientation.Horizontal;
            _model.LegendPlacement = LegendPlacement.Inside;
            _model.LegendPosition = LegendPosition.TopRight;
        }

        public void AddDataPoint(DataPoint dataPoint)
        {
            _series.Points.Add(dataPoint);
            _model.InvalidatePlot(false);
        }

        public void AddDataPoint(DateTime dateTime, int value)
        {
            AddDataPoint(new DataPoint(DateTimeAxis.ToDouble(dateTime), value));
        }
    }
}
