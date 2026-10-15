#include "types.h"
#include "game/Unk_02080440.h"

void Players_ClearTeammatesDefeated(void);
int MapScript_RunList14(void);

/* Runs Players_ClearTeammatesDefeated and the map script's list 14, then switches unk_18 to 20. */
void Game_EnterState20(void) {
  Players_ClearTeammatesDefeated();
  MapScript_RunList14();
  MAP_STATE->unk_18 = 20;
}
