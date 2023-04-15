#include "app_stopwatch.h"
#include "resources/icons.h"
#include "resources/fonts/InterRegular24.h"

void AppStopwatch::setup() {
  rtc = ESP32Time(0);
  started = false;
  paused = false;
}

void AppStopwatch::drawUI(TFT_eSPI tft) {
  TFT_eSprite s = TFT_eSprite(&tft);
  s.createSprite(tft.getViewportWidth(), tft.getViewportHeight());
  s.setSwapBytes(tft.getSwapBytes());

  if (!paused)
    currentMillis = millis() - pauseMillis;
  uint64_t ms = started ? currentMillis - startMillis : 0;
  int milli = map(ms % 1000 / 10, 0, 100, 0, 100);
  int second = ms / 1000 % 60;
  int minute = ms / 1000 / 60 % 60;
  int hour = ms / 1000 / 60 / 60 % 24;

  // clock
  s.setTextColor(TFT_WHITE, TFT_BLACK, true);
  s.setTextSize(1);
  s.setTextDatum(TC_DATUM);
  String hoursFiller = hour < 10 ? "0" : "";
  String minutesFiller = minute < 10 ? "0" : "";
  String secondsFiller = second < 10 ? "0" : "";
  String timeStr = hoursFiller + String(hour) + ":" + minutesFiller + String(minute) + ":" + secondsFiller + String(second);
  s.drawString(timeStr.c_str(), 160, 30, 7);

  // miliseconds
  s.setTextDatum(MC_DATUM);
  String msFiller = milli < 10 ? "0" : "";
  s.loadFont(InterRegular24);
  s.drawString(String(msFiller + String(milli)).c_str(), 160, 133, 4);
  s.unloadFont();
  int32_t startAngle = second % 2 == 0 ? 0 : map(milli, 0, 100, 0, 360);
  int32_t endAngle = second % 2 == 0 ? map(milli, 0, 100, 0, 360) : 360;
  s.drawArc(160, 130, 27, 25, 0, 360, s.color565(35, 35, 35), TFT_BLACK, true);
  s.drawArc(160, 130, 27, 25, startAngle, endAngle, TFT_WHITE, TFT_BLACK, true);

  s.pushSprite(0, 0);
  s.deleteSprite();
}

void AppStopwatch::buttonTopClick() {
  if (!started) {
    startMillis = millis();
    currentMillis = startMillis;
    pauseMillis = 0;
    started = true;
  } else {
    paused = !paused;
    pauseMillis = millis() - currentMillis;
  }
}

void AppStopwatch::buttonBottomClick() {
  started = false;
  paused = false;
}

std::unique_ptr<AppStopwatch> appStopwatch(new AppStopwatch("Stopwatch", icon_chrono.pixel_data, 30, 30));