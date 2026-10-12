#include "types.h"
#include "game/ov003_unit.h"
#include "game/ov003_marker.h"

extern Ov003TileInfo data_ov003_02364d88[];

Ov003Unit* Ov003Marker_FindNearestUnit(Ov003Marker* marker);
void Ov003Marker_SpawnEffect(Ov003Marker* marker, int velX, int velY);
int Math_Distance(int x1, int y1, int x2, int y2);
void Ov003_StepToward(int x1, int y1, int x2, int y2, int length, int* outX,
                      int* outY);

/* Fires one effect per frame in a burst of Ov003TileInfo.burstLength frames
 * (burstCount counts them). The first frame picks the nearest unit as the
 * target and aims at it; later frames aim at the same unit, the vector cut to
 * Ov003TileInfo.reach tiles. The burst ends, and cooldown starts its wait of
 * burstWait frames, when the count is reached or the target is gone. Needed to
 * match: the declaration order. */
void Ov003Marker_FireBurst(Ov003Marker* marker) {
  int shotX;
  Ov003Unit* target;
  int shotY;
  int dx;
  int reach;
  int dy;

  if (marker->burstCount != 0) {
    if (marker->target == NULL) {
      marker->burstCount = 0;
      marker->cooldown = data_ov003_02364d88[marker->kind].burstWait;
      return;
    }
    reach = (data_ov003_02364d88[marker->kind].reach << 7) >> 3;
    dx = (marker->target->x >> 3) - (((marker->x << 7) + 64) >> 3);
    dy = (marker->target->y >> 3) - (((marker->y << 7) + 64) >> 3);
    if (reach < Math_Distance(dx, dy, 0, 0)) {
      Ov003_StepToward(0, 0, dx, dy, reach, &shotX, &shotY);
      dx = shotX;
      dy = shotY;
    }
  } else {
    target = Ov003Marker_FindNearestUnit(marker);
    if (target == NULL) {
      return;
    }
    dx = (target->x >> 3) - (((marker->x << 7) + 64) >> 3);
    dy = (target->y >> 3) - (((marker->y << 7) + 64) >> 3);
    marker->target = target;
  }
  Ov003Marker_SpawnEffect(marker, dx, dy);
  marker->burstCount++;
  if (marker->burstCount != data_ov003_02364d88[marker->kind].burstLength) {
    return;
  }
  marker->burstCount = 0;
  marker->cooldown = data_ov003_02364d88[marker->kind].burstWait;
}
