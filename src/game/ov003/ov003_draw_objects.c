#include "types.h"
#include "game/ov003_object.h"

void func_ov003_0235a670(Ov003Object *object);

void Ov003_DrawObjects(void)
{
    Ov003Object *object = data_ov003_02369738;
    int i;

    for (i = 0; i < OV003_OBJECT_COUNT; i++, object++) {
        if (object->active != 0) {
            func_ov003_0235a670(object);
        }
    }
}
