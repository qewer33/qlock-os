#include "app_dino.h"
#include "resources.h"

void AppDino::setup() {}
void AppDino::drawUI(TFT_eSPI tft) {}
void AppDino::buttonTopClick() {}
void AppDino::buttonBottomClick() {}

std::unique_ptr<AppDino> appDino(new AppDino("Dino", icon_dino.pixel_data, 30, 30));
