#include "apps.h"
#include "Arduino.h"
#include "TFT_eSPI.h"

#include "lib/animation.h"
#include "resources.h"
#include "resources/fonts/InterRegular24.h"
#include "themes.h"

#include "apps/about/app_about.h"
#include "apps/dino/app_dino.h"
#include "apps/flashlight/app_flashlight.h"
#include "apps/racer/app_racer.h"
#include "apps/smartconfig/app_smartconfig.h"
#include "apps/stopwatch/app_stopwatch.h"
#include "apps/system_monitor/app_system_monitor.h"
#include "apps/themes/app_themes.h"
#include "apps/time_config/app_time_config.h"

std::vector<std::unique_ptr<App>> apps;
unsigned int currentAppIndex = 0;
int indicatorY = 0;
int appsY = 0;

App::App(String name, const unsigned char *icon, unsigned int iconWidth, unsigned int iconHeight) {
  this->name = name;
  this->icon = icon;
  this->iconWidth = iconWidth;
  this->iconHeight = iconHeight;
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
  apps.emplace_back(std::move(appSystemMonitor));
  apps.emplace_back(std::move(appSmartconfig));
  apps.emplace_back(std::move(appTimeConfig));
  apps.emplace_back(std::move(appAbout));
}

void drawAppsListUI(TFT_eSPI tft) {
  TFT_eSprite s = TFT_eSprite(&tft);
  s.createSprite(tft.getViewportWidth(), tft.getViewportHeight());
  s.setSwapBytes(tft.getSwapBytes());

  indicatorY = currentAppIndex <= 1 ? animateInt(indicatorY, currentAppIndex * 50, 20) : 50;
  appsY = currentAppIndex < 1 ? 0 : animateInt(appsY, (currentAppIndex - 1) * 50, 20);

  s.fillRect(0, indicatorY, 320, 50, themes[currentThemeIndex]->accentColorSecondary);
  s.fillRect(0, indicatorY, 3, 50, themes[currentThemeIndex]->accentColor);

  s.loadFont(InterRegular24);
  for (int i = 0; i < apps.size(); i++) {
    TFT_eSprite appIcon = TFT_eSprite(&tft);
    appIcon.createSprite(apps[i]->iconWidth, apps[i]->iconHeight);
    appIcon.setSwapBytes(false);
    appIcon.pushImage(0, 0, apps[i]->iconWidth, apps[i]->iconHeight, (uint16_t *)apps[i]->icon);
    appIcon.pushToSprite(&s, 14, i * 50 + 10 - appsY, TFT_BLACK);
    s.setTextColor(themes[currentThemeIndex]->foregroundColor);
    s.setTextSize(1);
    s.setTextDatum(CL_DATUM);
    s.drawString(apps[i]->name, 60, i * 50 + 29 - appsY);
  }
  s.unloadFont();

  s.pushSprite(0, 0);
  s.deleteSprite();
}