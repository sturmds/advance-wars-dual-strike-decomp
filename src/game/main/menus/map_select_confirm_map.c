#include "types.h"
#include "game/map_header.h"
#include "game/map_select.h"

extern MenuState gMenuState;

void Sound_PlaySe(int id);
void func_020b9890(void);
/* Stores the battle's map and second map in the game state. */
void State_SetMapIds(int mapId, int secondMapId);
void func_0205f538(void);

/* Map that goes with the selected one in a two-map battle; 0 if none. */
#define SECOND_MAP_ID() (Ov000_gMapHeaders[SELECTED_MAP_ID()].variantMapIds[0])

/* Takes the selected map for the battle, then removes the menu buttons
 * and starts a task (func_0205f538). In a two-map battle outside the War
 * Room, the map chosen before (firstMapId) comes first and the selected one
 * becomes the second map. */
void MapSelect_ConfirmMap(void) {
  Sound_PlaySe(0x10006);
  func_020b9890();
  if (STATE->gameMode == GAME_MODE_WAR_ROOM) {
    State_SetMapIds(SELECTED_MAP_ID(), SECOND_MAP_ID());
    STATE->unk_59 = TABLES.subStrip;
  } else if (STATE->twoMaps == 0) {
    State_SetMapIds(SELECTED_MAP_ID(), 0);
  } else {
    State_SetMapIds(gMenuState.setup.firstMapId, SELECTED_MAP_ID());
  }
  func_0205f538();
}
