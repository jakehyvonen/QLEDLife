using System;
using System.Diagnostics;
using System.IO.Ports;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace QLEDLife
{
    public class SerialComm
    {
        public SerialPort arduinoBoard = new SerialPort();
        public event EventHandler TharBeDataToParse;
        public string dataReceived;

        public void OpenArduinoConnection()
        {
            if (!arduinoBoard.IsOpen)
            {
                arduinoBoard.DataReceived += arduinoBoard_DataReceived;
                arduinoBoard.PortName = "COM3";
                arduinoBoard.BaudRate = 9600;
                arduinoBoard.Open();
            }
            else
            {
                throw new InvalidOperationException("The Serial Port is already open!");
            }
        }

        public void SendArduinoCommand(string command, string commandValue, int channelInt)
        {
            if (arduinoBoard.IsOpen)
            {
                //Debug.Write("received this int: ");
                string channelString = channelInt.ToString();
                if (channelInt < 10)     //I have to account for the two digits in string processing in Arduino's code
                {
                    channelString = string.Concat("0", channelString);
                }
                //Debug.WriteLine(channelString);
                string serialCommand = string.Concat(command, channelString, commandValue, "#");
                Debug.Write("serialCommand = ");
                Debug.WriteLine(serialCommand);
                arduinoBoard.Write(serialCommand);
            }
            else
            {
                throw new InvalidOperationException("Can't get data if port is closed");
            }
        }

        void arduinoBoard_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            dataReceived = arduinoBoard.ReadTo("\x03");//Read until the EOT code

            EventHandler handler = TharBeDataToParse;

            if (handler != null)
            {
                handler(this, EventArgs.Empty);
            }

            Debug.Write("Arduino data received: ");
            Debug.WriteLine(dataReceived);
        }

    }
}
