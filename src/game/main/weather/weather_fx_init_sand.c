#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/weather.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern char gWeatherSandFileName[]; /* "0b2" */
/* 16 rows of three: vx, vy and sprite frame. A flat array: a [16][3] table
 * does not match. */
extern u16 gWeatherParticleSpeeds[];

u32 File_UnpackTo(void* dest, const char* path);
u32 Rng_Next(void);

#define FX (data_027c027c->weatherFx)

/* Loads the sand sprites into OBJ VRAM and puts every particle at a random
 * place, with a speed from gWeatherParticleSpeeds; sand moves sideways 20
 * times faster than the table says. */
void WeatherFx_InitSand(void) {
  int i;
  int row;

  File_UnpackTo(data_027c0280->objVram, gWeatherSandFileName);
  for (i = 0; i < WEATHER_PARTICLE_COUNT; i++) {
    FX->particles[i].x = (Rng_Next() % 255) << 8;
    FX->particles[i].y = (Rng_Next() % 255) << 8;
    row = (i % 16) * 3;
    FX->particles[i].vx = gWeatherParticleSpeeds[row] * 20;
    FX->particles[i].vy = gWeatherParticleSpeeds[row + 1];
    FX->particles[i].frame = gWeatherParticleSpeeds[row + 2];
  }
}
