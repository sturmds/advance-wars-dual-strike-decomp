#include "types.h"
#include "game/Unk_020d8c8c.h"

void func_0202244c(u32 *handle);

void func_020f28a8(Unk_020d8c8c *owner) {
    if (owner->handle == NULL) {
        return;
    }
    func_0202244c(owner->handle);
}
