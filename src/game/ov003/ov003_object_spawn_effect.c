#include "types.h"
#include "game/ov003_effect.h"
#include "game/ov003_unit.h"

extern Ov003Object data_ov003_02369bb8[];
extern Ov003EffectGroup data_ov003_02369738[];
extern u8 data_ov003_02364d8a[][20];
extern u8 data_ov003_02364d8b[][20];
extern u8 data_ov003_02364d8d[][20];

/* Starts an effect in the first free entry of the object's group, as many
 * entries as its kind allows (data_ov003_02364d8d). Does nothing when all are
 * in use. The effect starts where the object is, with the given velocity. */
void Ov003Object_SpawnEffect(Ov003Object *object, int velX, int velY)
{
    Ov003Effect *effect;
    int i;
    int count;

    effect = data_ov003_02369738[object + 16 - data_ov003_02369bb8].effects;
    count = data_ov003_02364d8d[object->kind][0];
    for (i = 0; i < count; i++, effect++) {
        if (effect->lifetime == 0) {
            break;
        }
    }
    if (i == count) {
        return;
    }
    effect->x = (object->x << 7) + 64;
    effect->y = (object->y << 7) + 64;
    effect->kind = object->kind;
    effect->velX = velX;
    effect->velY = velY;
    effect->lifetime = data_ov003_02364d8b[object->kind][0];
    effect->unk_0d = data_ov003_02364928->row;
    effect->unk_08 = 0;
    effect->owner = (object - data_ov003_02369bb8) | 0x80;
    object->unk_0a = data_ov003_02364d8a[object->kind][0];
}
