#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/power_damage.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern TaskScriptCmd gPowerEffectTaskDesc[];
extern char data_02168998[];
extern char data_02168994[];

/* Hawke's CO Power: 1 HP to every enemy unit, 1 HP back to his own. */
void BlackWave(void* parent) {
  StartPowerDamage(gPowerEffectTaskDesc, data_02168998, data_02168994,
                   data_027c027c->unk_48[data_027c0284]->currentPlayer, 10, 10,
                   WEATHER_KEEP, FALSE, NULL, parent);
}
