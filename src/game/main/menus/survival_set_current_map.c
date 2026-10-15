#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_022a7b80.h"

int Survival_GetMapId(int mode, int index);

extern Unk_022a7b80 data_022a7b80;

/* Makes the map at the run's position (data_022a7b80.mapIndex) the one the
 * game state loads. */
void Survival_SetCurrentMap(void) {
  MAP_STATE->unk_1c4->mapId =
      Survival_GetMapId(data_022a7b80.mode, data_022a7b80.mapIndex);
}
