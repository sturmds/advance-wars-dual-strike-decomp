#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])
#define UNIT(i) (GAME->units[(i) + GAME->currentUnitBase])

/* The L button: the current player's next unit that has not waited this turn,
 * looking at slots 1-50 after cycleUnitIndex, then wrapping round to 1. NULL
 * when there is none. cycleUnitIndex goes back to 0 after slot 50. */
MapUnit* CurrentPlayer_NextIdleUnit(void) {
  u16 i;

  for (i = GAME->cycleUnitIndex + 1; i < 51; i++) {
    if (UNIT(i).type != 0 && !(UNIT(i).flags & MAP_UNIT_WAITED)) {
      GAME->cycleUnitIndex = i;
      if (GAME->cycleUnitIndex >= 50) {
        GAME->cycleUnitIndex = 0;
      }
      return &UNIT(i);
    }
  }
  for (i = 1; i < GAME->cycleUnitIndex + 1; i++) {
    if (UNIT(i).type != 0 && !(UNIT(i).flags & MAP_UNIT_WAITED)) {
      GAME->cycleUnitIndex = i;
      if (GAME->cycleUnitIndex >= 50) {
        GAME->cycleUnitIndex = 0;
      }
      return &UNIT(i);
    }
  }
  return NULL;
}
