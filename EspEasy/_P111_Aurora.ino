#ifdef USES_P111

//#######################################################################################################
//###########################         Plugin 111: Aurora Inverter               #########################
//#######################################################################################################
/*
   Version: 2.0
   Description: Arduino based Power Monitor for Power-One / ABB Phototovoltaic Inverters
   Example of usage:

      REALTIME DATA
      http://<ESP IP address>/control?cmd=aurora,ask

      HISTORY DATA
      http://<ESP IP address>/control?cmd=aurora,day,(1-366)

   Needs: EspEasy
   Author: mtt
   Copyright: (c) 2019-2020 mttstt
   License: MIT
   License URI: http://en.wikipedia.org/wiki/MIT_License
   Status : "Proof of concept"

   PIN successuful tested (hardware serial):

         RS485    Wemos D1 mini
         Vcc      3v3
         Gnd      G
         RO       TX (GPIO-1)
         RI       RX (GPIO-3)
         RE/DE    D0 (GPIO-16)

   References:
   https://github.com/H4ndl3/pvimon/blob/master/pvimon.ino
   http://www.drhack.it/images/PDF/AuroraCommunicationProtocol_4_2.pdf
   http://www.gianlucaghettini.net/lettura-fotovoltaico-da-remoto-con-nodemcu/
   https://github.com/jrbenito/ABBAurora
   https://forum.arduino.cc/index.php?topic=154407.0
 */

#include <ESPeasySerial.h>
#include "src/Globals/ESPEasy_time.h"

#define PLUGIN_111
#define PLUGIN_ID_111         111
#define PLUGIN_NAME_111       "Inverter logger (Aurora)"
#define PLUGIN_VALUENAME1_111 "DailyEnergy"     //variable output of the plugin. The label is in quotation marks
#define PLUGIN_VALUENAME2_111 "MonthEnergy"
#define PLUGIN_VALUENAME3_111 "YearEnergy"
#define PLUGIN_111_DEBUG  false             //set to true for extra log info in the debug

//RS485 control
#define RS485Transmit HIGH
#define RS485Receive LOW
#define TX 1                // GPIO-1 (TX)
#define RX 3                // GPIO-3 (RX)
#define baudrate 19200      //baudrate RS485
#define SSerialTxControl D0 //GPIO-16 (D0)



String stampaDataTime(unsigned long scn)
{
// String rtn=String(day())+String(F("/"))+String(month())+String(F("/"))+String(year())+String(F(" "))+String(hour())+String(F(":"))+String(minute())+String(F(":"))+String(second());
 return "qq";
 //return rtn;

};

String TransmissionState(byte id) {
  switch (id)
  {
  case 0:
    return F("Everything is OK."); break;
  case 51:
    return F("Command is not implemented"); break;
  case 52:
    return F("Variable does not exist"); break;
  case 53:
    return F("Variable value is out of range"); break;
  case 54:
    return F("EEprom not accessible"); break;
  case 55:
    return F("Not Toggled Service Mode"); break;
  case 56:
    return F("Can not send the command to internal micro"); break;
  case 57:
    return F("Command not Executed"); break;
  case 58:
    return F("The variable is not available, retry"); break;
  default:
    return F("Sconosciuto"); break;
  }
}

/*
String DcDcState(byte id) {
  switch (id)
  {
  case 0:
    return F("DcDc OFF"); break;
  case 1:
    return F("Ramp Start"); break;
  case 2:
    return F("MPPT"); break;
  case 3:
    return F("Not Used"); break;
  case 4:
    return F("Input OC"); break;
  case 5:
    return F("Input UV"); break;
  case 6:
    return F("Input OV"); break;
  case 7:
    return F("Input Low"); break;
  case 8:
    return F("No Parameters"); break;
  case 9:
    return F("Bulk OV"); break;
  case 10:
    return F("Communication Error"); break;
  case 11:
    return F("Ramp Fail"); break;
  case 12:
    return F("Internal Error"); break;
  case 13:
    return F("Input mode Error"); break;
  case 14:
    return F("Ground Fault"); break;
  case 15:
    return F("Inverter Fail"); break;
  case 16:
    return F("DcDc IGBT Sat"); break;
  case 17:
    return F("DcDc ILEAK Fail"); break;
  case 18:
    return F("DcDc Grid Fail"); break;
  case 19:
    return F("DcDc Comm.Error"); break;
  default:
    return F("Sconosciuto"); break;
  }
}
*/

String GlobalState(byte id) {
  switch (id)
    {
    case 0:
      return F("Sending Parameters"); break;
    case 1:
      return F("Wait Sun / Grid"); break;
    case 2:
      return F("Checking Grid"); break;
    case 3:
      return F("Measuring Riso"); break;
    case 4:
      return F("DcDc Start"); break;
    case 5:
      return F("Inverter Start"); break;
    case 6:
      return F("Run"); break;
    case 7:
      return F("Recovery"); break;
    case 8:
      return F("Pausev"); break;
    case 9:
      return F("Ground Fault"); break;
    case 10:
      return F("OTH Fault"); break;
    case 11:
      return F("Address Setting"); break;
    case 12:
      return F("Self Test"); break;
    case 13:
      return F("Self Test Fail"); break;
    case 14:
      return F("Sensor Test + Meas.Riso"); break;
    case 15:
      return F("Leak Fault"); break;
    case 16:
      return F("Waiting for manual reset"); break;
    case 17:
      return F("Internal Error E026"); break;
    case 18:
      return F("Internal Error E027"); break;
    case 19:
      return F("Internal Error E028"); break;
    case 20:
      return F("Internal Error E029"); break;
    case 21:
      return F("Internal Error E030"); break;
    case 22:
      return F("Sending Wind Table"); break;
    case 23:
      return F("Failed Sending table"); break;
    case 24:
      return F("UTH Fault"); break;
    case 25:
      return F("Remote OFF"); break;
    case 26:
      return F("Interlock Fail"); break;
    case 27:
      return F("Executing Autotest"); break;
    case 30:
      return F("Waiting Sun"); break;
    case 31:
      return F("Temperature Fault"); break;
    case 32:
      return F("Fan Staucked"); break;
    case 33:
      return F("Int.Com.Fault"); break;
    case 34:
      return F("Slave Insertion"); break;
    case 35:
      return F("DC Switch Open"); break;
    case 36:
      return F("TRAS Switch Open"); break;
    case 37:
      return F("MASTER Exclusion"); break;
    case 38:
      return F("Auto Exclusion"); break;
    case 98:
      return F("Erasing Internal EEprom"); break;
    case 99:
      return F("Erasing External EEprom"); break;
    case 100:
      return F("Counting EEprom"); break;
    case 101:
      return F("Freeze"); break;
    default:
      return F("Sconosciuto"); break;
    }
  }


String InverterState(byte id) {
  switch (id)
    {
    case 0:
      return F("Stand By"); break;
    case 1:
      return F("Checking Grid"); break;
    case 2:
      return F("Run"); break;
    case 3:
      return F("Bulk OV"); break;
    case 4:
      return F("Out OC"); break;
    case 5:
      return F("IGBT Sat"); break;
    case 6:
      return F("Bulk UV"); break;
    case 7:
      return F("Degauss Error"); break;
    case 8:
      return F("No Parameters"); break;
    case 9:
      return F("Bulk Low"); break;
    case 10:
      return F("Grid OV"); break;
    case 11:
      return F("Communication Error"); break;
    case 12:
      return F("Degaussing"); break;
    case 13:
      return F("Starting"); break;
    case 14:
      return F("Bulk Cap Fail"); break;
    case 15:
      return F("Leak Fail"); break;
    case 16:
      return F("DcDc Fail"); break;
    case 17:
      return F("Ileak Sensor Fail"); break;
    case 18:
      return F("SelfTest: relay inverter"); break;
    case 19:
      return F("SelfTest : wait for sensor test"); break;
    case 20:
      return F("SelfTest : test relay DcDc + sensor"); break;
    case 21:
      return F("SelfTest : relay inverter fail"); break;
    case 22:
      return F("SelfTest timeout fail"); break;
    case 23:
      return F("SelfTest : relay DcDc fail"); break;
    case 24:
      return F("Self Test 1"); break;
    case 25:
      return F("Waiting self test start"); break;
    case 26:
      return F("Dc Injection"); break;
    case 27:
      return F("Self Test 2"); break;
    case 28:
      return F("Self Test 3"); break;
    case 29:
      return F("Self Test 4"); break;
    case 30:
      return F("Internal Error"); break;
    case 31:
      return F("Internal Error"); break;
    case 40:
      return F("Forbidden State"); break;
    case 41:
      return F("Input UC"); break;
    case 42:
      return F("Zero Power"); break;
    case 43:
      return F("Grid Not Present"); break;
    case 44:
      return F("Waiting Start"); break;
    case 45:
      return F("MPPT"); break;
    case 46:
      return F("Grid Fail"); break;
    case 47:
      return F("Input OC"); break;
    default:
      return F("Sconosciuto"); break;
    }
}

String AlarmState(byte id) {
switch (id)
  {
  case 0:
    return F("No Alarm"); break;
  case 1:
    return F("Sun Low"); break;
  case 2:
    return F("Input OC"); break;
  case 3:
    return F("Input UV"); break;
  case 4:
    return F("Input OV"); break;
  case 5:
    return F("Sun Low"); break;
  case 6:
    return F("No Parameters"); break;
  case 7:
    return F("Bulk OV"); break;
  case 8:
    return F("Comm.Error"); break;
  case 9:
    return F("Output OC"); break;
  case 10:
    return F("IGBT Sat"); break;
  case 11:
    return F("Bulk UV"); break;
  case 12:
    return F("Internal error"); break;
  case 13:
    return F("Grid Fail"); break;
  case 14:
    return F("Bulk Low"); break;
  case 15:
    return F("Ramp Fail"); break;
  case 16:
    return F("Dc / Dc Fail"); break;
  case 17:
    return F("Wrong Mode"); break;
  case 18:
    return F("Ground Fault"); break;
  case 19:
    return F("Over Temp."); break;
  case 20:
    return F("Bulk Cap Fail"); break;
  case 21:
    return F("Inverter Fail"); break;
  case 22:
    return F("Start Timeout"); break;
  case 23:
    return F("Ground Fault"); break;
  case 24:
    return F("Degauss error"); break;
  case 25:
    return F("Ileak sens.fail"); break;
  case 26:
    return F("DcDc Fail"); break;
  case 27:
    return F("Self Test Error 1"); break;
  case 28:
    return F("Self Test Error 2"); break;
  case 29:
    return F("Self Test Error 3"); break;
  case 30:
    return F("Self Test Error 4"); break;
  case 31:
    return F("DC inj error"); break;
  case 32:
    return F("Grid OV"); break;
  case 33:
    return F("Grid UV"); break;
  case 34:
    return F("Grid OF"); break;
  case 35:
    return F("Grid UF"); break;
  case 36:
    return F("Z grid Hi"); break;
  case 37:
    return F("Internal error"); break;
  case 38:
    return F("Riso Low"); break;
  case 39:
    return F("Vref Error"); break;
  case 40:
    return F("Error Meas V"); break;
  case 41:
    return F("Error Meas F"); break;
  case 42:
    return F("Error Meas Z"); break;
  case 43:
    return F("Error Meas Ileak"); break;
  case 44:
    return F("Error Read V"); break;
  case 45:
    return F("Error Read I"); break;
  case 46:
    return F("Table fail"); break;
  case 47:
    return F("Fan Fail"); break;
  case 48:
    return F("UTH"); break;
  case 49:
    return F("Interlock fail"); break;
  case 50:
    return F("Remote Off"); break;
  case 51:
    return F("Vout Avg errror"); break;
  case 52:
    return F("Battery low"); break;
  case 53:
    return F("Clk fail"); break;
  case 54:
    return F("Input UC"); break;
  case 55:
    return F("Zero Power"); break;
  case 56:
    return F("Fan Stucked"); break;
  case 57:
    return F("DC Switch Open"); break;
  case 58:
    return F("Tras Switch Open"); break;
  case 59:
    return F("AC Switch Open"); break;
  case 60:
    return F("Bulk UV"); break;
  case 61:
    return F("Autoexclusion"); break;
  case 62:
    return F("Grid df / dt"); break;
  case 63:
    return F("Den switch Open"); break;
  case 64:
    return F("Jbox fail"); break;
  default:
    F("Sconosciuto"); break;
  }
}


String Version1(byte id) {
          if (((char)id) == 'i')  { return F("Aurora 2 kW indoor"); }
          else if (((char)id) == 'o')  { return F("Aurora 2 kW outdoor"); }
          else if (((char)id) == 'I')  { return F("Aurora 3.6 kW indoor"); }
          else if (((char)id) == 'O')  { return F("Aurora 3.0 - 3.6 kW outdoor"); }
          else if (((char)id) == '5')  { return F("Aurora 5.0 kW outdoor"); }
          else if (((char)id) == '6')  { return F("Aurora 6 kW outdoor"); }
          else if (((char)id) == 'P')  { return F("3 - phase interface (3G74)"); }
          else if (((char)id) == 'C')  { return F("Aurora 50kW module"); }
          else if (((char)id) == '4')  { return F("Aurora 4.2kW new"); }
          else if (((char)id) == '3')  { return F("Aurora 3.6kW new"); }
          else if (((char)id) == '2')  { return F("Aurora 3.3kW new"); }
          else if (((char)id) == '1')  { return F("Aurora 3.0kW new"); }
          else if (((char)id) == 'D')  { return F("Aurora 12.0kW"); }
          else if (((char)id) == 'X')  { return F("Aurora 10.0kW"); }
          else { return F("Sconosciuto"); };
}

String Version2(byte id) {
          if (((char)id) == 'A')  { return F("USA - UL1741"); }
          else if (((char)id) == 'E')  { return F("Germany - VDE0126"); }
          else if (((char)id) == 'S')  { return F("Spain - DR 1663 / 2000"); }
          else if (((char)id) == 'I')  { return F("Italy - ENEL DK 5950"); }
          else if (((char)id) == 'U')  { return F("UK - UK G83"); }
          else if (((char)id) == 'K')  { return  F("Australia - AS 4777"); }
          else  { return F("Sconosciuto"); };
}

String Version3(byte id) {
          if (((char)id) == 'N')  { return F("Transformerless Version"); }
          else if (((char)id) == 'K')  { return F("Transformer Version"); }
          else if (((char)id) == 't')  { return F("Transformer HF version"); }
          else if (((char)id) == 'X')  { return F("Dummy transformer type"); }
          else { return F("Sconosciuto"); };
}

String Version4(byte id) {
          if (((char)id) == 'N')  { return F("Wind version"); }
          else if (((char)id) == 'K')  { return  F("PV version"); }
          else if (((char)id) == 'X')  { return F("Dummy inverter type"); }
          else { return F("Sconosciuto"); };
}


class clsAurora {

private:
  int MaxAttempt = 1;
  byte Address = 0;
  void clearData(byte *data, byte len) { for (int i = 0; i < len; i++) { data[i] = 0; } }


  int Crc16(byte *data, int offset, int count)
  {
    byte BccLo = 0xFF;
    byte BccHi = 0xFF;
    for (int i = offset; i < (offset + count); i++)
    {
      byte New = data[offset + i] ^ BccLo;
      byte Tmp = New << 4;
      New = Tmp ^ New;
      Tmp = New >> 5;
      BccLo = BccHi;
      BccHi = New ^ Tmp;
      Tmp = New << 3;
      BccLo = BccLo ^ Tmp;
      Tmp = New >> 4;
      BccLo = BccLo ^ Tmp;
    }
    return (int)word(~BccHi, ~BccLo);
  }


  bool Send(byte address, byte param0, byte param1, byte param2, byte param3, byte param4, byte param5, byte param6)
  {
    SendStatus = false;
    ReceiveStatus = false;
    byte SendData[10];
    SendData[0] = address;
    SendData[1] = param0;
    SendData[2] = param1;
    SendData[3] = param2;
    SendData[4] = param3;
    SendData[5] = param4;
    SendData[6] = param5;
    SendData[7] = param6;
    int crc = Crc16(SendData, 0, 8);
    SendData[8] = lowByte(crc);
    SendData[9] = highByte(crc);
    clearReceiveData();
/*  =================================================
    String log = "AURORA - Send data:";
    log += SendData[0]; log +=',';
    log += SendData[1]; log +=',';
    log += SendData[2]; log +=',';
    log += SendData[3]; log +=',';
    log += SendData[4]; log +=',';
    log += SendData[5]; log +=',';
    log += SendData[6]; log +=',';
    log += SendData[7]; log +=',';
    log += SendData[8]; log +=',';
    log += SendData[9]; log +='-';
    addLog(LOG_LEVEL_INFO,log);
  ================================================= */
    for (int i = 0; i < MaxAttempt; i++)
    {
      digitalWrite(SSerialTxControl, RS485Transmit);
      delay(50);

      if (Serial.write(SendData, sizeof(SendData)) != 0) {
            Serial.flush();
            SendStatus = true;
            digitalWrite(SSerialTxControl, RS485Receive);
            int rec = Serial.readBytes(ReceiveData, sizeof(ReceiveData));
            //=================================================
            String log = F("AURORA - Received data: ");
            log = ReceiveData[0]; log += F(",");
            log += ReceiveData[1]; log += F(",");
            log += ReceiveData[2]; log += F(",");
            log += ReceiveData[3]; log += F(",");
            log += ReceiveData[4]; log += F(",");
            log += ReceiveData[5]; log += F(",");
            log += ReceiveData[6]; log += F(",");
            log += ReceiveData[7]; log += F(":");
            log += rec;
            addLog(LOG_LEVEL_INFO,log);
            //================================================= */
            if (rec != 0) {
              if ((int)word(ReceiveData[7], ReceiveData[6]) == Crc16(ReceiveData, 0, 6)) {
                ReceiveStatus = true;

                //addLog(LOG_LEVEL_INFO,F("CRC OK"));

                break;
              }
            }
      }
      else {
            //addLog(LOG_LEVEL_INFO,F("Error while sending data"));
            digitalWrite(SSerialTxControl, RS485Receive); return(false);
           }
    }
    return ReceiveStatus;
  }


  union {
    byte asBytes[4];
    float asFloat;
  } foo;


  union {
    byte asBytes[4];
    unsigned long asUlong;
  } ulo;


public:
  bool SendStatus = false;
  bool ReceiveStatus = false;
  byte ReceiveData[8];

  clsAurora(byte address) {
    Address = address;
    SendStatus = false;
    ReceiveStatus = false;
    clearReceiveData();
  }

  void clearReceiveData() {
    clearData(ReceiveData, 8);
  }

/*
  typedef struct {
    String TransmissionState;
    String GlobalState;
    String InverterState;
    byte Channel1State;
    byte Channel2State;
    String AlarmState;
    bool ReadState;
  } DataState;

  DataState State;
*/

  String ReadState(byte id) {
    bool b = Send(Address, (byte)50, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0);
    if (b == false) {
      ReceiveData[0] = 255;
      ReceiveData[1] = 255;
      ReceiveData[2] = 255;
      ReceiveData[3] = 255;
      ReceiveData[4] = 255;
      ReceiveData[5] = 255;
    }
    switch (id)
      {
      case 0:
          return TransmissionState(ReceiveData[0]); break;
      case 1:
          return GlobalState(ReceiveData[1]); break;
      case 2:
          return InverterState(ReceiveData[2]); break;
      case 3:
          return String(ReceiveData[3]); break;
      case 4:
          return String(ReceiveData[4]); break;
      case 5:
          return String(ReceiveData[5]); break;
    }
}

/*
  typedef struct {
    byte TransmissionState;
    byte GlobalState;
    String Par1;
    String Par2;
    String Par3;
    String Par4;
    bool ReadState;
  } DataVersion;
  DataVersion Version;*/

  String ReadVersion() {
    bool b =  Send(Address, (byte)58, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0);
    if (b == false) {
        ReceiveData[0] = 255;
        ReceiveData[1] = 255;
    }
    String str = Version1(ReceiveData[2]);
    str += Version2(ReceiveData[3]);
    str += Version3(ReceiveData[4]);
    str += Version4(ReceiveData[5]);
    return str;
 }

/*
  typedef struct {
    byte TransmissionState;
    byte GlobalState;
    float Valore;
    bool ReadState;
  } DataDSP;
  DataDSP DSP;
*/

   /* # 59 Measure request to the DSP - Type of Measure:

      1 Grid Voltage* For three-phases systems is the mean
      2 Grid Current* For three-phases systems is the mean
      3 Grid Power* For three-phases systems is the mean
      4 Frequency For three-phases systems is the mean
      5 Vbulk For Inverter with more Bulk is the sum
      6 Ileak (Dc/Dc)
      7 Ileak (Inverter)
      8 Pin1* Pin for single channel module
      9 Pin2
      21 Inverter Temperature
      22 Booster Temperature
      23 Input 1 Voltage Input Voltage for single channel module
      24 - - -
      25 Input 1 Current* Input Current for single channel module
      26 Input 2 Voltage
      27 Input 2 Current
      28 Grid Voltage (Dc/Dc)
      29 Grid Frequency (Dc/Dc)
      30 Isolation Resistance (Riso)
      31 Vbulk (Dc/Dc)
      32 Average Grid Voltage (VgridAvg)
      33 VbulkMid
      34 Power Peak
      35 Power Peak Today
      36 Grid Voltage neutral
      37 Wind Generator Frequency
      38 Grid Voltage neutral-phase
      39 Grid Current phase r
      40 Grid Current phase s
      41 Grid Current phase t
      42 Frequency phase r
      43 Frequency phase s
      44 Frequency phase t
      45 Vbulk +
      46 Vbulk -
      47 Supervisor Temperature
      48 Alim. Temperature
      49 Heat Sink Temperature
      50 Temperature 1
      51 Temperature 2
      52 Temperature 3
      53 Fan 1 Speed
      54 Fan 2 Speed
      55 Fan 3 Speed
      56 Fan 4 Speed
      57 Fan 5 Speed
      58 Power Saturation limit (Der.)
      59 Riferimento Anello Bulk
      60 Vpanel micro
      61 Grid Voltage phase r
      62 Grid Voltage phase s
      63 Grid Voltage phase t

      Global=
      if 1 requires the Global Measurements (Only For a Master)
      if 0 requires the Module Measurements (Master and Slave)
  */

  float ReadDSP(byte type, byte global) {
    bool b;
    if ((((int)type >= 1 && (int)type <= 9) || ((int)type >= 21 && (int)type <= 63)) && ((int)global >= 0 && (int)global <= 1)) {
      b = Send(Address, (byte)59, type, global, (byte)0, (byte)0, (byte)0, (byte)0);
      if (b == false) {
        ReceiveData[0] = 255;
        ReceiveData[1] = 255;
      }
    }
    else {
      b = false;
      clearReceiveData();
      ReceiveData[0] = 255;
      ReceiveData[1] = 255;
    }
    //DSP.TransmissionState = ReceiveData[0];
    //DSP.GlobalState = ReceiveData[1];
    foo.asBytes[0] = ReceiveData[5];
    foo.asBytes[1] = ReceiveData[4];
    foo.asBytes[2] = ReceiveData[3];
    foo.asBytes[3] = ReceiveData[2];
    return foo.asFloat;
  }

/*
  typedef struct {
    byte TReadTimeDateransmissionState;
    byte GlobalState;
    unsigned long Secondi;
    bool ReadState;
  } DataTimeDate;
  DataTimeDate TimeDate;
*/
  unsigned long ReadTimeDate() {
    bool b = Send(Address, (byte)70, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0);
    if (b == false) {
      ReceiveData[0] = 255;
      ReceiveData[1] = 255;
    }
    //TimeDate.TransmissionState = ReceiveData[0];
    //TimeDate.GlobalState = ReceiveData[1];
    return ((unsigned long)ReceiveData[2] << 24) + ((unsigned long)ReceiveData[3] << 16) + ((unsigned long)ReceiveData[4] << 8) + (unsigned long)ReceiveData[5];
    //return TimeDate.ReadState;
  }


/*
  typedef struct {
    byte TransmissionState;
    byte GlobalState;
    byte Alarms1;
    byte Alarms2;
    byte Alarms3;
    byte Alarms4;
    bool ReadState;
  } DataLastFourAlarms;

  DataLastFourAlarms LastFourAlarms;
*/

  String ReadLastFourAlarms() {
    bool b = Send(Address, (byte)86, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0);
    if (b == false) {
      ReceiveData[0] = 255;
      ReceiveData[1] = 255;
      ReceiveData[2] = 255;
      ReceiveData[3] = 255;
      ReceiveData[4] = 255;
      ReceiveData[5] = 255;
    }
    String str = AlarmState((byte)ReceiveData[2]); str += F(", ");
    str += AlarmState((byte)ReceiveData[3]); str += F(", ");
    str += AlarmState((byte)ReceiveData[4]); str += F(", ");
    str += AlarmState((byte)ReceiveData[5]);

    return str;
 }



bool ReadJunctionBoxState(byte nj) {
    return Send(Address, (byte)200, nj, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0);
  }

bool ReadJunctionBoxVal(byte nj, byte par) {
    return Send(Address, (byte)201, nj, par, (byte)0, (byte)0, (byte)0, (byte)0);
  }

  /* Inverters
  typedef struct {
    String PN;
    bool ReadState;
  } DataSystemPN;
  DataSystemPN SystemPN;
*/
  String ReadSystemPN() {
    Send(Address, (byte)52, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0);
    return String(String((char)ReceiveData[0]) + String((char)ReceiveData[1]) + String((char)ReceiveData[2]) + String((char)ReceiveData[3]) + String((char)ReceiveData[4]) + String((char)ReceiveData[5]));
  }

/*
  typedef struct {
    String SerialNumber;
    bool ReadState;
  } DataSystemSerialNumber;
  DataSystemSerialNumber SystemSerialNumber;
*/

  String ReadSystemSerialNumber() {
    Send(Address, (byte)63, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0);
    return String(String((char)ReceiveData[0]) + String((char)ReceiveData[1]) + String((char)ReceiveData[2]) + String((char)ReceiveData[3]) + String((char)ReceiveData[4]) + String((char)ReceiveData[5]));
  }

/*
  typedef struct {
    byte TransmissionState;
    byte GlobalState;
    String Week;
    String Year;
    bool ReadState;
  } DataManufacturingWeekYear;
  DataManufacturingWeekYear ManufacturingWeekYear;
*/

  String ReadManufacturingWeekYear() {
      bool b = Send(Address, (byte)65, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0);
      if (b == false) {
        ReceiveData[0] = 255;
        ReceiveData[1] = 255;
      }
      return String( String((char)ReceiveData[2]) + String((char)ReceiveData[3]) + F(",") + String((char)ReceiveData[4]) + String((char)ReceiveData[5]) );
  }

/*
  typedef struct {
    byte TransmissionState;
    byte GlobalState;
    String Release;
    bool ReadState;
  } DataFirmwareRelease;
  DataFirmwareRelease FirmwareRelease;
*/
  String ReadFirmwareRelease() {
    bool b = Send(Address, (byte)72, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0);
    if (b == false) {
      ReceiveData[0] = 255;
      ReceiveData[1] = 255;
    }
    //FirmwareRelease.TransmissionState = ReceiveData[0];
    //FirmwareRelease.GlobalState = ReceiveData[1];
    return  String(String((char)ReceiveData[2]) + "." + String((char)ReceiveData[3]) + "." + String((char)ReceiveData[4]) + "." + String((char)ReceiveData[5]));
  }


/*
  typedef struct {
    byte TransmissionState;
    byte GlobalState;
    unsigned long Energia;
    bool ReadState;
  } DataCumulatedEnergy;
  DataCumulatedEnergy CumulatedEnergy;
*/
  /*  par=
      0) Daily Energy
      1) Weekly Energy
      2) Not used
      3) Month Energy (Energy from the first day of current calendar month)
      4) Year Energy (Energy from the first day of current calendar year)
      5) Total Energy (total lifetime)
      6) Partial Energy (cumulated since reset)
      1      Daily Energy
      2      Weekly Energy
      3      Monthly Energy
      4      Yearly Energy
      5       nDays Energy
      6       Total Energy (total lifetime)
      7       Partial Energy (cumulated since reset)
  */
   unsigned long ReadCumulatedEnergy(byte par) {
    bool b;
    if ((int)par >= 0 && (int)par <= 6) {
      b = Send(Address, (byte)78, par, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0);
      if (b == false) {
        ReceiveData[0] = 255;
        ReceiveData[1] = 255;
      }
    }
    else {
      b = false;
      clearReceiveData();
      ReceiveData[0] = 255;
      ReceiveData[1] = 255;
    }
    //CumulatedEnergy.TransmissionState = ReceiveData[0];
    //CumulatedEnergy.GlobalState = ReceiveData[1];
    if (b == true) {
        ulo.asBytes[0] = ReceiveData[5];
        ulo.asBytes[1] = ReceiveData[4];
        ulo.asBytes[2] = ReceiveData[3];
        ulo.asBytes[3] = ReceiveData[2];
        addLog(LOG_LEVEL_INFO, F("AURORA: ReadCumulatedEnergy ok! "));
        return ulo.asUlong;
    }
    if (b == false) {
       addLog(LOG_LEVEL_INFO, F("AURORA: ReadCumulatedEnergy error! "));
       return 0; }
}


 /* # 79 Daily Cumulated Energy ** Experimental ** */

 /*
  typedef struct {
    byte TransmissionState;
    byte GlobalState;
    String kwh;
    String day;
    bool ReadState;
  } DailyDataCumulatedEnergy;
  DailyDataCumulatedEnergy DailyCumulatedEnergy;
*/

  String ReadDailyCumulatedEnergy(byte id, byte par) {
    bool b;
    if ((int)par >= 1 && (int)par <= 366) {
      b = Send(Address, (byte)79, par, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0);
      if (b == false) {
        ReceiveData[0] = 255;
        ReceiveData[1] = 255;
      }
    }
    else {
      b = false;
      clearReceiveData();
      ReceiveData[0] = 255;
      ReceiveData[1] = 255;
    }
    switch (id)
      {
      case 0:
          return String(ReceiveData[0]); break;
      case 1:
          return String(ReceiveData[1]); break;
      case 2:
          return String( (int)word(~ReceiveData[2], ~ReceiveData[3]) ); break;
      case 3:
          return String( (int)word(~ReceiveData[4], ~ReceiveData[5]) ); break;
      }
  }

/*
  bool WriteBaudRateSetting(byte baudcode) {
    if ((int)baudcode >= 0 && (int)baudcode <= 3) {
      return Send(Address, (byte)85, baudcode, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0);
    }
    else {
      clearReceiveData();
      return false;
    }
  }

  // Central
  bool ReadFlagsSwitchCentral()  { return Send(Address, (byte)67, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0); }
  bool ReadCumulatedEnergyCentral(byte var, byte ndays_h, byte ndays_l, byte global) { return Send(Address, (byte)68, var, ndays_h, ndays_l, global, (byte)0, (byte)0); }
  bool ReadFirmwareReleaseCentral(byte var) { return Send(Address, (byte)72, var, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0); }
  bool ReadBaudRateSettingCentral(byte baudcode, byte serialline) { return Send(Address, (byte)85, baudcode, serialline, (byte)0, (byte)0, (byte)0, (byte)0); }
  bool ReadSystemInfoCentral(byte var) { return Send(Address, (byte)101, var, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0); }
  bool ReadJunctionBoxMonitoringCentral(byte cf, byte rn, byte njt, byte jal, byte jah) {return Send(Address, (byte)103, cf, rn, njt, jal, jah, (byte)0); }
  bool ReadSystemPNCentral() { return Send(Address, (byte)105, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0); }
  bool ReadSystemSerialNumberCentral() { return Send(Address, (byte)107, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0, (byte)0); }
*/
};


//=====================================
clsAurora*  Inverter = NULL;
//=====================================

boolean Plugin_111(byte function, struct EventStruct *event, String& string)
{
        boolean success = false;
        switch (function)
        {
        case PLUGIN_DEVICE_ADD:
        {
          Device[++deviceCount].Number = PLUGIN_ID_111;
        //  Device[deviceCount].Type = DEVICE_TYPE_SINGLE; //how the device is connected
          Device[deviceCount].SendDataOption = true;
          Device[deviceCount].Ports = 0;
        //  Device[deviceCount].VType = SENSOR_TYPE_SWITCH; //type of value the plugin will return, used only for Domoticz
          Device[deviceCount].PullUpOption = false;
          Device[deviceCount].InverseLogicOption = false;
          Device[deviceCount].FormulaOption = false;
          Device[deviceCount].ValueCount = 1;  //number of output variables. The value should match the number of keys PLUGIN_VALUENAME1_xxx
          Device[deviceCount].TimerOption = true;
          Device[deviceCount].TimerOptional = false;
          Device[deviceCount].GlobalSyncOption = true;
          Device[deviceCount].DecimalsOnly = false;
          break;
        }

        case PLUGIN_GET_DEVICENAME:
        {
           string = F(PLUGIN_NAME_111);
           break;
        }

        case PLUGIN_GET_DEVICEVALUENAMES:
        {
           //called when the user opens the module configuration page
           //it allows to add a new row for each output variable of the plugin
           strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_111));
           //strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[1], PSTR(PLUGIN_VALUENAME2_111));
           //strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[2], PSTR(PLUGIN_VALUENAME3_111));
           break;
        }

        case PLUGIN_GET_DEVICEGPIONAMES:
        {
          //  serialHelper_getGpioNames(event);
            break;
        }

        case PLUGIN_WEBFORM_LOAD:
        {
           addFormNumericBox(F("PVI Address"), F("plugin_111_pviaddr"), PCONFIG(0),1,255);
          // serialHelper_webformLoad(event);
           //after the form has been loaded, set success and break
           success = true;
           break;
        }

        case PLUGIN_WEBFORM_SAVE:
        {
           //this case defines the code to be executed when the form is submitted
           //the plugin settings should be saved to PCONFIG(x)
           //ping configuration should be read from CONFIG_PIN1 and stored

           //serialHelper_webformSave(event);

           PCONFIG(0) = getFormItemInt(F("plugin_111_pviaddr"));

           //after the form has been saved successfuly, set success and break
           success = true;
           break;
        }

        case PLUGIN_WEBFORM_SHOW_CONFIG:
        {
           //string += String(ExtraTaskSettings.TaskDevicePluginConfigLong[0]);
           success = true;
           break;
        }


        case PLUGIN_INIT:
        {
           // this case defines code to be executed when the plugin is initialised
           if (Inverter) { delete Inverter; Inverter = nullptr; }; Inverter = new clsAurora( PCONFIG(0) );
           pinMode(SSerialTxControl, OUTPUT);
           Serial.setTimeout(500);
           Serial.begin(baudrate);
           Serial.flush();
           //============================================================================================
           if ( Inverter != nullptr ) {
              addLog(LOG_LEVEL_INFO, F("INIT: Aurora Inverter created!"));
              addLog(LOG_LEVEL_INFO, F("INIT: PVI Address "));
            //  addLog(LOG_LEVEL_INFO, PCONFIG(0));
           }
           else {
             addLog(LOG_LEVEL_INFO, F("INIT: Aurora Inverter ERROR!"));
           }
           success = true;
           break;
        }

        case PLUGIN_READ:
        {
           //code to be executed to read data
           //It is executed according to the delay configured on the device configuration page, only once
           //after the plugin has read data successfuly, set success and break
           UserVar[event->BaseVarIndex + 0] = Inverter->ReadCumulatedEnergy(0);
           //UserVar[event->BaseVarIndex + 1] = Inverter->ReadCumulatedEnergy(3);
           //UserVar[event->BaseVarIndex + 2] = Inverter->ReadCumulatedEnergy(4);

           success = true;
           break;
        }

        case PLUGIN_EXIT:
        {
        }

        case PLUGIN_WRITE:
        {
           //this case defines code to be executed when the plugin executes an action (command).
           //Commands can be accessed via rules or via http.
           //As an example, http://192.168.1.12//control?cmd=(ask/day)
           //implies that there exists the command "dothis"
           //parse string to extract the command

           String TmpStr;
           String rfType;

           if ( GetArgv(string.c_str(), TmpStr, 1) )  { rfType = TmpStr.c_str(); }

           if ( rfType.equalsIgnoreCase("aurora")  ) {

                if ( GetArgv(string.c_str(), TmpStr, 2) )  { rfType = TmpStr.c_str(); }

                if ( rfType.equalsIgnoreCase("ask") ) { read_RS485(PCONFIG(0)); success = true;}

                if ( rfType.equalsIgnoreCase("day") ) {
                   if ( GetArgv(string.c_str(), TmpStr, 3) ) { read_day( TmpStr.toInt(), PCONFIG(0) ); success = true; }
                }
           }
        break;
        }
     }
     return success;
}
//==========================================================================

void read_day( int day, int pconfig ){
  printWebString += F("Aurora Inverter - PVI Address: "); printWebString += String(pconfig); printWebString += F("<BR>");
  printWebString += F("URL: <a href=\"http://"); printWebString += String(WiFi.localIP().toString());printWebString += F("/control?cmd=aurora,ask");
  printWebString += F("\">http://");printWebString += String(WiFi.localIP().toString());printWebString += F("/control?cmd=aurora,ask"); printWebString += F("</a><BR>");

  printWebString += F("Data time: "); printWebString += stampaDataTime(Inverter->ReadTimeDate()); printWebString += F("<BR><BR>");

  printWebString += F("Day: "); printWebString += Inverter->ReadDailyCumulatedEnergy(2, day); printWebString +=F("<BR>");
  printWebString += F("Kwh: "); printWebString += Inverter->ReadDailyCumulatedEnergy(3, day); printWebString += F("<BR>");
}


void read_RS485(int pconfig){
  printWebString += F("Aurora Inverter - PVI Address: "); printWebString += String(pconfig); printWebString += F("<BR>");
  printWebString += F("<BR>Use URL: <a href=\"http://"); printWebString += String(WiFi.localIP().toString());printWebString += F("/control?cmd=aurora,ask");
  printWebString += F("\">http://");printWebString += String(WiFi.localIP().toString());printWebString += F("/control?cmd=aurora,ask"); printWebString += F("</a><BR>");

  printWebString += F("Data time: ");printWebString += stampaDataTime(Inverter->ReadTimeDate()); printWebString += F("<BR><BR>");

  printWebString += F("Daily Energy: "); printWebString += Inverter->ReadCumulatedEnergy(0); printWebString += F("<BR>");
  printWebString += F("Weekly Energy: "); printWebString += Inverter->ReadCumulatedEnergy(1); printWebString += F("<BR>");
  printWebString += F("Month Energy: "); printWebString += Inverter->ReadCumulatedEnergy(3); printWebString += F("<BR>");
  printWebString += F("Year Energy: "); printWebString += Inverter->ReadCumulatedEnergy(4); printWebString += F("<BR>");

  printWebString += F("LastFourAlarms: ");
  printWebString += Inverter->ReadLastFourAlarms(); printWebString += F("<BR>");

  printWebString += F("SystemPN: ");
  printWebString += Inverter->ReadSystemPN(); printWebString += F("<BR>");

  printWebString += F("SystemSerialNumber: ");
  printWebString += Inverter->ReadSystemSerialNumber(); printWebString += F("<BR>");

  printWebString += F("ReadFirmwareRelease: ");
  printWebString += Inverter->ReadFirmwareRelease(); printWebString += F("<BR>");

  printWebString += F("ManufacturingWeekYear: ");
  printWebString += Inverter->ReadManufacturingWeekYear(); printWebString += F("<BR>");

  printWebString += F("Version: ");
  printWebString += Inverter->ReadVersion(); printWebString += F("<BR>");

  printWebString += F("TransmissionState: "); printWebString += Inverter->ReadState(0); printWebString += F("<BR>");
  printWebString += F("GlobalState: "); printWebString += Inverter->ReadState(1); printWebString += F("<BR>");
  printWebString += F("InverterState: "); printWebString += Inverter->ReadState(2); printWebString += F("<BR>");
  printWebString += F("Channel1State: "); printWebString += Inverter->ReadState(3); printWebString += F("<BR>");
  printWebString += F("Channel2State: "); printWebString += Inverter->ReadState(4); printWebString += F("<BR>");
  printWebString += F("AlarmState: "); printWebString += Inverter->ReadState(5); printWebString += F("<BR>");

  printWebString += F("Inverter Temperature (C): "); printWebString += Inverter->ReadDSP(21,0); printWebString += F("<BR>");
  printWebString += F("Booster Temperature (C): "); printWebString += Inverter->ReadDSP(22,0); printWebString += F("<BR>");
  printWebString += F("Input 1 Voltage (Volt): "); printWebString += Inverter->ReadDSP(23,1); printWebString += F("<BR>");
  printWebString += F("Input 1 Current (Ampere): "); printWebString += Inverter->ReadDSP(25,1); printWebString += F("<BR>");
  printWebString += F("Input 2 Voltage (Volt): "); printWebString += Inverter->ReadDSP(26,1); printWebString += F("<BR>");
  printWebString += F("Input 2 Current (Ampere): "); printWebString += Inverter->ReadDSP(27,1); printWebString += F("<BR>");
  printWebString += F("Riso : "); printWebString += Inverter->ReadDSP(30,0); printWebString += F("<BR>");
  printWebString += F("Power Peak (Watt): ");printWebString += Inverter->ReadDSP(34,0); printWebString += F("<BR>");
  printWebString += F("Power Peak Today (Watt): "); printWebString += Inverter->ReadDSP(35,0); printWebString += F("<BR>");
//  addLog(LOG_LEVEL_INFO,log);
}

#endif
