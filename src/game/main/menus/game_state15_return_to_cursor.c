#include "types.h"
#include "game/Unk_02080440.h"
#include "game/menu_button.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void Sound_PlaySe(int se);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Game state 15 sub-state change: closes the menu buttons, plays the cancel
 * sound and sets unk_1c back to 0 (the free cursor). */
void Game_State15_ReturnToCursor(void) {
  MenuButtons_CloseAll();
  Sound_PlaySe(0x10009);
  GAME->unk_1c = 0;
}
