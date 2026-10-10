#ifndef GAME_UNK_OV005_0236BBAC_H
#define GAME_UNK_OV005_0236BBAC_H

#include "types.h"

/* Overlay 5 state reached through data_ov005_0236bbac. */
typedef struct Unk_ov005_0236bbac {
    int page;                          /* page shown, 0 to lastPage; L and R step through */
    int lastPage;
    int selection;                     /* selected entry of the page */
    int shownSelection;                /* selection of the last frame; -1 forces a redraw */
    int pageStartSelection;            /* selection after a page change */
    u8 pad_14[0x24 - 0x14];
    int aTouched;                      /* set by touch buttons, cleared when handled */
    int bTouched;
    int xTouched;
    int lTouched;
    u32 unk_34;
    int coId;                          /* CO whose picture is shown; -1: none */
    u8 pad_3c[0x50 - 0x3c];
    void *unk_50;                      /* passed to Task_Destroy (task system) */
} Unk_ov005_0236bbac;

#endif
