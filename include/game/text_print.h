#ifndef GAME_TEXT_PRINT_H
#define GAME_TEXT_PRINT_H

#include "types.h"

/* Both start the text printer task at tile (x, y) of a BG screen map and
 * return its script slot (from ScriptSlot_Start). */
s8 Text_Print(s16 x, s16 y, u16* map, const void* text, u16 unk_30, u16 unk_32);
s8 Text_PrintId(s16 x, s16 y, u16* map, u32 textId, u16 unk_30, u16 unk_32);

/* Width in pixels of text in Text_Print's font: each glyph's width plus 1
 * between glyphs. */
int Text_GetPrintWidth(const void* text);

#endif
