#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/menu_help.h"
#include "game/play_records.h"
#include "game/visual_mode.h"
#include "game/screen_other_map.h"

void PopupMenu_Refresh(void);

#define CURRENT_MAP ((&data_027c0288.unk_00)[data_027c0284])
#define OTHER_MAP ((&data_027c0288.unk_00)[1 - data_027c0284])

#define SWITCH_TO(state, savedGfx, savedGame) \
  savedGfx = data_027c0280;                   \
  Screen_SetCurrentGfx(state);                \
  savedGame = data_027c027c;                  \
  Screen_SetCurrent(state)

#define SWITCH_BACK(savedGfx, savedGame) \
  Screen_SetCurrent(savedGame);          \
  Screen_SetCurrentGfx(savedGfx)

/* Battle menu handler (table entries 0x02165dc0 to 0x02165e40, one per value):
 * steps Unk_02080440_48.visualMode through 0-4, on both maps of a two-map battle
 * when the other map's unk_1c4->unk_30 is 0, draws the matching help text,
 * refreshes the menu and saves the value in gPlayRecords. ActingUnit_StartAttack
 * picks the attack animation task from visualMode, so the name is a guess. */
void Menu_CycleAnimationMode(int a0, int a1, u32 flags) {
  MenuHelpFive help = gAnimationMenuHelpFns;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedGame;
  u8 mode;
  u32 otherUnk30;

  SWITCH_TO(CURRENT_MAP, savedGfx, savedGame);
  MAP_STATE->visualMode++;
  if (MAP_STATE->visualMode > VISUAL_D) {
    MAP_STATE->visualMode = VISUAL_NONE;
  }
  mode = MAP_STATE->visualMode;
  SWITCH_BACK(savedGfx, savedGame);
  if (MAP_STATE->secondMapId != 0) {
    SWITCH_TO(OTHER_MAP, savedGfx, savedGame);
    otherUnk30 = MAP_STATE->unk_1c4->unk_30;
    SWITCH_BACK(savedGfx, savedGame);
    if (otherUnk30 == 0) {
      SWITCH_TO(OTHER_MAP, savedGfx, savedGame);
      MAP_STATE->visualMode = mode;
      SWITCH_BACK(savedGfx, savedGame);
    }
  }
  help.fn[MAP_STATE->visualMode](a0, a1, flags);
  PopupMenu_Refresh();
  gPlayRecords.visualMode = MAP_STATE->visualMode;
}
