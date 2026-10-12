#include "types.h"
#include "game/force_rank_screen.h"
#include "game/menu_button.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Sound_PlaySe(int id);
void ForceRankScreen_DrawSlotRow(int x, int y, int textTile, int skill);
int ForceRankScreen_FindChosenSlot(int skill);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

#define CURSOR_COLUMN(cursor) ((cursor) >> 3)
#define CURSOR_ROW(cursor) ((cursor) & 7)

/* Redraws the row of the selected slot on the second screen. */
static inline void RedrawSelectedSlot(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  ForceRankScreen_DrawSlotRow(
      1, 15 + gForceRankScreen->selectedSlot * 2,
      256 + gForceRankScreen->selectedSlot * 64,
      gForceRankScreen->chosenSkills[gForceRankScreen->selectedSlot]);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}

/* Callback of the skill buttons (probably onConfirm). A skill already in a slot is taken out of
 * it, and that slot becomes the selected one. Otherwise the skill goes into
 * the selected slot (selectedSlot), which then moves to the next slot, up to the
 * last open one; picking "no skill" moves it back one slot instead. */
void ForceRankScreen_ToggleSkill(MenuButton* button) {
  int slot;

  slot = ForceRankScreen_FindChosenSlot(
      gForceRankScreen->columns[CURSOR_COLUMN(button->id)]
          .skills[CURSOR_ROW(button->id)]);
  if (slot != -1) {
    gForceRankScreen->selectedSlot = slot;
    gForceRankScreen->chosenSkills[gForceRankScreen->selectedSlot] = 0;
    RedrawSelectedSlot();
    Sound_PlaySe(0x10009);
    return;
  }
  gForceRankScreen->chosenSkills[gForceRankScreen->selectedSlot] =
      gForceRankScreen->columns[CURSOR_COLUMN(button->id)]
          .skills[CURSOR_ROW(button->id)];
  RedrawSelectedSlot();
  if (gForceRankScreen->columns[CURSOR_COLUMN(button->id)]
          .skills[CURSOR_ROW(button->id)] == 0) {
    Sound_PlaySe(0x10009);
    if (gForceRankScreen->selectedSlot != 0) {
      gForceRankScreen->selectedSlot--;
    }
  } else {
    Sound_PlaySe(0x52);
    if (gForceRankScreen->selectedSlot < 3 &&
        gForceRankScreen->selectedSlot < gForceRankScreen->rank - 1) {
      gForceRankScreen->selectedSlot++;
    }
  }
}
