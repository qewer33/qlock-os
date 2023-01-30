#pragma once

#include "../../apps.h"
#include "ESP32Time.h"
#include "TFT_eSPI.h"
#include "time.h"

class AppStopwatch : public App {
public:
  ESP32Time rtc;
  bool started;
  bool paused;
  uint64_t startMillis;
  uint64_t currentMillis;
  uint64_t pauseMillis;
  using App::App;
  void setup() override;
  void drawUI(TFT_eSPI tft) override;
  void buttonTopClick() override;
  void buttonBottomClick() override;
};

extern std::unique_ptr<AppStopwatch> appStopwatch;