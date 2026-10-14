#ifndef GAME_RANGE_MAP_H
#define GAME_RANGE_MAP_H

/* Unk_02080440_48.rangeMap holds one row pointer per map row. The range
 * fills write through those pointers, so pointing the rows at another
 * per-tile buffer (MapTiles.rangeScratch, rangeScratch2, threatSum) makes
 * the next fill land there. MapTiles.rangeMapTiles is the default. */

/* Points rangeMap's rows into tileMap, one byte per tile. */
void RangeMap_SetBuffer(void* tileMap);

/* Sets every tile that rangeMap points to to value. */
void RangeMap_Fill(int value);

/* Sets every tile of a per-tile map to value, without touching rangeMap. */
void TileMap_Fill(void* tileMap, u8 value);

/* Direction of a RangeMap_FillCone cone, and the facing (variant) of the
 * cannon map objects. The Minicannons on Fortress Isle (map 0x0ae) face
 * away from the island this way. */
enum ConeDirection { CONE_DOWN, CONE_UP, CONE_LEFT, CONE_RIGHT };

/* Breadth-first search that fills rangeMap with move costs. Two lists of
 * nodes take turns: one holds the tiles to expand, the other collects the
 * tiles found from them. */
#define MOVE_SEARCH_LIST_SIZE 350

typedef struct MoveSearchNode {
  u8 x;
  u8 y;
  u8 from; /* 0: end of list, 1: the start tile, else the direction taken to get here (2 up, 3 down, 4 left, 5 right) */
  u8 cost; /* move cost of the path to this tile */
} MoveSearchNode;

/* Pointed to by data_027c0758. */
typedef struct MoveSearch {
  u8 moveCost
      [0x20]; /* per terrain id, filled by MoveSearch_LoadMoveCosts for the unit type */
  s16 maxCost;   /* cost limit of the search */
  s16 moveRange; /* movement of the unit per turn */
  u8 x;          /* tile being expanded */
  u8 y;
  u8 unitType; /* MoveSearch_FindTile: unit type searched for */
  u8 pad_27[1];
  MoveSearchNode lists[2][MOVE_SEARCH_LIST_SIZE];
} MoveSearch;

#endif
