#include "types.h"
#include "game/Unk_02088d04.h"

void Sprites_ResetLists(void);
void OamBuffer_Split(int headCount);
void Font_LoadDigits(int a);
void Gfx_LoadBmap09f(void *dst, int unused, int paletteSlot);
void Gfx_RequestLayerCopy(int layer);
void Wallpaper_LoadSingle(void);
void SecondScreenTask_Create(int a);
void UnitInfoPanel_ClearUnit(void);
int Link_IsActive(void);
void SpriteTask_Create(int x, int y, int tile, int paletteSlot);

extern Unk_027c0280 *data_027c0280;

void ScreenOverlays_Create(void)
{
    Sprites_ResetLists();
    OamBuffer_Split(0);
    Font_LoadDigits(0);
    Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
    Gfx_RequestLayerCopy(1);
    Wallpaper_LoadSingle();
    SecondScreenTask_Create(0);
    UnitInfoPanel_ClearUnit();
    if (Link_IsActive() == 1) {
        SpriteTask_Create(4, 4, 0x3a0, 15);
    }
}
