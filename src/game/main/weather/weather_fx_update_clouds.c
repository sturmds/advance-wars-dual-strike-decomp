#include "types.h"
#include "game/Unk_02080440.h"
#include "game/weather.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 gWeatherCloudAnim[];

BOOL func_020fa6f8(void);
void SpriteFile_Queue(void* data, int frame, int b, int x, int y, int tile,
                      int c, int d);

#define FX (data_027c027c->weatherFx)
#define VIEW (data_027c027c->unk_48[data_027c0284]->unk_1c4)

/* In clear weather, moves the four cloud shadows and draws those on screen.
 * Their positions wrap at 512 pixels, or 1024 on maps wider or taller than
 * 32 tiles. Skipped when the low 7 bits of the tileset byte are set or the
 * game state's func_020fa6f8 callback says so. "Clouds" is a guess from the
 * clear-weather check. */
void WeatherFx_UpdateClouds(void) {
  int i;
  u32 maskX;
  u32 maskY;
  int x;
  int y;

  if (VIEW->weather != WEATHER_CLEAR) {
    return;
  }
  if (VIEW->tileset & 0x7f) {
    return;
  }
  if (func_020fa6f8()) {
    return;
  }

  maskX = VIEW->mapWidth > 32 ? 0x3ff : 0x1ff;
  maskY = VIEW->mapHeight > 32 ? 0x3ff : 0x1ff;

  for (i = 0; i < WEATHER_CLOUD_COUNT; i++) {
    FX->clouds[i].x += FX->clouds[i].vx;
    FX->clouds[i].y += FX->clouds[i].vy;
  }

  for (i = 0; i < WEATHER_CLOUD_COUNT; i++) {
    FX->clouds[i].frame++;
    x = ((FX->clouds[i].x >> 8) & maskX) - VIEW->scrollX;
    y = ((FX->clouds[i].y >> 8) & maskY) - VIEW->scrollY;
    if (x >= 0 && x <= 255 && y >= 0 && y <= 255) {
      SpriteFile_Queue(gWeatherCloudAnim, 0, FX->clouds[i].frame, x & 0x1ff,
                       y & 0xff, 0x1800, 0, 1);
    }
  }
}
