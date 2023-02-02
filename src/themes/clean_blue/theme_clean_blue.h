#pragma once

#include "themes.h"

class ThemeCleanBlue : public Theme {
public:
  using Theme::Theme;
  void drawHomeUI(TFT_eSPI tft, ESP32Time rtc, uint32_t batteryStatus) override;
};

extern std::unique_ptr<ThemeCleanBlue> themeCleanBlue;