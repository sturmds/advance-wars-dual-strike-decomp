#include "types.h"
#include "game/Unk_02080440.h"
#include "game/weather.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u16 gWeatherRainSprites[][4];

int func_01ff9f44(void);
void Oam_PutObjList(int x, int y, const u16* sprite, int tileAdd);

#define FX (data_027c027c->weatherFx)
#define VIEW (data_027c027c->unk_48[data_027c0284]->unk_1c4)

/* Moves every rain drop, then draws half of them: the first 16 or the last
 * 16, as func_01ff9f44 returns an even or odd value. Each drop has its own
 * sprite frame. */
void WeatherFx_UpdateRain(void) {
  s16 i;
  s16 start;

  for (i = 0; i < WEATHER_PARTICLE_COUNT; i++) {
    FX->particles[i].x += FX->particles[i].vx;
    FX->particles[i].y += FX->particles[i].vy;
  }

  start = (func_01ff9f44() & 1) * 16;
  for (i = start; i < start + 16; i++) {
    Oam_PutObjList(((FX->particles[i].x >> 8) - VIEW->scrollX) & 0xff,
                   ((FX->particles[i].y >> 8) - VIEW->scrollY) & 0xff,
                   gWeatherRainSprites[FX->particles[i].frame], 0);
  }
}
