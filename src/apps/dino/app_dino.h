#pragma once

#include "apps.h"

#include "Preferences.h"
#include "TFT_eSPI.h"
#include "vector"

enum DinoPlayerState { RUN, JUMP, SNEAK };

struct DinoTreeObject {
  int32_t x;
  uint32_t count;
  bool big;
};

struct DinoBirdObject {
  int32_t x;
};

class AppDino : public App {
public:
  bool gameOver;
  uint32_t score;
  uint32_t highscore;
  uint32_t gameSpeed;
  int32_t groundX;
  int32_t playerY;
  int32_t playerYVelo;
  int32_t playerBaseY;
  DinoPlayerState playerState;
  uint32_t playerAnimState;
  std::vector<DinoTreeObject> trees;
  std::vector<DinoBirdObject> birds;
  Preferences preferences;
  using App::App;
  void setup() override;
  void drawUI(TFT_eSPI tft) override;
  void buttonTopClick() override;
  void buttonBottomClick() override;
};

extern std::unique_ptr<AppDino> appDino;
