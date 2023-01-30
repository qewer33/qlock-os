#include "app_themes.h"
#include "pin_config.h"
#include "resources/fonts/InterRegular24.h"
#include "resources/icons.h"
#include "themes.h"

void AppThemes::setup() {}

void AppThemes::drawUI(TFT_eSPI tft) {
  TFT_eSprite s = TFT_eSprite(&tft);
  s.createSprite(tft.getViewportWidth(), tft.getViewportHeight());
  s.setSwapBytes(tft.getSwapBytes());

  int indicatorY = currentThemeIndex <= 1 ? currentThemeIndex * 50 : 50;

  s.fillRect(0, indicatorY, 320, 50, themes[currentThemeIndex]->accentColorSecondary);
  s.fillRect(0, indicatorY, 3, 50, themes[currentThemeIndex]->accentColor);

  s.loadFont(InterRegular24);
  for (int i = 0; i < themes.size(); i++) {
    int y = currentThemeIndex <= 1 ? i * 50 : i * 50 - (currentThemeIndex - 1) * 50;

    s.fillSmoothCircle(30, y + 25, 15, themes[i]->accentColor);
    s.setTextColor(themes[i]->foregroundColor);
    s.setTextSize(1);
    s.setTextDatum(CL_DATUM);
    s.drawString(themes[i]->name, 60, y + 29, 4);
  }

  s.unloadFont();
  s.pushSprite(0, 0);
  s.deleteSprite();
}

void AppThemes::buttonTopClick() {
  if (currentThemeIndex != 0)
    currentThemeIndex--;
}

void AppThemes::buttonBottomClick() {
  if (currentThemeIndex != themes.size() - 1)
    currentThemeIndex++;
}

void AppThemes::exit() {
  preferences.begin(PREFS_KEY);
  preferences.putUInt("current_theme", currentThemeIndex);
  preferences.end();
}

std::unique_ptr<AppThemes> appThemes(new AppThemes("Themes", icon_theme.pixel_data, 30, 30));