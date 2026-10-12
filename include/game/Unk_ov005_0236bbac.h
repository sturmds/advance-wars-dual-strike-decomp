#ifndef GAME_UNK_OV005_0236BBAC_H
#define GAME_UNK_OV005_0236BBAC_H

#include "types.h"
#include "game/text_ref.h"

/* One cell of the CO grid: 8 bytes, 8 cells per page. */
typedef struct Ov005CoCell {
  TextRef name;
  u16 unk_04;
  u8 coId; /* OV005_CO_RANDOM_FIRST to OV005_CO_RANDOM_LAST: random pick from a group */
  u8 unk_07;
} Ov005CoCell;

#define OV005_CO_CELL_COUNT 67 /* cells over all pages */
#define OV005_CELLS_PER_PAGE 8
#define OV005_CELL_BUTTON_ID 0x80 /* MenuButton id of cell 0 of a page */

#define OV005_CO_RANDOM_FIRST 0x80
#define OV005_CO_RANDOM_LAST 0x85

/* One touch button of the page: 8 bytes. */
typedef struct Ov005CoButton {
  s16 unk_00;
  s16 id; /* MenuButton id */
  s16 x;
  s16 y;
} Ov005CoButton;

/* Task data of the cursor slide task (unk_50 below). */
typedef struct Ov005CoSlideTask {
  u8 pad_00[0x34];
  u32 step; /* state of the update function */
  u8 pad_38[0x44 - 0x38];
  int slideBack; /* 1: the cursor moved to a lower cell, see Ov005SoundRoom_SetSlideDirection */
} Ov005CoSlideTask;

#define MUSIC_KIND_SEQ 0    /* sequence, Sound_StartMusicSeq */
#define MUSIC_KIND_STREAM 1 /* stream, Sound_StartStream */

/* Overlay 5 state reached through data_ov005_0236bbac. */
typedef struct Unk_ov005_0236bbac {
  int page; /* page shown, 0 to lastPage; L and R step through */
  int lastPage;
  int selection;          /* selected entry of the page */
  int shownSelection;     /* selection of the last frame; -1 forces a redraw */
  int pageStartSelection; /* selection after a page change */
  int cursorWrap; /* 1: the cursor wrapped to the last cell, 2: to the first, 0: no wrap */
  int displayedRow; /* cell whose name shows; Ov005_UpdatePageTurn copies selection here mid-slide; -1: none */
  int displayedPage; /* page of displayedRow */
  int buttonCount;   /* buttons on the current page, at most 8 */
  int aTouched;      /* set by touch buttons, cleared when handled */
  int bTouched;
  int xTouched;
  int lTouched;
  u32 rTouched;
  int coId;       /* CO whose picture is shown; -1: none */
  int loadedCoId; /* CO whose picture is loaded; -1: none */
  fx32 pictureX;
  fx32 pictureY;
  struct Task* pictureTask; /* runs data_ov005_0236aedc */
  u8 pad_4c[0x50 - 0x4c];
  void* unk_50; /* passed to Task_Destroy (task system) */
  u8 pad_54[0x58 - 0x54];
  int musicId; /* music asked for; -1: none */
  int playingMusicId;
  int musicKind; /* MUSIC_KIND_* of musicId */
  int playingMusicKind;
  int musicChanging; /* 1 during the fade out */
  int musicPlaying;
  struct Task* musicTask; /* runs Ov005_UpdateMusic */
  void* spriteFileA;      /* sprite file the sprites below are made from */
  struct SpriteAnim* mainObjects[4]; /* placed on the main screen each frame */
  void* spriteFileB;
  struct SpriteAnim* subObjects[5]; /* placed on the sub screen each frame */
  s16 objScale1;    /* OBJ affine matrix 1 scale, 0x100 = 1x; -1: none */
  s16 objScale2;    /* OBJ affine matrix 2 scale */
  s16 objScaleBoth; /* used for both when 1 and 2 are -1; -1: 0x100 */
  Ov005CoCell* cells;
  u8 pad_ac[0xec - 0xac];
  Ov005CoButton* buttons;
  u8 pad_f0[0x1114 - 0xf0];
  int unk_1114;
  u8 pad_1118[0x1220 - 0x1118];
  s16 barHeights[16]; /* 0 to 10 tiles */
  u8 pad_1240[0x1320 - 0x1240];
  s16 shownBarHeights[16]; /* barHeights as last drawn */
} Unk_ov005_0236bbac;

#endif
