#include "Arduino.h"
#include "ESP32Time.h"
#include "OneButton.h"
#include "Preferences.h"
#include "TFT_eSPI.h"
#include "WiFi.h"
#include "esp_wifi.h"

#include "apps.h"
#include "lib/log.h"
#include "pin_config.h"
#include "resources.h"
#include "resources/fonts/InterRegular16.h"
#include "themes.h"

#define USE_DMA_TO_TFT

TFT_eSPI tft = TFT_eSPI();

uint32_t brightness = 200;

hw_timer_t *uiTimer = NULL;
volatile SemaphoreHandle_t timerSemaphore;

enum State {
  Home,
  AppsList,
  InApp,
};

State cState = Home;

Preferences preferences;

ESP32Time rtc(0);

OneButton btn1 = OneButton(PIN_BUTTON_1);
OneButton btn2 = OneButton(PIN_BUTTON_2);

uint32_t batteryStatus;

String wifi_ssid = "";
String wifi_passwd = "";

uint32_t sleepTimer = 0;

void WiFiConnected(WiFiEvent_t event, WiFiEventInfo_t info) { configTime(GMT_OFFSET_SEC, DAY_LIGHT_OFFSET_SEC, NTP_SERVER1); }

void ARDUINO_ISR_ATTR onTimer() { xSemaphoreGiveFromISR(timerSemaphore, NULL); }

void fadeScreen(uint32_t dly, bool reverse) {
  int t = reverse ? brightness : 0;
  int i = reverse ? -1 : 1;

  while (reverse ? t >= 0 : t < brightness) {
    t += i;
    ledcWrite(0, t);
    delay(dly);
  }
}

void refreshPreferences() {
  preferences.begin(PREFS_KEY);
  brightness = preferences.getUInt("brightness", 200);
  wifi_ssid = preferences.getString("wifi_ssid", "");
  wifi_passwd = preferences.getString("wifi_passwd", "");
  currentThemeIndex = preferences.getUInt("current_theme", 0);
  preferences.end();
}

void enterSleep() {
  // sleep sequence
  fadeScreen(2, true);
  tft.fillScreen(TFT_BLACK);
  digitalWrite(PIN_POWER_ON, LOW);
  delay(100);
  esp_wifi_stop();
  esp_sleep_enable_ext0_wakeup((gpio_num_t)PIN_BUTTON_2, 0); // 1 = High, 0 = Low
  esp_deep_sleep_start();
  // after wakeup
  esp_wifi_start();
}

void switchToHome();
void switchToAppsList();
void switchToApp();
bool connectToWifi();

void setup() {
  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);

  Serial.begin(9600);
  log(LOG_INFO, "Welcome to qlockOS!");

  refreshPreferences();
  log(LOG_SUCCESS, "Preferences loaded");

  tft.begin();
  log(LOG_SUCCESS, "TFT initiliazed");

  tft.setRotation(3);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);

  ledcSetup(0, 2000, 8);
  ledcAttachPin(PIN_LCD_BL, 0);
  ledcWrite(0, 0);

  btn1.setDebounceTicks(10);
  btn1.setClickTicks(150);
  btn1.setPressTicks(1000);

  btn2.setDebounceTicks(10);
  btn2.setClickTicks(150);
  btn2.setPressTicks(1000);

  btn1.attachClick([]() {
    sleepTimer = 0;
    switch (cState) {
    case Home:
      break;
    case AppsList:
      if (currentAppIndex != 0)
        currentAppIndex--;
      break;
    case InApp:
      apps[currentAppIndex]->buttonTopClick();
      break;
    }
  });

  btn1.attachDoubleClick([]() {
    sleepTimer = 0;
    switch (cState) {
    case Home:
      switchToAppsList();
      break;
    case AppsList:
      switchToApp();
      break;
    case InApp:
      apps[currentAppIndex]->exit();
      switchToAppsList();
      break;
    }
  });

  btn1.attachLongPressStart([]() {
    switch (cState) {
    case Home:
      enterSleep();
      break;
    case AppsList:
      break;
    case InApp:
      apps[currentAppIndex]->buttonTopLongPress();
      break;
    }
  });

  btn2.attachClick([]() {
    sleepTimer = 0;
    switch (cState) {
    case Home:
      break;
    case AppsList:
      if (currentAppIndex != apps.size() - 1)
        currentAppIndex++;
      break;
    case InApp:
      apps[currentAppIndex]->buttonBottomClick();
      break;
    }
  });

  btn2.attachDoubleClick([]() {
    sleepTimer = 0;
    switch (cState) {
    case Home:
      switchToAppsList();
      break;
    case AppsList:
      switchToHome();
      break;
    case InApp:
      apps[currentAppIndex]->exit();
      switchToHome();
      break;
    }
  });

  btn2.attachLongPressStart([]() {
    switch (cState) {
    case Home:
      break;
    case AppsList:
      break;
    case InApp:
      apps[currentAppIndex]->buttonBottomLongPress();
      break;
    }
  });
  log(LOG_SUCCESS, "Hardware buttons initiliazed");

  configTime(GMT_OFFSET_SEC, DAY_LIGHT_OFFSET_SEC, (const char *)nullptr);
  log(LOG_SUCCESS, "RTC time configured");

  timerSemaphore = xSemaphoreCreateBinary();
  uiTimer = timerBegin(0, 80, true);
  timerAttachInterrupt(uiTimer, &onTimer, true);
  timerAlarmWrite(uiTimer, 1000000, true);
  timerAlarmEnable(uiTimer);
  log(LOG_SUCCESS, "Hardware timer configured");

  batteryStatus = constrain(map((analogRead(PIN_BAT_VOLT) * 2 * 3.3 * 1000) / 4096, 3200, 3900, 0, 100), 0, 100);

  initApps();
  log(LOG_SUCCESS, "Apps initiliazed");
  initThemes();
  log(LOG_SUCCESS, "Themes initiliazed");

  WiFi.onEvent(WiFiConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);

  if (wifi_ssid != "") {
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid.c_str(), wifi_passwd.c_str());
    log(LOG_SUCCESS, "WiFi connection started");
  }

  themes[currentThemeIndex]->drawHomeUI(tft, rtc, batteryStatus);
  log(LOG_SUCCESS, "Home UI drawn");
  fadeScreen(2, false);
  log(LOG_SUCCESS, "Screen fade-in completed");

  log(LOG_SUCCESS, "qlockOS setup completed");
}

void loop() {
  btn1.tick();
  btn2.tick();

  // 1 second timer
  if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE) {
    if (cState != InApp)
      sleepTimer++;
    if (cState == Home) {
      batteryStatus = constrain(map((analogRead(PIN_BAT_VOLT) * 2 * 3.3 * 1000) / 4096, 3200, 3900, 0, 100), 0, 100);
      themes[currentThemeIndex]->drawHomeUI(tft, rtc, batteryStatus);
    }
  }

  if (sleepTimer == 30)
    enterSleep();

  switch (cState) {
  case Home:
    break;
  case AppsList:
    drawAppsListUI(tft);
    break;
  case InApp:
    apps[currentAppIndex]->drawUI(tft);
    break;
  }
}

void switchToHome() {
  refreshPreferences();
  fadeScreen(1, true);
  cState = Home;
  themes[currentThemeIndex]->drawHomeUI(tft, rtc, batteryStatus);
  fadeScreen(1, false);
}

void switchToAppsList() {
  refreshPreferences();
  fadeScreen(1, true);
  cState = AppsList;
  drawAppsListUI(tft);
  fadeScreen(1, false);
}

void switchToApp() {
  refreshPreferences();
  fadeScreen(1, true);
  cState = InApp;
  tft.fillScreen(TFT_BLACK);
  tft.pushImage(135, 60, 30, 30, (uint16_t *)apps[currentAppIndex]->icon);
  tft.setTextDatum(TC_DATUM);
  tft.loadFont(InterRegular16);
  tft.drawString(apps[currentAppIndex]->name.c_str(), 150, 100, 2);
  tft.unloadFont();
  fadeScreen(1, false);
  delay(250);
  apps[currentAppIndex]->setup();
  tft.fillScreen(TFT_BLACK);
  ledcWrite(0, 0);
  apps[currentAppIndex]->drawUI(tft);
  fadeScreen(1, false);
}
