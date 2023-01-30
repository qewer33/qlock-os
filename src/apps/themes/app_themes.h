#pragma once

#include "apps.h"

#include "Preferences.h"
#include "TFT_eSPI.h"

class AppThemes : public App {
public:
  Preferences preferences;
  using App::App;
  void setup() override;
  void drawUI(TFT_eSPI tft) override;
  void buttonTopClick() override;
  void buttonBottomClick() override;
  void exit() override;
};

extern std::unique_ptr<AppThemes> appThemes;