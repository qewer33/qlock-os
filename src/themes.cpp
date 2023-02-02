#include "themes.h"
#include "Arduino.h"

#include "themes/clean_blue/theme_clean_blue.h"
#include "themes/red_devil/theme_red_devil.h"
#include "themes/amaranthine/theme_amaranthine.h"

const char *months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

const char *days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

std::vector<std::unique_ptr<Theme>> themes;
uint32_t currentThemeIndex = 0;

Theme::Theme(String name, uint16_t backgroundColor, uint16_t foregroundColor, uint16_t accentColor, uint16_t accentColorSecondary) {
  this->name = name;
  this->backgroundColor = backgroundColor;
  this->foregroundColor = foregroundColor;
  this->accentColor = accentColor;
  this->accentColorSecondary = accentColorSecondary;
}

void Theme::drawHomeUI(TFT_eSPI tft, ESP32Time rtc, uint32_t batteryStatus) {}

void initThemes() {
  themes.emplace_back(std::move(themeCleanBlue));
  themes.emplace_back(std::move(themeRedDevil));
  themes.emplace_back(std::move(themeAmaranthine));
}