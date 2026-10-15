#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/power_damage.h"

extern TaskScriptCmd gPowerEffectTaskDesc[];
extern char data_021689a4[];
extern char data_0216899c[];

/* Olaf's CO Super Power: 2 HP to every enemy unit, snow. */
void WinterFury(void* parent) {
  StartPowerDamage(gPowerEffectTaskDesc, data_021689a4, data_0216899c,
                   data_027c027c->unk_48[data_027c0284]->currentPlayer, 20, 0,
                   WEATHER_POWER_SNOW, FALSE, NULL, parent);
}
