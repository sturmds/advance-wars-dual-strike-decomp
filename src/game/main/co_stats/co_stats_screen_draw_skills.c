#include "types.h"
#include "game/menu_state.h"
#include "game/Unk_02080440.h"
#include "game/screen_other_map.h"
#include "game/force_rank_screen.h"
#include "game/text_print.h"
#include "game/skills.h"

BOOL Skills_AreDisabled(void);
void ForceRank_LoadSets(int mode);
void Co_ListDefaultSkills(u8* out, int co);
void Player_ListEquippedSkills(u8* out, int player, int slot);
void Screen_SetCurrentBoth(void* state);
void SkillIcon_Load(int skill, void* dest);
void Gfx_ClearLayerMap(int layer);
void Bg_PutTileQuad(int x, int y, int firstTile, int palette);
void Gfx_RequestLayerCopy(int layer);

#define MENU (gMenuState.unk_0206d67c)

/* Needed to match: the original adds these field offsets to gMenuState's
 * address at run time instead of folding them into the literal. */
#define MENU_OFFSET(field) ((u32) & ((MenuState*)0)->unk_0206d67c.field)
#define MENU_SKILLS ((u8*)&gMenuState + MENU_OFFSET(skills))
#define MENU_SHOWN_CO \
  (((u8*)&gMenuState + (MENU.player - 1) * 2 + MENU.slot)[MENU_OFFSET(coShown)])

/* BG tile where the skill icons load, 4 tiles (2x2) per icon. */
#define SKILL_ICON_TILE 0x355

/* Draws the skills of the shown CO on layer 0: up to 4 rows of icon, name
 * and description from Ov000_gForceRankTable. Nothing is listed for player 1
 * while skills are disabled. In gameMode 3 with unk_0a8 set, the list comes
 * from the CO id in coShown; otherwise from the player's equipped skills,
 * read on the second map for the back slot of a two-map battle. */
void CoStatsScreen_DrawSkills(void) {
  Unk_02080440* saved027c;
  Unk_027c0280* saved0280;
  int count;
  int slot;
  int i;

  count = 0;
  if (MENU.player != 1 || Skills_AreDisabled() != TRUE) {
    ForceRank_LoadSets(MAP_STATE->gameMode);
    for (i = 0; i < SKILL_LIST_LENGTH; i++) {
      MENU.skills[i] = 0;
    }
    MENU.skills[SKILL_LIST_LENGTH] = 0xff;
    if (MENU.unk_0a8 == 1 && MAP_STATE->gameMode == 3) {
      Co_ListDefaultSkills(MENU_SKILLS, MENU_SHOWN_CO);
    } else {
      saved027c = data_027c027c;
      saved0280 = data_027c0280;
      if (MAP_STATE->secondMapId != 0 && OtherMap_GetUnk30() == 0 &&
          MENU.slot == 1) {
        Screen_SetCurrentBoth(data_027c0288.unk_04);
        slot = 0;
      } else {
        slot = MENU.slot;
      }
      Player_ListEquippedSkills(MENU_SKILLS, MENU.player, slot);
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
    }
    for (i = 0; i < SKILL_LIST_LENGTH; i++) {
      if (MENU.skills[i] == 0 || MENU.skills[i] == 0xff) {
        break;
      }
      count++;
    }
    for (i = 0; i < count; i++) {
      SkillIcon_Load(
          MENU.skills[i],
          data_027c0280->bgVram +
              (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14) +
              (SKILL_ICON_TILE + i * 4) * 32);
    }
  }
  Gfx_ClearLayerMap(0);
  Bg_PutTileQuad(10, 5, 0x2c8, 2);
  Text_PrintId(12, 5, data_027c0280->layer0Map, 0x80000038, 0, 128);
  for (i = 0; i < count; i++) {
    Bg_PutTileQuad(1, 7 + i * 2, SKILL_ICON_TILE + i * 4, 4);
    Text_PrintId(3, 7 + i * 2, data_027c0280->layer0Map,
                 Ov000_gForceRankTable[MENU.skills[i]].name.id, 0,
                 192 + i * 128);
    Text_PrintId(12, 7 + i * 2, data_027c0280->layer0Map,
                 Ov000_gForceRankTable[MENU.skills[i]].description.id, 0,
                 208 + i * 128);
  }
  Gfx_RequestLayerCopy(0);
}
