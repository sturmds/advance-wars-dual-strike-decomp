#include "types.h"
#include "game/task.h"
#include "game/terrain.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern TaskScriptCmd data_0216594c[];

#define GAME (data_027c027c->unk_48[data_027c0284])

void BuildMenu_FillList(int moveGroups);
void MenuDepth_Push(void);
void NumberFont_LoadLow(void);

/* Opens the unit build menu for the production tile (x, y): lists the unit
 * types of the tile's move groups (city, HQ and base: ground; airport: air;
 * port: sea), starts the menu task and pushes a menu level. */
void BuildMenu_Open(int x, int y) {
  int moveGroups = 0;

  switch (MAP_TILE_TERRAIN(
      GAME->mapTiles->tiles[x + GAME->mapTiles->rowStart[y]])) {
    case TERRAIN_CITY:
    case TERRAIN_HQ:
    case TERRAIN_BASE:
      moveGroups = MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD;
      break;
    case TERRAIN_AIRPORT:
      moveGroups = MOVE_GROUP_AIR;
      break;
    case TERRAIN_PORT:
      moveGroups = MOVE_GROUP_SEA;
      break;
  }
  BuildMenu_FillList(moveGroups);
  Task_Create(data_0216594c, TASK_LIST(3));
  MenuDepth_Push();
  NumberFont_LoadLow();
}
