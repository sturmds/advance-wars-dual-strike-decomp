#include "types.h"
#include "sdk/fx.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_022a7c5c.h"
#include "game/sprite_anim.h"
#include "game/task.h"
#include "game/unit_stats.h"
#include "sdk/fs.h"

extern char gSubDiveGfxFile3d[];
extern char gSubDiveGfxFile2d[];
extern char gSubDivePaletteFile[];
extern char gStealthHideGfxFile3d[];
extern char gStealthHideGfxFile2d[];
extern char gStealthHidePaletteFile[];
extern char data_02166cc8[]; /* "/bmap" */
extern u8 gSubDiveAnim[];
extern u8 gStealthHideAnim[];
extern const TaskScriptCmd data_02166410[];

void File_UnpackTo(void* dest, const char* fileName);
void Palette_LoadFromFile(const char* fileName, int a, int b, int c);
void Gfx_QueueUnpackedFileCopy(const char* fileName, void* dest);
void Gfx_QueueFileCopy(const char* fileName, void* dest, int size);
SpriteAnim3dTask* SpriteAnim3dTask_StartAlt(const void* file,
                                            const VecFx32* pos, u32 tileBase,
                                            u32 unk28, int animation,
                                            int layer);
void Sound_PlaySe(u32 soundId);

/* Plays the dive or emerge effect at the acting unit's destination tile:
 * sound 157 (dive) or 158 (emerge), and a sprite from /bmap, the Sub set
 * (files 072-074) or the Stealth set (075-077). Only the Sub set has an
 * emerge animation. Skipped when the tile is off screen, or when the current
 * player lacks Player.visionGrids bit 1 and the tile has MapTiles.unk_2338 clear. */
void Unit_PlayDiveEffect(int unitType, BOOL emerge) {
  int dx;
  int dy;
  char* gfx3d;
  char* gfx2d;
  char* palette;
  u8* anim;
  VecFx32 pos;
  int animation;

  if ((MAP_STATE->players[MAP_STATE->currentPlayer].visionGrids & 2) == 0 &&
      MAP_STATE->mapTiles
              ->unk_2338[MAP_STATE->actingUnitDestX +
                         MAP_STATE->mapTiles
                             ->rowStart[MAP_STATE->actingUnitDestY]] == 0) {
    return;
  }
  dx = MAP_STATE->actingUnitDestX - (MAP_STATE->unk_1c4->scrollX >> 4);
  if (dx < 0) {
    return;
  }
  if (dx > 15) {
    return;
  }
  dy = MAP_STATE->actingUnitDestY - (MAP_STATE->unk_1c4->scrollY >> 4);
  if (dy < 0) {
    return;
  }
  if (dy > 11) {
    return;
  }
  Sound_PlaySe(emerge == 0 ? 157 : 158);
  if (unitType == UNIT_SUB) {
    gfx3d = gSubDiveGfxFile3d;
    gfx2d = gSubDiveGfxFile2d;
    palette = gSubDivePaletteFile;
    anim = gSubDiveAnim;
    /* needed to match: an if, not a ternary, and after the file picks */
    if (emerge == 0) {
      animation = 0;
    } else {
      animation = 1;
    }
  } else {
    gfx3d = gStealthHideGfxFile3d;
    gfx2d = gStealthHideGfxFile2d;
    palette = gStealthHidePaletteFile;
    anim = gStealthHideAnim;
    animation = 0;
  }
  if (data_027c027c == data_027c0288.unk_00) {
    FS_ChangeDir(data_02166cc8);
    Gfx_QueueUnpackedFileCopy(gfx3d, data_022a7c5c->unk_68);
    Gfx_QueueFileCopy(palette, data_022a7c5c->unk_6c, 32);
    pos.x = (MAP_STATE->actingUnitDestX * 16 + 8) * 0x2000 / 256;
    pos.y = (MAP_STATE->actingUnitDestY * 16 + 8) * 0x6000 / 768;
    pos.z = 8;
    SpriteAnim3dTask_StartAlt(anim, &pos, (u32)data_022a7c5c->unk_68,
                              (u32)data_022a7c5c->unk_6c, animation, 0);
  } else {
    FS_ChangeDir(data_02166cc8);
    File_UnpackTo(data_027c0280->objVram + 0x3280, gfx2d);
    Palette_LoadFromFile(palette, 0, 608, 32);
    SpriteAnimTask_Start(
        anim, MAP_STATE->actingUnitDestX * 16 - MAP_STATE->unk_1c4->scrollX + 8,
        MAP_STATE->actingUnitDestY * 16 - MAP_STATE->unk_1c4->scrollY + 8,
        0x3194, animation, 0);
  }
  Task_Create(data_02166410, TASK_LIST(3));
}
