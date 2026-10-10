#include "types.h"
#include "game/screen_other_map.h"

/* Clears MapObject.variant (name still func_020da938). */
void func_020da938(MapObject *object);

/* Clears the variant of every kind-2 object on the other screen's map state. */
void OtherMapObjects_ClearKind2Variants(void)
{
    MapObject *object;
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx((&data_027c0288.unk_00)[1 - data_027c0284]);
    saved027c = data_027c027c;
    Screen_SetCurrent((&data_027c0288.unk_00)[1 - data_027c0284]);
    object = data_027c027c->unk_48[data_027c0284]->objects;
    while (object->kind != 0) {
        if (object->kind == 2) {
            func_020da938(object);
        }
        object++;
    }
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
}
