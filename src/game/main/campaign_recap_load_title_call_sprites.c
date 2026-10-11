#include "types.h"
#include "game/campaign_recap.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280 *data_027c0280;
extern const char data_0216a980[];

u8 *File_Load(const char *path);
void Data_Unpack(const void *src, void *dst);
void Mem_Free(void *block);
void Palette_Load(const void *src, u32 offset, u32 size);

/* Loads the res_titlecall sprites: tiles at the start of OBJ VRAM and two
 * 16-colour palettes. work is unused. */
void CampaignRecap_LoadTitleCallSprites(CampaignRecap *work)
{
    u8 *file = File_Load(data_0216a980);

    Data_Unpack(file, data_027c0280->objVram);
    Palette_Load(file + 0x7d8, 0x200, 0x20);
    Palette_Load(file + 0x35d8, 0x220, 0x20);
    Mem_Free(file);
}
