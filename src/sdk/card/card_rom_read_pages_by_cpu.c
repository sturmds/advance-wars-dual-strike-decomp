#pragma opt_propagation off
#include "sdk/card.h"

void CardRom_LoadCommandBytes(u32 commandHigh, u32 commandLow);   /* sends the first four, then the last four command bytes */
BOOL CardRom_TakeFromBufferedPage(CARDiRomPage *page);

/* Reads ROM pages with the CPU, straight into dst when src and dst are aligned and a full page is left, else through the page buffer. */
void CardRom_ReadPagesByCpu(CARDiRomPage *romPage)
{
    CARDiCommon *common = &data_022abb00;
    u32 i;
    u32 cnt;
    u32 *dst;
    u32 *buffer = (u32 *)romPage->data;

    while (TRUE) {
        u32 page = common->src & -GAMECARD_ROM_PAGE_BYTES;

        if (page != common->src || ((u32)(dst = (u32 *)common->dst) & 3) || common->len < GAMECARD_ROM_PAGE_BYTES) {
            dst = buffer;
            romPage->address = page;
        }
        CardRom_LoadCommandBytes((CARD_CMD_READ_PAGE << 24) | (page >> 8), page << 24);
        IO_GAMECARD_ROM_CTRL = romPage->control;
        i = 0;
        do {
            cnt = IO_GAMECARD_ROM_CTRL;
            if (cnt & CARD_DATA_READY) {
                u32 word = IO_GAMECARD_READ_PORT;

                if (i < GAMECARD_ROM_PAGE_BYTES) {
                    dst[i++] = word;
                }
            }
        } while (cnt & CARD_START);

        if (dst == (u32 *)data_022abb00.dst) {
            /* needed to match: the original reads and writes the card work by its symbol here */
            u32 nextSrc = common->src + GAMECARD_ROM_PAGE_BYTES;
            u32 nextDst = common->dst + GAMECARD_ROM_PAGE_BYTES;
            u32 left = common->len - GAMECARD_ROM_PAGE_BYTES;

            data_022abb00.src = nextSrc;
            data_022abb00.dst = nextDst;
            data_022abb00.len = left;
            if (left == 0) {
                return;
            }
        } else if (!CardRom_TakeFromBufferedPage(romPage)) {
            return;
        }
    }
}
