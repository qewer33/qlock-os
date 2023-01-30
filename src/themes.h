#pragma once

#include "Arduino.h"
#include "ESP32Time.h"
#include "TFT_eSPI.h"
#include "vector"

class Theme {
public:
  String name;
  uint16_t backgroundColor;
  uint16_t foregroundColor;
  uint16_t accentColor;
  uint16_t accentColorSecondary;
  Theme(String name, uint16_t backgroundColor, uint16_t foregroundColor, uint16_t accentColor, uint16_t accentColorSecondary);
  virtual void drawHomeUI(TFT_eSPI tft, ESP32Time rtc, uint32_t batteryStatus);
};

extern std::vector<std::unique_ptr<Theme>> themes;

extern uint32_t currentThemeIndex;

void initThemes();

extern const char *months[];

extern const char *days[];