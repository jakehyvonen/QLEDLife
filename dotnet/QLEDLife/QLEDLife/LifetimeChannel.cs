using System;
using System.Diagnostics;
using System.Windows.Media;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace QLEDLife
{
    public class LifetimeChannel
    {
        public LifetimeChannel()
        {
            Debug.WriteLine("LifetimeChannel created");
        }
        #region Members
        //initial set values
        double _setCurrent = 0.00;
        int _shortTermSpan = 10;
        int _spanSwitchTime = 600;
        int _longTermSpan = 600;
        int _highRangeInt = 1;
        double _alpha = 1.00;
        double _EQECF = 1.00;
        string _channelSavePath;
        //logically updated values
        string _channelName = "Unnamed Channel";
        int _channelIndex;
        bool _channelIsActive = false;
        bool _channelIsNOTActive = true;
        double _elapsedHours;
        double _lastVoltReading = -12;
        double _initialLuminanceReading = -12;
        double _lastLuminanceReading = -12;
        double _luminanceFraction = 1.0d;
        double _lastCECalc = -12;
        double _lastPECalc = -12;
        double _lastEQECalc = -12;
        Brush _backgroundColor = new SolidColorBrush(Colors.DarkGray);
        #endregion

        #region Properties

        public double SetCurrent
        {
            get { return _setCurrent; }
            set { _setCurrent = value; }

        }

        public int ShortTermSpan
        {
            get { return _shortTermSpan; }
            set { _shortTermSpan = value; }
        }

        public int SpanSwitchTime
        {
            get { return _spanSwitchTime; }
            set { _spanSwitchTime = value; }
        }

        public int LongTermSpan
        {
            get { return _longTermSpan; }
            set { _longTermSpan = value; }
        }

        public int HighRangeInt
        {
            get { return _highRangeInt; }
            set { _highRangeInt = value; }
        }

        public double Alpha
        {
            get { return _alpha; }
            set { _alpha = value; }
        }

        public double EQECF
        {
            get { return _EQECF; }
            set { _EQECF = value; }
        }

        public string ChannelSavePath
        {
            get { return _channelSavePath; }
            set { _channelSavePath = value; }
        }

        public string ChannelName
        {
            get { return _channelName; }
            set { _channelName = value; }
        }

        public int ChannelIndex

        {
            get { return _channelIndex; }
            set { _channelIndex = value; }
        }

        public bool ChannelIsActive
        {
            get { return _channelIsActive; }
            set { _channelIsActive = value; }
        }

        public bool ChannelIsNOTActive
        {
            get { return _channelIsNOTActive; }
            set { _channelIsNOTActive = value; }
        }
        public double ElapsedHours
        {
            get { return _elapsedHours; }
            set { _elapsedHours = value; }
        }

        public double LastVoltReading
        {
            get { return _lastVoltReading; }
            set { _lastVoltReading = value; }
        }

        public double InitialLuminanceReading
        {
            get { return _initialLuminanceReading; }
            set { _initialLuminanceReading = value; }
        }
        public double LastLuminanceReading
        {
            get { return _lastLuminanceReading; }
            set { _lastLuminanceReading = value; }
        }
        public double LuminanceFraction
        {
            get { return _luminanceFraction; }
            set { _luminanceFraction = value; }
        }
        public double LastCECalc
        {
            get { return _lastCECalc; }
            set { _lastCECalc = value; }
        }
        public double LastPECalc
        {
            get { return _lastPECalc; }
            set { _lastPECalc = value; }
        }
        public double LastEQECalc
        {
            get { return _lastEQECalc; }
            set { _lastEQECalc = value; }
        }
        public Brush BackgroundColor
        {
            get { return _backgroundColor; }
            set { _backgroundColor = value; }
        }

        #endregion



    }
}
