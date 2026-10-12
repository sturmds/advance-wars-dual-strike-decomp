#include "types.h"
#include "game/keypad.h"
#include "game/Unk_ov005_0236bbac.h"

void MenuButton_Select(int id);

extern Unk_ov005_0236bbac* data_ov005_0236bbac;

void Ov005SoundRoom_MoveCursorOnKeys(void) {
  Unk_ov005_0236bbac* state = data_ov005_0236bbac;
  int cursor = state->selection;

  if (gKeypad->repeat & PAD_KEY_UP) {
    if (cursor <= 0) {
      cursor = state->buttonCount - 1;
      state->cursorWrap = 1;
    } else {
      state->cursorWrap = 0;
      cursor--;
    }
  } else if (gKeypad->repeat & PAD_KEY_DOWN) {
    if (cursor >= state->buttonCount - 1) {
      state->cursorWrap = 2;
      cursor = 0;
    } else {
      state->cursorWrap = 0;
      cursor++;
    }
  }
  if (cursor != state->selection) {
    MenuButton_Select(state->buttons[cursor].id);
    state->pageStartSelection = cursor;
  }
  state->selection = cursor;
}
