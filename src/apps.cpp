#include "apps.h"
#include "Arduino.h"
#include "TFT_eSPI.h"
#include "WiFi.h"

#include "lib/animation.h"
#include "os_config.h"
#include "resources/fonts/InterRegular16.h"
#include "resources/fonts/InterRegular24.h"
#include "resources/icons.h"
#include "themes.h"

#include "apps/about/app_about.h"
#include "apps/dino/app_dino.h"
#include "apps/flashlight/app_flashlight.h"
#include "apps/racer/app_racer.h"
#include "apps/smartconfig/app_smartconfig.h"
#include "apps/stopwatch/app_stopwatch.h"
#include "apps/themes/app_themes.h"
#include "apps/time_config/app_time_config.h"

std::vector<std::unique_ptr<App>> apps;
unsigned int currentAppIndex = 0;
const int headerHeight = 35;
int indicatorY = headerHeight;
int appsY = headerHeight;

App::App(String name, const unsigned char *icon, unsigned int iconWidth, unsigned int iconHeight) {
  this->name = name;
  this->icon = icon;
  this->iconWidth = iconWidth;
  this->iconHeight = iconHeight;
  this->skipFirstRefresh = false;
}

void App::setup() {}
void App::drawUI(TFT_eSPI tft) {}
void App::exit() {}
void App::buttonTopClick() {}
void App::buttonTopLongPress() {}
void App::buttonBottomClick() {}
void App::buttonBottomLongPress() {}

void initApps() {
  apps.emplace_back(std::move(appStopwatch));
  apps.emplace_back(std::move(appFlashlight));
  apps.emplace_back(std::move(appRacer));
  apps.emplace_back(std::move(appDino));
  apps.emplace_back(std::move(appThemes));
  apps.emplace_back(std::move(appSmartconfig));
  apps.emplace_back(std::move(appTimeConfig));
  apps.emplace_back(std::move(appAbout));
}

void drawAppsListUI(TFT_eSPI tft, uint32_t batteryStatus) {
  TFT_eSprite s = TFT_eSprite(&tft);
  s.createSprite(tft.getViewportWidth(), tft.getViewportHeight());
  s.setSwapBytes(tft.getSwapBytes());

  indicatorY = currentAppIndex <= 1 ? animateInt(indicatorY, currentAppIndex * 50 + headerHeight, 20) : 50 + headerHeight;
  appsY = currentAppIndex < 1 ? headerHeight : animateInt(appsY, -(currentAppIndex - 1) * 50 + headerHeight, 20);

  // list
  s.fillSmoothRoundRect(0, indicatorY, 3, 50, 10, themes[currentThemeIndex]->accentColor);
  s.fillSmoothRoundRect(10, indicatorY, 305, 50, 8, themes[currentThemeIndex]->accentColorSecondary);
  s.drawSmoothRoundRect(10, indicatorY, 8, 8, 305, 50, themes[currentThemeIndex]->accentColor, themes[currentThemeIndex]->accentColorSecondary);

  s.loadFont(InterRegular24);
  for (int i = 0; i < apps.size(); i++) {
    TFT_eSprite appIcon = TFT_eSprite(&tft);
    appIcon.createSprite(apps[i]->iconWidth, apps[i]->iconHeight);
    appIcon.setSwapBytes(false);
    appIcon.pushImage(0, 0, apps[i]->iconWidth, apps[i]->iconHeight, (uint16_t *)apps[i]->icon);
    appIcon.pushToSprite(&s, 24, i * 50 + 10 + appsY, TFT_BLACK);
    s.setTextColor(themes[currentThemeIndex]->foregroundColor);
    s.setTextSize(1);
    s.setTextDatum(CL_DATUM);
    s.drawString(apps[i]->name, 70, i * 50 + 29 + appsY);
  }
  s.unloadFont();

  // header
  s.fillRect(0, 0, 320, 35, TFT_BLACK);

  s.loadFont(InterRegular16);
  s.setTextDatum(TL_DATUM);
  s.drawString(DEVICE_NAME, 5, 10);
  s.unloadFont();

  s.pushImage(194, 5, 25, 25, (uint16_t *)icon_no_ble_small.pixel_data);

  if (WiFi.status() == WL_CONNECTED)
    s.pushImage(221, 5, 25, 25, (uint16_t *)icon_wifi_small.pixel_data);
  else
    s.pushImage(221, 5, 25, 25, (uint16_t *)icon_no_wifi_small.pixel_data);

  auto batteryStatusIcon = (uint16_t *)icon_battery_charge_small.pixel_data;
  if(batteryStatus != 100) {
    switch (batteryStatus / 20) {
    case 0:
      batteryStatusIcon = (uint16_t *)icon_battery_0_small.pixel_data;
      break;
    case 1:
      batteryStatusIcon = (uint16_t *)icon_battery_20_small.pixel_data;
      break;
    case 2:
      batteryStatusIcon = (uint16_t *)icon_battery_40_small.pixel_data;
      break;
    case 3:
      batteryStatusIcon = (uint16_t *)icon_battery_60_small.pixel_data;
      break;
    case 4:
      batteryStatusIcon = (uint16_t *)icon_battery_80_small.pixel_data;
      break;
    case 5:
      batteryStatusIcon = (uint16_t *)icon_battery_100_small.pixel_data;
      break;
    }
  }
  s.pushImage(255, 5, 25, 25, batteryStatusIcon);
  s.loadFont(InterRegular16);
  s.setTextDatum(TL_DATUM);
  s.setTextColor(s.color565(69, 193, 72));
  s.drawString(String(batteryStatus), 285, 11);
  s.unloadFont();

  s.pushSprite(0, 0);
  s.deleteSprite();
}