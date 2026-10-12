#include "types.h"
#include "game/map_object.h"
#include "game/Unk_02080440.h"
#include "game/vision.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void VisionGrid_Reset(s8* grid, int value);

#define STATE (data_027c027c->unk_48[data_027c0284])

static inline BOOL Map_IsFoggy(void) {
  BOOL foggy = TRUE;
  Unk_02080440_1c4* info = STATE->unk_1c4;

  if (info->fog == 0 && info->weather != WEATHER_RAIN) {
    foggy = FALSE;
  }
  return foggy;
}

/* Rebuilds both vision grids (unk_20 and unk_22). Without fog or rain every
 * tile is visible. In fog or rain the grids start dark, then each player
 * 1-4 adds what its units and properties see, and every tile of a map object
 * is lit. */
void Map_RebuildVision(void) {
  MapObject* object;
  int endX;
  int endY;
  int x;
  int y;

  VisionGrid_Reset(VISION_GRID(STATE, STATE->visionGrid0), (s16)!Map_IsFoggy());
  VisionGrid_Reset(VISION_GRID(STATE, STATE->visionGrid1), (s16)!Map_IsFoggy());
  if (!Map_IsFoggy()) {
    return;
  }
  Player_AddVision(1);
  Player_AddVision(2);
  Player_AddVision(3);
  Player_AddVision(4);
  for (object = STATE->objects; object->kind != 0; object++) {
    endX = object->x + object->width;
    endY = object->y + object->height;
    for (x = object->x; x < endX; x++) {
      for (y = object->y; y < endY; y++) {
        Vision_AddArea(x, y, 0, 3, 1, 0);
      }
    }
  }
}
