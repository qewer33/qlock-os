#include "app_about.h"
#include "resources.h"
#include "resources/fonts/InterRegular16.h"
#include "resources/fonts/InterRegular24.h"

void AppAbout::drawUI(TFT_eSPI tft) {
  TFT_eSprite s = TFT_eSprite(&tft);
  s.createSprite(tft.getViewportWidth(), tft.getViewportHeight());
  s.setSwapBytes(tft.getSwapBytes());

  s.pushImage(130, 10, qlockos_logo.width, qlockos_logo.height, (uint16_t *)qlockos_logo.pixel_data);
  s.setTextColor(TFT_WHITE, TFT_BLACK, true);
  s.setTextDatum(TC_DATUM);
  s.loadFont(InterRegular24);
  s.drawString("qlockOS v0.1", 160, 90, 4);
  s.unloadFont();
  s.loadFont(InterRegular16);
  s.drawString("developed by qewer33", 160, 125, 2);
  s.drawString("Running on LILYGO T-Display S3", 160, 145, 2);

  s.unloadFont();
  s.pushSprite(0, 0);
  s.deleteSprite();
}

std::unique_ptr<AppAbout> appAbout(new AppAbout("About", icon_info.pixel_data, 30, 30));