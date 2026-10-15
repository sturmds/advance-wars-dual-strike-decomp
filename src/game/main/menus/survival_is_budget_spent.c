#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_022a7b80.h"

u32 Survival_GetTimeLeft(void);

extern Unk_022a7b80 data_022a7b80;

/* True in Survival when the player has run out of what the mode limits:
 * time left on the map (modes 0 and 3), funds on hand (1 and 4) or days
 * (2 and 5, past the budget in data_022a7b80.budgetLeft). */
BOOL Survival_IsBudgetSpent(void) {
  if (MAP_STATE->gameMode != GAME_MODE_SURVIVAL) {
    return FALSE;
  }
  switch (data_022a7b80.mode) {
    case 0:
    case 3:
      if (Survival_GetTimeLeft() == 0) {
        return TRUE;
      }
      break;
    case 2:
    case 5:
      /* needed to match: unsigned compare */
      if (MAP_STATE->day > (u32)data_022a7b80.budgetLeft) {
        return TRUE;
      }
      break;
    case 1:
    case 4:
      if (MAP_STATE->players[1].funds == 0) {
        return TRUE;
      }
      break;
  }
  return FALSE;
}
