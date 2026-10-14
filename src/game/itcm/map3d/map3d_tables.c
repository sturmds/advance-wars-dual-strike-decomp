#include "types.h"
#include "text_data.h"

/* The values come from the ROM at build time (config/AWRE/data_inc.txt).
 * mwcc emits these tables smallest first, which is also their order in the
 * ROM. */
#pragma section TEXT_DATA begin

/* 32 zero bytes that Map3d_LoadTextures copies over texture VRAM to clear
 * it. */
u32 gMap3dClearBlock[] = {
#include "data/itcm/gMap3dClearBlock.inc.c"
};

/* The 32 colours Map3d_InitScreen loads into the toon table. */
u16 gMap3dToonTable[] = {
#include "data/itcm/gMap3dToonTable.inc.c"
};

/* Texture block of each unit type, one row of 27 per army style
 * (Unit3d_GetTextureOffset). */
u32 gUnit3dTextureBlocks[][27] = {
#include "data/itcm/gUnit3dTextureBlocks.inc.c"
};

#pragma section TEXT_DATA end
