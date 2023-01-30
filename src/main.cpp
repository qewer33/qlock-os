#include "Arduino.h"
#include "ESP32Time.h"
#include "OneButton.h"
#include "Preferences.h"
#include "TFT_eSPI.h"
#include "WiFi.h"
#include "esp_wifi.h"

#include "apps.h"
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
  NetworkSync,
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
  Serial.println("Welcome to Qlock-OS!");

  refreshPreferences();

  tft.begin();

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
    case NetworkSync:
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
    case NetworkSync:
      break;
    }
  });

  btn1.attachLongPressStart(enterSleep);

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
    case NetworkSync:
      break;
    }
  });

  btn2.attachDoubleClick([]() {
    sleepTimer = 0;
    switch (cState) {
    case Home:
      currentAppIndex = 0;
      switchToAppsList();
      break;
    case AppsList:
      switchToHome();
      break;
    case InApp:
      apps[currentAppIndex]->exit();
      switchToHome();
      break;
    case NetworkSync:
      break;
    }
  });

  btn2.attachLongPressStart([]() { cState = NetworkSync; });

  timerSemaphore = xSemaphoreCreateBinary();
  uiTimer = timerBegin(0, 80, true);
  timerAttachInterrupt(uiTimer, &onTimer, true);
  timerAlarmWrite(uiTimer, 1000000, true);
  timerAlarmEnable(uiTimer);

  initApps();
  initThemes();

  if (wifi_ssid != "") {
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(wifi_ssid.c_str(), wifi_passwd.c_str());
    delay(250);
    configTime(GMT_OFFSET_SEC, DAY_LIGHT_OFFSET_SEC, NTP_SERVER1);
  }

  themes[currentThemeIndex]->drawHomeUI(tft, rtc, batteryStatus);
  fadeScreen(2, false);
}

void loop() {
  btn1.tick();
  btn2.tick();

  if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE) {
    if (cState != InApp)
      sleepTimer++;
    batteryStatus = constrain(map((analogRead(PIN_BAT_VOLT) * 2 * 3.3 * 1000) / 4096, 3200, 4200, 0, 100), 0, 100);
    if (cState == Home)
      themes[currentThemeIndex]->drawHomeUI(tft, rtc, batteryStatus);
  }

  if (sleepTimer == 60)
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
  case NetworkSync:
    tft.fillScreen(TFT_BLACK);
    connectToWifi();
    delay(1000);
    cState = Home;
    tft.fillScreen(TFT_BLACK);
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
  delay(500);
  tft.fillScreen(TFT_BLACK);
  apps[currentAppIndex]->setup();
  apps[currentAppIndex]->drawUI(tft);
}

bool connectToWifi() {
  tft.setTextColor(TFT_WHITE, TFT_BLACK, true);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Attempting to connect...", 160, 10, 2);

  WiFi.disconnect();
  WiFi.mode(WIFI_AP_STA);
  WiFi.beginSmartConfig();

  String text;
  int t = 0;

  while (t < 400) {
    t++;
    delay(100);
    if (WiFi.smartConfigDone()) {
      tft.drawString("Successfully connected", 160, 25, 2);
      tft.drawString(String("SSID: " + WiFi.SSID()).c_str(), 160, 40, 2);
      tft.drawString(String("PSWD: " + WiFi.psk()).c_str(), 160, 55, 2);
      preferences.begin(PREFS_KEY);
      preferences.putString("wifi_ssid", WiFi.SSID());
      preferences.putString("wifi_passwd", WiFi.psk());
      preferences.end();
      break;
    }
  }

  if (t >= 400) {
    WiFi.disconnect();
    tft.drawString("Connection attempt timed out", 160, 25, 2);
    return false;
  }
  return true;
}
