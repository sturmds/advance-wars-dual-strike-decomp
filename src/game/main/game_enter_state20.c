#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void Players_ClearTeammatesDefeated(void);
int MapScript_RunList14(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Runs Players_ClearTeammatesDefeated and the map script's list 14, then switches unk_18 to 20. */
void Game_EnterState20(void) {
  Players_ClearTeammatesDefeated();
  MapScript_RunList14();
  STATE->unk_18 = 20;
}
