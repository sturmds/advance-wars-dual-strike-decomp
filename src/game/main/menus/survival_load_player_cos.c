#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_022a7b80.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_022a7b80 data_022a7b80;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Puts the run's saved COs and their starting power meters into slot 1 of
 * the game state, which the next battle setup reads. */
void Survival_LoadPlayerCos(void) {
  int i;

  for (i = 0; i < (int)ARRAY_COUNT(data_022a7b80.coIds); i++) {
    STATE->playerCos[1][i] = data_022a7b80.coIds[i] & 0x7f;
    STATE->unk_aa[i] = data_022a7b80.powerCharge[i];
  }
}
