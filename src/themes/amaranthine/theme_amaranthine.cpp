#include "theme_amaranthine.h"
#include "resources/fonts/InterRegular16.h"
#include "resources/fonts/InterRegular24.h"
#include "resources/fonts/OutfitBold110.h"
#include "resources/fonts/SmoochRegular48.h"
#include "resources/icons.h"

#include "TFT_eSPI.h"
#include "WiFi.h"

std::unique_ptr<ThemeAmaranthine> themeAmaranthine(new ThemeAmaranthine("Amaranthine", 0x0000, 0xFFFF, 0x6872, 0x1805));

float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void ThemeAmaranthine::drawHomeUI(TFT_eSPI tft, ESP32Time rtc, uint32_t batteryStatus) {
  TFT_eSprite s = TFT_eSprite(&tft);
  s.createSprite(tft.getViewportWidth(), tft.getViewportHeight());
  s.setSwapBytes(tft.getSwapBytes());

  /*
  int32_t cx = tft.width() / 4 + 10;
  int32_t cy = tft.height() / 2;
  int32_t radius = tft.height() / 2 - 20;

  s.drawSmoothArc(cx, cy, radius + 10, radius + 8, 0, 360, accentColor, backgroundColor);
  s.fillCircle(cx, cy, radius + 8, accentColorSecondary);

  float hourAngle = mapf(rtc.getHour() + mapf(rtc.getMinute(), 0, 60, 0, 1), 0, 24, 0, TWO_PI) - HALF_PI;
  int32_t hourArmX = cx + cos(hourAngle) * (radius - 20);
  int32_t hourArmY = cy + sin(hourAngle) * (radius - 20);
  s.drawWideLine(cx, cy, hourArmX, hourArmY, 5, foregroundColor, backgroundColor);

  float minuteAngle = mapf(rtc.getMinute() + mapf(rtc.getSecond(), 0, 60, 0, 1), 0, 60, 0, TWO_PI) - HALF_PI;
  int32_t minuteArmX = cx + cos(minuteAngle) * radius;
  int32_t minuteArmY = cy + sin(minuteAngle) * radius;
  s.drawWideLine(cx, cy, minuteArmX, minuteArmY, 5, foregroundColor, backgroundColor);

  float secondAngle = mapf(rtc.getSecond(), 0, 60, 0, TWO_PI) - HALF_PI;
  int32_t secondArmX = cx + round(cos(secondAngle) * radius);
  int32_t secondArmY = cy + round(sin(secondAngle) * radius);
  s.drawWideLine(cx, cy, secondArmX, secondArmY, 3, foregroundColor, backgroundColor);
  int32_t secondArmExtX = cx + round(-cos(secondAngle) * (radius - 45));
  int32_t secondArmExtY = cy + round(-sin(secondAngle) * (radius - 45));
  s.drawWideLine(cx, cy, secondArmExtX, secondArmExtY, 3, foregroundColor, backgroundColor);

  s.fillCircle(cx, cy, 10, backgroundColor);
  s.fillCircle(cx, cy, 6, accentColor);

  s.loadFont(InterRegular24);
  s.setTextDatum(MC_DATUM);
  String secondsFiller = rtc.getSecond() < 10 ? "0" : "";
  s.drawString(String(secondsFiller + String(rtc.getSecond())).c_str(), 40, 75);
  */

  s.loadFont(OutfitBold110);
  s.setTextDatum(MC_DATUM);
  String hoursFiller = rtc.getHour(true) < 10 ? "0" : "";
  String minutesFiller = rtc.getMinute() < 10 ? "0" : "";
  String timeStr = hoursFiller + String(rtc.getHour(true)) + ":" + minutesFiller + String(rtc.getMinute());
  s.drawString(timeStr.c_str(), 160, 95);
  s.unloadFont();

  s.loadFont(InterRegular16);
  s.drawString(rtc.getDay() + " " + String(months[rtc.getMonth()]) + " " + rtc.getYear(), 160, 145);
  s.unloadFont();

  s.loadFont(SmoochRegular48);
  s.setTextColor(s.color565(196, 45, 250));
  s.drawString(days[rtc.getDayofWeek() - 1], 160, 95);
  s.unloadFont();

  s.unloadFont();
  s.pushSprite(0, 0);
  s.deleteSprite();
}