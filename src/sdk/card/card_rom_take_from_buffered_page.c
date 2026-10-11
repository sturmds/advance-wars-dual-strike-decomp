#include "sdk/card.h"

void Mem_CopyBytes(const void *src, void *dst, u32 size);

/* Copies the part of the read that lies in the buffered page; TRUE while bytes are left. */
BOOL CardRom_TakeFromBufferedPage(CARDiRomPage *page)
{
    CARDiCommon *common = &data_022abb00;
    u32 pageStart = common->src & ~(GAMECARD_ROM_PAGE_BYTES - 1);

    if (pageStart == page->address) {
        u32 offset = common->src - pageStart;
        u32 size = GAMECARD_ROM_PAGE_BYTES - offset;

        if (size > common->len) {
            size = common->len;
        }
        Mem_CopyBytes(page->data + offset, (void *)common->dst, size);
        common->src += size;
        common->dst += size;
        common->len -= size;
    }
    return common->len != 0;
}
