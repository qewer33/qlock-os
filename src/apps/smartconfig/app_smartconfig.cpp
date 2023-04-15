#include "app_smartconfig.h"
#include "os_config.h"
#include "resources/icons.h"
#include "resources/fonts/InterRegular16.h"
#include "resources/fonts/InterRegular24.h"

#include "WiFi.h"

void AppSmartconfig::setup() { done = false; }

void AppSmartconfig::drawUI(TFT_eSPI tft) {
  if (!done) {
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

      String wifi_ssid = preferences.getString("wifi_ssid", "");
      String wifi_passwd = preferences.getString("wifi_passwd", "");
      if (wifi_ssid != "") {
        WiFi.mode(WIFI_AP_STA);
        WiFi.begin(wifi_ssid.c_str(), wifi_passwd.c_str());
      }
    }

    done = true;
  }
}

std::unique_ptr<AppSmartconfig> appSmartconfig(new AppSmartconfig("Connect to WiFi", icon_wifi.pixel_data, 30, 30));