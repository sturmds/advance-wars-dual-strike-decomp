#include "types.h"
#include "game/Unk_02080440.h"

/* Name of the current map: the map state record starts with a copy of it
 * (BattleSave_LoadMap copies Map_GetName there). */
const char* Map_GetCurrentName(void) {
  return (const char*)data_027c027c->unk_48[data_027c0284]->unk_1c4;
}
