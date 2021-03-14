#include "esphome.h"
#include "ABBAurora.h"
#include "ABBAuroraStrings.h"

#define WAIT 50
#define RX1 3			// GPIO-3
#define TX1 1			// GPIO-1
#define INVERTER_ADDRESS 2
#define TX_CONTROL_GPIO 16 // useless with a TTL to RS485 with outoflow (no DE/RE command)


class AuroraSensor : public PollingComponent {
public:
ABBAurora *inverter;
ABBAuroraStrings *str;

TextSensor *TransmissionState_sensor = new TextSensor();
TextSensor *GlobalState_sensor = new TextSensor();
TextSensor *InverterState_sensor = new TextSensor();
TextSensor *AlarmState_sensor = new TextSensor();
TextSensor *Firmware_sensor = new TextSensor();
TextSensor *SystemSerialNumber_sensor = new TextSensor();
TextSensor *LastFourAlarms_sensor = new TextSensor();
TextSensor *ManufacturingWeekYear_sensor = new TextSensor();
TextSensor *Version_sensor = new TextSensor();
Sensor *PowerIn1_sensor = new Sensor();
Sensor *PowerIn2_sensor = new Sensor();
Sensor *GridPower_sensor = new Sensor();
Sensor *TemperatureInverter_sensor = new Sensor();
Sensor *TemperatureBooster_sensor = new Sensor();
Sensor *DayCumulatedEnergy_sensor = new Sensor();
Sensor *MonthCumulatedEnergy_sensor = new Sensor();
Sensor *YearCumulatedEnergy_sensor = new Sensor();
Sensor *VIn1_sensor = new Sensor();
Sensor *IIn1_sensor = new Sensor();
Sensor *VIn2_sensor = new Sensor();
Sensor *IIn2_sensor = new Sensor();
Sensor *IsolationResistance_sensor = new Sensor();
Sensor *PowerPeak_sensor = new Sensor();
Sensor *PowerPeakToday_sensor = new Sensor();

AuroraSensor() : PollingComponent(60000) { }

void setup() override
  {
   ABBAurora::setup(Serial, RX1, TX1, TX_CONTROL_GPIO);
   inverter = new ABBAurora(INVERTER_ADDRESS);
   str = new ABBAuroraStrings();
   ESP_LOGD("Aurora", "Setup ok");
  }

void update() override
  {
    ESP_LOGD("Aurora", "Init update data");
    char buf[80];
    char bof[80];
    delay(WAIT);

    if ( inverter->ReadState() )
      {

        buf[0] = '\0';str->GlobalState(inverter->State.GlobalState).toCharArray(buf, sizeof(buf));
        GlobalState_sensor->publish_state(buf);

        buf[0] = '\0';str->InverterState(inverter->State.InverterState).toCharArray(buf, sizeof(buf));
        InverterState_sensor->publish_state(buf);

        buf[0] = '\0';str->AlarmState(inverter->State.AlarmState).toCharArray(buf, sizeof(buf));
        AlarmState_sensor->publish_state(buf);
      }
    //delay(WAIT);
    if ( inverter->ReadFirmwareRelease() )
      {
        buf[0] = '\0';(inverter->FirmwareRelease.Release).toCharArray(buf, sizeof(buf));
        Firmware_sensor->publish_state(buf);
      }
    //delay(WAIT);
    if ( inverter->ReadSystemSerialNumber() )
        {
          buf[0] = '\0';(inverter->SystemSerialNumber.SerialNumber).toCharArray(buf, sizeof(buf));
          SystemSerialNumber_sensor->publish_state(buf);
        }
    //delay(WAIT);
    if ( inverter->ReadManufacturingWeekYear() )
        {
          bof[0] = '\0';(inverter->ManufacturingWeekYear.Week).toCharArray(bof, sizeof(bof));
          buf[0] = '\0';(inverter->ManufacturingWeekYear.Year).toCharArray(buf, sizeof(buf));
          strcat(buf, ", "); strcat(buf, bof);
          ManufacturingWeekYear_sensor->publish_state(buf);
        }
    //delay(WAIT);
    if ( inverter->ReadLastFourAlarms() )
        {
          buf[0] = '\0';
          bof[0] = '\0';str->AlarmState(inverter->LastFourAlarms.Alarms1).toCharArray(bof, sizeof(bof));
          strcat(buf, bof);  strcat(buf, ", ");
          bof[0] = '\0';str->AlarmState(inverter->LastFourAlarms.Alarms2).toCharArray(bof, sizeof(bof));
          strcat(buf, bof); strcat(buf, ", ");
          bof[0] = '\0';str->AlarmState(inverter->LastFourAlarms.Alarms3).toCharArray(bof, sizeof(bof));
          strcat(buf, bof); strcat(buf, ", ");
          bof[0] = '\0';str->AlarmState(inverter->LastFourAlarms.Alarms4).toCharArray(bof, sizeof(bof));
          strcat(buf, bof);
          LastFourAlarms_sensor->publish_state(buf);
        }
    //delay(WAIT);
    if ( inverter->ReadVersion() )
        {
          buf[0] = '\0';
          bof[0] = '\0';(inverter->Version.Par1).toCharArray(bof, sizeof(bof));
          strcat(buf, bof);  strcat(buf, ", ");
          bof[0] = '\0';(inverter->Version.Par2).toCharArray(bof, sizeof(bof));
          strcat(buf, bof); strcat(buf, ", ");
          bof[0] = '\0';(inverter->Version.Par3).toCharArray(bof, sizeof(bof));
          strcat(buf, bof); strcat(buf, ", ");
          bof[0] = '\0';(inverter->Version.Par4).toCharArray(bof, sizeof(bof));
          strcat(buf, bof);
          Version_sensor->publish_state(buf);
        }

 //----------------------------------------------------------------------------


    //ESP_LOGD("Aurora", "Init AuroraSensor update data");
    //delay(WAIT);
    if (inverter->ReadDSPValue(POWER_IN_1, MODULE_MESSUREMENT))
       {
        PowerIn1_sensor->publish_state(inverter->DSP.Value);
       }
    //delay(WAIT);
    if (inverter->ReadDSPValue(POWER_IN_2, MODULE_MESSUREMENT))
       {
        PowerIn2_sensor->publish_state(inverter->DSP.Value);
       }
    //delay(WAIT);
    if (inverter->ReadDSPValue(GRID_POWER, MODULE_MESSUREMENT))
       {
       	GridPower_sensor->publish_state(inverter->DSP.Value);
       }
    //delay(WAIT);
    if (inverter->ReadDSPValue(TEMPERATURE_INVERTER, MODULE_MESSUREMENT))
       {
        TemperatureInverter_sensor->publish_state(inverter->DSP.Value);
       }
    //delay(WAIT);
    if (inverter->ReadDSPValue(TEMPERATURE_BOOSTER, MODULE_MESSUREMENT))
       {
       	TemperatureBooster_sensor->publish_state(inverter->DSP.Value);
       }
    //delay(WAIT);
    if (inverter->ReadCumulatedEnergy(CURRENT_DAY))
       {
      	DayCumulatedEnergy_sensor->publish_state(inverter->CumulatedEnergy.Energy);
       }
    //delay(WAIT);
    if (inverter->ReadCumulatedEnergy(CURRENT_MONTH))
       {
        MonthCumulatedEnergy_sensor->publish_state(inverter->CumulatedEnergy.Energy);
       }
    //delay(WAIT);
    if (inverter->ReadCumulatedEnergy(CURRENT_YEAR))
       {
        YearCumulatedEnergy_sensor->publish_state(inverter->CumulatedEnergy.Energy);
       }
    //delay(WAIT);
    if (inverter->ReadDSPValue(V_IN_1, GLOBAL_MESSUREMENT))
       {
        VIn1_sensor->publish_state(inverter->DSP.Value);
       }
    //delay(WAIT);
    if (inverter->ReadDSPValue(I_IN_1, GLOBAL_MESSUREMENT))
       {
        IIn1_sensor->publish_state(inverter->DSP.Value);
        }
    //delay(WAIT);
    if (inverter->ReadDSPValue(V_IN_2, GLOBAL_MESSUREMENT))
       {
        VIn2_sensor->publish_state(inverter->DSP.Value);
       }
    //delay(100);
    if (inverter->ReadDSPValue(I_IN_2, GLOBAL_MESSUREMENT))
       {
        IIn2_sensor->publish_state(inverter->DSP.Value);
       }
    //delay(WAIT);
    if (inverter->ReadDSPValue(ISOLATION_RESISTANCE, MODULE_MESSUREMENT))
       {
        IsolationResistance_sensor->publish_state(inverter->DSP.Value);
       }
    //delay(WAIT);
    if (inverter->ReadDSPValue(POWER_PEAK, MODULE_MESSUREMENT))
       {
        PowerPeak_sensor->publish_state(inverter->DSP.Value);
       }
    //delay(WAIT);
    if (inverter->ReadDSPValue(POWER_PEAK_TODAY, MODULE_MESSUREMENT))
       {
        PowerPeakToday_sensor->publish_state(inverter->DSP.Value);
       }
    }
};
