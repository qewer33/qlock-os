#include "app_racer.h"
#include "app_racer_res.h"
#include "os_config.h"
#include "resources/icons.h"

const unsigned char *tree_images[] = {tree1.pixel_data, tree2.pixel_data, tree3.pixel_data, tree4.pixel_data, tree5.pixel_data, tree6.pixel_data};

void AppRacer::setup() {
  gameOver = true;
  score = 0;
  carLane = 0;
  gameSpeed = 2;
  carSpawnSkip = false;
  cars.clear();
  trees.clear();

  preferences.begin(PREFS_KEY);
  highscore = preferences.getUInt("racer_hiscore", 0);
  preferences.end();
}

void AppRacer::drawUI(TFT_eSPI tft) {
  TFT_eSprite s = TFT_eSprite(&tft);
  s.createSprite(tft.getViewportWidth(), tft.getViewportHeight());
  s.setSwapBytes(tft.getSwapBytes());

  s.fillSprite(s.color565(37, 37, 37));

  s.fillRect(0, 0, 320, 25, s.color565(207, 26, 53));
  s.setTextColor(TFT_WHITE);
  s.drawString("Racer", 5, 5, 2);
  s.setTextDatum(TR_DATUM);
  s.drawString(String("Score: " + String(score) + "  HI: " + String(highscore)).c_str(), 315, 5, 2);

  if (!gameOver) {
    score++;

    if (score % (40 / gameSpeed) == 0) {
      trees.emplace_back(RacerTreeObject{320, (uint32_t)random(6)});
    }

    if (score % (200 / gameSpeed) == 0) {
      if (!carSpawnSkip)
        cars.emplace_back(RacerCarObject{320, (uint32_t)random(2)});
      carSpawnSkip = false;
    }

    if (score % 500 == 0) {
      gameSpeed++;
      carSpawnSkip = true;
    }

    s.pushImage(0, 25, racer_bg.width, racer_bg.height, (uint16_t *)racer_bg.pixel_data);

    for (int i = 0; i < trees.size(); i++) {
      if (!gameOver)
        trees[i].x -= gameSpeed * 2;
      if (trees[i].x < -40)
        trees.erase(trees.begin());

      s.pushImage(trees[i].x, 25, 40, 40, (uint16_t *)tree_images[trees[i].type]);
    }

    for (int i = 0; i < cars.size(); i++) {
      if (!gameOver)
        cars[i].x -= gameSpeed;
      if (cars[i].x < -60)
        cars.erase(cars.begin());

      if (carLane == cars[i].lane && cars[i].x < racecar.width + 10 && cars[i].x > -50)
        gameOver = true;

      s.pushImage(cars[i].x, 80 + 42 * cars[i].lane, blue_car.width, blue_car.height, (uint16_t *)blue_car.pixel_data);
    }

    s.pushImage(10, 80 + 42 * carLane, racecar.width, racecar.height, (uint16_t *)racecar.pixel_data);
  } else if (score == 0) {
    s.pushImage(64, 64, racer_banner.width, racer_banner.height, (uint16_t *)racer_banner.pixel_data);
  } else {
    s.pushImage(60, 64, racer_crash_banner.width, racer_crash_banner.height, (uint16_t *)racer_crash_banner.pixel_data);
  }

  s.pushSprite(0, 0);
  s.deleteSprite();
}

void AppRacer::buttonTopClick() {
  if (gameOver) {
    setup();
    gameOver = false;
  } else
    carLane = 0;
}

void AppRacer::buttonBottomClick() {
  if (gameOver) {
    setup();
    gameOver = false;
  } else
    carLane = 1;
}

void AppRacer::exit() {
  if (score > highscore) {
    preferences.begin(PREFS_KEY);
    preferences.putUInt("racer_hiscore", score);
    preferences.end();
  }
}

std::unique_ptr<AppRacer> appRacer(new AppRacer("Racer", icon_racer.pixel_data, 30, 30));