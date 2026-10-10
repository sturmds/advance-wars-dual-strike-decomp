#ifndef GAME_UNK_OV005_0236BBAC_H
#define GAME_UNK_OV005_0236BBAC_H

#include "types.h"

#define MUSIC_KIND_SEQ    0           /* sequence, Sound_StartMusicSeq */
#define MUSIC_KIND_STREAM 1           /* stream, Sound_StartStream */

/* Overlay 5 state reached through data_ov005_0236bbac. */
typedef struct Unk_ov005_0236bbac {
    int page;                          /* page shown, 0 to lastPage; L and R step through */
    int lastPage;
    int selection;                     /* selected entry of the page */
    int shownSelection;                /* selection of the last frame; -1 forces a redraw */
    int pageStartSelection;            /* selection after a page change */
    int unk_14;
    int unk_18;
    int unk_1c;
    u8 pad_20[0x24 - 0x20];
    int aTouched;                      /* set by touch buttons, cleared when handled */
    int bTouched;
    int xTouched;
    int lTouched;
    u32 unk_34;
    int coId;                          /* CO whose picture is shown; -1: none */
    int unk_3c;
    fx32 pictureX;
    fx32 pictureY;
    u8 pad_48[0x50 - 0x48];
    void *unk_50;                      /* passed to Task_Destroy (task system) */
    u8 pad_54[0x58 - 0x54];
    int musicId;                       /* music asked for; -1: none */
    int playingMusicId;
    int musicKind;                     /* MUSIC_KIND_* of musicId */
    int playingMusicKind;
    int musicChanging;                 /* 1 during the fade out */
    int musicPlaying;
    u8 pad_70[0x78 - 0x70];
    struct Unk_02022404 *mainObjects[4]; /* placed on the main screen each frame */
    u8 pad_88[0x8c - 0x88];
    struct Unk_02022404 *subObjects[5]; /* placed on the sub screen each frame */
    s16 objScale1;                     /* OBJ affine matrix 1 scale, 0x100 = 1x; -1: none */
    s16 objScale2;                     /* OBJ affine matrix 2 scale */
    s16 objScaleBoth;                  /* used for both when 1 and 2 are -1; -1: 0x100 */
} Unk_ov005_0236bbac;

#endif
