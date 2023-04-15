#include "app_flashlight.h"
#include "os_config.h"
#include "resources/fonts/InterRegular24.h"

#include "Arduino.h"

void AppFlashlight::setup() {
  preferences.begin(PREFS_KEY);
  brightness = brightness = preferences.getUInt("brightness", 200);
  preferences.end();
}

void AppFlashlight::drawUI(TFT_eSPI tft) {
  TFT_eSprite s = TFT_eSprite(&tft);
  s.createSprite(tft.getViewportWidth(), tft.getViewportHeight());
  s.setSwapBytes(tft.getSwapBytes());

  s.fillSprite(TFT_WHITE);
  s.setTextDatum(MC_DATUM);
  s.setTextColor(TFT_BLACK);
  s.loadFont(InterRegular24);
  s.drawString(String(map(brightness, 50, 250, 0, 100)) + "%", 160, 85, 4);
  s.unloadFont();

  s.pushSprite(0, 0);
  s.deleteSprite();
}

void AppFlashlight::buttonTopClick() {
  if (brightness < 250)
    brightness += 10;
  ledcWrite(0, brightness);
  Serial.println(brightness);
}

void AppFlashlight::buttonBottomClick() {
  if (brightness > 50)
    brightness -= 10;
  ledcWrite(0, brightness);
  Serial.println(brightness);
}

void AppFlashlight::exit() {
  preferences.begin(PREFS_KEY);
  preferences.putUInt("brightness", brightness);
  preferences.end();
}

std::unique_ptr<AppFlashlight> appFlashlight(new AppFlashlight("Flashlight", icon_flashlight.pixel_data, 30, 30));