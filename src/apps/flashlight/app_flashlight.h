#pragma once

#include "apps.h"
#include "resources/icons.h"

#include "Preferences.h"
#include "TFT_eSPI.h"

class AppFlashlight : public App {
public:
  uint32_t brightness;
  Preferences preferences;
  using App::App;
  void setup() override;
  void drawUI(TFT_eSPI tft) override;
  void buttonTopClick() override;
  void buttonBottomClick() override;
  void exit() override;
};

extern std::unique_ptr<AppFlashlight> appFlashlight;