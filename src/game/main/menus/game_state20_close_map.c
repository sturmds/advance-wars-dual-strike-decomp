#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440_48 data_02183b08;

BOOL EventScript_IsRunning(void);
void func_020b9e30(void);
void Game_CloseFirstMap(void);

/* Handler for game state 20, once no event script is running. Marks this
 * map as over (Unk_02080440_1c4.unk_30, which Map_IsThisMapsTurn reads on the
 * other map). The first map then gets Game_CloseFirstMap. */
void Game_State20_CloseMap(void) {
  if (EventScript_IsRunning()) {
    return;
  }
  MAP_STATE->unk_1c4->unk_30 = 1;
  if (MAP_STATE == &data_02183b08) {
    Game_CloseFirstMap();
  } else {
    func_020b9e30();
  }
}
