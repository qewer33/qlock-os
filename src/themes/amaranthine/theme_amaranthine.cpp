#include "theme_amaranthine.h"
#include "resources/fonts/InterRegular16.h"
#include "resources/fonts/InterRegular24.h"
#include "resources/icons.h"

#include "TFT_eSPI.h"
#include "WiFi.h"

std::unique_ptr<ThemeAmaranthine> themeAmaranthine(new ThemeAmaranthine("Amaranthine", 0x0000, 0xFFFF, 0x6872, 0x1805));

void ThemeAmaranthine::drawHomeUI(TFT_eSPI tft, ESP32Time rtc, uint32_t batteryStatus) {
  TFT_eSprite s = TFT_eSprite(&tft);
  s.createSprite(tft.getViewportWidth(), tft.getViewportHeight());
  s.setSwapBytes(tft.getSwapBytes());

  int32_t radius = tft.height()/2-20;
  s.drawSmoothArc(tft.width()/2, tft.height()/2, radius + 6, radius, 0, 360, accentColor, backgroundColor);

  float hourAngle = map(rtc.getHour() + map(rtc.getMinute(), 0, 60, 0, 1), 0, 24, 0, TWO_PI) - HALF_PI;
  int32_t hourArmX = tft.width()/2 + cos(hourAngle) * radius;
  int32_t hourArmY = tft.height()/2 + sin(hourAngle) * radius;
  s.drawWideLine(tft.width()/2, tft.height()/2, hourArmX, hourArmY, 6, foregroundColor, backgroundColor);

  float minuteAngle = map(rtc.getMinute() + map(rtc.getSecond(), 0, 60, 0, 1), 0, 60, 0, TWO_PI) - HALF_PI;
  int32_t minuteArmX = tft.width()/2 + cos(90) * (radius - 30);
  int32_t minuteArmY = tft.height()/2 + sin(180) * (radius - 30);
  s.drawWideLine(tft.width()/2, tft.height()/2, minuteArmX, minuteArmY, 6, foregroundColor, backgroundColor);

  int32_t secondAngle = map(rtc.getSecond(), 0, 60, 0, 360) - 90;
  int32_t secondArmX = tft.width()/2 + round(cos(-secondAngle) * radius);
  int32_t secondArmY = tft.height()/2 + round(sin(-secondAngle) * radius);
  s.drawWideLine(tft.width()/2, tft.height()/2, secondArmX, secondArmY, 3, foregroundColor, backgroundColor);

  s.unloadFont();
  s.pushSprite(0, 0);
  s.deleteSprite();
}