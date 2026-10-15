#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_022a7b80.h"

int Survival_GetModeBudget(int mode);
int Survival_GetMapId(int mode, int index);

extern Unk_022a7b80 data_022a7b80;

/* Starts a Survival run of the mode variant in data_022a7b80.mode: back to
 * the first map, the budget full (budget and budgetLeft), no points yet, and
 * the game state set to load the first map. */
void Survival_StartRun(void) {
  data_022a7b80.mapIndex = 0;
  data_022a7b80.budget = Survival_GetModeBudget(data_022a7b80.mode);
  data_022a7b80.budgetLeft = Survival_GetModeBudget(data_022a7b80.mode);
  data_022a7b80.points = 0;
  MAP_STATE->unk_1c4->mapId =
      Survival_GetMapId(data_022a7b80.mode, data_022a7b80.mapIndex);
}
