#include "types.h"
#include "game/ov003_marker.h"

extern Ov003Marker data_ov003_02369bb8[4];

/* Clears hitDone of every busy marker, so the next impact can hit it again. */
void Ov003_ClearMarkerHits(void) {
  Ov003Marker* marker = data_ov003_02369bb8;
  int i;

  for (i = 0; i < 4; i++, marker++) {
    if (marker->busy) {
      marker->hitDone = 0;
    }
  }
}
