#include "types.h"
#include "game/ov003_marker.h"

void Ov003Marker_FireAtNearestUnit(Ov003Marker* marker);
void Ov003Marker_FireBurst(Ov003Marker* marker);

/* Runs the marker's action (kind 7 has its own) once both countdowns have reached
 * 0, then steps the countdowns and unk_09_6 down. */
void Ov003Marker_Step(Ov003Marker* marker) {
  if (marker->unk_0a == 0 && marker->cooldown == 0) {
    if (marker->kind == 7) {
      Ov003Marker_FireBurst(marker);
    } else {
      Ov003Marker_FireAtNearestUnit(marker);
    }
  }
  if (marker->unk_0a != 0) {
    marker->unk_0a--;
  }
  if (marker->cooldown != 0) {
    marker->cooldown--;
  }
  if (marker->unk_09_6 != 0) {
    marker->unk_09_6--;
  }
}
