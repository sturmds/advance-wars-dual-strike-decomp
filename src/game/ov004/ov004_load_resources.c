#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/Unk_02088d04.h"
#include "game/menu_button.h"
#include "game/screen_contexts.h"
#include "game/task.h"

void FS_ChangeDir(const char* dir);
u32 Ov004_GetFileSize(const char* path);
void File_ReadTo(void* dest, const char* fileName);
void Mem_Copy32(const void* src, void* dst, u32 size);
int strcmp(const char* a, const char* b); /* strcmp */
void Co_LoadGraphicsToBuffer(
    int coId, void* dest); /* guess: unpacks the CO's picture to dest */
void Co_LoadPalette(int coId, int paletteSlot);

extern Ov004ResourceFile data_ov004_022becf8[];
void Ov004Task_EaseBg1ScrollY(void);
void Ov004Task_EaseBg1ScrollYBack(void);
void Ov004Task_EaseLayer3ScrollY(void);
void Ov004Task_EaseUnk904TwoStage(void);
void Ov004Task_FadeBothHoldFade(void);
void Ov004Task_FadeHoldFade(void);
void Ov004Task_InitBlendedScene(void);
void Ov004Task_InitButtonScene(void);
void Ov004Task_InitLayer1BothScreensDark(void);
void Ov004Task_InitLayer1Scene(void);
void Ov004Task_InitLayer1SceneDark(void);
void Ov004Task_InitPictureScene(void);
void Ov004Task_InitScene(void);
void Ov004Task_InitSpriteScene(void);
void Ov004Task_PlayTurningQuadScene(void);
void Ov004Task_PlayTurningQuadScene2(void);
void Ov004Task_PlayTwoTurningQuadsScene(void);
void Ov004Task_PulseAlphaBlend(void);
void Ov004Task_QueueSpiralSprite(void);
void Ov004Task_ResetState(void);
void Ov004Task_RunBannerTimeline(void);
void Ov004Task_RunPictureQuadTimeline(void);
void Ov004Task_RunPictureTimeline(void);
void Ov004Task_RunPressStartScreen(void);
void Ov004Task_RunSceneTimeline(void);
void Ov004Task_RunSlidingQuadsTimeline(void);
void Ov004Task_SetupEntryScene(void);
void Ov004Task_SetupLayer2Scene(void);
void Ov004Task_SetupLayer3Scene(void);
void Ov004Task_SetupLayer3SceneWithLayers1And2(void);
void Ov004Task_SlideAndZoomBg3(void);
void Ov004Task_SlideBgScrollX(void);
void Ov004Task_SlideLayer2ScrollY(void);
void Ov004Task_SlideLayers1And2ScrollX(void);
void Ov004_AllocState(void);
void Ov004_ApplyBgCharBase(void);
void Ov004_BlendAlphaTween(void);
void Ov004_BlendFadeIn(void);
void Ov004_BlendFadeInOut(void);
void Ov004_EaseLayer1View(void);
void Ov004_EaseMaskedValueTwice(void);
void Ov004_EaseScaleSettle(void);
void Ov004_EaseScaleToHalf(void);
void Ov004_EaseUnk904Twice(void);
void Ov004_EaseUnk908Twice(void);
void Ov004_EaseValueOutAndBack(void);
void Ov004_EntryAppear(void);
void Ov004_InitTween(void);
void Ov004_RunSceneSequence(void);
void Ov004_ScrollLayer3Up(void);
void Ov004_SlideLayers1And2X(void);
void Ov004_SlideLayers1And2Y(void);
void Ov004_SlideLayers1And3Y(void);
extern u8 data_02178b08[];
extern u8 data_02179b08[];
extern u8 data_0217ab08[];
extern u8 data_0217eb08[];
extern u8 data_02180b08[];
void func_ov004_022ade28(void);
void func_ov004_022ade44(void);
void func_ov004_022b0000(void);
void func_ov004_022b004c(void);
void func_ov004_022b0050(void);
void func_ov004_022b0174(void);
void func_ov004_022b0198(void);
void func_ov004_022b01b8(void);
void func_ov004_022b01dc(void);
void func_ov004_022b01fc(void);
void func_ov004_022b02f8(void);
void func_ov004_022b0304(void);
void func_ov004_022b0444(void);
void func_ov004_022b0464(void);
void func_ov004_022b0484(void);
void func_ov004_022b05b4(void);
void func_ov004_022b05d8(void);
void func_ov004_022b05fc(void);
void func_ov004_022b061c(void);
void func_ov004_022b06d0(void);
void func_ov004_022b0784(void);
void func_ov004_022b0790(void);
void func_ov004_022b08dc(void);
void func_ov004_022b08f0(void);
void func_ov004_022b0904(void);
void func_ov004_022b0918(void);
void func_ov004_022b092c(void);
void func_ov004_022b0940(void);
void func_ov004_022b0954(void);
void func_ov004_022b0964(void);
void func_ov004_022b0984(void);
void func_ov004_022b0a38(void);
void func_ov004_022b188c(void);
void func_ov004_022b19d4(void);
void func_ov004_022b1a00(void);
void func_ov004_022b1a2c(void);
void func_ov004_022b1a58(void);
void func_ov004_022b1cd4(void);
void func_ov004_022b1cf8(void);
void func_ov004_022b1d18(void);
void func_ov004_022b1d38(void);
void func_ov004_022b1dec(void);
void func_ov004_022b1e10(void);
void func_ov004_022b1e34(void);
void func_ov004_022b1e58(void);
void func_ov004_022b1e78(void);
void func_ov004_022b1f2c(void);
void func_ov004_022b1f50(void);
void func_ov004_022b1f74(void);
void func_ov004_022b29d4(void);
void func_ov004_022b2b04(void);
void func_ov004_022b2b28(void);
void func_ov004_022b2c68(void);
void func_ov004_022b3f38(void);
void func_ov004_022b4114(void);
void func_ov004_022b4130(void);
void func_ov004_022b414c(void);
void func_ov004_022b43c8(void);
void func_ov004_022b43d4(void);
void func_ov004_022b43f4(void);
void func_ov004_022b44a8(void);
void func_ov004_022b44c4(void);
void func_ov004_022b44e0(void);
void func_ov004_022b4628(void);
void func_ov004_022b4648(void);
void func_ov004_022b4668(void);
void func_ov004_022b4688(void);
void func_ov004_022b473c(void);
void func_ov004_022b4758(void);
void func_ov004_022b4774(void);
void func_ov004_022b4898(void);
void func_ov004_022b48ac(void);
void func_ov004_022b4f2c(void);
void func_ov004_022b5050(void);
void func_ov004_022b5070(void);
void func_ov004_022b5094(void);
void func_ov004_022b50b4(void);
void func_ov004_022b5168(void);
void func_ov004_022b5190(void);
void func_ov004_022b6814(void);
void func_ov004_022b6834(void);
void func_ov004_022b68dc(void);
void func_ov004_022b68f0(void);
void func_ov004_022b6904(void);
void func_ov004_022b6a9c(void);
void func_ov004_022b6ad0(void);
void func_ov004_022b6b04(void);
void func_ov004_022b6cac(void);
void func_ov004_022b6ce8(void);
void func_ov004_022b6d20(void);
void func_ov004_022b6ebc(void);
void func_ov004_022b6ee4(void);
void func_ov004_022b6f08(void);
void func_ov004_022b7120(void);
void func_ov004_022b714c(void);
void func_ov004_022b7178(void);
void func_ov004_022b71a4(void);
void func_ov004_022b7380(void);
void func_ov004_022b738c(void);
void func_ov004_022b7524(void);
void func_ov004_022b875c(void);
void func_ov004_022b8888(void);
void func_ov004_022b8894(void);
void func_ov004_022b89c0(void);
void func_ov004_022b89cc(void);
void func_ov004_022b8af0(void);
void func_ov004_022b99f0(void);
void func_ov004_022b9a08(void);
void func_ov004_022b9b2c(void);
void func_ov004_022b9b38(void);
void func_ov004_022baa9c(void);
void func_ov004_022baccc(void);
extern u8 data_0217fb08[];

char data_ov004_022bde40[] = {
#include "data/ov004/data_ov004_022bde40.inc.c"
};
/* "/syogun" */
char data_ov004_022bde38[] = {
#include "data/ov004/data_ov004_022bde38.inc.c"
};
char data_ov004_022bde48[] = {
#include "data/ov004/data_ov004_022bde48.inc.c"
};
char data_ov004_022bde64[] = {
#include "data/ov004/data_ov004_022bde64.inc.c"
};
char data_ov004_022bde54[] = {
#include "data/ov004/data_ov004_022bde54.inc.c"
};
char data_ov004_022bde74[] = {
#include "data/ov004/data_ov004_022bde74.inc.c"
};
char data_ov004_022bdeb4[] = {
#include "data/ov004/data_ov004_022bdeb4.inc.c"
};
char data_ov004_022bde84[] = {
#include "data/ov004/data_ov004_022bde84.inc.c"
};
char data_ov004_022bde94[] = {
#include "data/ov004/data_ov004_022bde94.inc.c"
};
char data_ov004_022bdea4[] = {
#include "data/ov004/data_ov004_022bdea4.inc.c"
};
char data_ov004_022bdec4[] = {
#include "data/ov004/data_ov004_022bdec4.inc.c"
};
TaskScriptCmd data_ov004_022bdf80[] = {
#include "data/ov004/data_ov004_022bdf80.inc.c"
};
TaskScriptCmd data_ov004_022bdf68[] = {
#include "data/ov004/data_ov004_022bdf68.inc.c"
};
TaskScriptCmd data_ov004_022bdef0[] = {
#include "data/ov004/data_ov004_022bdef0.inc.c"
};
TaskScriptCmd data_ov004_022bdf20[] = {
#include "data/ov004/data_ov004_022bdf20.inc.c"
};
TaskScriptCmd data_ov004_022bdf50[] = {
#include "data/ov004/data_ov004_022bdf50.inc.c"
};
TaskScriptCmd data_ov004_022bded8[] = {
#include "data/ov004/data_ov004_022bded8.inc.c"
};
TaskScriptCmd data_ov004_022bdf08[] = {
#include "data/ov004/data_ov004_022bdf08.inc.c"
};
TaskScriptCmd data_ov004_022bdf38[] = {
#include "data/ov004/data_ov004_022bdf38.inc.c"
};
TaskScriptCmd data_ov004_022be698[] = {
#include "data/ov004/data_ov004_022be698.inc.c"
};
TaskScriptCmd data_ov004_022be678[] = {
#include "data/ov004/data_ov004_022be678.inc.c"
};
TaskScriptCmd data_ov004_022be658[] = {
#include "data/ov004/data_ov004_022be658.inc.c"
};
TaskScriptCmd data_ov004_022be638[] = {
#include "data/ov004/data_ov004_022be638.inc.c"
};
TaskScriptCmd data_ov004_022be618[] = {
#include "data/ov004/data_ov004_022be618.inc.c"
};
TaskScriptCmd data_ov004_022be558[] = {
#include "data/ov004/data_ov004_022be558.inc.c"
};
TaskScriptCmd data_ov004_022be5b8[] = {
#include "data/ov004/data_ov004_022be5b8.inc.c"
};
TaskScriptCmd data_ov004_022be5d8[] = {
#include "data/ov004/data_ov004_022be5d8.inc.c"
};
TaskScriptCmd data_ov004_022be6b8[] = {
#include "data/ov004/data_ov004_022be6b8.inc.c"
};
TaskScriptCmd data_ov004_022be6d8[] = {
#include "data/ov004/data_ov004_022be6d8.inc.c"
};
TaskScriptCmd data_ov004_022be738[] = {
#include "data/ov004/data_ov004_022be738.inc.c"
};
TaskScriptCmd data_ov004_022be758[] = {
#include "data/ov004/data_ov004_022be758.inc.c"
};
TaskScriptCmd data_ov004_022be798[] = {
#include "data/ov004/data_ov004_022be798.inc.c"
};
TaskScriptCmd data_ov004_022be7b8[] = {
#include "data/ov004/data_ov004_022be7b8.inc.c"
};
TaskScriptCmd data_ov004_022be4d8[] = {
#include "data/ov004/data_ov004_022be4d8.inc.c"
};
TaskScriptCmd data_ov004_022be4b8[] = {
#include "data/ov004/data_ov004_022be4b8.inc.c"
};
TaskScriptCmd data_ov004_022be498[] = {
#include "data/ov004/data_ov004_022be498.inc.c"
};
TaskScriptCmd data_ov004_022be478[] = {
#include "data/ov004/data_ov004_022be478.inc.c"
};
TaskScriptCmd data_ov004_022be458[] = {
#include "data/ov004/data_ov004_022be458.inc.c"
};
TaskScriptCmd data_ov004_022be438[] = {
#include "data/ov004/data_ov004_022be438.inc.c"
};
TaskScriptCmd data_ov004_022be418[] = {
#include "data/ov004/data_ov004_022be418.inc.c"
};
TaskScriptCmd data_ov004_022be078[] = {
#include "data/ov004/data_ov004_022be078.inc.c"
};
TaskScriptCmd data_ov004_022be3d8[] = {
#include "data/ov004/data_ov004_022be3d8.inc.c"
};
TaskScriptCmd data_ov004_022be3b8[] = {
#include "data/ov004/data_ov004_022be3b8.inc.c"
};
TaskScriptCmd data_ov004_022be398[] = {
#include "data/ov004/data_ov004_022be398.inc.c"
};
TaskScriptCmd data_ov004_022be378[] = {
#include "data/ov004/data_ov004_022be378.inc.c"
};
TaskScriptCmd data_ov004_022be358[] = {
#include "data/ov004/data_ov004_022be358.inc.c"
};
TaskScriptCmd data_ov004_022be338[] = {
#include "data/ov004/data_ov004_022be338.inc.c"
};
TaskScriptCmd data_ov004_022be0f8[] = {
#include "data/ov004/data_ov004_022be0f8.inc.c"
};
TaskScriptCmd data_ov004_022be118[] = {
#include "data/ov004/data_ov004_022be118.inc.c"
};
TaskScriptCmd data_ov004_022be2d8[] = {
#include "data/ov004/data_ov004_022be2d8.inc.c"
};
TaskScriptCmd data_ov004_022be2b8[] = {
#include "data/ov004/data_ov004_022be2b8.inc.c"
};
TaskScriptCmd data_ov004_022be298[] = {
#include "data/ov004/data_ov004_022be298.inc.c"
};
TaskScriptCmd data_ov004_022be278[] = {
#include "data/ov004/data_ov004_022be278.inc.c"
};
TaskScriptCmd data_ov004_022be258[] = {
#include "data/ov004/data_ov004_022be258.inc.c"
};
TaskScriptCmd data_ov004_022be1d8[] = {
#include "data/ov004/data_ov004_022be1d8.inc.c"
};
TaskScriptCmd data_ov004_022be218[] = {
#include "data/ov004/data_ov004_022be218.inc.c"
};
TaskScriptCmd data_ov004_022be238[] = {
#include "data/ov004/data_ov004_022be238.inc.c"
};
TaskScriptCmd data_ov004_022be3f8[] = {
#include "data/ov004/data_ov004_022be3f8.inc.c"
};
TaskScriptCmd data_ov004_022be4f8[] = {
#include "data/ov004/data_ov004_022be4f8.inc.c"
};
TaskScriptCmd data_ov004_022be538[] = {
#include "data/ov004/data_ov004_022be538.inc.c"
};
TaskScriptCmd data_ov004_022be578[] = {
#include "data/ov004/data_ov004_022be578.inc.c"
};
TaskScriptCmd data_ov004_022be5f8[] = {
#include "data/ov004/data_ov004_022be5f8.inc.c"
};
TaskScriptCmd data_ov004_022be6f8[] = {
#include "data/ov004/data_ov004_022be6f8.inc.c"
};
TaskScriptCmd data_ov004_022be778[] = {
#include "data/ov004/data_ov004_022be778.inc.c"
};
TaskScriptCmd data_ov004_022be7d8[] = {
#include "data/ov004/data_ov004_022be7d8.inc.c"
};
TaskScriptCmd data_ov004_022be0d8[] = {
#include "data/ov004/data_ov004_022be0d8.inc.c"
};
TaskScriptCmd data_ov004_022be0b8[] = {
#include "data/ov004/data_ov004_022be0b8.inc.c"
};
TaskScriptCmd data_ov004_022be098[] = {
#include "data/ov004/data_ov004_022be098.inc.c"
};
TaskScriptCmd data_ov004_022bdfb8[] = {
#include "data/ov004/data_ov004_022bdfb8.inc.c"
};
TaskScriptCmd data_ov004_022be058[] = {
#include "data/ov004/data_ov004_022be058.inc.c"
};
TaskScriptCmd data_ov004_022bdff8[] = {
#include "data/ov004/data_ov004_022bdff8.inc.c"
};
TaskScriptCmd data_ov004_022be038[] = {
#include "data/ov004/data_ov004_022be038.inc.c"
};
TaskScriptCmd data_ov004_022be138[] = {
#include "data/ov004/data_ov004_022be138.inc.c"
};
TaskScriptCmd data_ov004_022be178[] = {
#include "data/ov004/data_ov004_022be178.inc.c"
};
TaskScriptCmd data_ov004_022be198[] = {
#include "data/ov004/data_ov004_022be198.inc.c"
};
TaskScriptCmd data_ov004_022be1f8[] = {
#include "data/ov004/data_ov004_022be1f8.inc.c"
};
TaskScriptCmd data_ov004_022be2f8[] = {
#include "data/ov004/data_ov004_022be2f8.inc.c"
};
TaskScriptCmd data_ov004_022be518[] = {
#include "data/ov004/data_ov004_022be518.inc.c"
};
TaskScriptCmd data_ov004_022be598[] = {
#include "data/ov004/data_ov004_022be598.inc.c"
};
TaskScriptCmd data_ov004_022be718[] = {
#include "data/ov004/data_ov004_022be718.inc.c"
};
TaskScriptCmd data_ov004_022be7f8[] = {
#include "data/ov004/data_ov004_022be7f8.inc.c"
};
TaskScriptCmd data_ov004_022bdf98[] = {
#include "data/ov004/data_ov004_022bdf98.inc.c"
};
TaskScriptCmd data_ov004_022bdfd8[] = {
#include "data/ov004/data_ov004_022bdfd8.inc.c"
};
TaskScriptCmd data_ov004_022be018[] = {
#include "data/ov004/data_ov004_022be018.inc.c"
};
TaskScriptCmd data_ov004_022be158[] = {
#include "data/ov004/data_ov004_022be158.inc.c"
};
TaskScriptCmd data_ov004_022be1b8[] = {
#include "data/ov004/data_ov004_022be1b8.inc.c"
};
TaskScriptCmd data_ov004_022be318[] = {
#include "data/ov004/data_ov004_022be318.inc.c"
};
TaskScriptCmd data_ov004_022be818[] = {
#include "data/ov004/data_ov004_022be818.inc.c"
};
MenuButtonTemplate data_ov004_022be840[] = {
#include "data/ov004/data_ov004_022be840.inc.c"
};
GfxLayer data_ov004_022be868[] = {
#include "data/ov004/data_ov004_022be868.inc.c"
};
GfxLayer data_ov004_022be8b8[] = {
#include "data/ov004/data_ov004_022be8b8.inc.c"
};
GfxLayer data_ov004_022be958[] = {
#include "data/ov004/data_ov004_022be958.inc.c"
};
GfxLayer data_ov004_022be9a8[] = {
#include "data/ov004/data_ov004_022be9a8.inc.c"
};
GfxLayer data_ov004_022be908[] = {
#include "data/ov004/data_ov004_022be908.inc.c"
};
TaskScriptCmd data_ov004_022be9f8[] = {
#include "data/ov004/data_ov004_022be9f8.inc.c"
};
Ov004SlotInit data_ov004_022bea58[] = {
#include "data/ov004/data_ov004_022bea58.inc.c"
};
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
  for (i = 0; i < OV004_RESOURCE_COUNT; i++) {
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
      coId = (u32)name & ~OV004_RESOURCE_KIND_MASK;
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
