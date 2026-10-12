#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"
#include "game/unlocks.h"
#include "game/Unk_ov006_0235f690.h"

void Ov006_SetEntryPage(int next);
void func_ov006_023576f4(int arg);
void func_ov006_02357734(void);
void func_ov006_02358fec(void);
void Ov006_UpdateEntryView(int index);

/* Per-button cursor steps (added to the button id), copied to the stack
 * before use. The sizes are those of the copies. */
typedef struct {
  s8 step[25];
} CursorSteps25;
typedef struct {
  s8 step[26];
} CursorSteps26;
typedef struct {
  s8 step[24];
} CursorSteps24;
typedef struct {
  s8 step[18];
} CursorSteps18;

extern CursorSteps18 data_ov006_0236b700;
extern CursorSteps18 data_ov006_0236b714;
extern CursorSteps24 data_ov006_0236b728;
extern CursorSteps24 data_ov006_0236b740;
extern CursorSteps25 data_ov006_0236b758;
extern CursorSteps25 data_ov006_0236b774;
extern CursorSteps25 data_ov006_0236b790;
extern CursorSteps25 data_ov006_0236b7ac;
extern CursorSteps26 data_ov006_0236b7c8;
extern CursorSteps26 data_ov006_0236b800;
extern CursorSteps26 data_ov006_0236b81c;
extern CursorSteps26 data_ov006_0236b838;

extern Unk_ov006_0235f690* data_ov006_0236d640;

#define STATE data_ov006_0236d640

/* Key handler of the entry grid buttons (ids 0x40 and up) in the Design
 * Room. B goes back from request 2 or 6, A shows the entry, R and L call
 * func_ov006_023576f4, Y and X step the page (unitPage, 1-4, in unit mode;
 * terrainPage, 0-4, in terrain mode) and wrap. The d-pad moves the cursor by
 * a per-button step from a table picked by the mode (and by game flag 0x2b in
 * unit mode), with screen 0 made current while it selects the new button. */
void Ov006_EntryButtonKeys(MenuButton* button) {
  int next;
  s8* steps;
  int target;
  Unk_02080440* saved027c;
  Unk_027c0280* saved0280;

  if (gKeypad->pressed & PAD_BUTTON_B) {
    switch (STATE->request) {
      case 2:
        func_ov006_02357734();
        return;
      case 6:
        func_ov006_02358fec();
        return;
    }
    return;
  }
  if (gKeypad->pressed & PAD_BUTTON_A) {
    Ov006_UpdateEntryView(button->id - 0x40);
    return;
  }
  if (gKeypad->pressed & (PAD_BUTTON_R | PAD_BUTTON_L)) {
    if (gKeypad->pressed & PAD_BUTTON_R) {
      func_ov006_023576f4(0);
    } else if (gKeypad->pressed & PAD_BUTTON_L) {
      func_ov006_023576f4(1);
    }
  }
  if (gKeypad->pressed & (PAD_BUTTON_X | PAD_BUTTON_Y)) {
    if (gKeypad->pressed & PAD_BUTTON_Y) {
      if (STATE->unitMode == 1) {
        next = STATE->unitPage - 1;
        if (next < 1) {
          next += 4;
        }
      }
      if (STATE->unitMode == 0) {
        next = STATE->terrainPage - 1;
        if (next < 0) {
          next += 5;
        }
      }
    } else if (gKeypad->pressed & PAD_BUTTON_X) {
      if (STATE->unitMode == 1) {
        next = STATE->unitPage + 1;
        if (next >= 5) {
          next = 1;
        }
      }
      if (STATE->unitMode == 0) {
        next = STATE->terrainPage + 1;
        if (next >= 5) {
          next = 0;
        }
      }
    }
    Ov006_SetEntryPage(next);
    return;
  }

  steps = NULL;
  if ((gKeypad->pressed & PAD_KEY_RIGHT) || (gKeypad->repeat & PAD_KEY_RIGHT)) {
    CursorSteps25 rightA = data_ov006_0236b790;
    CursorSteps26 rightB = data_ov006_0236b81c;
    CursorSteps24 rightTerrain = data_ov006_0236b728;
    if (STATE->unitMode == 1) {
      steps = GameFlag_IsSet(0x2b) ? rightB.step : rightA.step;
    } else if (STATE->unitMode == 0) {
      steps = rightTerrain.step;
    }
  }
  if ((gKeypad->pressed & PAD_KEY_LEFT) || (gKeypad->repeat & PAD_KEY_LEFT)) {
    CursorSteps25 leftA = data_ov006_0236b7ac;
    CursorSteps26 leftB = data_ov006_0236b838;
    CursorSteps24 leftTerrain = data_ov006_0236b740;
    if (STATE->unitMode == 1) {
      steps = GameFlag_IsSet(0x2b) ? leftB.step : leftA.step;
    } else if (STATE->unitMode == 0) {
      steps = leftTerrain.step;
    }
  }
  if (steps != NULL) {
    target = button->id + steps[button->id - 0x40];
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    MenuButton_Select(target);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }

  steps = NULL;
  if ((gKeypad->pressed & PAD_KEY_UP) || (gKeypad->repeat & PAD_KEY_UP)) {
    CursorSteps25 upA = data_ov006_0236b758;
    CursorSteps26 upB = data_ov006_0236b7c8;
    CursorSteps18 upTerrain = data_ov006_0236b700;
    if (STATE->unitMode == 1) {
      steps = GameFlag_IsSet(0x2b) ? upB.step : upA.step;
    } else if (STATE->unitMode == 0) {
      steps = upTerrain.step;
    }
  }
  if ((gKeypad->pressed & PAD_KEY_DOWN) || (gKeypad->repeat & PAD_KEY_DOWN)) {
    CursorSteps25 downA = data_ov006_0236b774;
    CursorSteps26 downB = data_ov006_0236b800;
    CursorSteps18 downTerrain = data_ov006_0236b714;
    if (STATE->unitMode == 1) {
      steps = GameFlag_IsSet(0x2b) ? downB.step : downA.step;
    } else if (STATE->unitMode == 0) {
      steps = downTerrain.step;
    }
  }
  if (steps != NULL) {
    /* needed to match: this block has its own saved pointers and
         * reuses `next` for the target */
    Unk_027c0280* oldGfx;
    Unk_02080440* oldState;
    next = button->id + steps[button->id - 0x40];
    oldGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    oldState = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    MenuButton_Select(next);
    Screen_SetCurrent(oldState);
    Screen_SetCurrentGfx(oldGfx);
  }
}
