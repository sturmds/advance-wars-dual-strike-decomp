#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/power_damage.h"

extern TaskScriptCmd gPowerEffectTaskDesc[];
extern char data_02168998[];
extern char data_0216899c[];

/* Hawke's CO Super Power: 2 HP to every enemy unit, 2 HP back to his own. */
void BlackStorm(void* parent) {
  StartPowerDamage(gPowerEffectTaskDesc, data_02168998, data_0216899c,
                   data_027c027c->unk_48[data_027c0284]->currentPlayer, 20, 20,
                   WEATHER_KEEP, FALSE, NULL, parent);
}
