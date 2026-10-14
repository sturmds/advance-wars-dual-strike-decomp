#include "types.h"
#include "game/ov003_unit.h"
#include "game/ov003_marker.h"

extern Ov003Marker data_ov003_02369bb8[];

void Ov003Marker_Step(Ov003Marker* marker);

/* Steps every busy marker unless Ov003State.unk_1db_7 is set. */
void Ov003_StepMarkers(void) {
  Ov003Marker* marker = data_ov003_02369bb8;
  int i;

  if (data_ov003_02364928->unk_1db_7) {
    return;
  }
  for (i = 0; i < 4; i++, marker++) {
    if (marker->busy) {
      Ov003Marker_Step(marker);
    }
  }
}
