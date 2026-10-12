#include "types.h"
#include "game/Unk_ov005_0236bbac.h"

extern Unk_ov005_0236bbac* data_ov005_0236bbac;

BOOL Ov005SoundRoom_CellChanged(void) {
  Unk_ov005_0236bbac* state = data_ov005_0236bbac;
  Ov005CoCell* cell = &state->cells[state->selection + state->page * 8];

  if (state->musicId == cell->unk_04 && state->musicKind == cell->unk_07) {
    return FALSE;
  }
  return TRUE;
}
