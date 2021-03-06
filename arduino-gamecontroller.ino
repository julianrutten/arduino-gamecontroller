#include <Joystick.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LED_PIN 13
#define LCD_ADDRESS 0x27
#define LCD_MAXCHARS 20
#define LCD_ROWS 4
#define HZ 40
#define DEBUG 1
#include "Button.h"
#include "Axis.h"

LiquidCrystal_I2C lcd(LCD_ADDRESS, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Set the LCD I2C address

Joystick_ joystick;
Button buttons[] = {
  Button(A0,0,true,true),
  Button(A1,1,true,true),
  Button(A2,2,true),
  Button(A3,3,true,true),
  Button(4,4,true,true),
  Button(5,5,true,true),
  Button(6,6,true)
  };
uint8_t buttonCount = sizeof(buttons) / sizeof(Button);
ButtonManager buttonManager(buttons,buttonCount,&joystick);

Axis axes[] = {
   Axis(A4,0)
};
uint8_t axesCount = sizeof(axes) / sizeof(Axis);
AxisManager axesManager(axes,axesCount,&joystick);

int currentLine = 0;
void setup() {
  if(LCD_ADDRESS != false){
    lcd.begin(20,4);
    lcd.setCursor(0,0);
  }
  joystick.begin();
}

String make_str(String str){
    for(int i = 0; i < (LCD_MAXCHARS - str.length()); i++) {
        str += ' ';  
    }
    return str;
}
void printMessages(String messages[LCD_ROWS]){
  for(int i=0;i<(LCD_ROWS - 1);i++){
      if(messages[i].length() > 0){
          if(DEBUG == 1){
            Serial.println(messages[i]);
          }
          if(currentLine >= LCD_ROWS){
            lcd.clear();
            currentLine = 0;
          }
          lcd.setCursor(0,currentLine);
          lcd.print(make_str(messages[i]));
          currentLine++;
      }
  }
}

void loop() {
  long startedAt = millis();

  String buttonMessages[buttonCount];
  buttonManager.tick(buttonMessages);

  String axesMessages[axesCount];
  axesManager.tick(axesMessages);
  
  if(LCD_ADDRESS != false){
    printMessages(buttonMessages);
//    printMessages(axes Messages);
  }
  
  //Sleep if we can
  int sleep = (1000 / HZ) - (millis() - startedAt);
  if(sleep > 0){
    delay(sleep);
  }
}
