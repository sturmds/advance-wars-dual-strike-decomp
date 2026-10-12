#include "types.h"
#include "game/ov003_effect.h"
#include "game/ov003_unit.h"

extern Ov003EffectGroup data_ov003_02369738[];
extern u8 data_ov003_02364d8a[][20];
extern u8 data_ov003_02364d8b[][20];
extern u8 data_ov003_02364d8c[][20];
extern u8 data_ov003_02364d8d[][20];
extern Ov003Unit data_ov003_02369538[];

u32 Math_Sqrt32(u32 value);
void Ov003_StepToward(int a0, int a1, int x, int y, int limit, int* outX,
                      int* outY);

/* Starts an effect for the unit in the first free entry of its group, with the
 * given velocity. Kind 5 limits the speed to its table value (data_ov003_02364d8c).
 * A zero velocity on the other kinds becomes one unit step the way the unit
 * faces (facing: 1 up, 2 down, 3 left, 4 right). */
void Ov003Unit_SpawnEffect(Ov003Unit* unit, int velX, int velY) {
  Ov003Effect* effect;
  int newX;
  int limit;
  int newY;
  int i;
  int unitIndex;
  int count;

  unitIndex = unit - data_ov003_02369538;
  effect = data_ov003_02369738[unitIndex].effects;
  count = data_ov003_02364d8d[unit->kind][0];
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
  effect->velX = velX;
  effect->velY = velY;
  if (effect->kind == 5) {
    limit = (data_ov003_02364d8c[effect->kind][0] << 7) >> 3;
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
  effect->lifetime = data_ov003_02364d8b[unit->kind][0];
  effect->row = unit->row;
  effect->unk_08 = 0;
  effect->owner = unitIndex;
  unit->unk_15 = data_ov003_02364d8a[unit->kind][0];
  unit->unk_17_0 = 0;
  unit->unk_11 = 30;
}
