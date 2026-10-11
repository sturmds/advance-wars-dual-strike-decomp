#include "types.h"
#include "game/design_map.h"
#include "game/screen_other_map.h"

extern DesignMapRecord data_02291774;
extern u8 data_021674c4[];

void DesignMap_BuildRecord(void *template, DesignMapRecord *record, int flag);
void Map_SetDataBuffer(void *buffer);
void Player_SaveArmyColours(void);
void MapPreview_Start(void *vram, u16 *tileMap, int firstTile, int palette);
void Gfx_RequestLayerCopy(int layer);

/* Builds a map record from the template data_021674c4 and draws its preview
 * (a guess from the callers, the Design Room screens) at tile (x, y) of layer 0, using tiles from firstTile on and
 * the given palette. */
void DesignMap_DrawTemplatePreview(int x, int y, int firstTile, int palette)
{
    DesignMapRecord *const record = &data_02291774; /* const: needed to match */

    DesignMap_BuildRecord(data_021674c4, record, 1);
    Map_SetDataBuffer(record);
    Player_SaveArmyColours();
    MapPreview_Start(data_027c0280->bgVram + (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14),
                  data_027c0280->layer0Map + (x + y * 32), firstTile, palette);
    Gfx_RequestLayerCopy(0);
}
