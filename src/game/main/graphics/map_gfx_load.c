#include "types.h"
#include "game/Unk_02080440.h"

void MapGfx_LoadTextures(int weather, int tileset);
void WeatherFx_LoadGraphics(void);
void WeatherFx_InitClouds(void);
void WeatherFx_InitSnow(void);
void WeatherFx_InitRain(void);
void WeatherFx_InitSand(void);

void MapGfx_Load(int weather, int tilesetByte) {
  MapGfx_LoadTextures(weather, tilesetByte & 0x7f);
  WeatherFx_LoadGraphics();
  switch (weather) {
    case WEATHER_CLEAR:
      WeatherFx_InitClouds();
      break;
    case WEATHER_SNOW:
      WeatherFx_InitSnow();
      break;
    case WEATHER_RAIN:
      WeatherFx_InitRain();
      break;
    case WEATHER_SAND:
      WeatherFx_InitSand();
      break;
  }
}
