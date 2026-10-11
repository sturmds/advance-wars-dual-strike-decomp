#include "types.h"
#include "game/task.h"
#include "game/Unk_021694c0.h"

extern Unk_021694c0 *data_021694c0;

void func_0200432c(u16 arg, Task *parent);

/* Starts the func_0200432c child task of parent with argument 30, unless the
 * scene's unk_08 or unk_0c is 9. */
void func_020f3858(Task *parent)
{
    if (data_021694c0->unk_08 == 9) {
        return;
    }
    if (data_021694c0->unk_0c == 9) {
        return;
    }
    func_0200432c(30, parent);
}
