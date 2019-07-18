﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Ports;
using System.Windows.Forms;
using System.Threading;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.IO;
using LEO;
using System.IO.Compression;

namespace LEO
{
    public class Device
    {
        public struct SystemConfig_def
        {
            public int CoreClock;
            public int PeriphClock;
            public string MCU;
            public string FW_Version;
            public string FREE_RTOS_Version;
            public string HAL_Version;
            public bool isShield;
            public int VDDA_actual;
            public int VDDA_target;
        }
        public struct CommsConfig_def
        {
            public int bufferLength;
            public int UartSpeed;
            public string TX_pin;
            public string RX_pin;
            public bool useUsb;
            public string DP_pin;
            public string DM_pin;
        }
        public struct ScopeConfig_def
        {
            public bool isScope;
            public int maxSamplingFrequency;
            public int realSmplFreq;
            public int maxBufferLength;
            public int maxNumChannels;
            public string[] pins;
            public int VRef;
            public int VRefInt;
            public int munRanges;
            public int[,] ranges;
            public byte[] buffer;
            public UInt16[,] samples;
            public string[][] inputs;
            public int[] numOfInputs;
            public int[] defInputs;
            public double[] timeBase;
            public Scope.TRIG_MODE mode;
            public double maxTime;
            public int sampligFreq;
            public int actualChannels;
            public int actualRes;
        }
        public struct GeneratorConfig_def
        {
            public bool isGen;
            public int maxSamplingFrequency;
            public int BufferLength;
            public int dataDepth;
            public int numChannels;
            public string[] pins;
            public int VRefMax;
            public int VRefMin;
            public int VRefInt;
        }

        public struct SyncPwmConfig_def
        {
            public bool isSyncPwm;
            public int periphClock;
            public int maxFreq;
            public int numOfChannels;
            public string[] pins;
        }

        public struct LogAnlysConfig_def
        {
            public bool isLogAnlys;
            public int postTrigPeriphClock;
            public int timeBasePeriphClock;
            public int samplingFreq;            
            public int bufferLength;
            public int numOfChannels;
            public UInt16[] samples;
            public string[] pins;
            public int triggerChannel;
            internal double maxTime;
        }

        public struct PwmGenConfig_def
        {
            public bool isPwmGen;
            public int pwmFrequency;
            public int pwmResolution;
            public int numChannels;
            public string[] pins;
        }

        public struct CounterConfig_def
        {
            public bool isCnt;
            public int counterPeriphClock;
            public int gatePeriphClock;
            public string modes;
            public string[] pins;

            /* When dutyCycle and pulseWidth are sent both, it works, but after coming back
               to frequency measuring app stalls. Therefore, pulseWidth is given to Counter app
               this way. */
            public double pulseWidth;
        }

        enum FormOpened { NONE, SCOPE, VOLTMETER, GENERATOR, VOLT_SOURCE, FREQ_ANALYSIS, SYNC_PWM_GENERATOR, LOG_ANLYS }
        FormOpened ADCFormOpened = FormOpened.NONE;
        FormOpened DACFormOpened = FormOpened.NONE;
        FormOpened SyncPwmOpened = FormOpened.NONE;
        FormOpened LogAnlysOpened = FormOpened.NONE;

        public enum GenModeOpened { NONE, DAC, PWM }
        public static GenModeOpened GenOpened = GenModeOpened.NONE;
        int trigP;
        public static GenModeOpened GenMode
        {
            get
            {
                return GenOpened;
            }
        }

        private SerialPort port;
        private string portName;
        private string name;
        private string mcu;
        private int speed;
        public SystemConfig_def systemCfg;
        public CommsConfig_def commsCfg;
        public ScopeConfig_def scopeCfg;
        public GeneratorConfig_def genCfg;                
        public PwmGenConfig_def pwmGenCfg;
        public SyncPwmConfig_def syncPwmCfg;
        public LogAnlysConfig_def logAnlysCfg;
        public CounterConfig_def cntCfg;
        private StreamWriter logWriter;
        private List<String> logger = new List<String>();
        private const bool writeLog = false;
        Scope Scope_form;
        Generator Gen_form;
        Voltmeter Volt_form;
        VoltageSource Source_form;
        BodePlot FreqAnalysis_form;
        Counter Counter_form;
        SyncPwmGenerator SyncPwm_form;
        LogicAnalyzer LogAnlys_form; 

        SynchronizationContext syncContext;
        Reporting report = new Reporting();
        static Semaphore commsSemaphore = new Semaphore(1, 1);  // Dostupná kapacita=1; Celková=1
        static Semaphore logSemaphore = new Semaphore(1, 1);  // Dostupná kapacita=1; Celková=1
        private int semaphoreTakenBy = 0;
        private bool portError = false;

        private string LastCommand = "";

        private int index = 1;

        /* Counter vars */
        double freq;
        int buff;
        string cntMessage;
        float cntMessageFloat;
        double cntMessageDouble;

        /* Logic analyzer vars */
        int receiveDataLength;        

        int lastError = 0;
        public Device(string portName, string name, int speed)
        {
            syncContext = SynchronizationContext.Current;
            this.portName = portName;
            this.name = name;
            this.speed = speed;
        }
        public bool Equals(Device dev)
        {
            if (dev.portName.Equals(this.portName) && dev.name.Equals(this.name))
            {
                return true;
            }
            else {
                return false;
            }
        }
        public string get_processor()
        {
            return this.mcu;
        }
        public string get_name()
        {
            return this.name;
        }
        public string get_port()
        {
            return this.portName;
        }

        public void close_port()
        {
            if (port.IsOpen)
            {
                logTextNL("PORT zavřen: " + this.portName);
                if (writeLog) {

                    logWriter.Close();
                    FileStream inFile = new FileStream("logfile" + index + ".txt", FileMode.Open);
                    
                    // Create the compressed file.
                    using (FileStream outFile = File.Create("log" + index + ".gz"))
                    {
                        using (GZipStream Compress = new GZipStream(outFile,CompressionMode.Compress))
                        {
                            // Copy the source file into the compression stream.
                            byte[] buffer = new byte[4096];
                            int numRead;
                            while ((numRead = inFile.Read(buffer, 0, buffer.Length)) != 0)
                            {
                                Compress.Write(buffer, 0, numRead);
                            }
                        }
                    }

                    inFile.Close();

                    if (File.Exists("logfile" + index + ".txt"))
                    {
                        File.Delete("logfile" + index + ".txt");
                    }


                }
                try
                {
                    port.Close();
                    port.Dispose();
                }
                catch (Exception ex)
                {
                    //do nothing
                }

            }
        }

        public bool open_port()
        {
            bool result = false;

            try
            {
                if (writeLog)
                {
                    try
                    {
                        if (File.Exists("logfile" + index + ".txt"))
                        {
                            File.Delete("logfile" + index + ".txt");
                        }
                        if (File.Exists("logfile" + index + ".gz"))
                        {
                            File.Delete("logfile" + index + ".gz");
                        }
                        logWriter = File.AppendText("logfile" + index + ".txt");
                    }
                    catch (Exception ex)
                    {
                        index++;
                    }
                }
                Log("PORT otevřen: " + portName + "  Baudrate:" + speed + "  Zařízení:" + name);
            }
            catch (Exception ex)
            {
                report.Sendreport("Fatal error during opening log file", ex, this, logger, 78641);
                return false;
            }


            try
            {
                portError = false;
                this.port = new SerialPort();
                this.port.PortName = portName;
                this.port.ReadBufferSize = 128 * 1024;
                this.port.BaudRate = speed;
                port.Open();
                port.Write(Commands.RESET_DEVICE + ";");
                Thread.Sleep(200);
                load_config();
                port.Close();                
                port.ReadTimeout = 10000;
                port.WriteTimeout = 1000;
                port.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort_DataReceived);
                port.ErrorReceived += new System.IO.Ports.SerialErrorReceivedEventHandler(this.serialPort_ErrorReceived);
                port.Open();
                result = true;
            }
            catch (Exception ex)
            {
                report.Sendreport("Fatal error during connecting to device 354135", ex, this, logger, 318461);
            }

            return result;
        }


        public bool is_open()
        {
            return this.port.IsOpen;
        }

        public void load_config()
        {
            try
            {
                if (port.IsOpen)
                {
                    /* ------------------------------------------------------------------------------ */
                    /* --------------------------- SYSTEM CONFIG REQUEST ---------------------------- */
                    /* ------------------------------------------------------------------------------ */
                    int wait = 50;
                    port.Write(Commands.SYSTEM + ":" + Commands.CONFIGRequest + ";");
                    char[] msg_char = new char[256];
                    byte[] msg_byte = new byte[256];
                    Thread.Sleep(wait);
                    int toRead = port.BytesToRead;
                    port.Read(msg_byte, 0, toRead);
                    msg_char = System.Text.Encoding.ASCII.GetString(msg_byte).ToCharArray();

                    if (new string(msg_char, 0, 4).Equals("SYST"))
                    {
                        this.systemCfg.CoreClock = BitConverter.ToInt32(msg_byte, 4);
                        this.systemCfg.PeriphClock = BitConverter.ToInt32(msg_byte, 8);
                        this.systemCfg.MCU = new string(msg_char, 12, toRead - 16);
                        this.mcu = this.systemCfg.MCU;
                    }

                    port.Write(Commands.VersionRequest + ";");
                    Thread.Sleep(wait);
                    toRead = port.BytesToRead;
                    port.Read(msg_byte, 0, toRead);
                    msg_char = System.Text.Encoding.ASCII.GetString(msg_byte).ToCharArray();

                    if (new string(msg_char, 0, 4).Equals("VER_"))
                    {
                        this.systemCfg.FW_Version = new string(msg_char, 16, 8);
                        if (!this.systemCfg.FW_Version.Substring(0, 4).Equals(FW_version.ACTUAL_FW))
                        {
                            logTextNL("Nekompatibilni verze:\r\n");
                            MessageBox.Show("FW is incompatible with current LEO version \r\nExpected: " + FW_version.ACTUAL_FW + "\r\nIn MCU: " + this.systemCfg.FW_Version.Substring(0, 4) + "\r\nSome errors may occur", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        }
                        this.systemCfg.FREE_RTOS_Version = new string(msg_char, 32, 6);
                        this.systemCfg.HAL_Version = new string(msg_char, 44, 6);
                    }


                    /* ------------------------------------------------------------------------------ */
                    /* ----------------------------- IS SHIELD CONECTED ----------------------------- */
                    /* ------------------------------------------------------------------------------ */
                    port.Write(Commands.IS_SHIELD_CONNECTED + ";");
                    Thread.Sleep(wait);
                    toRead = port.BytesToRead;
                    port.Read(msg_byte, 0, toRead);
                    msg_char = System.Text.Encoding.ASCII.GetString(msg_byte).ToCharArray();

                    if (new string(msg_char, 0, 4).Equals("ACK_"))
                    {
                        this.systemCfg.isShield = true;
                    }
                    else {
                        this.systemCfg.isShield = false;
                    }

                    /* ------------------------------------------------------------------------------ */
                    /* ---------------------------- COMM CONFIG REQUEST ----------------------------- */
                    /* ------------------------------------------------------------------------------ */
                    port.Write(Commands.COMMS + ":" + Commands.CONFIGRequest + ";");
                    Thread.Sleep(wait);
                    toRead = port.BytesToRead;
                    port.Read(msg_byte, 0, toRead);
                    msg_char = System.Text.Encoding.ASCII.GetString(msg_byte).ToCharArray();

                    if (new string(msg_char, 0, 4).Equals("COMM"))
                    {
                        this.commsCfg.bufferLength = BitConverter.ToInt32(msg_byte, 4);
                        this.commsCfg.UartSpeed = BitConverter.ToInt32(msg_byte, 8);
                        this.commsCfg.TX_pin = new string(msg_char, 12, 4);
                        this.commsCfg.RX_pin = new string(msg_char, 16, 4);
                        if (toRead > 24)
                        {
                            this.commsCfg.DP_pin = new string(msg_char, 24, 4);
                            this.commsCfg.DM_pin = new string(msg_char, 28, 4);
                            this.commsCfg.useUsb = true;
                        }
                        else
                        {
                            this.commsCfg.useUsb = false;
                        }
                    }

                    /* ------------------------------------------------------------------------------ */
                    /* ---------------------------- SCOPE CONFIG REQUEST ---------------------------- */
                    /* ------------------------------------------------------------------------------ */
                    port.Write(Commands.SCOPE + ":" + Commands.CONFIGRequest + ";");
                    Thread.Sleep(wait);
                    toRead = port.BytesToRead;
                    port.Read(msg_byte, 0, toRead);
                    msg_char = System.Text.Encoding.ASCII.GetString(msg_byte).ToCharArray();

                    if (new string(msg_char, 0, 4).Equals("OSCP"))
                    {
                        scopeCfg.isScope = true;
                        scopeCfg.maxSamplingFrequency = BitConverter.ToInt32(msg_byte, 4);
                        scopeCfg.maxBufferLength = BitConverter.ToInt32(msg_byte, 8);
                        scopeCfg.maxNumChannels = BitConverter.ToInt32(msg_byte, 12);
                        scopeCfg.pins = new string[scopeCfg.maxNumChannels];
                        for (int i = 0; i < this.scopeCfg.maxNumChannels; i++)
                        {
                            scopeCfg.pins[i] = new string(msg_char, 16 + 4 * i, 4);
                        }
                        scopeCfg.VRef = BitConverter.ToInt32(msg_byte, 16 + 4 * scopeCfg.maxNumChannels);
                        scopeCfg.VRefInt = BitConverter.ToInt32(msg_byte, 20 + 4 * scopeCfg.maxNumChannels);

                        scopeCfg.munRanges = (toRead - 28 - 4 * scopeCfg.maxNumChannels) / 4;
                        scopeCfg.ranges = new int[2, scopeCfg.munRanges];
                        for (int i = 0; i < this.scopeCfg.munRanges; i++)
                        {
                            scopeCfg.ranges[0, i] = BitConverter.ToInt16(msg_byte, 24 + 4 * scopeCfg.maxNumChannels + 4 * i);
                            scopeCfg.ranges[1, i] = BitConverter.ToInt16(msg_byte, 26 + 4 * scopeCfg.maxNumChannels + 4 * i);
                        }


                    }
                    else
                    {
                        scopeCfg.isScope = false;
                    }

                    /* ------------------------------------------------------------------------------ */
                    /* ------------------------- SCOPE GPIO INPUTS REQUEST -------------------------- */
                    /* ------------------------------------------------------------------------------ */
                    port.Write(Commands.SCOPE + ":" + Commands.GET_SCOPE_INPUTS + ";");
                    Thread.Sleep(2 * wait);
                    toRead = port.BytesToRead;
                    port.Read(msg_byte, 0, toRead);
                    msg_char = System.Text.Encoding.ASCII.GetString(msg_byte).ToCharArray();

                    if (new string(msg_char, 0, 4).Equals("INP_"))
                    {
                        scopeCfg.defInputs = new int[scopeCfg.maxNumChannels];

                        string inp = new String(msg_char);
                        string[] chanels = inp.Split('/');

                        string[] ch1 = { " " };
                        string[] ch2 = { " " };
                        string[] ch3 = { " " };
                        string[] ch4 = { " " };

                        if (chanels.Length >= 3)
                        {
                            scopeCfg.defInputs[0] = msg_byte[4];
                            ch1 = chanels[1].Split(':');
                        }

                        if (chanels.Length >= 4)
                        {
                            scopeCfg.defInputs[1] = msg_byte[5];
                            ch2 = chanels[2].Split(':');
                        }

                        if (chanels.Length >= 5)
                        {
                            scopeCfg.defInputs[2] = msg_byte[6];
                            ch3 = chanels[3].Split(':');
                        }

                        if (chanels.Length >= 6)
                        {
                            scopeCfg.defInputs[3] = msg_byte[7];
                            ch4 = chanels[4].Split(':');
                        }

                        scopeCfg.numOfInputs = new int[4];


                        int num_chan = chanels.Length - 2;
                        int max_inp = Math.Max(Math.Max(ch1.Length, ch2.Length), Math.Max(ch3.Length, ch4.Length));
                        scopeCfg.inputs = new string[num_chan][];
                        scopeCfg.numOfInputs = new int[num_chan];

                        for (int l = 0; l < num_chan; l++)
                        {

                            for (int n = 0; n < max_inp; n++)
                            {
                                switch (l)
                                {
                                    case 0:
                                        if (n == 0)
                                        {
                                            scopeCfg.inputs[0] = new string[ch1.Length];
                                        }
                                        if (n < ch1.Length)
                                        {
                                            scopeCfg.numOfInputs[l] = ch1.Length;
                                            scopeCfg.inputs[l][n] = ch1[n];
                                        }
                                        break;
                                    case 1:
                                        if (n == 0)
                                        {
                                            scopeCfg.inputs[1] = new string[ch2.Length];
                                        }
                                        if (n < ch2.Length)
                                        {
                                            scopeCfg.numOfInputs[l] = ch2.Length;
                                            scopeCfg.inputs[l][n] = ch2[n];
                                        }

                                        break;
                                    case 2:
                                        if (n == 0)
                                        {
                                            scopeCfg.inputs[2] = new string[ch3.Length];
                                        }
                                        if (n < ch3.Length)
                                        {
                                            scopeCfg.numOfInputs[l] = ch3.Length;
                                            scopeCfg.inputs[l][n] = ch3[n];
                                        }
                                        break;
                                    case 3:
                                        if (n == 0)
                                        {
                                            scopeCfg.inputs[3] = new string[ch4.Length];
                                        }
                                        if (n < ch4.Length)
                                        {
                                            scopeCfg.numOfInputs[l] = ch4.Length;
                                            scopeCfg.inputs[l][n] = ch4[n];
                                        }
                                        break;
                                }
                            }
                        }
                    }
                    else {
                        scopeCfg.inputs = new string[1][];
                        scopeCfg.inputs[0] = new string[1];
                        scopeCfg.inputs[0][0] = "-";
                    }

                    /* ------------------------------------------------------------------------------ */
                    /* ------------------------- GENERATOR CONFIG REQUEST --------------------------- */
                    /* ------------------------------------------------------------------------------ */
                    port.Write(Commands.GENERATOR + ":" + Commands.CONFIGRequest + ";");
                    Thread.Sleep(wait);
                    toRead = port.BytesToRead;
                    port.Read(msg_byte, 0, toRead);
                    msg_char = System.Text.Encoding.ASCII.GetString(msg_byte).ToCharArray();

                    if (new string(msg_char, 0, 4).Equals("GEN_"))
                    {
                        genCfg.isGen = true;
                        genCfg.maxSamplingFrequency = BitConverter.ToInt32(msg_byte, 4);
                        genCfg.BufferLength = BitConverter.ToInt32(msg_byte, 8);
                        genCfg.dataDepth = BitConverter.ToInt32(msg_byte, 12);
                        genCfg.numChannels = BitConverter.ToInt32(msg_byte, 16);
                        genCfg.pins = new string[genCfg.numChannels];
                        for (int i = 0; i < this.genCfg.numChannels; i++)
                        {
                            genCfg.pins[i] = new string(msg_char, 20 + 4 * i, 4);
                        }
                        genCfg.VRefMin = BitConverter.ToInt32(msg_byte, 20 + 4 * genCfg.numChannels);
                        genCfg.VRefMax = BitConverter.ToInt32(msg_byte, 24 + 4 * genCfg.numChannels);
                        systemCfg.VDDA_target = BitConverter.ToInt32(msg_byte, 28 + 4 * genCfg.numChannels);
                        systemCfg.VDDA_actual = systemCfg.VDDA_target;
                        genCfg.VRefInt = BitConverter.ToInt32(msg_byte, 32 + 4 * genCfg.numChannels);
                    }
                    else
                    {
                        genCfg.isGen = false;
                    }

                    port.DiscardInBuffer();

                    /* ------------------------------------------------------------------------------ */
                    /* ------------------------ PWM GENERATOR CONFIG REQUEST ------------------------ */
                    /* ------------------------------------------------------------------------------ */
                    port.Write(Commands.GENERATOR + ":" + Commands.GEN_PWM_CONFIGRequest + ";");
                    Thread.Sleep(wait);
                    toRead = port.BytesToRead;
                    port.Read(msg_byte, 0, toRead);
                    msg_char = System.Text.Encoding.ASCII.GetString(msg_byte).ToCharArray();

                    if (new string(msg_char, 0, 4).Equals("GENP"))
                    {
                        pwmGenCfg.isPwmGen = true;
                        pwmGenCfg.numChannels = BitConverter.ToInt32(msg_byte, 4);
                        pwmGenCfg.pins = new string[pwmGenCfg.numChannels];
                        for (int i = 0; i < this.genCfg.numChannels; i++)
                        {
                            pwmGenCfg.pins[i] = new string(msg_char, 8 + 4 * i, 4);
                        }
                    }
                    else
                    {
                        pwmGenCfg.isPwmGen = false;
                    }

                    port.DiscardInBuffer();

                    /* ------------------------------------------------------------------------------ */
                    /* ------------ GENERATOR OF SYNCHRONIZED PWM SIGNALS CONFIG REQUEST ------------ */
                    /* ------------------------------------------------------------------------------ */
                    port.Write(Commands.SYNC_PWM_GEN + ":" + Commands.CONFIGRequest + ";");
                    Thread.Sleep(wait);
                    toRead = port.BytesToRead;
                    port.Read(msg_byte, 0, toRead);
                    msg_char = Encoding.ASCII.GetString(msg_byte).ToCharArray();

                    if (new string(msg_char, 0, 4).Equals("SYNP"))
                    {
                        syncPwmCfg.isSyncPwm = true;
                        syncPwmCfg.periphClock = BitConverter.ToInt32(msg_byte, 4);
                        syncPwmCfg.maxFreq = BitConverter.ToInt32(msg_byte, 8);
                        syncPwmCfg.numOfChannels = BitConverter.ToInt32(msg_byte, 12);
                        syncPwmCfg.pins = new string[syncPwmCfg.numOfChannels];
                        for (int i = 0; i < this.syncPwmCfg.numOfChannels; i++)
                        {
                            syncPwmCfg.pins[i] = new string(msg_char, 16 + 4 * i, 4);
                        }
                    }
                    else
                    {
                        pwmGenCfg.isPwmGen = false;
                    }

                    port.DiscardInBuffer();

                    /* ------------------------------------------------------------------------------ */
                    /* ---------------------- LOGIC ANALYZER CONFIG REQUEST ------------------------- */
                    /* ------------------------------------------------------------------------------ */
                    port.Write(Commands.LOG_ANLYS + ":" + Commands.CONFIGRequest + ";");
                    Thread.Sleep(wait);
                    toRead = port.BytesToRead;
                    port.Read(msg_byte, 0, toRead);
                    msg_char = Encoding.ASCII.GetString(msg_byte).ToCharArray();

                    if (new string(msg_char, 0, 4).Equals("LOGA"))
                    {
                        logAnlysCfg.isLogAnlys = true;
                        logAnlysCfg.postTrigPeriphClock = BitConverter.ToInt32(msg_byte, 4);
                        logAnlysCfg.timeBasePeriphClock = BitConverter.ToInt32(msg_byte, 8);
                        logAnlysCfg.samplingFreq = BitConverter.ToInt32(msg_byte, 12);
                        logAnlysCfg.bufferLength = BitConverter.ToInt32(msg_byte, 16);
                        logAnlysCfg.numOfChannels = BitConverter.ToInt32(msg_byte, 20);
                        logAnlysCfg.pins = new string[logAnlysCfg.numOfChannels];
                        for (int i = 0; i < this.logAnlysCfg.numOfChannels; i++)
                        {
                            logAnlysCfg.pins[i] = new string(msg_char, 24 + 4 * i, 4);
                        }
                    }
                    else
                    {
                        logAnlysCfg.isLogAnlys = false;
                    }

                    port.DiscardInBuffer();

                    /* ------------------------------------------------------------------------------ */
                    /* --------------------------- COUNTER CONFIG REQUEST --------------------------- */
                    /* ------------------------------------------------------------------------------ */
                    port.Write(Commands.COUNTER + ":" + Commands.CONFIGRequest + ";");
                    Thread.Sleep(wait);
                    toRead = port.BytesToRead;
                    port.Read(msg_byte, 0, toRead);
                    msg_char = System.Text.Encoding.ASCII.GetString(msg_byte).ToCharArray();

                    if (new string(msg_char, 0, 4).Equals("CNT_"))
                    {
                        cntCfg.isCnt = true;
                        cntCfg.counterPeriphClock = BitConverter.ToInt32(msg_byte, 4);
                        cntCfg.gatePeriphClock = BitConverter.ToInt32(msg_byte, 8);
                        cntCfg.modes = new string(msg_char, 12, toRead - 37);
                        cntCfg.pins = new string(msg_char, 23, toRead - 11).Split(' ');

                        /* Without periphClocks reception */
                        //cntCfg.modes = new string(msg_char, 4, toRead - 29);
                        //cntCfg.pins = new string(msg_char, 15, toRead - 11).Split(' ');                        
                    }
                    else
                    {
                        cntCfg.isCnt = false;
                    }
                }
            }
            catch (Exception ex)
            {
                report.Sendreport("Fatal error during reading log", ex, this, logger, 03246);
            }
        }

        public void wait_for_data(int watch)
        {
            Thread.Sleep(1);            
            if (watch <= 0)
            {
                 throw (new Exception("Reading timeout")); 
            }
        }

        private void serialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            char[] inputMsg = new char[4];
            byte[] inputData = new byte[4];
            int watchDog = 2500;
            logTextNL("Příjem dat: " + port.BytesToRead.ToString());

            while (port.IsOpen && port.BytesToRead > 0)
            {

                /* inputMsg[0] = inputMsg[1]; //safe for un aligned messages
                  inputMsg[1] = inputMsg[2];
                  inputMsg[2] = inputMsg[3];
                  inputMsg[3] = (char)port.ReadChar();*/

                try
                {
                    while (port.BytesToRead < 4)
                    {
                        wait_for_data(watchDog--);
                        if (watchDog < 0)
                        {
                            // port.Read(inputMsg, 0, 2);
                        }
                    }
                    port.Read(inputMsg, 0, 4);
                    switch (new string(inputMsg, 0, 4))
                    {
                        case Commands.SCOPE_INCOME:
                            int res;
                            int leng;
                            int currChan;
                            int numChan;
                            int i = 0;
                            int partsLen = 4096;
                            while (port.BytesToRead < 12)
                            {
                                wait_for_data(watchDog--);
                            }
                            port.Read(inputData, 0, 4);
                            scopeCfg.realSmplFreq = inputData[0] * 256 * 256 * 256 + inputData[1] * 256 * 256 + inputData[2] * 256 + inputData[3];
                            res = port.ReadByte();
                            leng = port.ReadByte() * 65536 + port.ReadByte() * 256 + port.ReadByte();
                            port.Read(inputData, 0, 2);
                            currChan = port.ReadByte();
                            numChan = port.ReadByte();
                            scopeCfg.buffer = new Byte[leng];
                            int wasRead = 0;
                            int toRead = (leng - wasRead) > partsLen ? partsLen : (leng - wasRead);

                            while (toRead > 0 && port.IsOpen)
                            {
                                ///// if (port.BytesToRead <= leng + wasRead)
                                /// {
                                wasRead += port.Read(scopeCfg.buffer, wasRead, Math.Min(port.BytesToRead, toRead));
                                ///}
                                ///else {
                                ///   wasRead += port.Read(scopeCfg.buffer, wasRead, toRead);
                                /// }

                                toRead = (leng - wasRead) > partsLen ? partsLen : (leng - wasRead);
                            }

                            if (!port.IsOpen)
                            {
                                break;
                            }


                            if (res > 8) //resolution >8 bits
                            {
                                if (currChan == 1)
                                {
                                    scopeCfg.samples = new UInt16[numChan, leng / 2];
                                }
                                if (systemCfg.isShield)
                                {
                                    ushort depth = (ushort)Math.Pow(2, res);
                                    for (i = 0; i < leng / 2; i++)
                                    {
                                        scopeCfg.samples[currChan - 1, i] = (ushort)(depth - BitConverter.ToUInt16(scopeCfg.buffer, i * 2));
                                    }
                                }
                                else
                                {
                                    for (i = 0; i < leng / 2; i++)
                                    {
                                        scopeCfg.samples[currChan - 1, i] = BitConverter.ToUInt16(scopeCfg.buffer, i * 2);
                                    }
                                }
                            }
                            else  //resolution <=8 bits
                            {
                                if (currChan == 1)
                                {
                                    scopeCfg.samples = new UInt16[numChan, leng];
                                }

                                if (systemCfg.isShield)
                                {
                                    ushort depth = (ushort)Math.Pow(2, res);
                                    for (i = 0; i < leng; i++)
                                    {
                                        scopeCfg.samples[currChan - 1, i] = (ushort)(depth - scopeCfg.buffer[i]);
                                    }
                                }
                                else
                                {
                                    for (i = 0; i < leng; i++)
                                    {
                                        scopeCfg.samples[currChan - 1, i] = scopeCfg.buffer[i];
                                    }
                                }
                            }




                            if (currChan == numChan)
                            {
                                if (res > 8)
                                {
                                    scopeCfg.timeBase = new double[leng / 2];
                                    generate_time_base(scopeCfg.realSmplFreq, leng / 2);
                                }
                                else
                                {
                                    scopeCfg.timeBase = new double[leng];
                                    generate_time_base(scopeCfg.realSmplFreq, leng);
                                }
                                scopeCfg.actualChannels = numChan;
                                scopeCfg.actualRes = res;
                                logRecieved("SCOPE DATA RECIEVED: Leng " + leng + ", Res " + res + ", Chan " + currChan + " of " + numChan);
                                Thread.Sleep(10);
                                switch (ADCFormOpened)
                                {
                                    case FormOpened.SCOPE:
                                        Scope_form.add_message(new Message(Message.MsgRequest.SCOPE_NEW_DATA));
                                        break;
                                    case FormOpened.VOLTMETER:
                                        Volt_form.add_message(new Message(Message.MsgRequest.VOLT_NEW_DATA));
                                        break;
                                    case FormOpened.FREQ_ANALYSIS:
                                        FreqAnalysis_form.add_message(new Message(Message.MsgRequest.BODE_NEW_DATA));
                                        break;
                                }
                            }
                            //Console.WriteLine("SCOPE DATA RECIEVED: Leng "+leng+", Res "+res+", Chan "+currChan+" of "+numChan);
                            break;

                        case Commands.ACKNOWLEDGE:
                            logRecieved("ACK");
                            //Console.WriteLine(Commands.ACKNOWLEDGE);
                            break;
                        case Commands.SCOPE_OK:
                            logRecieved("S_OK");
                            //Console.WriteLine(Commands.ACKNOWLEDGE);
                            break;
                        case Commands.SAMPLING:
                            logRecieved("SMPL");
                            //Console.WriteLine(Commands.ACKNOWLEDGE);
                            break;
                        case Commands.TRIGGERED:
                            //Console.WriteLine(Commands.TRIGGERED);
                            logRecieved("TRIG");
                            if (ADCFormOpened == FormOpened.SCOPE)
                            {
                                Scope_form.add_message(new Message(Message.MsgRequest.SCOPE_TRIGGERED));
                            }
                            break;
                        case Commands.GEN_OK:
                            //Console.WriteLine(Commands.TRIGGERED);
                            logRecieved("GEN_OK");
                            switch (DACFormOpened)
                            {
                                case FormOpened.GENERATOR:
                                    Gen_form.add_message(new Message(Message.MsgRequest.GEN_OK));
                                    break;
                                case FormOpened.VOLT_SOURCE:
                                    Source_form.add_message(new Message(Message.MsgRequest.GEN_OK));
                                    break;
                                case FormOpened.FREQ_ANALYSIS:
                                    FreqAnalysis_form.genMessage(new Message(Message.MsgRequest.GEN_OK));
                                    break;
                            }
                            break;
                        case Commands.GEN_NEXT:
                            //Console.WriteLine(Commands.TRIGGERED);
                            logRecieved("NEXT");
                            if (DACFormOpened == FormOpened.GENERATOR)
                            {
                                Gen_form.add_message(new Message(Message.MsgRequest.GEN_NEXT));
                            }
                            else if (DACFormOpened == FormOpened.FREQ_ANALYSIS)
                            {
                                FreqAnalysis_form.genMessage(new Message(Message.MsgRequest.GEN_NEXT));
                            }
                            break;
                        case Commands.GENERATOR:
                            while (port.BytesToRead < 8)
                            {
                                wait_for_data(watchDog--);
                            }
                            port.Read(inputMsg, 0, 4);
                            port.Read(inputData, 0, 4);
                            if (DACFormOpened == FormOpened.GENERATOR)
                            {
                                Gen_form.add_message(new Message(Message.MsgRequest.GEN_FRQ, new string(inputMsg, 0, 4), inputData[1] * 256 * 256 + inputData[2] * 256 + inputData[3]));
                            }
                            else if (DACFormOpened == FormOpened.FREQ_ANALYSIS)
                            {
                                FreqAnalysis_form.genMessage(new Message(Message.MsgRequest.GEN_FRQ, new string(inputMsg, 0, 4), inputData[1] * 256 * 256 + inputData[2] * 256 + inputData[3]));
                            }
                            //Console.WriteLine(Commands.TRIGGERED);
                            logRecieved("GEN_FRQ?" + new string(inputMsg, 1, 3) + " CH" + inputData[3].ToString());
                            break;
                        case Commands.GEN_PWM_REAL_FREQ_CH1:
                            while (port.BytesToRead < 8)
                            {
                                wait_for_data(watchDog--);
                            }

                            byte[] inputValRealFreq = new byte[8];
                            port.Read(inputValRealFreq, 0, 8);
                            double message = 0;

                            try
                            {
                                message = BitConverter.Int64BitsToDouble(BitConverter.ToInt64(inputValRealFreq, 0));
                                Counter_form.add_message(new Message(Message.MsgRequest.GEN_PWM_REAL_FREQ_CH1, "GEN_REAL_FREQ", message));
                                logRecieved("Gen PWM Real Freq " + message.ToString());
                            }
                            catch (Exception ex)
                            {
                                logRecieved("Gen PWM Real Freq not parsed  " + message.ToString());
                            }
                            break;
                        case Commands.GEN_PWM_REAL_FREQ_CH2:
                            while (port.BytesToRead < 8)
                            {
                                wait_for_data(watchDog--);
                            }

                            byte[] inputValRealFreq2 = new byte[8];
                            port.Read(inputValRealFreq2, 0, 8);
                            double message2 = 0;

                            try
                            {
                                message2 = BitConverter.Int64BitsToDouble(BitConverter.ToInt64(inputValRealFreq2, 0));
                                Counter_form.add_message(new Message(Message.MsgRequest.GEN_PWM_REAL_FREQ_CH2, "GEN_REAL_FREQ", message2));
                                logRecieved("Gen PWM Real Freq " + message2.ToString());
                            }
                            catch (Exception ex)
                            {
                                logRecieved("Gen PWM Real Freq not parsed  " + message2.ToString());
                            }
                            break;
                        /* -------------------------------------------------------------------------------------------------------------------------------- */
                        /* ---------------------------------------------- LOGIC ANALYZER RECEIVED MESSAGES ------------------------------------------------ */
                        /* -------------------------------------------------------------------------------------------------------------------------------- */
                        //case Commands.LOG_ANLYS_USER_TRIGGER:
                        //    while (port.BytesToRead < 4)
                        //    {
                        //        wait_for_data(watchDog--);
                        //    }
                        //    port.Read(inputData, 0, 4);
                        //    int userTrigger = BitConverter.ToInt32(inputData, 0);
                        //    Debug.WriteLine(userTrigger.ToString());
                        //    LogAnlys_form.add_message(new Message(Message.MsgRequest.LOG_ANLYS_USER_TRIGGER, "LOG_ANLYS_USER_TRIGGER", userTrigger));
                        //    break;

                        case Commands.LOG_ANLYS_TRIGGER_POINTER:
                            while (port.BytesToRead < 4)
                            {
                                wait_for_data(watchDog--);
                            }
                            port.Read(inputData, 0, 4);
                            int triggerPointer = trigP = BitConverter.ToInt32(inputData, 0);                            
                            LogAnlys_form.add_message(new Message(Message.MsgRequest.LOG_ANLYS_TRIGGER_POINTER, "LOG_ANLYS_TRIG_POINTER", triggerPointer));
                            break;

                        case Commands.LOG_ANLYS_DATA_LENGTH:
                            while (port.BytesToRead < 4)
                            {
                                wait_for_data(watchDog--);
                            }
                            port.Read(inputData, 0, 4);
                            int dataLength = receiveDataLength = BitConverter.ToInt32(inputData, 0);
                            break;

                        case Commands.LOG_ANLYS_DATA:
                            watchDog = 5000;
                            //while (port.IsOpen && port.BytesToRead < receiveDataLength)
                            //{
                            //    wait_for_data(watchDog--);
                            //}

                            //if (!port.IsOpen)
                            //{
                            //    break;
                            //}

                            //byte[] receiveArray = new byte[receiveDataLength];
                            //logAnlysCfg.samples = new ushort[(uint)(receiveDataLength / 2)];

                            //port.Read(receiveArray, 0, receiveDataLength);

                            //for (int j = 0; j < (uint)(receiveDataLength / 2); j++)
                            //{
                            //    logAnlysCfg.samples[j] = BitConverter.ToUInt16(receiveArray, j * 2);
                            //}
                            //LogAnlys_form.add_message(new Message(Message.MsgRequest.LOG_ANLYS_DATA, "LOG_ANLYS_DATA"));
                            //break;
                            const int blockSize = 50;                         // SHOULD BE SET TO 100 !!!                    
                            int blockNum = receiveDataLength / blockSize;       // number of blocks to be received
                                                                                //watchDog = 5000;
                            byte[] receiveArray = new byte[blockSize];          // temporary block array used in FOR loop 
                            byte[] completeArray = new byte[receiveDataLength]; // complete array gradually filled by temp block array

                            for (int ix = 0; ix < blockNum; ix++)
                            {
                                while (port.IsOpen && (port.BytesToRead < blockSize))
                                {
                                    wait_for_data(watchDog--);
                                }

                                //port.Read(receiveArray, 0, blockSize);          // read the block of data into temporary block array                          
                                //Array.Copy(receiveArray, 0, completeArray, ix * blockSize, blockSize);  // adding the temp array data to completeArray from required shifted index
                                port.Read(completeArray, ix * blockSize, blockSize);

                                Thread.Yield();
                            }
                            /* receiveDataLength is doubled because of 16-bit sampling, but sending 8-bit array */
                            logAnlysCfg.samples = new ushort[(uint)(receiveDataLength / 2)];
                            /* Make from received 8-bit array back the 16-bit */
                            for (int j = 0; j < (uint)(receiveDataLength / 2); j++)
                            {
                                logAnlysCfg.samples[j] = BitConverter.ToUInt16(completeArray, j * 2);
                            }
                            LogAnlys_form.add_message(new Message(Message.MsgRequest.LOG_ANLYS_DATA, "LOG_ANLYS_DATA"));
                            break;
                        /* -------------------------------------------------------------------------------------------------------------------------------- */
                        /* -------------------------------------------------- COUNTER RECEIVED MESSAGES --------------------------------------------------- */
                        /* -------------------------------------------------------------------------------------------------------------------------------- */
                        /************************* ETR data *************************/
                        case Commands.CNT_ETR_DATA:
                            while (port.BytesToRead < 8) // 16
                            {
                                wait_for_data(watchDog--);
                            }

                            byte[] inputValEtr = new byte[8];
                            port.Read(inputValEtr, 0, 8);

                            try
                            {

                                cntMessageDouble = BitConverter.Int64BitsToDouble(BitConverter.ToInt64(inputValEtr, 0)); //BitConverter.ToSingle(inputValEtr, 0);                                                                                               
                                Counter_form.add_message(new Message(Message.MsgRequest.COUNTER_ETR_DATA, "ETR_DATA", cntMessageDouble));
                                logRecieved("CNT ETR " + cntMessageDouble.ToString());
                            }
                            catch (Exception ex)
                            {
                                logRecieved("Counter freq ETR was not parsed  " + cntMessageDouble.ToString());
                            }
                            break;
                        /************************* IC1 data *************************/
                        case Commands.CNT_IC1_DATA:
                            while (port.BytesToRead < 8)
                            {
                                wait_for_data(watchDog--);
                            }

                            byte[] inputValIc1 = new byte[8];
                            port.Read(inputValIc1, 0, 8);

                            try
                            {
                                cntMessageDouble = BitConverter.Int64BitsToDouble(BitConverter.ToInt64(inputValIc1, 0));
                                Counter_form.add_message(new Message(Message.MsgRequest.COUNTER_IC1_DATA, "IC1_DATA", cntMessageDouble));
                                logRecieved("CNT IC1 " + cntMessageDouble.ToString());
                            }
                            catch (Exception ex)
                            {
                                logRecieved("Counter freq IC1 was not parsed  " + cntMessageDouble.ToString());
                            }                            
                            break;
                        /************************* IC2 data *************************/
                        case Commands.CNT_IC2_DATA:
                            while (port.BytesToRead < 8)
                            {
                                wait_for_data(watchDog--);
                            }

                            byte[] inputValIc2 = new byte[8];
                            port.Read(inputValIc2, 0, 8);

                            try
                            {
                                cntMessageDouble = BitConverter.Int64BitsToDouble(BitConverter.ToInt64(inputValIc2, 0));
                                Counter_form.add_message(new Message(Message.MsgRequest.COUNTER_IC2_DATA, "IC2_DATA", cntMessageDouble));
                                logRecieved("CNT IC2 " + cntMessageDouble.ToString());
                            }
                            catch (Exception ex)
                            {
                                logRecieved("Counter freq IC2 was not parsed  " + cntMessageDouble.ToString());
                            }
                            break;
                        /************************* IC1 Duty Cycle *************************/
                        case Commands.CNT_DUTY_CYCLE_RECEIVE:
                            while (port.BytesToRead < 8)
                            {
                                wait_for_data(watchDog--);
                            }

                            byte[] inputValIcDc = new byte[8]; 
                            port.Read(inputValIcDc, 0, 8);

                            try
                            {
                                cntMessageDouble = BitConverter.Int64BitsToDouble(BitConverter.ToInt64(inputValIcDc, 0));
                                Counter_form.add_message(new Message(Message.MsgRequest.COUNTER_IC_DUTY_CYCLE, "IC_DUTY_CYCLE", cntMessageDouble));
                                logRecieved("CNT Duty Cycle " + cntMessageDouble.ToString());
                            }
                            catch (Exception ex)
                            {
                                logRecieved("Counter IC Duty Cycle was not parsed  " + cntMessageDouble.ToString());
                            }
                            break;
                        /************************* IC1 Pulse Width  *************************/
                        case Commands.CNT_PULSE_WIDTH_RECEIVE:
                            while (port.BytesToRead < 8)
                            {
                                wait_for_data(watchDog--);
                            }
                            byte[] inputValIcPw1 = new byte[8];
                            port.Read(inputValIcPw1, 0, 8);

                            try
                            {    
                                cntMessageDouble = BitConverter.Int64BitsToDouble(BitConverter.ToInt64(inputValIcPw1, 0));
                                Counter_form.add_message(new Message(Message.MsgRequest.COUNTER_IC_PULSE_WIDTH, "IC_PULSE_WIDTH", cntMessageDouble));
                                logRecieved("CNT Pulse Width " + cntMessageDouble.ToString());
                            }
                            catch (Exception ex)
                            {
                                logRecieved("Counter Pulse Width was not parsed  " + cntMessageDouble.ToString());
                            }
                            break;
                        /************************* REF data *************************/
                        case Commands.CNT_REF_DATA:
                            while (port.BytesToRead < 8)
                            {
                                wait_for_data(watchDog--);
                            }

                            byte[] inputValRef = new byte[8];                            
                            port.Read(inputValRef, 0, 8);

                            try
                            {
                                cntMessageDouble = BitConverter.Int64BitsToDouble(BitConverter.ToInt64(inputValRef, 0));                                
                                Counter_form.add_message(new Message(Message.MsgRequest.COUNTER_REF_DATA, "REF_DATA", cntMessageDouble));
                                logRecieved("CNT REF data " + cntMessageDouble.ToString());
                            }
                            catch (Exception ex)
                            {
                                logRecieved("Counter buffer REF was not parsed  " + cntMessageDouble.ToString());
                            }
                            break;
                        /************************* REF warning *************************/
                        case Commands.CNT_REF_WARN:
                            try
                            {
                                Counter_form.add_message(new Message(Message.MsgRequest.COUNTER_REF_WARN, "REF_WARN", 1));
                            }
                            catch (Exception ex)
                            {
                                logRecieved("Counter buffer REF was not parsed");
                            }
                            break;
                        /************************* TI TIMEOUT *************************/
                        case Commands.CNT_TI_TIMEOUT_OCCURED:
                            try
                            {                                                                
                                Counter_form.add_message(new Message(Message.MsgRequest.COUNTER_TI_TIMEOUT, "TI_TIMEOUT", 1));
                            }
                            catch (Exception ex)
                            {
                                logRecieved("Counter buffer TI was not parsed");
                            }
                            break;
                        /************************* TI DATA *************************/
                        case Commands.CNT_TI_DATA:
                            while (port.BytesToRead < 8)
                            {
                                wait_for_data(watchDog--);
                            }

                            byte[] inputValBuf2 = new byte[8];
                            port.Read(inputValBuf2, 0, 8);

                            try
                            {
                                cntMessageDouble = BitConverter.Int64BitsToDouble(BitConverter.ToInt64(inputValBuf2, 0)); //BitConverter.ToSingle(inputValEtr, 0);                                                                                               
                                Counter_form.add_message(new Message(Message.MsgRequest.COUNTER_TI_DATA, "TI_BUF2_BIGGER", cntMessageDouble));
                                logRecieved("CNT Time Interval Data parsed" + cntMessageDouble.ToString());
                            }
                            catch (Exception ex)
                            {
                                logRecieved("CNT Time Interval Data NOT parsed!" + cntMessageDouble.ToString());
                            }
                            break;
                        /************************* Default *************************/
                        default:
                            if (inputMsg[0] == Commands.ERROR)
                            {
                                try
                                {
                                    int err = int.Parse(new string(inputMsg, 1, 3));

                                    logRecieved("ERROR " + err);
                                    if (err == 107)
                                    {
                                        break;
                                    }
                                    if (err == 999 && LastCommand.Equals(Commands.SCOPE + ":" + Commands.SCOPE_NEXT + ";"))
                                    {
                                        LastCommand = "";
                                        this.send(Commands.SCOPE + ":" + Commands.SCOPE_NEXT + ";");
                                        //MessageBox.Show("Recovered from Err\r\n" + new string(inputMsg, 0, 4) + "\r\n" + getErrReason(err), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                    }
                                    if (lastError != err)
                                    {
                                        MessageBox.Show("Error recieved \r\n" + new string(inputMsg, 0, 4) + "\r\n" + getErrReason(err), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                        //scopeCfg.mode = Scope.mode_def.IDLE;
                                        lastError = err;
                                    }
                                }
                                catch (Exception ex)
                                {
                                    logRecieved("Unknown error " + new string(inputMsg, 0, 4));
                                    logTextNL(ex.ToString());
                                    logTextNL(Environment.StackTrace.ToString());
                                    if (lastError != -1)
                                    {

                                        MessageBox.Show("Unknow Error recieved \r\n" + new string(inputMsg, 0, 4), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                        lastError = -1;
                                        //Console.WriteLine(new string(inputMsg, 0, 4));
                                    }
                                }
                            }
                            else
                            {
                                int red = port.BytesToRead;
                                char[] bu = new char[red];
                                port.Read(bu, 0, red);
                                string s = new string(bu);
                                logRecieved("Unknown message " + "(0x" + String.Format("{0:X}", Convert.ToByte(inputMsg[0])) + String.Format("{0:X}", Convert.ToByte(inputMsg[1]), 2) + String.Format("{0:X}", Convert.ToByte(inputMsg[2]), 2) + String.Format("{0:X}", Convert.ToByte(inputMsg[3]), 2) + ")" + new string(inputMsg, 0, 4) + " (buffer:" + red + ":" + s);
                                if (lastError != -1)
                                {
                                    MessageBox.Show("Unknow message recieved \r\n" + red + "\r\n" + s + "\r\n(0x" + String.Format("{0:X}", Convert.ToByte(inputMsg[0]), 2) + String.Format("{0:X}", Convert.ToByte(inputMsg[1]), 2) + String.Format("{0:X}", Convert.ToByte(inputMsg[2]), 2) + String.Format("{0:X}", Convert.ToByte(inputMsg[3]), 2) + ")" + new string(inputMsg, 0, 4), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                                    lastError = -1;


                                    //Console.WriteLine(new string(inputMsg, 0, 4));
                                }
                            }
                            break;
                    }
                    if (!port.IsOpen)
                    {
                        break;
                    }
                }
                catch (System.ArgumentException ex)
                {
                    if (port.IsOpen)
                    {
                        port.DiscardInBuffer();
                        logTextNL("communication error:\r\n"+ex.ToString());
                        logTextNL(Environment.StackTrace.ToString());
                        report.Sendreport("Mismatch communication Error recieved", ex, this, logger, 31681);
                        MessageBox.Show("Unknow error \r\n", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
                catch (Exception ex)
                {
                    logTextNL("communication error:\r\n" + ex.ToString());
                    logTextNL(Environment.StackTrace.ToString());
                    MessageBox.Show("Unknow error \r\n", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                Thread.Yield();
            }
        }

        private void serialPort_ErrorReceived(object sender, System.IO.Ports.SerialErrorReceivedEventArgs e)
        {
            logTextNL("Serial port error recieved:\r\n");
            report.Sendreport("Serial port error recieved", new Exception("Communication error handler"), this, logger, 13587);
        }



        private void generate_time_base(int sampling, int lenght)
        {
            for (int i = 0; i < lenght; i++)
            {
                scopeCfg.timeBase[i] = (double)(i) / sampling;
            }
            scopeCfg.maxTime = (double)(lenght) / sampling;
        }

        public void open_scope()
        {
            if(LogAnlysOpened == FormOpened.LOG_ANLYS)
            {
                close_logAnlys();
            }            
            
            /* Close Synch PWM generator if Scope opened (due to DMA2) */
            /*if (SyncPwmOpened == FormOpened.SYNC_PWM_GENERATOR)
            {
                close_syncPwm_gen();                
            }*/
            if (ADCFormOpened == FormOpened.VOLTMETER)
            {
                close_volt();
            }
            if (DACFormOpened == FormOpened.FREQ_ANALYSIS)
            {
                close_freq_analysis();
            }

            if (Scope_form == null || Scope_form.IsDisposed)
            {
                Scope_form = new Scope(this);
                Scope_form.Show();
                ADCFormOpened = FormOpened.SCOPE;
                Thread.Sleep(200);
                Scope_form.scope_start();
            }
            else
            {
                Scope_form.BringToFront();
            }
        }

        public void close_scope()
        {
            if (Scope_form != null)
            {
                Scope_form.Close();
                ADCFormOpened = FormOpened.NONE;
            }
        }

        public void scopeClosed()
        {
            ADCFormOpened = FormOpened.NONE;
        }

        public void open_gen()
        {            
            if (DACFormOpened == FormOpened.VOLT_SOURCE)
            {
                close_source();
            }
            if (DACFormOpened == FormOpened.FREQ_ANALYSIS)
            {
                close_freq_analysis();
            }
            if (GenOpened == GenModeOpened.PWM)
            {
                close_gen();
            }

            if (Gen_form == null || Gen_form.IsDisposed)
            {
                GenOpened = GenModeOpened.DAC;
                Gen_form = new Generator(this);
                Gen_form.Show();
                DACFormOpened = FormOpened.GENERATOR;
            }
            else
            {
                Gen_form.BringToFront();
            }
        }

        public void close_gen()
        {            
            if (Gen_form != null)
            {
                Gen_form.Close();                
            }
        }

        public void open_pwm_gen()
        {
            if (LogAnlysOpened == FormOpened.LOG_ANLYS)
            {
                close_logAnlys();
            }

            if (DACFormOpened == FormOpened.VOLT_SOURCE)
            {
                close_source();
            }
            if (DACFormOpened == FormOpened.FREQ_ANALYSIS)
            {
                close_freq_analysis();
            }
            if (GenOpened == GenModeOpened.DAC)
            {
                close_gen();
            }

            if (Gen_form == null || Gen_form.IsDisposed)
            {
                GenOpened = GenModeOpened.PWM;
                Gen_form = new Generator(this);
                Gen_form.Show();
                DACFormOpened = FormOpened.GENERATOR;
            }
            else
            {
                Gen_form.BringToFront();
            }
        }

        public void open_volt()
        {
            if (ADCFormOpened == FormOpened.SCOPE)
            {
                close_scope();
            }
            if (DACFormOpened == FormOpened.FREQ_ANALYSIS)
            {
                close_freq_analysis();
            }

            close_logAnlys();

            if (Volt_form == null || Volt_form.IsDisposed)
            {
                Volt_form = new Voltmeter(this);
                Volt_form.Show();
                ADCFormOpened = FormOpened.VOLTMETER;
            }
            else
            {
                Volt_form.BringToFront();
            }
        }

        public void close_volt()
        {
            if (Volt_form != null)
            {
                Volt_form.Close();
                ADCFormOpened = FormOpened.NONE;
            }
        }

        public void open_counter()
        {

            if (LogAnlysOpened == FormOpened.LOG_ANLYS)
            {
                close_logAnlys();
            }

            if (Counter_form == null || Counter_form.IsDisposed)
            {
                Counter_form = new Counter(this);
                Counter_form.Show();
            }
            else
            {
                Counter_form.BringToFront();
            }
        }

        public void close_counter()
        {
            if (Counter_form != null)
            {
                Counter_form.Close();
            }
        }

        public void open_syncPwm_gen()
        {
            /*if (ADCFormOpened == FormOpened.SCOPE)
            {
                close_scope();
            }*/

            if (SyncPwm_form == null || SyncPwm_form.IsDisposed)
            {
                SyncPwmOpened = FormOpened.SYNC_PWM_GENERATOR;
                SyncPwm_form = new SyncPwmGenerator(this);
                SyncPwm_form.Show();
            }
            else
            {
                SyncPwm_form.BringToFront();
            }
        }

        public void close_syncPwm_gen()
        {
            if (SyncPwm_form != null)
            {
                SyncPwm_form.Close();
                SyncPwmOpened = FormOpened.NONE;
            }
        }

        public void open_logAnlys()
        {
            if (ADCFormOpened == FormOpened.SCOPE)
            {
                close_scope();
            }

            close_counter();  
            close_volt();

            if (GenOpened == GenModeOpened.PWM)
            {
                close_gen();
            }            

            if (LogAnlys_form == null || LogAnlys_form.IsDisposed)
            {
                LogAnlysOpened = FormOpened.LOG_ANLYS;
                LogAnlys_form = new LogicAnalyzer(this);
                LogAnlys_form.Show();
            }
            else
            {
                LogAnlys_form.BringToFront();
            }
        }

        public void close_logAnlys()
        {
            if (LogAnlys_form != null)
            {
                LogAnlys_form.Close();
                LogAnlysOpened = FormOpened.NONE;
            }            
        }

        public void open_source()
        {
            if (DACFormOpened == FormOpened.GENERATOR)
            {
                close_gen();
            }
            if (DACFormOpened == FormOpened.FREQ_ANALYSIS)
            {
                close_freq_analysis();
            }

            if (Source_form == null || Source_form.IsDisposed)
            {
                GenOpened = GenModeOpened.DAC;
                Source_form = new VoltageSource(this);
                Source_form.Show();
                DACFormOpened = FormOpened.VOLT_SOURCE;
            }
            else
            {
                Source_form.BringToFront();
            }
        }

        public void close_source()
        {
            if (Source_form != null)
            {
                Source_form.Close();
                DACFormOpened = FormOpened.NONE;
            }
        }

        public void open_freq_analysis()
        {
            if (DACFormOpened == FormOpened.GENERATOR)
            {
                close_gen();
            }
            else if (DACFormOpened == FormOpened.VOLT_SOURCE)
            {
                close_source();
            }

            if (ADCFormOpened == FormOpened.SCOPE)
            {
                close_scope();
            }
            else if (ADCFormOpened == FormOpened.VOLTMETER)
            {
                close_volt();
            }

            if (FreqAnalysis_form == null || FreqAnalysis_form.IsDisposed)
            {
                FreqAnalysis_form = new BodePlot(this);
                FreqAnalysis_form.Show();
                DACFormOpened = FormOpened.FREQ_ANALYSIS;
                ADCFormOpened = FormOpened.FREQ_ANALYSIS;
            }
            else
            {
                FreqAnalysis_form.BringToFront();
            }

        }

        public void close_freq_analysis()
        {
            if (FreqAnalysis_form != null)
            {
                FreqAnalysis_form.Close();
                ADCFormOpened = FormOpened.NONE;
                DACFormOpened = FormOpened.NONE;
            }
        }

        public void voltClosed()
        {
            ADCFormOpened = FormOpened.NONE;
        }

        public void sourceClosed()
        {
            DACFormOpened = FormOpened.NONE;
        }

        public SystemConfig_def getSystemCfg()
        {
            return this.systemCfg;
        }
        public CommsConfig_def getCommsCfg()
        {
            return this.commsCfg;
        }

        public void set_scope_mode(Scope.TRIG_MODE mod)
        {
            this.scopeCfg.mode = mod;
        }

        public Scope.TRIG_MODE get_scope_mode()
        {
            return scopeCfg.mode;
        }

        public bool takeCommsSemaphore(int ms)
        {
            bool result = false;
            result = commsSemaphore.WaitOne(ms);
            if (!result)
            {
                Exception ex = new Exception("Unable to take semaphore");
                logTextNL(ex.ToString());
                logTextNL(Environment.StackTrace.ToString());
                throw ex;
            }
            semaphoreTakenBy = ms;
            return result;
        }

        public void giveCommsSemaphore()
        {
            commsSemaphore.Release();
        }


        public void send(string s)
        {
            try
            {
                LastCommand = s;
                port.Write(s);

                logSend(s);
            }
            catch (Exception ex)
            {
                logTextNL("Data se nepodařilo odeslat:\r\n" + ex);
                Console.WriteLine(ex);
                portError = true;
                
            }
        }

        public bool isPortError()
        {
            if (portError)
            {
                portError = false;
                return true;
            }
            else {
                return false;
            }
        }

        public bool send_short_2byte(int l)
        {
            try
            {
                byte[] bt = BitConverter.GetBytes(l);
                byte[] se = new byte[2];
                se[0] = bt[0];
                se[1] = bt[1];
                logText(l.ToString("D4") + "(0x" + BitConverter.ToString(se, 0).Replace("-", "") + ")");
                port.Write(bt, 0, 2);
                return true;
            }
            catch (Exception ex)
            {
                logTextNL("Data se nepodařilo odeslat:\r\n" + ex);
                Console.WriteLine(ex);
                portError = true;
                return false;
            }
            // Console.WriteLine(l.ToString());
        }


        public void send_short(int l)
        {
            try
            {
                byte[] bt = BitConverter.GetBytes(l);
                byte[] se = new byte[4];
                se[0] = 0;
                se[1] = 0;
                se[2] = bt[0];
                se[3] = bt[1];
                logTextNL(l.ToString() + "(0x" + BitConverter.ToString(se, 0).Replace("-", "") + ")");
                port.Write(bt, 0, 4);
            }
            catch (Exception ex)
            {
                logTextNL("Data se nepodařilo odeslat:\r\n" + ex);
                Console.WriteLine(ex);
                portError = true;
            }
            // Console.WriteLine(l.ToString());
        }

        public void send_int(int l)
        {
            try
            {
                byte[] bt = BitConverter.GetBytes(l);
                byte[] se = new byte[4];

                se[0] = bt[0];
                se[1] = bt[1];
                se[2] = bt[2];
                se[3] = bt[3];
                logTextNL(l.ToString() + "(0x" + BitConverter.ToString(se, 0).Replace("-", "") + ")");
                port.Write(se, 0, 4);
            }
            catch (Exception ex)
            {
                logTextNL("Data se nepodařilo odeslat:\r\n" + ex);
                Console.WriteLine(ex);
                portError = true;
            }
            // Console.WriteLine(l.ToString());
        }

        public void Log(string logMessage)
        {
            try
            {

                if (writeLog)
                {
                    logSemaphore.WaitOne(1000);
                    logWriter.Write("\r\nLog Entry : ");
                    logWriter.WriteLine("{0} {1}", DateTime.Now.ToLongTimeString(),
                    DateTime.Now.ToLongDateString());
                    logWriter.WriteLine("  :{0}", logMessage);
                    logWriter.WriteLine("-------------------------------");
                    logSemaphore.Release();
                }
                addLoggerString("Log Entry : " + DateTime.Now.ToLongTimeString() + "  " + DateTime.Now.ToLongDateString() + " :" + logMessage + "\r\n---------------------------\r\n");

            }
            catch (Exception ex)
            {

            }
        }

        public void LogEnd()
        {
            try
            {


                if (writeLog)
                {
                    logSemaphore.WaitOne(1000);
                    logWriter.WriteLine("Konec logu");
                    logWriter.WriteLine("-------------------------------");
                    logWriter.WriteLine("\r\n\r\n\r\n");
                    logSemaphore.Release();
                }
                addLoggerString("Konec logu\r\n--------------------------\r\n");

            }
            catch (Exception ex)
            {

            }
        }

        public void logSend(string s)
        {
            try
            {

                if (writeLog)
                {
                    logSemaphore.WaitOne(1000);
                    logWriter.Write("Send (" + DateTime.Now.Minute + ":" + DateTime.Now.Second + "." + DateTime.Now.Millisecond + "): " + s + "\r\n");
                    logSemaphore.Release();
                }
                addLoggerString("Send (" + DateTime.Now.Minute + ":" + DateTime.Now.Second + "." + DateTime.Now.Millisecond + "): " + s + "\r\n");

            }
            catch (Exception ex)
            {

            }
        }

        public void logRecieved(string s)
        {
            if (writeLog)
            {
                logSemaphore.WaitOne(1000);
                logWriter.Write("Recieved (" + DateTime.Now.Minute + ":" + DateTime.Now.Second + "." + DateTime.Now.Millisecond + "): " + s + "\r\n");
                logSemaphore.Release();
            }
            addLoggerString("Recieved (" + DateTime.Now.Minute + ":" + DateTime.Now.Second + "." + DateTime.Now.Millisecond + "): " + s + "\r\n");
        }

        public void logTextNL(string s)
        {
            if (writeLog)
            {
                logSemaphore.WaitOne(1000);
                logWriter.Write(s + "\r\n");
                logSemaphore.Release();
            }
            addLoggerString(s + "\r\n");
        }

        public void logText(string s)
        {
            if (writeLog)
            {
                logSemaphore.WaitOne(1000);
                logWriter.Write(s + ", ");
                logSemaphore.Release();
            }
            addLoggerString(s + ", ");
        }

        public void addLoggerString(string log)
        {
            logger.Add(log);
            if (logger.Count() > 100)
            {
                logger.RemoveAt(0);
            }
        }

        public List<String> getLogger()
        {
            return this.logger;
        }

        public string getErrReason(int err)
        {
            string result = null;
            switch (err)
            {
                case 1:
                    result = "Reading configuration error";
                    break;
                case 50:
                    result = "Scope - Invalid feature";
                    break;
                case 54:
                    result = "Scope - Invalid param";
                    break;
                case 55:
                    result = "Scope - Unsuported resolution";
                    break;
                case 56:
                    result = "Scope - Invalid trigger channel";
                    break;
                case 57:
                    result = "Scope - Invalid sampling frequency";
                    break;
                case 58:
                    result = "Scope - Buffer size error";
                    break;
                case 61:
                    result = "Scope - Invalid ADC input channel setting";
                    break;
                case 100:
                    result = "Gen - Invalid feature";
                    break;
                case 101:
                    result = "Gen - Invalid state";
                    Gen_form.add_message(new Message(Message.MsgRequest.GEN_ERR));
                    break;
                case 102:
                    result = "Gen - Writing data out of memory";
                    Gen_form.add_message(new Message(Message.MsgRequest.GEN_ERR));
                    break;
                case 103:
                    result = "Gen - Buffer size error";
                    Gen_form.add_message(new Message(Message.MsgRequest.GEN_ERR));
                    break;
                case 104:
                    result = "Gen - Missing data";
                    break;
                case 105:
                    result = "Gen - To high samling frequency";
                    break;
                case 107:
                    result = "Gen - Frequency is inaccurate";
                    break;
                case 108:
                    result = "Gen - Frequency mismatch";
                    break;
                case 109:
                    result = "Gen - Invalid data";
                    break;
                case 255:
                    result = "Unknown error";
                    break;
                case 533:
                    result = "Communication error";
                    break;
                case 999:
                    result = "Unsuported function";
                    break;
            }
            return result;
        }

        //end class
    }
}
