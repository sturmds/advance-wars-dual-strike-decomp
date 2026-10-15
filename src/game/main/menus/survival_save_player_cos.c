#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_022a7b80.h"

extern Unk_022a7b80 data_022a7b80;

/* Saves the front and back CO of slot 1 in the run record. */
void Survival_SavePlayerCos(void) {
  data_022a7b80.coIds[0] = MAP_STATE->playerCos[1][0] & 0x7f;
  data_022a7b80.coIds[1] = MAP_STATE->playerCos[1][1] & 0x7f;
}
