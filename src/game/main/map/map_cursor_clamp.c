#include "types.h"
#include "game/Unk_02080440.h"

/* Keeps the map cursor on the map. */
void MapCursor_ClampToMap(void) {
  if (MAP_STATE->cursorX < 0) {
    MAP_STATE->cursorX = 0;
  }
  if (MAP_STATE->cursorX >= MAP_STATE->unk_1c4->mapWidth) {
    MAP_STATE->cursorX = MAP_STATE->unk_1c4->mapWidth - 1;
  }
  if (MAP_STATE->cursorY < 0) {
    MAP_STATE->cursorY = 0;
  }
  if (MAP_STATE->cursorY >= MAP_STATE->unk_1c4->mapHeight) {
    MAP_STATE->cursorY = MAP_STATE->unk_1c4->mapHeight - 1;
  }
}
