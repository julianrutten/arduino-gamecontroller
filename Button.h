#include <Joystick.h>
#ifndef DEBUG
#define DEBUG false
#endif
class ButtonPushResponse {
  public:
    bool pushed;
    bool released;
    uint8_t number;
};

class Button {
  public:
      uint8_t pin;
      Button(uint8_t pinNumber,uint8_t buttonsBefore);
      Button(uint8_t pinNumber,uint8_t buttonsBefore, bool toggleMe);
      Button(uint8_t pinNumber,uint8_t buttonsBefore, bool toggleMe, bool toggleSeperately);
      Button(uint8_t pinNumber,uint8_t buttonsBefore, bool toggleMe, bool toggleSeperately,bool invertMe);

      void init(uint8_t pinNumber,uint8_t buttonsBefore, bool toggleMe, bool toggleSeperately,bool invertMe);
      ButtonPushResponse refresh(Joystick_ *joystick);
          String message(ButtonPushResponse *response);
  private:
      ButtonPushResponse push(bool rising, Joystick_ *joystick);
       
      bool isToggle;
      bool toggleSeperately;
      bool invert;
      uint8_t number;
      bool state;
      bool pushing;
      uint8_t pushingNumber;
};

class ButtonManager {
    public:
        ButtonManager(Button *_buttons,size_t _buttonCount,Joystick_ *_joystick): buttons(_buttons),buttonCount(_buttonCount),joystick(_joystick) {};
        void tick(String* messages);
    private: 
        Button *buttons;
        size_t buttonCount;
        Joystick_ *joystick;
};
