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
        public OxyPlot.PlotModel Plot { get => _model; set => _model = value; }

        private LineSeries _series;
        public LineSeries Series { get => _series; set => _series = value; }

        public PlotModel()
        {
            _model = new OxyPlot.PlotModel();
            _series = new LineSeries();
            _model.Series.Add(_series);

            _model.Title = "Gas Concentration";
            _series.Title = "PPM";


            _model.Axes.Add(new DateTimeAxis //X-axis
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
            });

            _model.Axes.Add(new LinearAxis //Y-axis
            {
                Position = AxisPosition.Left,
                MinorGridlineStyle = LineStyle.Solid,
                MinorGridlineColor = OxyColors.LightGray,
                TickStyle = TickStyle.Outside,
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
            });

            _model.LegendOrientation = LegendOrientation.Horizontal;
            _model.LegendPlacement = LegendPlacement.Inside;
            _model.LegendPosition = LegendPosition.TopRight;
        }
    }
}
