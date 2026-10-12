#include "types.h"
#include "sdk/card_arm7.h"

void CardRom_LoadCommandBytes_Arm7(u32 commandHigh, u32 commandLow);
int CardRom_TakeFromBufferedPage_Arm7(CARDiRomPage *page);

/* Reads the requested ROM range one 512 byte block at a time. A block goes straight to the destination when that is word aligned and a whole block fits; otherwise it goes through the page buffer. */
void CardRom_ReadPagesByCpu_Arm7(CARDiRomPage *page)
{
    CARDiArm7Common *common = &data_arm7_03809280;
    u32 *out;
    u32 *pageWords = (u32 *)page->data;

    for (;;) {
        u32 pageStart = common->src & ~(GAMECARD_ROM_PAGE_BYTES - 1);
        u32 i;
        u32 control;

        if (pageStart != common->src || ((u32)(out = (u32 *)common->dst) & 3) != 0 || common->len < GAMECARD_ROM_PAGE_BYTES) {
            out = pageWords;
            page->address = pageStart;
        }
        CardRom_LoadCommandBytes_Arm7((pageStart >> 8) | ((u32)CARD_CMD_READ_PAGE << 24), pageStart << 24);
        i = 0;
        IO_GAMECARD_ROM_CTRL = page->control;
        do {
            control = IO_GAMECARD_ROM_CTRL;
            if (control & CARD_DATA_READY) {
                u32 word = IO_GAMECARD_READ_PORT;

                /* the bound is a byte size, though the buffer counts words */
                if (i < GAMECARD_ROM_PAGE_BYTES) {
                    out[i] = word;
                    i++;
                }
            }
        } while (control & CARD_START);
        if ((u8 *)out == common->dst) {
            u32 left;

            common->src += GAMECARD_ROM_PAGE_BYTES;
            common->dst += GAMECARD_ROM_PAGE_BYTES;
            left = common->len - GAMECARD_ROM_PAGE_BYTES;
            common->len = left;
            if (left == 0) {
                break;
            }
        } else if (CardRom_TakeFromBufferedPage_Arm7(page) == 0) {
            break;
        }
    }
}
