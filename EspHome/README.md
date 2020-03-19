# Inverter Power One Aurora
Image from Home Assistant
<p align="center">
   <a><img src='https://raw.githubusercontent.com/mttstt/Power-One-monitor/master/Asset/HomeAssistant.png' height='400'></a>
</p>


### Hardware
Wemos d1 mini + TTL to RS485

<p align="center">
   <a><img src='https://raw.githubusercontent.com/mttstt/Power-One-monitor/master/Asset/aurora.jpg' height='400'></a>
</p>


## Software
Esphome Custom component

### Modify "ABB Aurora"
In order to use the lib "ABB Aurora" with ESP8266 modify the setup function of the following file .platformio\lib\ABB Aurora_ID340\src\ABBAurora.cpp) in this way:

      void ABBAurora::setup(HardwareSerial &hardwareSerial, byte RXGpioPin, byte TXGpioPin, byte TXControllPin)
      {
        TXPinControl = TXControllPin;
        pinMode(TXPinControl, OUTPUT);
        digitalWrite(TXPinControl, LOW);
        serial = &hardwareSerial;
        #if defined(ESP32)
          serial->begin(19200, SERIAL_8N1, RXGpioPin, TXGpioPin, false, 500);
        #elif defined(ESP8266)
          serial->begin(19200,SERIAL_8N1);
        #endif
      }


### Esphome Command

      esphome aurora.yaml compile

      esphome aurora.yaml run
