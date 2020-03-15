#include "esphome.h"
#include "ABBAurora.h"
#include "ABBAuroraStrings.h"

#define RX1 3			// GPIO-3
#define TX1 1			// GPIO-1
#define INVERTER_ADDRESS 2
#define TX_CONTROL_GPIO 16


class AuroraTextSensor : public PollingComponent, public TextSensor {
 public:
  ABBAurora *inverter;
  ABBAuroraStrings *str;


  TextSensor *TransmissionState_sensor = new TextSensor();
  TextSensor *GlobalState_sensor = new TextSensor();
  TextSensor *InverterState_sensor = new TextSensor();
  TextSensor *AlarmState_sensor = new TextSensor();
  TextSensor *Firmware_sensor = new TextSensor();
  TextSensor *SystemSerialNumber_sensor = new TextSensor();
  TextSensor *ReadLastFourAlarms_sensor = new TextSensor();
  TextSensor *ReadManufacturingWeekYear_sensor = new TextSensor();

  AuroraTextSensor() : PollingComponent(120000) { }

  void setup() override
  {
    ABBAurora::setup(Serial, RX1, TX1, TX_CONTROL_GPIO);
    inverter = new ABBAurora(INVERTER_ADDRESS);
    str = new ABBAuroraStrings();
  }
  void update() override
  {
    ESP_LOGD("Aurora", "Init AuroraTextSensor update data");
    char buf[80];
    char bof[80];
    if ( inverter->ReadState() )
      {
        delay(100);
        buf[0] = '\0';str->TransmissionState(inverter->State.TransmissionState).toCharArray(buf, sizeof(buf));
        //ESP_LOGD("Aurora", "TransmissionState sensor is: %s", buf);
        TransmissionState_sensor->publish_state(buf);

        delay(100);
        buf[0] = '\0';str->GlobalState(inverter->State.GlobalState).toCharArray(buf, sizeof(buf));
        //ESP_LOGD("Aurora", "GlobalState sensor is: %s", buf);
        GlobalState_sensor->publish_state(buf);

        delay(100);
        buf[0] = '\0';str->InverterState(inverter->State.InverterState).toCharArray(buf, sizeof(buf));
        //ESP_LOGD("Aurora", "InverterState sensor is: %s", buf);
        InverterState_sensor->publish_state(buf);

        delay(100);
        buf[0] = '\0';str->AlarmState(inverter->State.AlarmState).toCharArray(buf, sizeof(buf));
        //ESP_LOGD("Aurora", "AlarmState sensor is: %s", buf);
        AlarmState_sensor->publish_state(buf);
      }
    delay(100);
    if ( inverter->ReadFirmwareRelease() )
      {
        buf[0] = '\0';(inverter->FirmwareRelease.Release).toCharArray(buf, sizeof(buf));
        //ESP_LOGD("Aurora", "Aurora Firmware is: %s", buf);
        Firmware_sensor->publish_state(buf);
      }
    delay(100);
    if ( inverter->ReadSystemSerialNumber() )
        {
          buf[0] = '\0';(inverter->SystemSerialNumber.SerialNumber).toCharArray(buf, sizeof(buf));
          SystemSerialNumber_sensor->publish_state(buf);
        }
    delay(100);
    if ( inverter->ReadManufacturingWeekYear() )
        {
          bof[0] = '\0';(inverter->ManufacturingWeekYear.Week).toCharArray(bof, sizeof(bof));
          buf[0] = '\0';(inverter->ManufacturingWeekYear.Year).toCharArray(buf, sizeof(buf));
          strcat(buf, ", "); strcat(buf, bof);
          ReadManufacturingWeekYear_sensor->publish_state(buf);
        }
    delay(100);
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
          ReadLastFourAlarms_sensor->publish_state(buf);
        }
  }
};




  class AuroraSensor : public PollingComponent {
  public:
  ABBAurora *inverter;

  Sensor *PowerIn1_sensor = new Sensor();
  Sensor *PowerIn2_sensor = new Sensor();
  Sensor *GridPower_sensor = new Sensor();
  Sensor *TemperatureInverter_sensor = new Sensor();
  Sensor *TemperatureBooster_sensor = new Sensor();
  Sensor *DayCumulatedEnergy_sensor = new Sensor();
  Sensor *MonthCumulatedEnergy_sensor = new Sensor();
  Sensor *YearCumulatedEnergy_sensor = new Sensor();

  AuroraSensor() : PollingComponent(60000) { }

  void setup() override
  {
    inverter = new ABBAurora(INVERTER_ADDRESS);
  }

  void update() override
  {
    ESP_LOGD("Aurora", "Init AuroraSensor update data");
    delay(100);
    if (inverter->ReadDSPValue(POWER_IN_1, MODULE_MESSUREMENT))
       {
      //	ESP_LOGD("Aurora", "POWER_IN_1 sensor is: %f", inverter->DSP.Value);
  	    PowerIn1_sensor->publish_state(inverter->DSP.Value);
       }
    delay(100);
    if (inverter->ReadDSPValue(POWER_IN_2, MODULE_MESSUREMENT))
       {
      //   ESP_LOGD("Aurora", "POWER_IN_2 sensor is: %f", inverter->DSP.Value);
    	   PowerIn2_sensor->publish_state(inverter->DSP.Value);
       }
    delay(100);
    if (inverter->ReadDSPValue(GRID_POWER, MODULE_MESSUREMENT))
       {
       //	 ESP_LOGD("Aurora", "GRID_POWER sensor is: %f", inverter->DSP.Value);
      	 GridPower_sensor->publish_state(inverter->DSP.Value);
       }
    delay(100);
    if (inverter->ReadDSPValue(TEMPERATURE_INVERTER, MODULE_MESSUREMENT))
       {
        // ESP_LOGD("Aurora", "TEMPERATURE_INVERTER sensor is: %f", inverter->DSP.Value);
    	   TemperatureInverter_sensor->publish_state(inverter->DSP.Value);
       }
    delay(100);
    if (inverter->ReadDSPValue(TEMPERATURE_BOOSTER, MODULE_MESSUREMENT))
       {
       	// ESP_LOGD("Aurora", "TEMPERATURE_BOOSTER sensor is: %f", inverter->DSP.Value);
      	 TemperatureBooster_sensor->publish_state(inverter->DSP.Value);
       }
    delay(100);
    if (inverter->ReadCumulatedEnergy(CURRENT_DAY))
       {
      	 // ESP_LOGD("Aurora", "CURRENT DAY Energy sensor is: %lu", inverter->CumulatedEnergy.Energy);
    	   DayCumulatedEnergy_sensor->publish_state(inverter->CumulatedEnergy.Energy);
       }
    delay(100);
    if (inverter->ReadCumulatedEnergy(CURRENT_MONTH))
       {
         // ESP_LOGD("Aurora", "CURRENT MONTH Energy sensor is: %lu", inverter->CumulatedEnergy.Energy);
  	     MonthCumulatedEnergy_sensor->publish_state(inverter->CumulatedEnergy.Energy);
       }
    delay(100);
    if (inverter->ReadCumulatedEnergy(CURRENT_YEAR))
       {
         // ESP_LOGD("Aurora", "CURRENT YEAR Energy sensor is: %lu", inverter->CumulatedEnergy.Energy);
  	     YearCumulatedEnergy_sensor->publish_state(inverter->CumulatedEnergy.Energy);
       }
    }
    };
