#ifndef GAME_WEATHER_H
#define GAME_WEATHER_H

#include "types.h"

/* Weather values as CO powers pass them. Weather_Set takes them, turns
 * WEATHER_POWER_SNOW into 1 (snow) and picks a longer duration for it. */
#define WEATHER_CLEAR 0
#define WEATHER_SNOW 1
#define WEATHER_RAIN 2
#define WEATHER_SAND 3
#define WEATHER_POWER_SNOW 0xfe
#define WEATHER_KEEP (-1) /* StartPowerDamage only: no change; stored as 0xff */

/* Work block of the task that draws the weather (started from
 * data_02166d18 by Weather_Set). */
typedef struct WeatherTaskWork {
  u8 pad_00[0x20];
  s16 weather; /* WEATHER_* the task shows */
} WeatherTaskWork;

/* One record of gWeatherInfo, per WEATHER_* value (0 to 3). */
typedef struct WeatherInfo {
  u32 sound; /* Sound_PlaySe id played when the weather starts */
  const char* files
      [4]; /* pointers to short name strings such as "006"; no reader found yet */
} WeatherInfo;

/* One rain drop, snow flake or grain of sand. Positions and speeds are in
 * 1/256 pixels on the whole map; the drawing wraps them to the screen. */
typedef struct WeatherParticle {
  u16 x;
  u16 y;
  u16 vx;
  u16 vy;
  u16 frame; /* sprite frame for rain and snow */
  u8 pad_0a[0x0c - 0x0a];
} WeatherParticle;

#define WEATHER_PARTICLE_COUNT 32

/* A cloud shadow drawn over the map in clear weather. Position and speed
 * in 1/256 pixels. */
typedef struct WeatherCloud {
  u32 x;
  u32 y;
  u32 vx;
  u32 vy;
  u16 frame; /* counts up each update */
  u8 pad_12[0x14 - 0x12];
} WeatherCloud;

#define WEATHER_CLOUD_COUNT 4

/* Pointed to by Unk_02080440.weatherFx. */
typedef struct WeatherFx {
  WeatherParticle particles[WEATHER_PARTICLE_COUNT];
  WeatherCloud clouds[WEATHER_CLOUD_COUNT];
} WeatherFx;

#endif
