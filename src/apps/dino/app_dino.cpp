#include "app_dino.h"
#include "app_dino_res.h"
#include "resources.h"

void AppDino::setup() {
  gameOver = false;
  score = 0;
  gameSpeed = 6;
  groundX = 0;
  playerYVelo = 0;
  playerBaseY = 170 - dino_ground.height - dino_player_run_1.height + 20;
  playerY = playerBaseY;
  playerAnimState = 0;
  playerState = RUN;
}

void AppDino::drawUI(TFT_eSPI tft) {
  TFT_eSprite s = TFT_eSprite(&tft);
  s.createSprite(tft.getViewportWidth(), tft.getViewportHeight());
  s.setSwapBytes(tft.getSwapBytes());

  s.fillSprite(TFT_WHITE);

  if (!gameOver) {
    score++;

    if (score % 500 == 0)
      gameSpeed++;

    // draw ground
    groundX -= gameSpeed;
    s.pushImage(groundX, tft.height() - dino_ground.height, dino_ground.width, dino_ground.height, (uint16_t *)dino_ground.pixel_data);
    s.pushImage(groundX + dino_ground.width, tft.height() - dino_ground.height, dino_ground.width, dino_ground.height,
                (uint16_t *)dino_ground.pixel_data);

    if (groundX < -dino_ground.width)
      groundX = 0;

    // spawn and draw obstacles
    if (score % (40 / gameSpeed) == 0) {
      trees.emplace_back(DinoTreeObject{320, (uint32_t)random(4), (bool)random(1)});
    }

    for (int i = 0; i < trees.size(); i++) {
    }

    // player jump
    playerY += playerYVelo;

    if (playerY < playerBaseY) {
      playerYVelo += 1;
    } else {
      playerYVelo = 0;
      playerY = playerBaseY;
      if (playerState == JUMP)
        playerState = RUN;
    }

    // draw player
    if (score % 2 == 0) {
      if (playerAnimState == 0)
        playerAnimState = 1;
      else if (playerAnimState == 1)
        playerAnimState = 0;
    }

    TFT_eSprite player = TFT_eSprite(&tft);
    player.createSprite(dino_player_sneak_1.width, dino_player_run_1.height);
    player.setSwapBytes(false);
    switch (playerState) {
    case RUN:
      if (playerAnimState == 0)
        player.pushImage(0, 0, dino_player_run_1.width, dino_player_run_1.height, (uint16_t *)dino_player_run_1.pixel_data);
      else
        player.pushImage(0, 0, dino_player_run_2.width, dino_player_run_2.height, (uint16_t *)dino_player_run_2.pixel_data);
      break;
    case JUMP:
      player.pushImage(0, 0, dino_player_jump.width, dino_player_jump.height, (uint16_t *)dino_player_jump.pixel_data);
      break;
    case SNEAK:
      if (playerAnimState == 0)
        player.pushImage(0, 0, dino_player_sneak_1.width, dino_player_sneak_1.height, (uint16_t *)dino_player_sneak_1.pixel_data);
      else
        player.pushImage(0, 0, dino_player_sneak_2.width, dino_player_sneak_2.height, (uint16_t *)dino_player_sneak_2.pixel_data);
      break;
    }
    player.pushToSprite(&s, 15, playerY, TFT_BLACK);

  } else if (score == 0) {
  } else {
  }

  s.pushSprite(0, 0);
  s.deleteSprite();
}

void AppDino::buttonTopClick() {
  if (playerState == RUN) {
    playerYVelo = -15;
    playerState = JUMP;
  }
}

void AppDino::buttonBottomClick() {
  if (playerState == RUN)
    playerState = SNEAK;
  else if (playerState == SNEAK)
    playerState = RUN;
}

std::unique_ptr<AppDino> appDino(new AppDino("Dino", icon_dino.pixel_data, 30, 30));
