#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/info_panel.h"
#include "game/unit_stats.h"
#include "game/skills.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_027c0280* data_027c0280;

/* Per side: x of each box from the panel's x. */
extern s16 data_0216635c[INFO_PANEL_SIDES][3]; /* terrain box */
extern s16 data_0216635e[INFO_PANEL_SIDES][3]; /* unit box */
extern s16 data_02166360[INFO_PANEL_SIDES][3]; /* cargo box */
extern u16 data_02166308[3]; /* panel width by unit boxes shown */
extern s8 Ov000_gUrbanTerrain[];
extern u8 gArmyPalettes[][32];
extern u8 data_ov000_022cb630[];
extern u8 data_02166430[];
extern u8 data_027c0010;
extern u8 data_027c0038;
extern u8 data_027c0028;

void InfoPanel_StepAwayFromCursor(void);
int InfoPanel_CountUnitBoxes(u8 x, u8 y);
void InfoPanel_DrawTerrainPicture(int x, int y, int panelX, int panelY,
                                  int side);
int Map_GetDisplayTile(int x, int y);
int DoubleTerrainStars(int playerIndex, int unitType, int terrain);
s32 HwDiv32(s32 numerator, s32 denominator);
void InfoPanel_DrawUnitPicture(int type, int panelX, int panelY, int side);
void Palette_Load(const void* src, u32 offset, u32 size);
void SpriteFile_Queue(void* data, int frame, int b, int x, int y, int tile,
                      int c, int d);
void func_020cb0cc(u16 x, u16 y);
void func_020cb0f4(u16 x, u16 y);
BOOL UnitType_HidesCargo(int type, int player, int slot);
void InfoPanel_DrawHiddenCargoMark(s16 x, s16 y, int slot);
void* Task_FindByScript(void* script);
void InfoPanel_DrawDamageForecast(int panelX, int panelY, int side);
void Oam_PutSprite(int x, int y, void* shape, int tile);
int MainLoop_GetFrameCount(void);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define GFX data_027c0280

/* Screen position of a layout item in the box at boxX. */
#define ITEM_X(boxX, item) (data_021663f4.item.x + (panelX + (boxX)))
#define ITEM_Y(item) (panelY + data_021663f4.item.y)

/* SpriteFile_Queue frames for the icons. */
#define ICON_NONE 0
#define ICON_CAPTURE 1
#define ICON_TILE_HP 2
#define ICON_AMMO 3
#define ICON_NO_AMMO 4

/* Draws the info panel for the tile at infoTileX, infoTileY: the terrain
 * box, the unit box and cargo box when a unit stands there, the window that
 * frames them and the panel's edge sprites. */
void InfoPanel_Draw(void) {
  int tileX;
  int tileY;
  int unitBoxes;
  u8 owner;
  Unk_02080440_48* state;
  MapUnit* unit;
  s16 panelX;
  s16 panelY;
  int side;
  s16 terrain;
  s16 captureLeft;
  s16* terrainBoxX;
  s16* unitBoxX;
  s16 tileHp;
  MapObject* object;
  /* needed to match: two zeroed stack words the code never reads */
  volatile int unusedA = 0;
  volatile int unusedB = 0;

  InfoPanel_StepAwayFromCursor();
  state = STATE;
  tileX = state->infoTileX;
  tileY = state->infoTileY;
  unitBoxes = InfoPanel_CountUnitBoxes(tileX, tileY);
  state = STATE;
  unit = &state->units[state->mapTiles
                           ->unitAt[state->mapTiles->rowStart[tileY] + tileX]];
  owner = MAP_UNIT_OWNER(state, unit);
  panelX = state->infoPanelX;
  if (panelX <= gInfoPanelRestPos[INFO_PANEL_LEFT].x) {
    panelY = gInfoPanelRestPos[INFO_PANEL_LEFT].y;
    side = INFO_PANEL_LEFT;
  } else {
    panelY = gInfoPanelRestPos[INFO_PANEL_RIGHT].y;
    side = INFO_PANEL_RIGHT;
  }
  InfoPanel_DrawTerrainPicture(tileX, tileY, panelX, panelY, side);

  terrain = MAP_TILE_TERRAIN(Map_GetDisplayTile(tileX, tileY));
  terrainBoxX = &data_0216635c[side][0];
  InfoPanel_DrawNumber(
      ITEM_X(*terrainBoxX, defenseStars), ITEM_Y(defenseStars),
      HwDiv32(DoubleTerrainStars(STATE->currentPlayer, 0, terrain), 10));
  captureLeft = 0;
  if (Ov000_gUrbanTerrain[MAP_TILE_TERRAIN(terrain)]) {
    captureLeft = MAP_UNIT_CAPTURE_POINTS;
  }

  if (unitBoxes >= 1) {
    InfoPanel_DrawUnitPicture(unit->type, panelX, panelY, side);
    Palette_Load(gArmyPalettes[STATE->players[owner].armyColour - 1], 0x380,
                 32);
    unitBoxX = &data_0216635e[side][0];
    /* the u8 and u16 argument casts in this block are needed to match */
    InfoPanel_DrawUnit(ITEM_X(*unitBoxX, unitIcon) & 0x1ff, ITEM_Y(unitIcon),
                       unit->type, owner, (u8)(unit->cargo[0] | unit->cargo[1]),
                       (u8)unit->captureProgress, unit->flags, 0);
    if (Ov000_gUnitStats[unit->type].ammo) {
      SpriteFile_Queue(data_ov000_022cb630, ICON_AMMO, 1,
                       ITEM_X(*unitBoxX, weaponIcon) & 0x1ff,
                       (u8)ITEM_Y(weaponIcon) | 0x400, 0x131d, 0, 0);
    } else {
      SpriteFile_Queue(data_ov000_022cb630, ICON_NO_AMMO, 1,
                       ITEM_X(*unitBoxX, weaponIcon) & 0x1ff,
                       (u8)ITEM_Y(weaponIcon) | 0x400, 0x131d, 0, 0);
    }

    state = STATE;
    if (state->players[state->currentPlayer].team !=
            state->players[MAP_UNIT_OWNER(state, unit)].team &&
        CheckSkillsEquipped(
            state->players[MAP_UNIT_OWNER(state, unit)].skills[0],
            SKILL_HIDDEN_HP)) {
      func_020cb0cc(ITEM_X(*unitBoxX, hp), ITEM_Y(hp));
    } else {
      /* shown hp: 1-10, rounded up */
      InfoPanel_DrawNumber(ITEM_X(*unitBoxX, hp), ITEM_Y(hp),
                           unit->hp ? HwDiv32(unit->hp - 1, 10) + 1 : 0);
    }
    if (unit->type == UNIT_OOZIUM) {
      func_020cb0f4(ITEM_X(*unitBoxX, fuel), ITEM_Y(fuel));
    } else {
      InfoPanel_DrawNumber(ITEM_X(*unitBoxX, fuel), ITEM_Y(fuel), unit->fuel);
    }
    if (Ov000_gUnitStats[unit->type].ammo) {
      InfoPanel_DrawNumber(ITEM_X(*unitBoxX, ammo), ITEM_Y(ammo), unit->ammo);
    }

    state = STATE;
    if (UnitType_HidesCargo(unit->type, state->currentPlayer,
                            (u16)MAP_UNIT_OWNER(state, unit))) {
      /* cargo not shown: one mark per seat */
      if (Ov000_gUnitStats[unit->type].carryTypes[0] == 1) {
        InfoPanel_DrawHiddenCargoMark(
            ITEM_X(data_02166360[side][0], cargoOnly) & 0x1ff,
            ITEM_Y(cargoOnly), 1);
      } else {
        InfoPanel_DrawHiddenCargoMark(
            ITEM_X(data_02166360[side][0], cargoFirst) & 0x1ff,
            ITEM_Y(cargoFirst), 2);
        InfoPanel_DrawHiddenCargoMark(
            ITEM_X(data_02166360[side][0], cargoSecond) & 0x1ff,
            ITEM_Y(cargoSecond), 1);
      }
    } else if (unit->cargo[1]) {
      MapUnit* cargo = &STATE->units[unit->cargo[0]];
      InfoPanel_DrawUnit(ITEM_X(data_02166360[side][0], cargoFirst) & 0x1ff,
                         ITEM_Y(cargoFirst), cargo->type, owner,
                         (u8)(cargo->cargo[0] | cargo->cargo[1]), 0, 0, 1);
      cargo = &STATE->units[unit->cargo[1]];
      InfoPanel_DrawUnit(ITEM_X(data_02166360[side][0], cargoSecond) & 0x1ff,
                         ITEM_Y(cargoSecond), cargo->type, owner,
                         (u8)(cargo->cargo[0] | cargo->cargo[1]), 0, 0, 2);
    } else if (unit->cargo[0]) {
      MapUnit* cargo = &STATE->units[unit->cargo[0]];
      InfoPanel_DrawUnit(ITEM_X(data_02166360[side][0], cargoOnly) & 0x1ff,
                         ITEM_Y(cargoOnly), cargo->type, owner,
                         (u8)(cargo->cargo[0] | cargo->cargo[1]), 0, 0, 1);
    }
    captureLeft -= unit->captureProgress;
  }

  if (Task_FindByScript(data_02166430)) {
    InfoPanel_DrawDamageForecast(panelX, panelY, side);
  }

  /* Window 0 frames the panel; the edge on the screen side is 0. 0x2000
     * and 0x8000 turn on window 0 and the OBJ window. */
  GFX->dispCnt |= 0x2000;
  GFX->dispCnt |= 0x8000;
  GFX->win0Left = 0;
  GFX->win0Right = 0;
  {
    s16 left = panelX + 1 + gInfoPanelWindowLeft[side][unitBoxes];
    if (left > 255) {
      GFX->win0Left = 0;
    } else if (left > 0) {
      GFX->win0Left = left;
    }
  }
  {
    s16 right = data_02166308[unitBoxes] +
                (panelX + 1 + gInfoPanelWindowLeft[side][unitBoxes]);
    if (right > 255) {
      GFX->win0Right = 0;
    } else if (right > 0) {
      GFX->win0Right = right;
    }
  }
  if (side == INFO_PANEL_LEFT) {
    GFX->win0Left = 0;
  }
  if (side == INFO_PANEL_RIGHT) {
    GFX->win0Right = 0;
  }
  GFX->win0Top = panelY;
  GFX->win0Bottom = panelY + 56;
  GFX->win0In.bg[GFX->layer0Bg] = 1;
  GFX->win0In.bg[GFX->layer1Bg] = 1;
  GFX->win0In.bg[GFX->layer2Bg] = 1;
  GFX->win0In.bg[GFX->layer3Bg] = 1;
  GFX->win0In.obj = 1;
  GFX->win0In.effect = 1;
  GFX->objWinIn.bg[GFX->layer0Bg] = 1;
  GFX->objWinIn.bg[GFX->layer1Bg] = 1;
  GFX->objWinIn.bg[GFX->layer2Bg] = 1;
  GFX->objWinIn.bg[GFX->layer3Bg] = 1;
  GFX->objWinIn.obj = 1;
  GFX->objWinIn.effect = 1;
  GFX->winOut.bg[GFX->layer0Bg] = 1;
  GFX->winOut.bg[GFX->layer1Bg] = 1;
  GFX->winOut.bg[GFX->layer2Bg] = 1;
  GFX->winOut.bg[GFX->layer3Bg] = 1;
  GFX->winOut.obj = 1;
  GFX->winOut.effect = 0;

  /* The panel's inner edge: three sprites down the window's open side,
     * flipped on the right. */
  if (side == INFO_PANEL_LEFT) {
    int x = (s16)(data_02166308[unitBoxes] +
                  (panelX + 1 + gInfoPanelWindowLeft[side][unitBoxes])) &
            0x1ff;
    Oam_PutSprite(x, (GFX->win0Top & 0xff) | 0x800, &data_027c0010, 0x331);
    Oam_PutSprite(x, ((GFX->win0Top + 32) & 0xff) | 0x800, &data_027c0038,
                  0x335);
    Oam_PutSprite(x, ((GFX->win0Top + 48) & 0xff) | 0x800, &data_027c0028,
                  0x337);
  } else {
    int x = (((s16)(panelX + 1 + gInfoPanelWindowLeft[side][unitBoxes]) - 8) &
             0x1ff) |
            0x1000;
    Oam_PutSprite(x, (GFX->win0Top & 0xff) | 0x800, &data_027c0010, 0x331);
    Oam_PutSprite(x, ((GFX->win0Top + 32) & 0xff) | 0x800, &data_027c0038,
                  0x335);
    Oam_PutSprite(x, ((GFX->win0Top + 48) & 0xff) | 0x800, &data_027c0028,
                  0x337);
  }

  /* Terrain box: hp of a map object or a tile with hp, else the capture
     * points left; a count below full blinks. */
  tileHp = 0;
  object = Map_GetObjectAt(tileX, tileY);
  if (object) {
    tileHp = object->hp;
  }
  state = STATE;
  if (state->tileHp[state->mapTiles->rowStart[tileY] + tileX] & 0x7f) {
    tileHp = state->tileHp[state->mapTiles->rowStart[tileY] + tileX] & 0x7f;
  }
  if (tileHp) {
    SpriteFile_Queue(data_ov000_022cb630, ICON_TILE_HP, 1,
                     ITEM_X(*terrainBoxX, terrainIcon) & 0x1ff,
                     (u8)ITEM_Y(terrainIcon) | 0x400, 0x131d, 0, 0);
    if (MainLoop_GetFrameCount() % 30 < 10) {
      return;
    }
    InfoPanel_DrawNumber(ITEM_X(*terrainBoxX, terrainCount),
                         ITEM_Y(terrainCount), tileHp);
  } else if (captureLeft) {
    SpriteFile_Queue(data_ov000_022cb630, ICON_CAPTURE, 1,
                     ITEM_X(*terrainBoxX, terrainIcon) & 0x1ff,
                     (u8)ITEM_Y(terrainIcon) | 0x400, 0x131d, 0, 0);
    if (captureLeft > 0 && captureLeft < MAP_UNIT_CAPTURE_POINTS &&
        MainLoop_GetFrameCount() % 30 < 10) {
      return;
    }
    InfoPanel_DrawNumber(ITEM_X(*terrainBoxX, terrainCount),
                         ITEM_Y(terrainCount), captureLeft);
  } else {
    SpriteFile_Queue(data_ov000_022cb630, ICON_NONE, 1,
                     ITEM_X(*terrainBoxX, terrainIcon) & 0x1ff,
                     (u8)ITEM_Y(terrainIcon) | 0x400, 0x131d, 0, 0);
  }
}
