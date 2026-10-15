#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_ov006_0235f690.h"
#include "game/keypad.h"

extern s16 data_02169454
    [][2]; /* per d-pad bit pattern (right, left, up, down): x step, y step */
extern s16 data_02169456[][2]; /* the same table from its y steps */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])
#define OV (data_ov006_0236d640)

#define MAP_LAST_X 29 /* Design Room maps are 30x20 tiles */
#define MAP_LAST_Y 19

/* Moves the editor's cursor tile (cursorX, cursorY) one step in the direction
 * of the d-pad keys in gKeypad->repeat, kept on the map. Does nothing while
 * the map view is scrolling. */
void Ov006_MoveCursor(void) {
  int dir;
  int step;
  int pos;
  s16 cur;

  if (MAP_STATE->unk_1c4->scrollFlags & 15) {
    return;
  }
  dir = (gKeypad->repeat >> 4) & 15;
  step = data_02169454[dir][0];
  cur = OV->cursorX;
  pos = cur + step;
  if (step > 0) {
    if (pos > MAP_LAST_X) {
      pos = MAP_LAST_X;
    }
    OV->viewX = pos;
    OV->cursorX = pos;
  } else if (step < 0) {
    /* overwritten below; kept because the original has it */
    if (OV->viewX != cur) {
      OV->viewX = cur;
    }
    if (pos < 0) {
      pos = 0;
    }
    OV->viewX = pos;
    OV->cursorX = pos;
  }
  cur = OV->cursorY;
  step = data_02169456[dir][0];
  pos = cur + step;
  if (step > 0) {
    if (pos > MAP_LAST_Y) {
      pos = MAP_LAST_Y;
    }
    OV->viewY = pos;
    OV->cursorY = pos;
  } else if (step < 0) {
    if (OV->viewY != cur) {
      OV->viewY = cur;
    }
    if (pos < 0) {
      pos = 0;
    }
    OV->viewY = pos;
    OV->cursorY = pos;
  }
  if (gKeypad->held &
      (PAD_KEY_RIGHT | PAD_KEY_LEFT | PAD_KEY_UP | PAD_KEY_DOWN)) {
    OV->viewX = OV->cursorX;
    OV->viewY = OV->cursorY;
  }
}
