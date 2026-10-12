#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/map_tileset_files.h"
#include "game/weather.h"

void FS_ChangeDir(const char* dir);
void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int fileOffset,
                          int paletteOffset, int size);
const char* MapGfx_GetWeatherPaletteFile(int weather, int tileset);
void Weather_LoadTileGraphics(int weather, int tileset, int tileOffset);
void MapGfx_LoadTilesetPalettes(int weather, int tileset, int arg);
void WeatherFx_InitClouds(void);
void WeatherFx_InitSnow(void);
void WeatherFx_InitRain(void);
void WeatherFx_InitSand(void);

extern Unk_027c0280* data_027c0280;
extern char data_0216a1a0[]; /* "/bmap" */

/* Loads the tiles and palette of a tileset, the weather's tile graphics, and
 * starts the weather effect. */
void MapGfx_LoadTilesetAndWeather(int weather, int tilesetByte) {
  int tileset = tilesetByte & 0x7f;

  FS_ChangeDir(data_0216a1a0);
  File_UnpackTo(data_027c0280->unk_120, data_02167e30[tileset].tiles);
  Palette_LoadFromFile(MapGfx_GetWeatherPaletteFile(weather, tileset), 0, 0x40,
                       0xc0);
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
