#include "types.h"
#include "game/Unk_02080440.h"
#include "game/event_script.h"
#include "game/player.h"
#include "game/script_slot.h"

extern u32 gLinkPlay;
extern ScriptCommand gMapScrollScript[];

BOOL EventScript_IsRunning(void);
s16 ScriptSlot_Find(const ScriptCommand* script);
void Font_LoadDigits(int baseTile);
void func_020f5888(int x, int y);
void Player_PlayTheme(int player);
void Panels_PlaceForCursor(void);
void Weather_StartNext(void);
void Units_WaitParalysed(void);
void func_020c4018(void);

/* Handler for game state 7. Waits for event scripts and the map scroll.
 * Scrolls to the current player's saved cursor tile, except for a player
 * that is not human while gLinkPlay (likely link play) is set, and waits
 * for that scroll.
 * Then plays the player's theme, starts the weather and switches to state 8. */
void Game_State7_ScrollToPlayerCursor(void) {
  if (EventScript_IsRunning()) {
    return;
  }
  if (ScriptSlot_Find(gMapScrollScript) != -1) {
    return;
  }
  Font_LoadDigits(0);
  if (gLinkPlay == 0 ||
      MAP_STATE->players[MAP_STATE->currentPlayer].controller ==
          PLAYER_CONTROLLER_HUMAN) {
    func_020f5888(MAP_STATE->players[MAP_STATE->currentPlayer].savedCursorX,
                  MAP_STATE->players[MAP_STATE->currentPlayer].savedCursorY);
  }
  if (ScriptSlot_Find(gMapScrollScript) != -1) {
    return;
  }
  Player_PlayTheme((u8)MAP_STATE->currentPlayer);
  Panels_PlaceForCursor();
  Weather_StartNext();
  Units_WaitParalysed();
  func_020c4018();
  MAP_STATE->unk_18 = 8;
}
