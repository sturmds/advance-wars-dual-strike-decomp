#include "types.h"
#include "game/menu_state.h"
#include "game/player.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen.h"

void Gfx_ClearLayerMap(int layer);
const char* File_LocalizePath(const char* path);
u8* File_Load(const char* path);
void Mem_Free(void* res);
void Data_Unpack(const void* src, void* dest);
void Mem_Copy32(const void* src, void* dest, u32 size);
void Palette_Load(const void* src, u32 offset, u32 size);
void Gfx_RequestLayerCopy(int layer);
BOOL Player_IsAlliedNations(int player);
void Sound_PlaySe(u32 soundId);

extern char data_0215c268[]; /* "/ohashi/res_syogunbreak" */
extern char data_0215c280[]; /* "/ohashi/res_syogunbreak_lang" */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])
#define PLAYER (&MAP_STATE->players[MAP_STATE->currentPlayer])
#define LAYER_TILES(layerBg) \
  (data_027c0280->bgVram + (data_027c0280->bgCnt[layerBg].charBase << 14))

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

/* Sound effects of the banner, Allied Nations and Black Hole. */
#define SE_SUPER_BREAK_ALLIED 191
#define SE_SUPER_BREAK_BLACK_HOLE 190

/* Bytes of the edge sprite data at 0x86c of the banner file. */
#define EDGE_SPRITE_FILE_SIZE 0x74

/* Step of the Super CO Power cut-in: sets BG0 to 256x512 and loads the
 * banner into it (tiles and map from res_syogunbreak_lang, the army's
 * palette from res_syogunbreak), loads the edge sprite's tiles, palette and
 * file, puts the banner below the screen (scroll 204) and plays its sound. */
void SuperBreak_LoadBanner(Task* task) {
  u8* res;
  u8* langRes;
  int i;

  Screen_SetLayerScreenSize(0, 2);
  Gfx_ClearLayerMap(0);
  res = File_Load(data_0215c268);
  langRes = File_Load(File_LocalizePath(data_0215c280));
  Data_Unpack(langRes, LAYER_TILES(data_027c0280->layer0Bg));
  Data_Unpack(langRes + 0x16c, data_027c0280->layer0Map);
  Palette_Load(res + 0x8e0 + (PLAYER->armyColour - 1) * 32, 0x20, 0x20);
  /* banner tiles use BG palette 1 */
  for (i = 0; i < BG_SCREEN_ENTRIES; i++) {
    data_027c0280->layer0Map[i] += 0x1000;
  }
  Data_Unpack(res + 0x6a4, data_027c0280->objVram + 0x3280);
  Palette_Load(res + 0x8e0 + (PLAYER->armyColour - 1) * 32, 0x260, 0x20);
  Mem_Copy32(res + 0x86c, gMenuState.bytes, EDGE_SPRITE_FILE_SIZE);
  Mem_Free(res);
  Mem_Free(langRes);
  Gfx_RequestLayerCopy(0);
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 204;
  FRAME(task) = 0;
  if (Player_IsAlliedNations(MAP_STATE->currentPlayer) == TRUE) {
    Sound_PlaySe(SE_SUPER_BREAK_ALLIED);
  } else {
    Sound_PlaySe(SE_SUPER_BREAK_BLACK_HOLE);
  }
}
