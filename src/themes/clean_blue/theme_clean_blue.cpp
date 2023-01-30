#include "theme_clean_blue.h"
#include "resources/fonts/InterRegular16.h"
#include "resources/fonts/InterRegular24.h"
#include "resources/icons.h"

#include "TFT_eSPI.h"
#include "WiFi.h"

std::unique_ptr<ThemeCleanBlue> themeCleanBlue(new ThemeCleanBlue("Clean Blue", 0x0000, 0xFFFF, 0x03FA, 0x0066));

void ThemeCleanBlue::drawHomeUI(TFT_eSPI tft, ESP32Time rtc, uint32_t batteryStatus) {
  TFT_eSprite s = TFT_eSprite(&tft);
  s.createSprite(tft.getViewportWidth(), tft.getViewportHeight());
  s.setSwapBytes(tft.getSwapBytes());

  // clock
  s.setTextColor(TFT_WHITE, TFT_BLACK, true);
  s.setTextSize(1);
  String hoursFiller = rtc.getHour(true) < 10 ? "0" : "";
  String minutesFiller = rtc.getMinute() < 10 ? "0" : "";
  String timeStr = hoursFiller + String(rtc.getHour(true)) + ":" + minutesFiller + String(rtc.getMinute());
  s.drawString(timeStr.c_str(), 30, 30, 7);
  s.drawFastVLine(189, 30, 50, s.color565(35, 35, 35));
  s.drawFastVLine(190, 30, 50, s.color565(35, 35, 35));
  s.loadFont(InterRegular16);
  s.drawString(days[rtc.getDayofWeek() - 1], 215, 27, 2);
  s.drawString(months[rtc.getMonth()], 215, 47, 2);
  s.drawNumber(rtc.getDay(), 215, 67, 2);
  s.unloadFont();

  // seconds
  s.loadFont(InterRegular24);
  s.setTextDatum(MC_DATUM);
  String secondsFiller = rtc.getSecond() < 10 ? "0" : "";
  s.drawString(String(secondsFiller + String(rtc.getSecond())).c_str(), 160, 133, 4);
  s.drawArc(160, 130, 27, 25, 0, 360, s.color565(35, 35, 35), TFT_BLACK, true);
  s.drawArc(160, 130, 27, 25, 0, rtc.getSecond() * 6, TFT_WHITE, TFT_BLACK, true);

  // wifi status
  s.pushImage(30, 112, icon_wifi.width, icon_wifi.height, (uint16_t *)icon_wifi.pixel_data);
  s.setTextColor(s.color565(31, 100, 204), TFT_BLACK, true);
  s.drawString((WiFi.status() == WL_CONNECTED) ? "WiFi" : "N/A", 68, 138, 4);

  // battery status
  s.pushImage(217, 108, icon_battery.width, icon_battery.height, (uint16_t *)icon_battery.pixel_data);
  s.setTextColor(s.color565(69, 193, 72), TFT_BLACK, true);
  s.drawString(String(String(batteryStatus) + "%").c_str(), 260, 138, 4);

  s.unloadFont();
  s.pushSprite(0, 0);
  s.deleteSprite();
}