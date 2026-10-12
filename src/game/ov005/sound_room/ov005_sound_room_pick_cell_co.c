#include "types.h"
#include "game/Unk_ov005_0236bbac.h"

int Rng_Next(void);

extern u8* data_ov005_0236af24[];

/* Sets state->coId from the cell under the cursor. Cell ids 0x80 to 0x85 stand
 * for a group of COs (table data_ov005_0236af24: a count, then the CO ids):
 * one is picked at random. */
void Ov005SoundRoom_PickCellCo(Unk_ov005_0236bbac* state) {
  int coId = state->cells[state->selection + state->page * 8].coId;

  switch (coId) {
    case 0x80:
    case 0x81:
    case 0x82:
    case 0x83:
    case 0x84:
    case 0x85: {
      u8* group = data_ov005_0236af24[coId - OV005_CO_RANDOM_FIRST];

      state->coId = group[(u32)Rng_Next() % group[0] + 1];
      break;
    }
    default:
      state->coId = coId;
      break;
  }
}
