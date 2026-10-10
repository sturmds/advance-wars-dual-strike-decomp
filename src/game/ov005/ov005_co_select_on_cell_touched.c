#include "types.h"
#include "game/Unk_ov005_0236bbac.h"

void Ov005CoSelect_SetSlideDirection(Unk_ov005_0236bbac *state);
void Ov005CoSelect_PickCellCo(Unk_ov005_0236bbac *state);

extern Unk_ov005_0236bbac *data_ov005_0236bbac;

/* Button callback: the cell buttons have ids 0x80 + cell number on the page. */
void Ov005CoSelect_OnCellTouched(u8 *buttonId)
{
    Unk_ov005_0236bbac *state = data_ov005_0236bbac;

    state->selection = *buttonId - 0x80;
    Ov005CoSelect_SetSlideDirection(state);
    Ov005CoSelect_PickCellCo(state);
    state->pageStartSelection = state->selection;
}
