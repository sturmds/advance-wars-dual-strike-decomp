#include "types.h"
#include "game/Unk_02080440.h"

void ObjGfx_LoadLanguageTiles(void);
void Gfx_LoadPlayerWindowFrame(int player);

/* Task callback: loads the language tiles and the current player's window frame. */
void PowerActivateTask_LoadWindowGfx(void) {
  ObjGfx_LoadLanguageTiles();
  Gfx_LoadPlayerWindowFrame(MAP_STATE->currentPlayer);
}
