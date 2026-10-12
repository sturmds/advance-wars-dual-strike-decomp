#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov004_022bf158.h"

void FS_ChangeDir(const char* dir);
u32 Ov004_GetFileSize(const char* path);
void File_ReadTo(void* dest, const char* fileName);
void Mem_Copy32(const void* src, void* dst, u32 size);
int strcmp(const char* a, const char* b); /* strcmp */
void Co_LoadGraphicsToBuffer(
    int coId, void* dest); /* guess: unpacks the CO's picture to dest */
void Co_LoadPalette(int coId, int paletteSlot);

extern Ov004ResourceFile data_ov004_022becf8[];
extern char data_ov004_022bde38[]; /* "/syogun" */
extern u8* data_ov004_022bf150;

#define BUFFER_SIZE 0x80000
#define VRAM_SIZE 0x20000

/* Loads every file of the list at data_ov004_022becf8 into state->resources,
 * up to 128. Each goes into the 0x80000-byte buffer at unk_1320 while it
 * fits, then into the 0x20000 bytes from unk_1328; the first that fits in
 * neither stops the load. Entries in "/syogun" are made from a CO id instead
 * of read from a file: the CO's picture, or its palette as loaded into the
 * first screen's palette slot 0. Ov004_FindResource finds them by name. */
void Ov004_LoadResources(Unk_ov004_022bf158* state) {
  BOOL done;
  Unk_02080440* savedState;
  u32 size;
  u32 bufferUsed;
  BOOL fromFile;
  const char* name;
  void* src;
  u32 vramUsed;
  const char* dir;
  const char* fileName;
  Ov004Resource* resource;
  Unk_027c0280* savedGfx;
  u32 i;
  int coId;
  u32 code;

  state->bufferUsed = 0;
  state->vramUsed = 0;
  done = FALSE;
  state->resourceCount = 0;
  resource = state->resources;
  for (i = 0; i < 128; i++) {
    dir = data_ov004_022becf8[i].dir;
    if (dir == NULL) {
      done = TRUE;
      break;
    }
    fileName = data_ov004_022becf8[i].name;
    name = fileName;
    fromFile = TRUE;
    if (strcmp(dir, data_ov004_022bde38) == 0) {
      code = (u32)name & OV004_RESOURCE_KIND_MASK;
      coId = (u32)name & 0xfff;
      fromFile = FALSE;
      fileName = NULL;
      dir = NULL;
      if (code == OV004_RESOURCE_CO_PICTURE) {
        src = data_ov004_022bf150;
        Co_LoadGraphicsToBuffer(coId, src);
        size = 0x4000; /* one CO picture */
      } else if (code == OV004_RESOURCE_CO_PALETTE) {
        savedGfx = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_00);
        savedState = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        Co_LoadPalette(coId, 0);
        src = data_027c0280->paletteBuffer;
        Screen_SetCurrent(savedState);
        Screen_SetCurrentGfx(savedGfx);
        size = 32;
      }
    }
    if (fromFile) {
      FS_ChangeDir(dir);
      size = Ov004_GetFileSize(fileName);
    }
    bufferUsed = state->bufferUsed + size;
    vramUsed = state->vramUsed + size;
    if (bufferUsed > BUFFER_SIZE) {
      if (vramUsed > VRAM_SIZE) {
        return;
      }
      resource->data = (void*)(state->vramArea + state->vramUsed);
      bufferUsed = state->bufferUsed;
    } else {
      resource->data = (u8*)state->buffer + state->bufferUsed;
      vramUsed = state->vramUsed;
    }
    resource->size = size;
    resource->dir = dir;
    resource->name = name;
    if (fromFile) {
      File_ReadTo(resource->data, fileName);
    } else {
      Mem_Copy32(src, resource->data, resource->size);
    }
    state->resourceCount++;
    resource++;
    state->bufferUsed = bufferUsed;
    state->vramUsed = vramUsed;
  }
  if (done) { /* needed to match: the original tests done after the loop */
    return;
  }
}
