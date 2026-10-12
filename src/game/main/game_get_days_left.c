#include "types.h"

int Game_GetDayLimit(void);
int Game_GetShownDay(void);

/* Days left including today, 0 when the battle has no day limit. */
int Game_GetDaysLeft(void) {
  int limit = Game_GetDayLimit();
  int day = Game_GetShownDay();

  if (limit == 0) {
    return 0;
  }
  return limit + 1 - day;
}
