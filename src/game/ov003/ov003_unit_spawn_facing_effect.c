#include "types.h"
#include "game/ov003_effect.h"
#include "game/ov003_unit.h"

void Sound_PlaySe(int se);

/* Starts an effect for the unit in the first free entry of its group, moving
 * the way the unit faces (facing: 1 up, 2 down, 3 left, 4 right). Also sets
 * the unit's unk_11 to 30. The sound plays only for the current slot's unit. */
void Ov003Unit_SpawnFacingEffect(Ov003Unit* unit) {
  Ov003Effect* effect;
  int i;
  int count;
  int unitIndex;
  int speed;

  unitIndex = unit - data_ov003_02369538;
  effect = OV003_EFFECT_GROUP(unitIndex);
  count = data_ov003_02364d88[unit->kind].burstLength;
  for (i = 0; i < count; i++, effect++) {
    if (effect->lifetime == 0) {
      break;
    }
  }
  if (i == count) {
    return;
  }
  effect->x = unit->x;
  effect->y = unit->y;
  effect->kind = unit->kind;
  speed = (data_ov003_02364d88[effect->kind].reach << 7) >> 3;
  switch (unit->facing) {
    case 1:
      effect->velX = 0;
      effect->velY = -speed;
      break;
    case 2:
      effect->velX = 0;
      effect->velY = speed;
      break;
    case 3:
      effect->velX = -speed;
      effect->velY = 0;
      break;
    case 4:
      effect->velX = speed;
      effect->velY = 0;
      break;
  }
  effect->lifetime = data_ov003_02364d88[unit->kind].effectLifetime;
  effect->row = unit->row;
  effect->unk_08 = 0;
  effect->owner = unitIndex;
  unit->unk_15 = data_ov003_02364d88[unit->kind].unk_02;
  unit->unk_17_0 = 0;
  unit->unk_11 = 30;
  if (unitIndex != data_ov003_02364928->currentSlot) {
    return;
  }
  switch (effect->kind) {
    case 1:
    case 2:
    case 3:
      Sound_PlaySe(245);
      break;
    case 4:
    case 5:
      Sound_PlaySe(246);
      break;
  }
}
