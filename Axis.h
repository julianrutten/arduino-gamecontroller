#include <Joystick.h>
#ifndef DEBUG
#define DEBUG false
#endif
struct AxisResponse {
    bool changed;
    uint16_t value;
    uint8_t number;
};

class Axis {
  public:
    Axis(uint8_t _pin,uint8_t _number);
    Axis(uint8_t _pin,uint8_t _number, bool _invert);
    
    void init(uint8_t _pin,uint8_t _number, bool _invert);
    AxisResponse refresh(Joystick_ *joystick);
    String message(AxisResponse *response);
  private:
    uint16_t value;
    uint8_t pin;
    bool invert;
    uint8_t number;
};

class AxisManager {
  public:
    AxisManager(Axis *_axes, size_t _axesCount, Joystick_ *_joystick): axes(_axes), axesCount(_axesCount), joystick(_joystick) {};
    void tick(String* messages);
  private:
    Axis *axes;
    size_t axesCount;
    Joystick_ *joystick;
};
