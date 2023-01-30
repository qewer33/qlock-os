#include "theme_red_devil.h"
#include "resources/fonts/InterRegular24.h"
#include "resources/icons.h"

#include "TFT_eSPI.h"
#include "WiFi.h"

std::unique_ptr<ThemeRedDevil> themeRedDevil(new ThemeRedDevil("Red Devil", 0x0000, 0xFFFF, 0xD145, 0x4041));

void ThemeRedDevil::drawHomeUI(TFT_eSPI tft, ESP32Time rtc, uint32_t batteryStatus) {
  TFT_eSprite s = TFT_eSprite(&tft);
  s.createSprite(tft.getViewportWidth(), tft.getViewportHeight());
  s.setSwapBytes(tft.getSwapBytes());

  s.fillRect(0, 0, 320, 100, this->accentColor);

  // clock
  s.setTextColor(this->backgroundColor, this->accentColor, true);
  s.setTextSize(1);
  String hoursFiller = rtc.getHour(true) < 10 ? "0" : "";
  String minutesFiller = rtc.getMinute() < 10 ? "0" : "";
  String timeStr = hoursFiller + String(rtc.getHour(true)) + ":" + minutesFiller + String(rtc.getMinute());
  s.drawString(timeStr.c_str(), 30, 26, 7);
  s.drawFastVLine(199, 26, 46, this->accentColorSecondary);
  s.drawFastVLine(200, 26, 46, this->accentColorSecondary);

  // date
  s.loadFont(InterRegular24);
  s.setTextColor(this->accentColor, this->backgroundColor, true);
  s.drawString(String(String(rtc.getDay()) + "/" + String(rtc.getMonth() + 1) + "/" + String(rtc.getYear())), 15, 125, 4);

  // seconds
  s.setTextColor(this->backgroundColor, this->accentColor, true);
  s.setTextDatum(MC_DATUM);
  String secondsFiller = rtc.getSecond() < 10 ? "0" : "";
  s.drawString(String(secondsFiller + String(rtc.getSecond())).c_str(), 255, 53, 4);
  s.drawArc(255, 50, 27, 25, 0, rtc.getSecond() * 6, this->backgroundColor, this->accentColor, true);

  // wifi status
  s.pushImage(155, 117, icon_wifi.width, icon_wifi.height, (uint16_t *)icon_wifi.pixel_data);
  s.setTextColor(s.color565(31, 100, 204), this->backgroundColor, true);
  s.drawString((WiFi.status() == WL_CONNECTED) ? "WiFi" : "N/A", 193, 143, 4);

  // battery status
  s.pushImage(232, 113, icon_battery.width, icon_battery.height, (uint16_t *)icon_battery.pixel_data);
  s.setTextColor(s.color565(69, 193, 72), this->backgroundColor, true);
  s.drawString(String(String(batteryStatus) + "%").c_str(), 275, 143, 4);

  s.unloadFont();
  s.pushSprite(0, 0);
  s.deleteSprite();
}