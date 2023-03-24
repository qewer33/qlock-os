#pragma once

#include "apps.h"

#include "Preferences.h"
#include "TFT_eSPI.h"

class AppSmartconfig : public App {
public:
  bool done;
  Preferences preferences;
  using App::App;
  void setup() override;
  void drawUI(TFT_eSPI tft) override;
};

extern std::unique_ptr<AppSmartconfig> appSmartconfig;