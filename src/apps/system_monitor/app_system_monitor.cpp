#include "app_system_monitor.h"
#include "pin_config.h"
#include "resources/fonts/InterRegular16.h"
#include "resources/icons.h"

#include "WiFi.h"

void AppSystemMonitor::setup() {
  updateTimer = 0;

  mcuTemp = (temperatureRead() - 32) / 1.8;
  freeHeap = esp_get_free_heap_size();
}

void AppSystemMonitor::drawUI(TFT_eSPI tft) {
  updateTimer++;
  if (updateTimer % 300 == 0) {
    mcuTemp = (temperatureRead() - 32) / 1.8;
    freeHeap = esp_get_free_heap_size();
  }

  TFT_eSprite s = TFT_eSprite(&tft);
  s.createSprite(tft.getViewportWidth(), tft.getViewportHeight());
  s.setSwapBytes(tft.getSwapBytes());
  s.fillSprite(TFT_BLACK);

  s.loadFont(InterRegular16);

  s.setTextDatum(TL_DATUM);
  s.drawString("MCU Temp", 10, 10, 2);
  s.drawString("Free Memory", 10, 60, 2);

  s.setTextDatum(TR_DATUM);
  s.drawString(String(mcuTemp) + " C", tft.getViewportWidth() - 10, 10, 2);
  s.drawString(String(freeHeap) + " Bytes", tft.getViewportWidth() - 10, 60, 2);

  s.pushSprite(0, 0);
  s.deleteSprite();

  delay(100);
}

std::unique_ptr<AppSystemMonitor> appSystemMonitor(new AppSystemMonitor("System Monitor", icon_system_monitor.pixel_data, 30, 30));