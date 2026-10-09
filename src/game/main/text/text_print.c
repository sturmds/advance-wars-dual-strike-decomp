#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280 *data_027c0280;
extern u8 data_022900b8;
extern u8 data_02158394[];

/* sets up printer to print text at tile (x, y) into map, with tile data at tiles */
void func_0201505c(void *printer, int x, int y, u8 *tiles, u16 *map, const void *text, u16 unk_30, u16 unk_32);
void func_02015da8(void *printer);
s8 func_02017698(const void *descriptor, int arg);    /* starts a task from descriptor */

/* Prints text at tile (x, y) of map, drawing the glyphs into the layer 0
 * tile data, through the printer in data_027c0280 and the task that
 * data_02158394 describes. */
void Text_Print(int x, int y, u16 *map, const void *text, u16 unk_30, u16 unk_32)
{
    Unk_027c0280 *gfx = data_027c0280;
    void *printer = gfx->textPrinter;

    data_022900b8 = 0;
    func_0201505c(printer, x, y, gfx->bgVram + (gfx->bgCnt[gfx->layer0Bg].charBase << 14), map, text, unk_30, unk_32);
    func_02015da8(printer);
    func_02017698(data_02158394, 0);
}
