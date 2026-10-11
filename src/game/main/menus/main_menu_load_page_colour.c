#include "types.h"
#include "game/main_menu.h"

void GX_BeginLoadTexPltt(void);
void GX_EndLoadTexPltt(void);
void Palette_Load(const void *src, int offset, u32 size);

#define VRAM_LCDC_F ((u16 *)0x06890000)
#define WORK ((MainMenuWork *)data_0229c71c)

/* Copies colour 4 of the page's palette into colour 11 of the 18 palettes
 * in bank F, and into the palette RAM entry 0x228. */
void MainMenu_LoadPageColour(void)
{
    MainMenuPage *page = *data_0229c71c;
    int i;

    GX_BeginLoadTexPltt();
    for (i = 0; i < 18; i++) {
        (&VRAM_LCDC_F[i * 16])[11] = WORK->unk_08[page->palette];
    }
    GX_EndLoadTexPltt();
    Palette_Load(&WORK->unk_08[page->palette], 0x228, 2);
}
