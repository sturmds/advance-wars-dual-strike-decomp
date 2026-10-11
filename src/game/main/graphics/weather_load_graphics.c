#include "types.h"
#include "game/Unk_02080440.h"

void Weather_LoadTileGraphics(int weather, int variant, int size);
void MapGfx_LoadTilesetPalettes(int weather, int variant, int size);
void WeatherFx_InitClouds(void);
void WeatherFx_InitSnow(void);
void WeatherFx_InitRain(void);
void WeatherFx_InitSand(void);

void Weather_LoadGraphics(int weather, int tilesetByte)
{
    int tileset = tilesetByte & 0x7f;

    Weather_LoadTileGraphics(weather, tileset, 0x94);
    MapGfx_LoadTilesetPalettes(weather, tileset, 8);
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
