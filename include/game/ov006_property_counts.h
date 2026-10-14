#ifndef GAME_OV006_PROPERTY_COUNTS_H
#define GAME_OV006_PROPERTY_COUNTS_H

#include "types.h"
#include "game/player.h"

/* Columns of the count table: the row icon, then five property tiles. */
#define COUNT_COLUMNS 6

/* The sprite tables Ov006_DrawPropertyCounts copies whole, as the code does:
 * the shape of each row's icon, the shape of each column, and the tile of
 * each cell. */
typedef struct RowShapes {
  void* shape[ARMY_SLOT_COUNT];
} RowShapes;

typedef struct ColumnShapes {
  void* shape[COUNT_COLUMNS];
} ColumnShapes;

typedef struct TileGrid {
  s16 tile[ARMY_SLOT_COUNT * COUNT_COLUMNS];
} TileGrid;

extern RowShapes data_ov006_0236aa70;
extern ColumnShapes data_ov006_0236aa84;
extern TileGrid data_ov006_0236aa9c;

#endif
