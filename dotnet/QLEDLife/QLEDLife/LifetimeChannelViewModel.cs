using System;
using System.Windows;
using System.Windows.Media;
using System.ComponentModel;
using System.Windows.Input;
using Microsoft.Win32;
using System.IO;
using OxyPlot;
using OxyPlot.Series;
using OxyPlot.Axes;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace QLEDLife
{
    public class LifetimeChannelViewModel : INotifyPropertyChanged
    {
        #region Construction
        public LifetimeChannelViewModel(ref SerialComm serialComm)
        {
            Debug.Write("LifetimeChannelViewModel created");

            _lifetimeChannel = new LifetimeChannel { };
            _serialComm = serialComm;
            Points = new List<DataPoint> { };
            plotModel = new PlotModel();
            plotModel.Axes.Add(new LinearAxis { Position = AxisPosition.Bottom, Minimum = 0 });
            plotModel.Axes.Add(new LinearAxis { Position = AxisPosition.Left, MajorGridlineStyle = LineStyle.Solid, MinorGridlineStyle = LineStyle.Dot, Title = "Luminance" });
            lineSeries = new LineSeries
            {
                MarkerType = MarkerType.Circle,
                MarkerSize = 1,
                MarkerStroke = OxyColors.Aqua
            };
            PlotModel.Series.Add(lineSeries);
            dataPointList = new List<DataPoint>();

        }
        #endregion

        #region Members
        SerialComm _serialComm;
        LifetimeChannel _lifetimeChannel;
        PlotModel plotModel;
        LineSeries lineSeries;
        static List<DataPoint> dataPointList;
        #endregion

        #region Properties
        public LifetimeChannel LifetimeChannel
        {
            get
            {
                return _lifetimeChannel;
            }
            set
            {
                _lifetimeChannel = value;
            }
        }
        public double SetCurrent
        {
            get { return LifetimeChannel.SetCurrent; }
            set
            {
                if (LifetimeChannel.SetCurrent != value)
                {
                    LifetimeChannel.SetCurrent = value;
                    RaisePropertyChanged("SetCurrent");
                }
            }
        }
        public int ShortTermSpan
        {
            get { return LifetimeChannel.ShortTermSpan; }
            set
            {
                if (LifetimeChannel.ShortTermSpan != value)
                {
                    LifetimeChannel.ShortTermSpan = value;
                    RaisePropertyChanged("ShortTermSpan");
                }
            }
        }
        public int SpanSwitchTime
        {
            get { return LifetimeChannel.SpanSwitchTime; }
            set
            {
                if (LifetimeChannel.SpanSwitchTime != value)
                {
                    LifetimeChannel.SpanSwitchTime = value;
                    RaisePropertyChanged("SpanSwitchTime");
                }
            }
        }
        public int LongTermSpan
        {
            get { return LifetimeChannel.LongTermSpan; }
            set
            {
                if (LifetimeChannel.LongTermSpan != value)
                {
                    LifetimeChannel.LongTermSpan = value;
                    RaisePropertyChanged("LongTermSpan");
                }
            }
        }
        public int HighRangeInt
        {
            get { return LifetimeChannel.HighRangeInt; }
            set
            {
                if (LifetimeChannel.HighRangeInt != value)
                {
                    LifetimeChannel.HighRangeInt = value;
                    RaisePropertyChanged("HighRangeInt");
                    //Debug.WriteLine(LifetimeChannel.HighRangeInt);
                }
            }
        }
        public double Alpha
        {
            get { return LifetimeChannel.Alpha; }
            set
            {
                if (LifetimeChannel.Alpha != value)
                {
                    LifetimeChannel.Alpha = value;
                    RaisePropertyChanged("Alpha");
                }
            }
        }
        public double EQECF
        {
            get { return LifetimeChannel.EQECF; }
            set
            {
                if (LifetimeChannel.EQECF != value)
                {
                    LifetimeChannel.EQECF = value;
                    RaisePropertyChanged("EQECF");
                }
            }
        }
        public string ChannelSavePath
        {
            get { return LifetimeChannel.ChannelSavePath; }
            set
            {
                if (LifetimeChannel.ChannelSavePath != value)
                {
                    LifetimeChannel.ChannelSavePath = value;
                    RaisePropertyChanged("ChannelSavePath");
                }
            }
        }
        public string ChannelName
        {
            get { return LifetimeChannel.ChannelName; }
            set
            {
                if (LifetimeChannel.ChannelName != value)
                {
                    LifetimeChannel.ChannelName = value;
                    RaisePropertyChanged("ChannelName");
                }
            }
        }

        public int ChannelIndex
        {
            get { return LifetimeChannel.ChannelIndex; }
            set
            {
                if (LifetimeChannel.ChannelIndex != value)
                {
                    LifetimeChannel.ChannelIndex = value;
                    RaisePropertyChanged("ChannelIndex");
                }
            }
        }

        public bool ChannelIsActive
        {
            get { return LifetimeChannel.ChannelIsActive; }
            set
            {
                if (LifetimeChannel.ChannelIsActive != value)
                {
                    LifetimeChannel.ChannelIsActive = value;
                    RaisePropertyChanged("ChannelIsActive");
                }
            }
        }
        public bool ChannelIsNOTActive
        {
            get { return LifetimeChannel.ChannelIsNOTActive; }
            set
            {
                if (LifetimeChannel.ChannelIsNOTActive != value)
                {
                    LifetimeChannel.ChannelIsNOTActive = value;
                    RaisePropertyChanged("ChannelIsNOTActive");
                }
            }
        }
        public double ElapsedHours
        {
            get { return LifetimeChannel.ElapsedHours; }
            set
            {
                if (LifetimeChannel.ElapsedHours != value)
                {
                    LifetimeChannel.ElapsedHours = value;
                    RaisePropertyChanged("ElapsedHours");
                }
            }
        }
        public double LastVoltReading
        {
            get { return LifetimeChannel.LastVoltReading; }
            set
            {
                if (LifetimeChannel.LastVoltReading != value)
                {
                    LifetimeChannel.LastVoltReading = value;
                    RaisePropertyChanged("LastVoltReading");
                }
            }
        }
        public double LastLuminanceReading
        {
            get { return LifetimeChannel.LastLuminanceReading; }
            set
            {
                if (LifetimeChannel.LastLuminanceReading != value)
                {
                    LifetimeChannel.LastLuminanceReading = value;
                    RaisePropertyChanged("LastLuminanceReading");
                }
            }
        }
        public double InitialLuminanceReading
        {
            get { return LifetimeChannel.InitialLuminanceReading; }
            set
            {
                if (LifetimeChannel.InitialLuminanceReading != value)
                {
                    LifetimeChannel.InitialLuminanceReading = value;
                    RaisePropertyChanged("InitialLuminanceReading");
                }
            }
        }
        public double LuminanceFraction
        {
            get { return LifetimeChannel.LuminanceFraction; }
            set
            {
                if (LifetimeChannel.LuminanceFraction != value)
                {
                    LifetimeChannel.LuminanceFraction = value;
                    RaisePropertyChanged("LuminanceFraction");
                }
            }
        }
        public double LastCECalc
        {
            get { return LifetimeChannel.LastCECalc; }
            set
            {
                if (LifetimeChannel.LastCECalc != value)
                {
                    LifetimeChannel.LastCECalc = value;
                    RaisePropertyChanged("LastCECalc");
                }
            }
        }
        public double LastPECalc
        {
            get { return LifetimeChannel.LastPECalc; }
            set
            {
                if (LifetimeChannel.LastPECalc != value)
                {
                    LifetimeChannel.LastPECalc = value;
                    RaisePropertyChanged("LastPECalc");
                }
            }
        }
        public double LastEQECalc
        {
            get { return LifetimeChannel.LastEQECalc; }
            set
            {
                if (LifetimeChannel.LastEQECalc != value)
                {
                    LifetimeChannel.LastEQECalc = value;
                    RaisePropertyChanged("LastEQECalc");
                }
            }
        }
        public Brush BackgroundColor
        {
            get { return LifetimeChannel.BackgroundColor; }
            set
            {
                if (LifetimeChannel.BackgroundColor != value)
                {
                    LifetimeChannel.BackgroundColor = value;
                    RaisePropertyChanged("BackgroundColor");
                }
            }
        }
        public PlotModel PlotModel
        {
            get { return plotModel; }
            set { plotModel = value; RaisePropertyChanged("PlotModel"); }
        }
        public LineSeries LineSeries //this is probably unnecessary
        {
            get { return lineSeries; }
            set { lineSeries = value; }
        }
        public IList<DataPoint> Points { get; private set; }


        #endregion


        #region INotifyPropertyChanged Members

        public event PropertyChangedEventHandler PropertyChanged;

        #endregion

        #region Methods

        private void UpdatePlotModel()
        {
            dataPointList.Add(new DataPoint(ElapsedHours, LastLuminanceReading));

            lineSeries.Points.Add(new DataPoint(ElapsedHours, LastLuminanceReading));

            PlotModel.Series.Add(lineSeries);
            PlotModel.InvalidatePlot(true);

        }

        private void RaisePropertyChanged(string propertyName)
        {
            // take a copy to prevent thread issues
            PropertyChangedEventHandler handler = PropertyChanged;
            if (handler != null)
            {
                handler(this, new PropertyChangedEventArgs(propertyName));
                /*
                Debug.Write(propertyName);
                Debug.Write(" has been changed on channel ");
                Debug.WriteLine(ChannelIndex);*/
            }
        }

        public void ParseData(string cmdString)
        {
            if (cmdString[4].ToString() != "V")
            {
                Debug.WriteLine("RECIEVED IMPROPERLY FORMATTED STRING");
            }
            int Vindex = 4;
            int Pindex = cmdString.IndexOf("P");
            int Iindex = cmdString.IndexOf("I");

            string lastVoltStr = cmdString.Substring(Vindex + 1, Pindex - Vindex - 1);  // the +/- is so that the "V" isn't included in the string
            double lastVoltDouble = double.Parse(lastVoltStr);
            LastVoltReading = LastVoltReading;

            string lastPStr = cmdString.Substring(Pindex + 1, Iindex - Pindex - 1);
            double lastPDouble = double.Parse(lastPStr);
            LastLuminanceReading = lastPDouble;

            string lastCurrStr = cmdString.Substring(Iindex + 1);
            float lastCurrFloat = float.Parse(lastCurrStr);
            // will implement current regulating later if necessary
            calcLED(lastVoltDouble, lastPDouble);
            saveData(ElapsedHours, LastVoltReading, LastLuminanceReading, LastCECalc, LastPECalc, LastEQECalc);
            Points.Add(new DataPoint(ElapsedHours, LastLuminanceReading));
            LineSeries.Points.Add(new DataPoint(ElapsedHours, LastLuminanceReading));
            UpdatePlotModel();
        }

        public void saveData(double elapsedHours, double voltageReading, double luminanceCalc, double CECalc, double PECalc, double EQECalc)
        {
            string line = string.Format("{0}\t{1}\t{2}\t{3}\t{4}\t{5}\t", elapsedHours.ToString(), voltageReading.ToString(), luminanceCalc.ToString(), CECalc.ToString(), PECalc.ToString(), EQECalc.ToString());
            FileStream fileStream = new FileStream(ChannelSavePath, FileMode.Append, FileAccess.Write);
            StreamWriter file = new StreamWriter(fileStream);
            file.WriteLine(line);
            file.Close();
        }

        private void calcLED(double lastVoltDouble, double lastPDouble)
        {
            Debug.WriteLine("lastVoltDouble:{0}, lastPDouble:{1}", lastVoltDouble, lastPDouble);
            double lastLuminanceDouble = Alpha * lastPDouble;
            //lastLuminanceDouble = 45;
            if (InitialLuminanceReading == -12)
            {
                InitialLuminanceReading = lastLuminanceDouble;
                Debug.WriteLine("initial luminance set");
            }
            double lastLFracDouble = lastLuminanceDouble / InitialLuminanceReading;
            double lastCEDouble = 0.004 * lastLuminanceDouble / SetCurrent;
            double lastPEDouble = Math.PI * lastCEDouble / lastVoltDouble;
            double lastEQEDouble = EQECF * lastPDouble / (SetCurrent * 25); // this needs to be fixed once the photocurrent measurement value is worked out

            LastVoltReading = lastVoltDouble;
            LastLuminanceReading = lastLuminanceDouble;
            LuminanceFraction = lastLFracDouble;
            LastCECalc = lastCEDouble;
            LastPECalc = lastPEDouble;
            LastEQECalc = lastEQEDouble;

            //LuminanceFraction = 0.96d;
        }


        #endregion

        #region Commands
        void UpdateChannelSavePathExecute()
        {
            //need to figure out how to implement
            Debug.WriteLine("channel save function called");
            SaveFileDialog saveFileDialog = new SaveFileDialog();
            saveFileDialog.DefaultExt = "DAT";
            saveFileDialog.Filter = "DAT File (*.DAT)|.DAT";
            string headers = "Time (hr)\tVoltage (V)\tLuminance (cd/m^2)\tCurrent Eff. (cd/A)\tPower Eff. (lm/W)\tEQE (%)\r\n";
            if (saveFileDialog.ShowDialog() == true)
            {
                ChannelSavePath = saveFileDialog.FileName;
                File.WriteAllText(saveFileDialog.FileName, headers);
            }

            Debug.WriteLine(saveFileDialog.FileName);
        }
        bool CanUpdateChannelSavePathExecute()
        {
            return true;
        }
        public ICommand UpdateChannelSavePath { get { return new RelayCommand(UpdateChannelSavePathExecute, CanUpdateChannelSavePathExecute); } }
        void StartChannelExecute()
        {
            if (ChannelSavePath != null)
            {
                ChannelIsNOTActive = false;
                ChannelIsActive = true;
                if (HighRangeInt == 0)
                {
                    string cmdString = SetCurrent.ToString("0.00");
                    _serialComm.SendArduinoCommand("setLo", cmdString, ChannelIndex);
                }
                if (HighRangeInt == 1)
                {
                    string cmdString = SetCurrent.ToString("00.0");
                    _serialComm.SendArduinoCommand("setHi", cmdString, ChannelIndex);
                }
                BackgroundColor = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#b3ffff"));

                //Debug.Write("ChannelIsNOTActive set to: ");
                //Debug.WriteLine(ChannelIsNOTActive);
            }
            else
            {
                MessageBox.Show("Save path not set!");
            }
        }
        bool CanStartChannelExecute()
        {
            return true;
        }
        public ICommand StartChannel { get { return new RelayCommand(StartChannelExecute, CanStartChannelExecute); } }
        void StopChannelExecute()
        {
            string msgString;
            msgString = string.Concat("Are you sure you want to stop: " + _lifetimeChannel.ChannelName + "?");
            MessageBoxResult confirmResult = MessageBox.Show(msgString,
                                    "Confirm", MessageBoxButton.YesNo, MessageBoxImage.Question);



            if (confirmResult == MessageBoxResult.Yes)
            {
                ChannelIsNOTActive = true;
                ChannelIsActive = false;
                if (HighRangeInt == 0)
                {
                    _serialComm.SendArduinoCommand("setLo", "0.00", ChannelIndex);
                }
                if (HighRangeInt == 1)
                {
                    _serialComm.SendArduinoCommand("setHi", "00.0", ChannelIndex);
                }
                //Debug.Write("ChannelIsNOTActive set to: ");
                //Debug.WriteLine(ChannelIsNOTActive);
                SetCurrent = 0.00;
                msgString = string.Concat(ChannelName + " set to 0 mA");
                MessageBox.Show(msgString);
            }
            else
            {
                msgString = string.Concat(ChannelName + " left unchanged");
                MessageBox.Show(msgString);
            }


        }
        bool CanStopChannelExecute()
        {
            return true;
        }
        public ICommand StopChannel { get { return new RelayCommand(StopChannelExecute, CanStopChannelExecute); } }

        #endregion
    }
}
