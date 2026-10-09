#include "types.h"

void Mem_Free(void *block);

typedef struct Unk_020fd3c0 {
    u8 pad_00[0x68];
    void *buffer;
} Unk_020fd3c0;

/* Frees the owner buffer at offset 0x68 if one was allocated. */
void func_020fd3c0(Unk_020fd3c0 *owner) {
    if (owner->buffer == NULL) {
        return;
    }
    Mem_Free(owner->buffer);
}
