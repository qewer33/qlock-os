#pragma once

#include "apps.h"

#include "TFT_eSPI.h"

class AppDino : public App {
public:
  using App::App;
  void setup() override;
  void drawUI(TFT_eSPI tft) override;
  void buttonTopClick() override;
  void buttonBottomClick() override;
};

extern std::unique_ptr<AppDino> appDino;
