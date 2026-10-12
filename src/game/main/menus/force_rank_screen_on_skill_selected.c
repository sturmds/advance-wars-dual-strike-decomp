#include "types.h"
#include "game/force_rank_screen.h"
#include "game/keypad.h"
#include "game/menu_button.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Sound_PlaySe(int id);
void ForceRankScreen_DrawSlotRow(int x, int y, int textTile, int skill);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

#define CURSOR_COLUMN(cursor) ((cursor) >> 3)
#define CURSOR_ROW(cursor) ((cursor) & 7)

/* onSelect of a skill button: stores the cursor, remembers the row (row 5
 * in the "no skill" column) unless the move was sideways, and shows the
 * skill's name and description at the bottom of the main screen. */
void ForceRankScreen_OnSkillSelected(MenuButton* button) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  Sound_PlaySe(0x10002);
  gForceRankScreen->cursorColumn = CURSOR_COLUMN(button->id);
  gForceRankScreen->cursorRow = CURSOR_ROW(button->id);
  if (!(gKeypad->repeat & (PAD_KEY_LEFT | PAD_KEY_RIGHT)) ||
      (gKeypad->repeat & (PAD_KEY_UP | PAD_KEY_DOWN))) {
    gForceRankScreen->row = gForceRankScreen->cursorRow;
    if (gForceRankScreen->cursorColumn == FORCE_RANK_COLUMN_NONE) {
      gForceRankScreen->row = 5;
    }
  }
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  ForceRankScreen_DrawSlotRow(
      1, 21, 256,
      gForceRankScreen->columns[CURSOR_COLUMN(button->id)]
          .skills[CURSOR_ROW(button->id)]);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}
