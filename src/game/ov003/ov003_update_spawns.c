#include "types.h"
#include "game/Unk_02160abc.h"
#include "game/Unk_02160ac0.h"
#include "game/ov003_unit.h"

int Ov003Unit_CountInCurrentRow(void);
int Ov003Unit_SpawnNextWithSprite(void);

/* Spawn tick. Normally tops the current row up to unk_1dc units. With
 * data_02160ac0->unk_7c set, each nonzero unk_1c0 counter runs down and a unit
 * spawns when it reaches 0; a failed spawn retries on the next call. */
void Ov003_UpdateSpawns(void) {
  int count;
  int i;

  if (data_02160abc->unk_0a != 0) {
    return;
  }
  if (data_02160ac0->unk_7c == 0) {
    count = Ov003Unit_CountInCurrentRow();
    if (count >= data_ov003_02364928->unk_1dc) {
      return;
    }
    do {
      if (Ov003Unit_SpawnNextWithSprite() == 0) {
        return;
      }
      count++;
    } while (count < data_ov003_02364928->unk_1dc);
  } else {
    for (i = 0; i < (int)ARRAY_COUNT(data_ov003_02364928->unk_1c0); i++) {
      if (data_ov003_02364928->unk_1c0[i] != 0) {
        data_ov003_02364928->unk_1c0[i]--;
        if (data_ov003_02364928->unk_1c0[i] == 0) {
          if (Ov003Unit_SpawnNextWithSprite() == 0) {
            data_ov003_02364928->unk_1c0[i] = 1;
          }
        }
      }
    }
  }
}
