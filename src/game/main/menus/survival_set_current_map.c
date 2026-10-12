#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_022a7b80.h"

int Survival_GetMapId(int mode, int index);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_022a7b80 data_022a7b80;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Makes the map at the run's position (data_022a7b80.mapIndex) the one the
 * game state loads. */
void Survival_SetCurrentMap(void) {
  STATE->unk_1c4->mapId =
      Survival_GetMapId(data_022a7b80.mode, data_022a7b80.mapIndex);
}
