#include "types.h"
#include "game/Unk_ov005_0235b574.h"
#include "sdk/os.h"
#include "sdk/gx_load.h"

void Ov005_DrawGlyphToTexture(Unk_ov005_0235b574* work, int cell, int glyph);

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
