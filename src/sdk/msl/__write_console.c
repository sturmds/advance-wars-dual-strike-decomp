/* needed to match: the original did not strength-reduce the buffer pointer. */
#pragma opt_strength_reduction off

#include "types.h"
#include "sdk/msl_file.h"

void Semihost_WriteChar(const char *charPtr);

/* Console write proc: sends each byte to the debugger console. */
int __write_console(u32 handle, u8 *buffer, u32 *count, void *refCon)
{
    u32 written;
    u32 total = *count;

    for (written = 0; written < total; written++) {
        Semihost_WriteChar((char *)buffer + written);
    }
    return 0;
}
