#ifndef GAME_RANGE_MAP_H
#define GAME_RANGE_MAP_H

/* Unk_02080440_48.rangeMap holds one row pointer per map row. The range
 * fills write through those pointers, so pointing the rows at another
 * per-tile buffer (MapTiles.rangeScratch, rangeScratch2, threatSum) makes
 * the next fill land there. MapTiles.rangeMapTiles is the default. */

/* Points rangeMap's rows into tileMap, one byte per tile. */
void RangeMap_SetBuffer(void *tileMap);

/* Sets every tile that rangeMap points to to value. */
void RangeMap_Fill(int value);

/* Sets every tile of a per-tile map to value, without touching rangeMap. */
void TileMap_Fill(void *tileMap, int value);

#endif
