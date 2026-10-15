#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/menu_help.h"
#include "game/play_records.h"
#include "game/screen_other_map.h"

void Screen_SetCurrentBoth(void* state);
void PopupMenu_Refresh(void);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);

extern u32 gLinkPlay;

/* Battle menu handler (table entries at gBattleMenuItems + 0xa0 to + 0x100,
 * one per value): steps the current player's aiBehaviour through 0-3 on both
 * screens, draws the matching help text, refreshes the menu and saves the
 * value in gPlayRecords for campaign or War Room. In link play it also sends
 * the change as unit action 27. */
void Menu_CycleAiBehaviour(int a0, int a1, u32 flags) {
  MenuHelpFour help = gAiBehaviourMenuHelpFns;
  Unk_02080440* savedGame = data_027c027c;
  u8 behaviour;
  u8 player;
  Unk_027c0280* savedGfx;

  player = MAP_STATE->currentPlayer;
  behaviour = MAP_STATE->players[player].aiBehaviour + 1;
  if (behaviour >= 4) {
    behaviour = AI_BEHAVIOUR_STRIKE;
  }
  savedGfx = data_027c0280;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  MAP_STATE->players[player].aiBehaviour = behaviour;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  MAP_STATE->players[player].aiBehaviour = behaviour;
  Screen_SetCurrent(savedGame);
  Screen_SetCurrentGfx(savedGfx);
  help.fn[behaviour](a0, a1, flags);
  PopupMenu_Refresh();
  if (MAP_STATE->gameMode != GAME_MODE_CAMPAIGN) {
    if (MAP_STATE->gameMode == GAME_MODE_WAR_ROOM) {
      gPlayRecords.warRoomAiBehaviour = behaviour;
    }
  } else {
    gPlayRecords.campaignAiBehaviour = behaviour;
  }
  if (gLinkPlay != 0) {
    UnitAction_Send(27, 0, player, behaviour);
  }
}
