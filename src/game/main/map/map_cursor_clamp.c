#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Keeps the map cursor on the map. */
void MapCursor_ClampToMap(void) {
  if (GAME->cursorX < 0) {
    GAME->cursorX = 0;
  }
  if (GAME->cursorX >= GAME->unk_1c4->mapWidth) {
    GAME->cursorX = GAME->unk_1c4->mapWidth - 1;
  }
  if (GAME->cursorY < 0) {
    GAME->cursorY = 0;
  }
  if (GAME->cursorY >= GAME->unk_1c4->mapHeight) {
    GAME->cursorY = GAME->unk_1c4->mapHeight - 1;
  }
}
