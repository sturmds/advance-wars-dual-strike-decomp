#include "types.h"
#include "game/Unk_02080440.h"

void Map_RebuildUnitTiles(void);
void func_020f79a4(void);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define HOOKS (data_027c027c->unk_50[data_027c0284])

/* Runs Map_RebuildUnitTiles (it derives view values from the map data), then the
 * two callbacks if set. Many menus and Map_PlacePredeployedUnits call it;
 * the purpose is not clear yet. */
void func_020c8888(void) {
  Map_RebuildUnitTiles();
  if (HOOKS->first != NULL) {
    HOOKS->first();
  }
  if (HOOKS->second != NULL) {
    HOOKS->second();
  }
  func_020f79a4();
}
