#include "types.h"
#include "game/Unk_ov005_0236bbac.h"

void Ov005SoundRoom_SetSlideDirection(Unk_ov005_0236bbac* state);
void Ov005SoundRoom_PickCellCo(Unk_ov005_0236bbac* state);

extern Unk_ov005_0236bbac* data_ov005_0236bbac;

/* Button callback: the cell buttons have ids 0x80 + cell number on the page. */
void Ov005SoundRoom_OnCellTouched(u8* buttonId) {
  Unk_ov005_0236bbac* state = data_ov005_0236bbac;

  state->selection = *buttonId - 0x80;
  Ov005SoundRoom_SetSlideDirection(state);
  Ov005SoundRoom_PickCellCo(state);
  state->pageStartSelection = state->selection;
}
