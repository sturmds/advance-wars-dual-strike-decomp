#include "types.h"
#include "game/ov003_object.h"

int func_020279c4(int x1, int y1, int x2, int y2);
void func_ov003_02361fc0(int x1, int y1, int x2, int y2, int length, int *outX, int *outY);
void func_ov003_0235ad3c(Ov003Object *object);

/* Kind 7: moves the object along the vector (offsetX, offsetY) by active
 * units per frame. On the frame it would pass the end of the vector it clears
 * active, moves nothing and calls func_ov003_0235ad3c. */
void Ov003Object_Travel(Ov003Object *object)
{
    int deltaX;
    int deltaY;
    int step;
    int travelled;
    int distance = func_020279c4(object->offsetX * 8, object->offsetY * 8, 0, 0);

    step = object->active;
    travelled = object->travelled;
    if (travelled + step > distance) {
        object->active = 0;
        step = distance - travelled;
    }
    func_ov003_02361fc0(0, 0, object->offsetX, object->offsetY, step, &deltaX, &deltaY);
    if (object->active != 0) {
        object->x += deltaX;
        object->y += deltaY;
        object->travelled += object->active;
    } else {
        func_ov003_0235ad3c(object);
    }
}
