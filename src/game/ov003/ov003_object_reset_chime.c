#include "types.h"
#include "game/Unk_02160abc.h"
#include "game/Unk_ov003_0235dd38.h"
#include "game/ov003_unit.h"

void Sound_PlaySe(int id);

/* Clears obj->unk_4c; plays sound effect 0xf3 when data_02160abc->unk_0a equals the row of the current slot. */
void Ov003_ObjectResetChime(Unk_ov003_0235dd38* obj) {
  Ov003State* state;

  obj->unk_4c = 0;
  state = data_ov003_02364928;
  if (data_02160abc->unk_0a == state->slots[state->currentSlot].row) {
    Sound_PlaySe(0xf3);
  }
}
