#pragma once

#include "apps.h"

#include "Preferences.h"
#include "TFT_eSPI.h"
#include "vector"

struct RacerTreeObject {
  int32_t x;
  uint32_t type;
};

struct RacerCarObject {
  int32_t x;
  uint32_t lane;
};

class AppRacer : public App {
public:
  bool gameOver;
  uint32_t score;
  uint32_t highscore;
  uint32_t carLane;
  uint32_t gameSpeed;
  Preferences preferences;
  std::vector<RacerTreeObject> trees;
  std::vector<RacerCarObject> cars;
  using App::App;
  void setup() override;
  void drawUI(TFT_eSPI tft) override;
  void buttonTopClick() override;
  void buttonBottomClick() override;
  void exit() override;
};

extern std::unique_ptr<AppRacer> appRacer;