#pragma once

#define DEVICE_NAME          "qewer33's Watch"
#define PREFS_KEY            "qlock-os"

// Device Pins
#define PIN_LCD_BL           38

#define PIN_LCD_D0           39
#define PIN_LCD_D1           40
#define PIN_LCD_D2           41
#define PIN_LCD_D3           42
#define PIN_LCD_D4           45
#define PIN_LCD_D5           46
#define PIN_LCD_D6           47
#define PIN_LCD_D7           48

#define PIN_POWER_ON         15

#define PIN_LCD_RES          5
#define PIN_LCD_CS           6
#define PIN_LCD_DC           7
#define PIN_LCD_WR           8
#define PIN_LCD_RD           9

#define PIN_BUTTON_1         0
#define PIN_BUTTON_2         14
#define PIN_BAT_VOLT         4

#define PIN_IIC_SCL          17
#define PIN_IIC_SDA          18

#define PIN_TOUCH_INT        16
#define PIN_TOUCH_RES        21

// Time Configuration
#define NTP_SERVER1          "pool.ntp.org"
#define NTP_SERVER2          "time.nist.gov"
#define GMT_OFFSET_SEC       (3600 * 2)
#define DAY_LIGHT_OFFSET_SEC 0