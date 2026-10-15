#include "types.h"
#include "game/Unk_02080440.h"
#include "game/screen_other_map.h"
#include "game/icon.h"

BOOL HotSeat_HasSeveralHumanTeams(void);
void Script_StartOnBothScreens(void);
int Player_GetNextInGame(int player);

/* Handler for game state 2. Starts the script on both screens when human
 * players of different teams share the DS, runs the screen config's unk_38
 * callback, loads the next player's army icon (79 + armyColour) into the
 * first screen's OBJ VRAM and switches to state 3, the next turn screen. */
void Game_State2_LoadNextArmyIcon(void) {
  Unk_027c0280* saved0280;
  u8 army;

  if (HotSeat_HasSeveralHumanTeams()) {
    Script_StartOnBothScreens();
  }
  if (data_027c027c->unk_50[data_027c0284]->unk_38 != NULL) {
    data_027c027c->unk_50[data_027c0284]->unk_38();
  }
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  army = MAP_STATE->players[Player_GetNextInGame(MAP_STATE->currentPlayer)]
             .armyColour;
  IconBank_Init(1, data_027c0280->objVram, 404, 19);
  Icon_Load(army + 79);
  Screen_SetCurrentGfx(saved0280);
  MAP_STATE->unk_18 = 3;
}
