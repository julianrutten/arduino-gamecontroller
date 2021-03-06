#include <Joystick.h>
#include <Arduino.h>
#include "Button.h"

Button::Button(uint8_t pinNumber,uint8_t buttonsBefore) {
    init(pinNumber,buttonsBefore, false, false,false);
}

Button::Button(uint8_t pinNumber,uint8_t buttonsBefore, bool toggleMe) {
    init(pinNumber,buttonsBefore, toggleMe, false,false);
}
 
Button::Button(uint8_t pinNumber,uint8_t buttonsBefore, bool toggleMe, bool toggleMeSeperately) {
    init(pinNumber,buttonsBefore, toggleMe, toggleMeSeperately,false);
}
Button::Button(uint8_t pinNumber,uint8_t buttonsBefore, bool toggleMe, bool toggleMeSeperately, bool invertMe) {
    init(pinNumber,buttonsBefore, toggleMe, toggleMeSeperately,invertMe);
}
void Button::init(uint8_t pinNumber,uint8_t buttonsBefore, bool toggleMe, bool toggleMeSeperately, bool invertMe){
  pinMode(pinNumber,INPUT);
  digitalWrite(pinNumber,HIGH);
  pin = pinNumber;
  //Only push button when states change
  isToggle = toggleMe;
  //Reserve 1 button for the optional toggle, to avoid button reassignment in games when changing controller layout.
  number = (buttonsBefore * 2);
   //Will push the reserved button when changing states on falling edge
  toggleSeperately = toggleMeSeperately;
  invert = invertMe;
}
String Button::message(ButtonPushResponse *response){
  String message = response->pushed ? "Pushed" : "Released";
  message = message + " " + response->number + " pin " + pin;
  return message;
}
ButtonPushResponse Button::push(bool rising, Joystick_ *joystick){
  pushing = true;
  pushingNumber = rising ? number : (toggleSeperately ? number + 1 : number);
  joystick->pressButton(pushingNumber);
  return (ButtonPushResponse) {true,false,pushingNumber};
}

ButtonPushResponse Button::refresh(Joystick_ *joystick){
    bool nextState = digitalRead(pin) == HIGH;
    if(invert){
      nextState = !nextState;
    }
    if(!isToggle){
      if(nextState != state){
        state = !state;
        joystick->setButton(number,nextState ? HIGH : LOW);
        return (ButtonPushResponse) {state,!state,number};
      }else{
        return (ButtonPushResponse) {false, false,number};
      }
    }
    
    if(pushing){
      pushing = false;
      //Turn around the state and release
      state = !state;
      joystick->releaseButton(pushingNumber);
      //We will say false to release here because it clutters the log for toggles
      return (ButtonPushResponse) {false,false, pushingNumber};
    }
    
    if(nextState && !state){
      //Rising edge 
      return push(true,joystick);
    }else if(!nextState && state){
      //Falling edge
      return push(false,joystick);
    }
    
    return (ButtonPushResponse) {false,false,number};
};

void ButtonManager::tick(String* messages){
    uint8_t messageCount = 0;
    for( unsigned int i = 0; i < buttonCount; i = i + 1 ){
        ButtonPushResponse response = buttons[i].refresh(joystick); 
        if(response.pushed || response.released){
          String message = buttons[i].message(&response);
          messages[messageCount] = message;
          messageCount++;
        }
    }
}
