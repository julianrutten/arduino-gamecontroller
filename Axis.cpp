#include <Joystick.h>
#include <Arduino.h>
#include "Axis.h"

Axis::Axis(uint8_t _pin,uint8_t _number) {
    init(_pin,_number,false);
}

Axis::Axis(uint8_t _pin,uint8_t _number, bool _invert) {
    init(_pin,_number,_invert);
}

void Axis::init(uint8_t _pin,uint8_t _number, bool _invert){
  pinMode(_pin,INPUT);
  pin = _pin;
  number = _number;
  invert = _invert;
}

String Axis::message(AxisResponse *response){
  return "";
  String message = "Axis";
  message = message + " " + response->value + " pin " + pin;
  return message;
}

AxisResponse Axis::refresh(Joystick_ *joystick){
    uint16_t _value = analogRead(pin);
//    Serial.println(value);
    if(invert){
      _value = 1023 - _value;
    }
    if(_value != value){
      value = _value;
      switch(number){
        case 0:
            joystick->setXAxis(value);
            break;
        case 1:
            joystick->setYAxis(value);
            break;
        case 2:
            joystick->setZAxis(value);
            break;
        case 3:
            joystick->setRxAxis(value);
            break;
        case 4:
            joystick->setRyAxis(value);
            break;
        case 5:
            joystick->setRzAxis(value);
            break;
      }
      return (AxisResponse) {true,value,number};
    }
    return (AxisResponse) {false,value,number};
};

void AxisManager::tick(String* messages){
    uint8_t messageCount = 0;
    for( unsigned int i = 0; i < axesCount; i = i + 1 ){
        AxisResponse response = axes[i].refresh(joystick); 
        if(response.changed){
          String message = axes[i].message(&response);
          messages[messageCount] = message;
          messageCount++;
        }
    }
}
