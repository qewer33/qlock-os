#pragma once

#include "apps.h"

#include "ESP32Time.h"
#include "Preferences.h"
#include "TFT_eSPI.h"

class AppTimeConfig : public App {
public:
  ESP32Time rtc;
  using App::App;
  void setup() override;
  void drawUI(TFT_eSPI tft) override;
};

extern std::unique_ptr<AppTimeConfig> appTimeConfig;