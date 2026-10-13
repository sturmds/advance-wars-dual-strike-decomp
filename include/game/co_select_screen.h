#ifndef GAME_CO_SELECT_SCREEN_H
#define GAME_CO_SELECT_SCREEN_H

#include "types.h"

/* Work area of the CO select screen (Campaign, War Room, Survival),
 * through data_0229c73c. The COs sit in a grid; a cursor holds the column
 * in bits 1 and up and the row (0 or 1) in bit 0. */
typedef struct CoSelectScreen {
  union {
    u8 coGrid
        [0x2c]; /* CO id per cell at (cursorColumn - 1) * 2 + cursorRow; 0: empty */
    struct {
      u8 pad_00[12]; /* the CO ids, then 6 bytes of per-cell flags */
      u8 cellSkills
          [3][2]
          [5]; /* [column - 1][row]: skill ids of the cell's CO, up to 4, 0 ends */
    };
  };
  u8* coList; /* CO ids of the CO list slots, ended by 0 */
  u8 unk_30; /* CO list slot of the cursor cell's CO, set by CoSelectScreen_Create */
  u8 cursorColumn; /* counts from 1 */
  u8 cursorRow;
  u8 coListOpen; /* 1 while the CO list is open */
  u8 coBeforeList; /* CO of the cursor cell when the CO list opened; cancel puts it back */
  u8 columnCount;
  u8 rowCount; /* up and down change row only when this is 2 */
  u8 unk_37; /* CO list slot; the draw frames it and marks empty cells beside it */
  u8 listAnchor; /* CO list slot set on opening and by up and down; left and right look in its row first */
} CoSelectScreen;

extern CoSelectScreen* data_0229c73c;

/* The CO in the cursor cell. Needed to match: coGrid read as rows of 2. */
#define CO_SELECT_CURSOR_CO \
  (((u8(*)[2])data_0229c73c \
        ->coGrid)[data_0229c73c->cursorColumn - 1][data_0229c73c->cursorRow])

/* Task that slides the big CO pictures on the bottom screen, one per grid
 * row (CoSelectScreen_SlidePictures). Fields are per row. */
typedef struct CoSelectPictureTask {
  u8 pad_00[0x2a]; /* Task header */
  u16 frame[2];    /* frames into the current slide, up to 8 */
  u16 waitFrames
      [2]; /* frames the picture has stayed out before the new CO loads */
  s16 slideFrom[2];
  s16 slideOffset
      [2];       /* pixels the picture is moved off screen; 144 is fully out */
  u8 co[2];      /* CO under the cursor */
  u8 shownCo[2]; /* CO whose picture is loaded */
  u8 slidingIn[2];
} CoSelectPictureTask;

/* One bit per CO id: the COs the CO select screen offers. */
extern u32 data_0229c740;

#endif
