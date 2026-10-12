#include "types.h"
#include "sdk/msl_file.h"

int Semihost_ReadChar(void);

/* Console read proc: reads bytes from the debugger console up to and including the first CR or LF. */
int __read_console(u32 handle, u8 *buffer, u32 *count, void *refCon)
{
    u32 read;
    u32 limit = *count;

    for (read = 0; read < limit; read++) {
        buffer[read] = Semihost_ReadChar();
        if (buffer[read] == '\r' || buffer[read] == '\n') {
            *count = read + 1;
            break;
        }
    }
    return 0;
}
