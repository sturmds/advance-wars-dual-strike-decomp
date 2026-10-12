/* WIP, 99.7%: only dst and len swap registers (r4/r8). */
/* Needed to match: the CARD library was built with propagation off, which keeps the flags below as values. */
#pragma opt_propagation off
#include "sdk/card.h"
#include "sdk/os.h"

extern u8 Oam_GetShapeCells[];   /* start of ITCM: the first function placed there */

u32 Cp15_DataTcmBase(void);         /* start of DTCM */
void IC_InvalidateRange(void *start, u32 size);
void DC_WaitWriteBufferEmpty(void);
void CardRom_OnPageDmaDone(void);
void CardRom_StartPageDma(void);

#define ITCM_START ((u32)Oam_GetShapeCells)
#define ITCM_END 0x02000000
#define DTCM_SIZE 0x4000
#define HEADER_COPY_ROM_CTRL_WORD (*(volatile u32 *)0x027ffe60)   /* card control word from the ROM header */

/* Probably CardRom_ReadViaDmaIfAligned: uses DMA when dst is word aligned, the channel
 * is 0-3, dst..dst+len is outside ITCM and DTCM, and src and len are whole
 * pages; then starts the first page read. */
BOOL func_021162d8(CARDiRomPage *page)
{
    CARDiCommon *common = &data_022abb00;
    u32 dst = common->dst;
    BOOL useDma = FALSE;
    BOOL pageAligned = FALSE;
    BOOL rangeOk = FALSE;
    BOOL dmaChannel = useDma;     /* needed to match: FALSE, but copied from useDma */
    int state;

    if ((dst & 3) == 0 && common->unk_24 <= 3) {
        dmaChannel = TRUE;
    }
    if (dmaChannel) {
        u32 len = common->len;
        u32 dtcm = Cp15_DataTcmBase();
        BOOL overlaps = TRUE;
        u32 end = len + dst;
        BOOL inItcm = FALSE;

        if (end > ITCM_START && dst < ITCM_END) {
            inItcm = overlaps;    /* needed to match: TRUE, but copied from overlaps */
        }

        if (!inItcm) {
            if (dtcm >= end || dtcm + DTCM_SIZE <= dst) {
                overlaps = FALSE;
            }
        }
        if (!overlaps) {
            rangeOk = TRUE;
        }
    }
    if (rangeOk) {
        if (((common->src | common->len) & (GAMECARD_ROM_PAGE_BYTES - 1)) == 0) {
            pageAligned = TRUE;
        }
    }
    if (pageAligned) {
        if (common->len != 0) {
            useDma = TRUE;
        }
    }
    page->control = (HEADER_COPY_ROM_CTRL_WORD & ~CARD_COMMAND_MASK) | CARD_START | CARD_RESET_HI | GAMECARD_XFER_ONE_PAGE;
    if (useDma) {
        state = OS_DisableInterrupts();
        DC_StoreRange((void *)common->dst, common->len);
        DC_InvalidateRange((void *)common->dst, common->len);
        IC_InvalidateRange((void *)common->dst, common->len);
        OS_SetIrqFunction(0x80000, CardRom_OnPageDmaDone);
        OS_ResetRequestIrqMask(0x80000);
        OS_EnableIrqMask(0x80000);
        DC_WaitWriteBufferEmpty();
        OS_RestoreInterrupts(state);
        CardRom_StartPageDma();
    }
    return useDma;
}
