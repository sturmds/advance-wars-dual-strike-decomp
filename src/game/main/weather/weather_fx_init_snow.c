#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/weather.h"

extern char gWeatherSnowFileName[]; /* "0b0" */
/* 16 rows of three: vx, vy and sprite frame (see WeatherFx_InitSand). */
extern u16 gWeatherParticleSpeeds[];

u32 File_UnpackTo(void* dest, const char* path);
u32 Rng_Next(void);

#define FX (data_027c027c->weatherFx)

/* Loads the snow sprites into OBJ VRAM and puts every flake at a random
 * place, with the speed from gWeatherParticleSpeeds as it is. */
void WeatherFx_InitSnow(void) {
  s16 i;
  int row;

  File_UnpackTo(data_027c0280->objVram, gWeatherSnowFileName);
  for (i = 0; i < WEATHER_PARTICLE_COUNT; i++) {
    FX->particles[i].x = (Rng_Next() % 255) << 8;
    FX->particles[i].y = (Rng_Next() % 255) << 8;
    row = (i % 16) * 3;
    FX->particles[i].vx = gWeatherParticleSpeeds[row];
    FX->particles[i].vy = gWeatherParticleSpeeds[row + 1];
    FX->particles[i].frame = gWeatherParticleSpeeds[row + 2];
  }
}
