#include "types.h"
#include "game/power_damage.h"

void Weather_Set(int weather);
void Power_ResetScrollAndRefresh(void);

/* Takes the PowerDamageTask (no caller found): sets the power's weather unless it
 * keeps the current one (stored as 0xff), then refreshes the map. */
void PowerDamage_SetWeather(PowerDamageTask* task) {
  if (task->weather != 0xff) {
    Weather_Set(task->weather);
  }
  Power_ResetScrollAndRefresh();
}
