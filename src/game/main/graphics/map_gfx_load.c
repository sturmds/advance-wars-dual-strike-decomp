#include "types.h"
#include "game/Unk_02080440.h"

void func_020f8d40(int weather, int tileset);
void WeatherFx_LoadGraphics(void);
void func_020d2794(void);
void WeatherFx_InitSnow(void);
void WeatherFx_InitRain(void);
void WeatherFx_InitSand(void);

void MapGfx_Load(int weather, int tilesetByte)
{
    func_020f8d40(weather, tilesetByte & 0x7f);
    WeatherFx_LoadGraphics();
    switch (weather) {
    case WEATHER_CLEAR:
        func_020d2794();
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
