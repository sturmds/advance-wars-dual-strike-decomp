#include "types.h"
#include "sdk/os.h"

void DC_WaitWriteBufferEmpty(void);
int MbParent_SendMpData(void *data, int size, int bitmap);

/* Writes the data out of the cache, rounded up to 32-byte lines, then sends
 * it to the children in bitmap. */
void func_0211b458(int size, int bitmap, void *data)
{
    DC_FlushRange(data, (size + 31) & ~31);
    DC_WaitWriteBufferEmpty();
    MbParent_SendMpData(data, size, bitmap);
}
