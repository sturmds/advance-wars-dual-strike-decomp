#include "types.h"
#include "sdk/os.h"

void *func_0210e494(int id);
void *func_0210e4a8(int id);
void func_0210e25c(int id, void *lo);

/* Takes size bytes from the bottom of arena id, aligned to align. Returns NULL if it does not fit. */
void *OS_AllocFromArenaLo(int id, u32 size, u32 align)
{
    u32 start;
    u32 end;
    u32 newLo;
    u32 oldLo = (u32)func_0210e494(id);

    if (oldLo == 0) {
        return NULL;
    }
    start = (oldLo + align - 1) & ~(align - 1);
    end = start + size;
    newLo = (end + align - 1) & ~(align - 1);
    if (newLo > (u32)func_0210e4a8(id)) {
        return NULL;
    }
    func_0210e25c(id, (void *)newLo);
    return (void *)start;
}
