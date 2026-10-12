#include "types.h"
#include "game/Unk_02080440.h"
#include "game/power_damage.h"

BOOL Unit_IsOnUrbanTerrain(const void* unit);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 gPowerEffectTaskDesc[];
extern char data_02168990[];
extern char data_02168994[];

/* Kindle's CO Power: 3 HP to enemy units on urban terrain. */
void UrbanBlight(void* parent) {
  StartPowerDamage(gPowerEffectTaskDesc, data_02168990, data_02168994,
                   data_027c027c->unk_48[data_027c0284]->currentPlayer, 30, 0,
                   WEATHER_KEEP, FALSE, Unit_IsOnUrbanTerrain, parent);
}
