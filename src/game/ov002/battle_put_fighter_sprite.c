#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSpriteTask.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

int Battle_GetMatchupValue(int side);
void func_ov002_0235643c(void* a, int b, int c, int d, int e, int f, int g,
                         int h, int i, int j, int k, int l, int m, int n);

#define MENU_OFFSET(field) ((u32) & ((MenuState*)0)->battle.field)

/* Needed to match: the side index is added to the address of gMenuState
 * first and the offset of the field last. */
#define SIDE_ACTOR(side)                                              \
  ((BattleCellTask*)((BattleEntry*)((u8*)&gMenuState +                \
                                    (side) * sizeof(BattleEntry[5]) + \
                                    MENU_OFFSET(unk_310)))            \
       ->unk_00)
#define SIDE_UNK_040(side) \
  (*(void**)((u8*)&gMenuState + (side) * 4 + MENU_OFFSET(unk_40)))
#define SIDE_DATA(side)                                                      \
  (*(BattleSideData**)((u8*)&gMenuState + (side) * sizeof(BattleSideData*) + \
                       MENU_OFFSET(unk_27c)))
#define SIDE_COLOUR(side) (*(u16*)((u8*)&gMenuState + (side) * 2 + 12))

/* Hands func_ov002_0235643c the position of the first actor of a side and the
 * point of the fighter's pose, both scaled by the slot's actor scale. slot
 * names the actor whose side and scale are used; what the call draws is a
 * guess. */
void Battle_PutFighterSprite(int slot, int pose) {
  BattleCellTask* source = SIDE_ACTOR(slot);
  u16 scale = source->scale;
  int side = source->side;
  BattleCellTask* actor = SIDE_ACTOR(side);
  const BattleFighterLayout* shapes = &SIDE_DATA(side)->layout->fighter;
  int tile = Battle_GetMatchupValue(side);

  func_ov002_0235643c(SIDE_UNK_040(side), tile, actor->posX, actor->posY,
                      actor->posZ, shapes->steps[pose].x * scale >> 8,
                      shapes->steps[pose].y * scale >> 8, -1, scale, scale,
                      SIDE_COLOUR(side), 8, 0, side);
}
