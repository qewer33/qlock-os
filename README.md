![banner](https://github.com/qewer33/qlock-os/blob/main/assets/Banner.png?raw=true)

qlock OS is a minimal but complete smartwatch firmware for the [LILYGO T-Display-S3](https://www.lilygo.cc/products/t-display-s3) development board. The entire system is navigatable via the two included buttons on the board. This repository and README also contains 3D printable STL and STEP files along with instructions to turn your T-Display-S3 board into a smartwatch.

## The System

qlock OS utilizes the esp32-arduino framework and the TFT_eSPI library for the user interface. The recommended way of development is using the PlatformIO VSCode extension.

qlock OS consists of themes and apps. Themes define the watchface (also called the home) along with the system colors. Apps are just applications, interactive little programs.

### Themes

Themes live under the `src/themes` directory. Each theme has it's own subdirectory and consists of 3 (or 2, one of them is optional) files.

```sh
src/
    themes/
        themename/
            theme_themename.cpp
            theme_themename.h
            # optional
            theme_themename_resources.h
```

The `theme_themename.h` file is the header file of the theme. It contains the class and the extern instance definition of the theme. Themes should extend the `Theme` class defined in `src/themes.h`. The instance should be an instance of the newly defined theme class wrapped in an `std::unique_pointer`.

The `theme_themename.cpp` file is the source file of the theme. The source file should define the instance and implement the `drawHomeUI()` function. The theme constructor has 3 arguments: `String name` is the name of the theme and the rest are colors in rgb565 format. The `drawHomeUI()` function takes 3 arguments: `TFT_eSPI tft` for drawing on the display, `ESP32Time rtc` for accessing the rtc time and `uint32_t batteryStatus` for acessing the battery status value.

The `theme_themename_resources.h` file contains the custom resources that are used by the theme. These resources can be fonts, icons etc. This file is not necessary if the theme doesn't have any custom resources.

The finished theme should be included in `src/themes.cpp` and should be added to the `themes` array in the `initThemes()` function.

### Apps

Apps live under the `src/apps` directory. Each app has it's own subdirectory and consists of 3 (or 2, one of them is optional) files.

```sh
src/
    apps/
        appname/
            app_appname.cpp
            app_appname.h
            # optional
            app_appname.h
```

## The Product

To assemble the complete smartwatch, you need 4 things: The non-touch version of the T-Display-S3, a suitable 1S 3.7V lipo battery (shouldn't exceed the size of the board and needs to be less than 5mm in thickness), 3D printed case and a 24mm spring-loaded watch strap.
