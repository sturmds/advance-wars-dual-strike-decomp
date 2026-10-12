#include "types.h"
#include "game/Unk_02080440.h"
#include "game/power_damage.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 gPowerEffectTaskDesc[];
extern char data_021689a0[];
extern char data_0216899c[];

/* Drake's CO Super Power: 2 HP to every enemy unit, enemy fuel halved, rain. */
void Typhoon(void* parent) {
  StartPowerDamage(gPowerEffectTaskDesc, data_021689a0, data_0216899c,
                   data_027c027c->unk_48[data_027c0284]->currentPlayer, 20, 0,
                   WEATHER_RAIN, TRUE, NULL, parent);
}
