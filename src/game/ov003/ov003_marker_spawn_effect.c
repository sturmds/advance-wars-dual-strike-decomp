#include "types.h"
#include "game/ov003_effect.h"
#include "game/ov003_marker.h"
#include "game/ov003_unit.h"

/* Starts an effect in the first free entry of the marker's group, as many
 * entries as its kind allows (Ov003TileInfo.burstLength). Does nothing when all are
 * in use. The effect starts where the marker is, with the given velocity. */
void Ov003Marker_SpawnEffect(Ov003Marker* marker, int velX, int velY) {
  Ov003Effect* effect;
  int i;
  int count;

  effect = OV003_EFFECT_GROUP(marker + OV003_UNIT_COUNT - data_ov003_02369bb8);
  count = data_ov003_02364d88[marker->kind].burstLength;
  for (i = 0; i < count; i++, effect++) {
    if (effect->lifetime == 0) {
      break;
    }
  }
  if (i == count) {
    return;
  }
  effect->x = (marker->x << 7) + 64;
  effect->y = (marker->y << 7) + 64;
  effect->kind = marker->kind;
  effect->velX = velX;
  effect->velY = velY;
  effect->lifetime = data_ov003_02364d88[marker->kind].effectLifetime;
  effect->row = data_ov003_02364928->row;
  effect->unk_08 = 0;
  effect->owner = (marker - data_ov003_02369bb8) | 0x80;
  marker->unk_0a = data_ov003_02364d88[marker->kind].unk_02;
}
