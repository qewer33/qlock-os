#pragma once

#include "apps.h"

#include "Preferences.h"
#include "TFT_eSPI.h"

class AppSystemMonitor : public App {
public:
  uint32_t updateTimer;
  float mcuTemp;
  uint64_t freeHeap;
  using App::App;
  void setup() override;
  void drawUI(TFT_eSPI tft) override;
};

extern std::unique_ptr<AppSystemMonitor> appSystemMonitor;