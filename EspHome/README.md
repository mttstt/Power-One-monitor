# Esphome

Tested on Wemos d1 mini


### Modify "ABB Aurora"

In order to use the lib "ABB Aurora" with ESP8266 modify setup function in this way:

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

### Esphome Custom component

esphome aurora.yaml run
