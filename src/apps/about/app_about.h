#pragma once

#include "apps.h"

#include "TFT_eSPI.h"

class AppAbout : public App {
public:
  using App::App;
  void drawUI(TFT_eSPI tft) override;
};

extern std::unique_ptr<AppAbout> appAbout;