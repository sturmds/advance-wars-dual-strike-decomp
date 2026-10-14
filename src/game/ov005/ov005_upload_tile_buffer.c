#include "types.h"
#include "game/Unk_ov005_0235b574.h"
#include "sdk/os.h"

void Ov005_DrawGlyphToTexture(Unk_ov005_0235b574* work, int cell, int glyph);
void GX_BeginLoadTex(void); /* guess: begins a VRAM load */
void GX_LoadTex(const void* src, u32 offset, u32 size);
void GX_EndLoadTex(void); /* guess: ends the load */

void Ov005_UploadTileBuffer(Unk_ov005_0235b574* work) {
  int i;

  for (i = 0; i < OV005_TEXTURE_GLYPHS; i++) {
    Ov005_DrawGlyphToTexture(work, i, i);
  }
  DC_FlushRange(work->buffer_6010, sizeof(work->buffer_6010));
  GX_BeginLoadTex();
  GX_LoadTex(work->buffer_6010, 0, sizeof(work->buffer_6010));
  GX_EndLoadTex();
}
