#include "types.h"
#include "game/ov003_object.h"

void func_ov003_0235a810(Ov003Object *object);
void func_ov003_0235a8d8(Ov003Object *object);
void func_ov003_0235a9e8(Ov003Object *object);

void Ov003_UpdateObjects(void)
{
    Ov003Object *object = data_ov003_02369738;
    int i;

    for (i = 0; i < OV003_OBJECT_COUNT; i++, object++) {
        if (object->active != 0) {
            switch (object->kind) {
            case 5:
                func_ov003_0235a8d8(object);
                break;
            case 7:
                func_ov003_0235a810(object);
                break;
            default:
                func_ov003_0235a9e8(object);
                break;
            }
        }
    }
}
