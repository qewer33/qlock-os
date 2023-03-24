![banner](https://github.com/qewer33/qlock-os/blob/main/assets/Banner.png?raw=true)

**:warning: qlockOS IS NOT COMPLETE YET! Some of the information here might be inaccurate.**

qlockOS is a minimal but complete smartwatch firmware for the [LILYGO T-Display-S3](https://www.lilygo.cc/products/t-display-s3) development board. The entire system is navigatable via the two included buttons on the board. This repository and README also contains 3D printable STL and STEP files along with instructions to turn your T-Display-S3 board into a smartwatch.

# The Product

To assemble the complete smartwatch, you need 4 things: The non-touch version of the T-Display-S3, a suitable 1S 3.7V lipo battery (shouldn't exceed the size of the board and needs to be less than 5mm in thickness), 3D printed case and a 24mm spring-loaded watch strap.

You can find the 3D case files under `3d/`.

Below are AliExpress links to the board and the watch strap:

-   Board: https://tr.aliexpress.com/item/1005004496543314.html
-   Strap (choose the 24mm option): https://tr.aliexpress.com/item/1005003974779477.html

The recommended battery is the Power-Xtra PX502030 3.7V 250 mAh Li-Polymer battery.

The complete smartwatch dimensions are 63mm x 40mm x 1.5mm. It doesn't have many hardware features such as sensors but there still is some space left inside the casing after placing the battery, so 1 or 2 sensor modules could technically be wired up and used.

![image_full](https://github.com/qewer33/qlock-os/blob/main/assets/image_full.jpg?raw=true)

# The System

qlockOS utilizes the esp32-arduino framework and the TFT_eSPI library for the user interface. The recommended way of development is using the PlatformIO VSCode extension.

qlockOS consists of themes and apps. Themes define the watchface (also called the home) along with the system colors. Apps are just applications, interactive little programs.

The default "Clear Blue" watchface:
![image_watchface](https://github.com/qewer33/qlock-os/blob/main/assets/image_watchface.jpg?raw=true)

Apps menu:
![image_apps](https://github.com/qewer33/qlock-os/blob/main/assets/image_apps.jpg?raw=true)

## Themes

Themes live under the `src/themes` directory. Each theme has it's own subdirectory and consists of 3 (or 2, one of them is optional) files.

```sh
src/
    themes/
        themename/
            theme_themename.cpp
            theme_themename.h
            # optional
            theme_themename_res.h
```

The `theme_themename.h` file is the header file of the theme. It contains the class and the extern instance definition of the theme. Themes should extend the `Theme` class defined in `src/themes.h`. The instance should be an instance of the newly defined theme class wrapped in an `std::unique_pointer`.

The `theme_themename.cpp` file is the source file of the theme. The source file should define the instance and implement the `drawHomeUI()` function. The theme constructor takes 5 arguments: the first argument `String name` is the name of the theme and the rest are colors in rgb565 format. The `drawHomeUI()` function takes 3 arguments: `TFT_eSPI tft` for drawing on the display, `ESP32Time rtc` for accessing the rtc time and `uint32_t batteryStatus` for acessing the battery status value.

The `theme_themename_reso.h` file contains the custom resources that are used by the theme. These resources can be fonts, icons etc. This file is not necessary if the theme doesn't have any custom resources.

The finished theme should be included in `src/themes.cpp` and should be added to the `themes` array in the `initThemes()` function.

## Apps

Apps live under the `src/apps` directory. Each app has it's own subdirectory and consists of 3 (or 2, one of them is optional) files.

```sh
src/
    apps/
        appname/
            app_appname.cpp
            app_appname.h
            # optional
            app_appname_res.h
```

The `app_appname.h` file is the header file of the app. It contains the class and the extern instance definition of the app. Apps should extend the `App` class defined in `src/apps.h`. The methods that are going to be used by the app should be overriden methods from the `App`class. The instance should be an instance of the newly defined app class wrapped in an `std::unique_pointer`. There are 7 methods that apps can override from the `App` class:

-   `setup()`: Runs before the app gets started. Useful for initializing variable defaults or loading preferences.
-   `drawUI(TFT_eSPI tft)`: Runs every frame when the app is running. Thismethod should draw the user interface of the app using `tft`.
-   `exit()`: Runs when the app gets exited. Useful for saving preferences.
-   `buttonTopClick()`: Runs when the top button gets clicked while in the app.
-   `buttonTopLongPress()`: Runs when the top button gets long pressed while in the app.
-   `buttonBottomClick()`: Runs when the bottom button gets clicked while in the app.
-   `buttonBottomLongPress()`: Runs when the bottom button gets long pressed while in the app.

The `app_appname.cpp` file is the source file of the app. The source file should define the instance and implement the necessary app methods. The app constructor takes 2 argumnets: the first argument `String name` is the name and the second argument `uint16_t* icon` is the icon resource of the app.

The `app_appname_res.h` file contains the custom resources that are used by the app. These resources can be fonts, icons etc. This file is not necessary if the app doesn't have any custom resources. The app icon should go in `src/resources/icons.h`, not the app resource file.

The finished app should be included in `src/apps.cpp` and should be added to the `apps` array in the `initApps()` function.

## Navigation

The system navigation needs some time to get used to but it's pretty intuitive once you get the hang of it. The reason of it's complexity is the T-Display-S3 only including 2 onboard buttons, so the entire system is navigatable via those two buttons only. qlockOS takes advantage of single clicks, double clicks and long presses along with conditional action switching to make these two buttons do more than they were intended to.

Below is a diagram of all the qlock OS related actions:

```sh
Home:
    Single clicks: N/A
    Double clicks: Switch to the apps menu
    Long presses:
        Top button: Enter sleep mode
        Bottom button: N/A

App Selector:
    Single clicks: Navigate up and down
    Double clicks:
        Top button: Open the selected app
        Bottom button: Switch to home
    Long presses: N/A

In App:
    Single clicks: App defined actions
    Double clicks:
        Top button: Exit the app and switch to the apps menu
        Bottom button: Exit the app and switch to home
    Long pressses: App defined actions

    # App defined actions

    Stopwatch:
        Single clicks:
            Top button: Start/Pause
            Bottom button: Reset

    Flashlight:
        Single clicks: Increase/Decrease brightness

    Racer:
        Single clicks: Move the car up or down

    Dino:
        Single clicks:
            Top button: Jump
            Bottom button: Toggle Sneak

    Themes:
        Single clicks: Change theme

```
