#include "types.h"
#include "game/ov003_effect.h"
#include "game/ov003_unit.h"

extern Ov003Unit data_ov003_02369538[];

void Sound_PlaySe(int se);
u32 Math_Sqrt32(u32 value);
void Ov003_GetScrollToCentreUnit(Ov003Unit* unit, s16* x, s16* y);
void Ov003_StepToward(int a0, int a1, int x, int y, int limit, int* outX,
                      int* outY);

/* Same as Ov003Unit_SpawnEffect, but sets the velocity to the target minus the
 * unit's position (x >> 3, relative to the point Ov003_GetScrollToCentreUnit gives)
 * and the sound plays for the current slot's unit. */
void Ov003Unit_SpawnEffectTowardPoint(Ov003Unit* unit, int targetX,
                                      int targetY) {
  Ov003Effect* effect;
  int i;
  int count;
  int unitIndex;
  int limit;
  int newX;
  int newY;
  s16 originX;
  s16 originY;

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
  Ov003_GetScrollToCentreUnit(unit, &originX, &originY);
  effect->x = unit->x;
  effect->y = unit->y;
  effect->kind = unit->kind;
  effect->velX = targetX - ((unit->x >> 3) - originX);
  effect->velY = targetY - ((unit->y >> 3) - originY);
  if (effect->kind == 5) {
    limit = (data_ov003_02364d88[effect->kind].reach << 7) >> 3;
    if (Math_Sqrt32(effect->velX * effect->velX + effect->velY * effect->velY) >
        limit) {
      Ov003_StepToward(0, 0, effect->velX, effect->velY, limit, &newX, &newY);
      effect->velX = newX;
      effect->velY = newY;
    }
  } else if (effect->velX == 0 && effect->velY == 0) {
    switch (unit->facing) {
      case 1:
        effect->velX = 0;
        effect->velY = -1;
        break;
      case 2:
        effect->velX = 0;
        effect->velY = 1;
        break;
      case 3:
        effect->velX = -1;
        effect->velY = 0;
        break;
      case 4:
        effect->velX = 1;
        effect->velY = 0;
        break;
    }
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
