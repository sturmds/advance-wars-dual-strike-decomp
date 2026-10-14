#include "types.h"
#include "game/Unk_02080440.h"

void Palette_Load(const void* src, u32 offset, u32 size);
void func_020d2ed0(int a0);
void UnitShowTask_ClearSlots(void);
void func_01ff9ac4(void);
void func_020d2190(void);
void ObjGfx_LoadLanguageTiles(void);
BOOL Map_IsCoSelectPhase(void);
void PowerPanel_LoadGfx(int player, BOOL coSelectPhase);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u16 data_021396f0[];

#define GAME (data_027c027c->unk_48[data_027c0284])
#define STATE_50 (data_027c027c->unk_50[data_027c0284])

void func_020f88ec(void) {
  Palette_Load(data_021396f0, 0x240, 32);
  func_020d2ed0(0x70);
  UnitShowTask_ClearSlots();
  func_01ff9ac4();
  func_020d2190();
  if (STATE_50->unk_58 != NULL) {
    STATE_50->unk_58(GAME->unk_1c4->weather, GAME->unk_1c4->tileset);
  }
  ObjGfx_LoadLanguageTiles();
  PowerPanel_LoadGfx(GAME->currentPlayer, Map_IsCoSelectPhase());
}
