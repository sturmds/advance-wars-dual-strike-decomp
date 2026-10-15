#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/weather.h"
#include "sdk/fs.h"

extern char gWeatherCloudDirName[];  /* "/bmap" */
extern char gWeatherCloudFileName[]; /* "0b3" */
/* Four rows of three: vx, vy and a third word (0) that is not read here. */
extern u32 gWeatherCloudSpeeds[];

u32 File_UnpackTo(void* dest, const char* path);
u32 Rng_Next(void);

#define FX (data_027c027c->weatherFx)
#define VIEW (data_027c027c->unk_48[data_027c0284]->unk_1c4)

/* Loads the cloud shadow sprites into OBJ VRAM, puts the four clouds at
 * random places with their speeds from gWeatherCloudSpeeds and a random
 * starting frame, and clears the particle frames. Positions wrap at 512
 * pixels, or 1024 on maps 32 tiles or more wide or tall (the update uses
 * more than 32). */
void WeatherFx_InitClouds(void) {
  int i;
  u32 maskX;
  u32 maskY;

  FS_ChangeDir(gWeatherCloudDirName);
  File_UnpackTo(data_027c0280->objVram, gWeatherCloudFileName);
  maskX = VIEW->mapWidth >= 32 ? 0x3ff : 0x1ff;
  maskY = VIEW->mapHeight >= 32 ? 0x3ff : 0x1ff;
  for (i = 0; i < WEATHER_CLOUD_COUNT; i++) {
    FX->clouds[i].x = (Rng_Next() & maskX) << 8;
    FX->clouds[i].y = (Rng_Next() & maskY) << 8;
    FX->clouds[i].vx = gWeatherCloudSpeeds[i * 3];
    FX->clouds[i].vy = gWeatherCloudSpeeds[i * 3 + 1];
    FX->clouds[i].frame = Rng_Next() % 255;
  }
  for (i = 0; i < WEATHER_PARTICLE_COUNT; i++) {
    FX->particles[i].frame = 0;
  }
}
