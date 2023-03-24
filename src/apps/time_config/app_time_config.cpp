#include "app_time_config.h"
#include "pin_config.h"
#include "resources/fonts/InterRegular16.h"
#include "resources/icons.h"

#include "WiFi.h"

void AppTimeConfig::setup() { rtc = ESP32Time(0); }

void AppTimeConfig::drawUI(TFT_eSPI tft) {
  TFT_eSprite s = TFT_eSprite(&tft);
  s.createSprite(tft.getViewportWidth(), tft.getViewportHeight());
  s.setSwapBytes(tft.getSwapBytes());

  s.setTextColor(TFT_WHITE, TFT_BLACK, true);
  s.setTextSize(1);
  s.setTextDatum(TC_DATUM);

  String hoursFiller = rtc.getHour(true) < 10 ? "0" : "";
  String minutesFiller = rtc.getMinute() < 10 ? "0" : "";
  String secondsFiller = rtc.getSecond() < 10 ? "0" : "";
  String timeStr =
      hoursFiller + String(rtc.getHour(true)) + ":" + minutesFiller + String(rtc.getMinute()) + ":" + secondsFiller + String(rtc.getSecond());
  s.drawString(timeStr.c_str(), 160, 30, 7);

  s.pushSprite(0, 0);
  s.deleteSprite();
}

std::unique_ptr<AppTimeConfig> appTimeConfig(new AppTimeConfig("Configure Time", icon_chrono.pixel_data, 30, 30));