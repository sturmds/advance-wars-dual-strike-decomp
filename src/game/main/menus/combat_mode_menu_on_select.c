#include "types.h"
#include "game/co_portrait_task.h"
#include "game/keypad.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"
#include "game/task.h"
#include "game/Unk_02160ac0.h"

void Sound_PlaySe(u32 soundId);

#define SE_CURSOR_MOVE 0x10002
CoPortraitTask* Task_FindByScript(const void* script);
void Co_UnpackPicture(int coId, int firstTile);
void DrawSelectedCoPowerText(void);

extern u8* data_0229e144;
extern u8 gCombatModeCos[];
extern const TaskScriptCmd data_021613d8[]; /* script of the CO portrait task */

/* Runs when a Combat mode menu slot gets selected. Stores the slot in
 * data_02160ac0->co, and also as the remembered row (data_0229e144)
 * unless the move was left or right only. Then tells the CO portrait task:
 * a portrait in view slides out (it comes back with the new CO), one that
 * was sliding out for this same CO slides back in. Redraws the CO Power
 * text. */
void CombatModeMenu_OnSelect(MenuButton* button) {
  CoPortraitTask* portrait;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  Sound_PlaySe(SE_CURSOR_MOVE);
  data_02160ac0->co = Menu_GetSelectedId();
  if (!(gKeypad->repeat & (PAD_KEY_RIGHT | PAD_KEY_LEFT)) ||
      (gKeypad->repeat & (PAD_KEY_UP | PAD_KEY_DOWN))) {
    *data_0229e144 = data_02160ac0->co;
  }
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  portrait = Task_FindByScript(data_021613d8);
  if (portrait->selectedSlot == Menu_GetSelectedId()) {
    /* The screens are not restored on this path. */
    return;
  }
  portrait->selectedSlot = Menu_GetSelectedId();
  Co_UnpackPicture(gCombatModeCos[portrait->selectedSlot], 0x1b0);
  if (portrait->waitFrames == 0) {
    if (portrait->selectedSlot == portrait->shownSlot) {
      if (portrait->slidingIn == 0) {
        portrait->slideFrom = portrait->offset;
        portrait->frame = 0;
        portrait->slidingIn = 1;
      }
    } else if (portrait->slidingIn == 1) {
      portrait->slideFrom = portrait->offset;
      portrait->frame = 0;
      portrait->slidingIn = 0;
    }
  }
  portrait->waitFrames = 0;
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
  DrawSelectedCoPowerText();
}
