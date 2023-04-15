#pragma once

#include "Arduino.h"
#include "TFT_eSPI.h"
#include "vector"

class App {
public:
  String name;
  const unsigned char *icon;
  unsigned int iconWidth;
  unsigned int iconHeight;

  App(String name, const unsigned char *icon, unsigned int iconWidth, unsigned int iconHeight);
  virtual void setup();
  virtual void drawUI(TFT_eSPI tft);
  virtual void exit();
  virtual void buttonTopClick();
  virtual void buttonTopLongPress();
  virtual void buttonBottomClick();
  virtual void buttonBottomLongPress();
};

extern std::vector<std::unique_ptr<App>> apps;

extern uint32_t currentAppIndex;

void initApps();
void drawAppsListUI(TFT_eSPI tft, uint32_t batteryStatus);