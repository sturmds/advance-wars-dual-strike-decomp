#include "types.h"
#include "game/task.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0 *data_021694c0;

void func_020042d8(u16 arg, Task *parent);

/* Starts the func_020042d8 child task of parent with argument 30, unless the
 * scene's unk_04 or unk_08 is 9. */
void func_020f3804(Task *parent)
{
    if (data_021694c0->unk_04 == 9) {
        return;
    }
    if (data_021694c0->unk_08 == 9) {
        return;
    }
    func_020042d8(30, parent);
}
