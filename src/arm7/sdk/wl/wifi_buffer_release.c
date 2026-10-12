#include "types.h"
#include "sdk/wl.h"

void Heap_ReturnBlock_Arm7(s32 id, s32 heap, void *ptr);

/* Takes a buffer out of its list and frees it. Returns the error of WifiBufList_Unlink, 0 when the buffer was freed. */
int WifiBuffer_Release(WlBufferList *list, WlBuffer *buffer)
{
    WlBufferAllocator *allocator = &WIFI_DRIVER_STATE->allocator;
    int error;

    if (buffer->magic != WIFI_MSG_BUFFER_TAG) {
        return 1;
    }
    error = WifiBufList_Unlink(list, buffer);
    if (error == 0) {
        switch (allocator->kind) {
        case 0:
            Heap_ReturnBlock_Arm7(allocator->a.arena, allocator->b.heap, buffer);
            break;
        case 1:
            allocator->b.free(buffer);
            break;
        }
    }
    return error;
}
