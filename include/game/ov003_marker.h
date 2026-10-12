#ifndef GAME_OV003_MARKER_H
#define GAME_OV003_MARKER_H

#include "types.h"

/* One of four 12-byte entries at data_ov003_02369bb8 (overlay 3): a fixed
 * gun on a terrain 21 or 26 tile that fires at units in reach. Found by tile
 * with Ov003_FindMarkerAtTile. */
typedef struct Ov003Marker {
  struct Ov003Unit* target; /* unit it follows; NULL when none */
  s16 x; /* tile column of terrain 21 or 26 (Ov003_LoadMap) */
  s16 y;
  u8 kind : 4;     /* 6 on terrain 21, 7 on 26; indexes Ov003TileInfo */
  u8 unk_08_4 : 3; /* row of the tile */
  u8 busy : 1;     /* set while the entry is in use */
  u8 hitDone : 1;  /* already hit by the current impact (Ov003Object_Impact) */
  u8 burstCount : 5; /* kind 7: shots fired in the current burst */
  u8 unk_09_6 : 2;   /* not 0: its map tiles use BG palette 13 */
  u8 unk_0a;
  u8 cooldown; /* frames until it may fire again */
} Ov003Marker;

typedef char Ov003Marker_size_check[sizeof(Ov003Marker) == 12 ? 1 : -1];

#endif
