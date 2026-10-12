#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void ObjGfx_LoadLanguageTiles(void);
void Gfx_LoadPlayerWindowFrame(int player);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Task callback: loads the language tiles and the current player's window frame. */
void PowerActivateTask_LoadWindowGfx(void) {
  ObjGfx_LoadLanguageTiles();
  Gfx_LoadPlayerWindowFrame(GAME->currentPlayer);
}
