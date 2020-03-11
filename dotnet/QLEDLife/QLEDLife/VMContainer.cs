using System;
using System.Windows;
using System.Windows.Threading;
using System.Diagnostics;
using System.Windows.Media;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace QLEDLife
{
    public class VMContainer
    {

        #region members
        LifetimeChannelViewModel[] _LCVMArray = new LifetimeChannelViewModel[NumberOfChannels];
        //LifetimeChannelViewModel _LCVM0;
        //thing
        int[] activeSampleInterval = new int[NumberOfChannels];
        int[] channelLoopCounter = new int[NumberOfChannels];
        double[] channelTimeCounter = new double[NumberOfChannels];
        SerialComm serialComm = new SerialComm();
        //DataParser dataParser = new DataParser();
        #endregion

        #region properties
        static public int NumberOfChannels = 16;
        public LifetimeChannelViewModel[] LCVMArray
        {
            get
            {
                return _LCVMArray;
            }
            set
            {
                _LCVMArray = value;
            }
        }

        #endregion

        #region Construction
        public VMContainer()
        {

            for (int i = 0; i < NumberOfChannels; i = i + 1)
            {

                LCVMArray[i] = new LifetimeChannelViewModel(ref serialComm);
                LCVMArray[i].LifetimeChannel.ChannelIndex = i;
                LCVMArray[i].LifetimeChannel.ChannelName = string.Concat("Channel " + i);
            }

            DispatcherTimer timer = new DispatcherTimer();
            timer.Interval = TimeSpan.FromSeconds(1);
            timer.Tick += Timer_Tick;
            timer.Start();

            serialComm.TharBeDataToParse += SerialComm_TharBeDataToParse;
            //_VMContainer.ReadCmdPending += VMContainer_ReadCmdPending;
            try
            {
                serialComm.OpenArduinoConnection();
            }
            catch (Exception)
            {
                MessageBox.Show("Error: Cannot connect to the Arduino Board - Configure the COM Port in the app.config file and check whether an Arduino Board is connected to your computer.");
            }
        }
        #endregion

        #region methods


        void SerialComm_TharBeDataToParse(object sender, EventArgs e)
        {

            string channelStr = serialComm.dataReceived.Substring(2, 2);
            Debug.Write("channelStr is: "); Debug.WriteLine(channelStr);
            Debug.WriteLine(serialComm.dataReceived);
            int channelInt;
            Int32.TryParse(channelStr, out channelInt);
            LCVMArray[channelInt].ParseData(serialComm.dataReceived);
            /*
            Dispatcher.BeginInvoke(DispatcherPriority.Input, new ThreadStart(() =>
            {
                //Debug.WriteLine("yar function works, m80");
                receiveBox.Text = serialComm.dataReceived;

            }));
            */
        }

        private void UpdateTileColor(int channelIndex, double lumFrac)
        {
            //sets the background color: >0.97 = blue, >0.90 = green, >0.75 = yellow, >0.60 = orange, >0.50 = red
            if (lumFrac < 0.97d && lumFrac >= 0.9d)
            {
                Debug.WriteLine("Luminance Fraction < 0.97");
                LCVMArray[channelIndex].BackgroundColor = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#b3ffb3"));

            }
            if (lumFrac < 0.9d && lumFrac >= 0.75d)
            {
                Debug.WriteLine("Luminance Fraction < 0.9");
                LCVMArray[channelIndex].BackgroundColor = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#ffff99"));

            }
            if (lumFrac < 0.75d && lumFrac >= 0.6d)
            {
                Debug.WriteLine("Luminance Fraction < 0.75");
                LCVMArray[channelIndex].BackgroundColor = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#ffcc99"));

            }
            if (lumFrac < 0.6d && lumFrac >= 0.5d)
            {
                Debug.WriteLine("Luminance Fraction < 0.6");
                LCVMArray[channelIndex].BackgroundColor = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#ff8080"));
            }
        }

        private void Timer_Tick(object sender, EventArgs e)
        {

            for (int i = 0; i < 16; i = i + 1)
            {
                //Debug.Write(LCVMArray[i].LifetimeChannel.ChannelIsActive);
                //Debug.WriteLine(LifetimeChannel[i].ChannelIndex);
                if (LCVMArray[i].LifetimeChannel.ChannelIsActive == true)
                {
                    //Debug.WriteLine(LCVMArray[i].BackgroundColor.ToString());
                    //Debug.Write("activeReadInterval value is "); Debug.WriteLine(activeReadIntervalArray[i]);
                    //Debug.Write("channelLoopCounter is "); Debug.WriteLine(channelLoopCounter[i]);
                    //Debug.Write("Channel "); Debug.Write(i); Debug.WriteLine(" is active");
                    if (activeSampleInterval[i] == 0)
                    {
                        activeSampleInterval[i] = LCVMArray[i].LifetimeChannel.ShortTermSpan;
                    }

                    if (channelLoopCounter[i] == activeSampleInterval[i])
                    {

                        serialComm.SendArduinoCommand("read", "both", i);
                        channelLoopCounter[i] = 0;
                        UpdateTileColor(i, LCVMArray[i].LuminanceFraction);
                    }
                    else
                    {

                        channelLoopCounter[i] = 1 + channelLoopCounter[i];
                    }
                    channelTimeCounter[i] = 1 + channelTimeCounter[i];
                    LCVMArray[i].ElapsedHours = channelTimeCounter[i] / 3600.00;

                    Debug.WriteLine(channelTimeCounter[i]);
                }

            }
        }
        #endregion
    }
}
