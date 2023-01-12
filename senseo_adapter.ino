

#define PRESS_DURATION 150

#include "pin.h"

unsigned long time_button_pressed = 0;


void pressPowerButton() {
  digitalWrite(POWER_BUTTON, HIGH);
  time_button_pressed = millis();
}

void pressSmallCoffeeButton() {
  digitalWrite(LEFT_BUTTON, HIGH);
  time_button_pressed = millis();
}

void pressBigCoffeeButton() {
  digitalWrite(RIGHT_BUTTON, HIGH);
  time_button_pressed = millis();
}

void releaseIfPressed() {
  if ((time_button_pressed != 0) && (millis() - time_button_pressed >= PRESS_DURATION)) {
    digitalWrite(POWER_BUTTON, LOW);
    digitalWrite(LEFT_BUTTON, LOW);
    digitalWrite(RIGHT_BUTTON, LOW);
    time_button_pressed = 0;
  }
}