#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/power_damage.h"

extern TaskScriptCmd gPowerEffectTaskDesc[];
extern char data_021689a0[];
extern char data_02168994[];

/* Drake's CO Power: 1 HP to every enemy unit, enemy fuel halved. */
void Tsunami(void* parent) {
  /* Needed to match: 0xff, where the other powers pass WEATHER_KEEP (-1). */
  StartPowerDamage(gPowerEffectTaskDesc, data_021689a0, data_02168994,
                   data_027c027c->unk_48[data_027c0284]->currentPlayer, 10, 0,
                   0xff, TRUE, NULL, parent);
}
