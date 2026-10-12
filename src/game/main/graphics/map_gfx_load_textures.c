#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_022a7c5c.h"
#include "game/map_tileset_files.h"
#include "game/player.h"

void FS_ChangeDir(const char* dir);
u32 File_UnpackTo(void* dest, const char* path);
void Gfx_LoadTexImageFile(const char* fileName, u32 offset);
void Gfx_LoadTexPaletteFile(const char* fileName, u32 offset, u32 size);
const char* MapGfx_GetPaletteFile(int weather, int tileset);
const char* MapGfx_GetWeatherPaletteFile(int weather, int tileset);
u8 Player_GetArmyStyle(int playerIndex);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_02080440_48 data_02183b08;
extern char data_0216a1a0[]; /* "/bmap" */
extern char data_0216a1a8[]; /* "020" */
extern char data_0216a1ac[]; /* "021" */
extern char data_0216a1b0[]; /* "022" */
extern char data_0216a1b4[]; /* "023" */
extern char data_0216a1b8[]; /* "024" */
extern char data_0216a1bc[]; /* "025" */
extern char data_0216a1c0[]; /* "026" */
extern char data_0216a1c4[]; /* "017" */
extern char data_0216a1c8[]; /* "018" */
extern char data_0216a1cc[]; /* "019" */
extern char data_0216a1d0[]; /* "01a" */
extern char data_0216a1d4[]; /* "01b" */

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Loads the map's 3D textures: two tileset images, the four frames of the
 * animated tiles, an image in each playing army's style,
 * and the map palette. Map 0xf8 on the first map state has its own
 * animation frames. */
void MapGfx_LoadTextures(int weather, int tileset) {
  /* needed to match: this declaration order sets the stack slots */
  const char* texA;
  const char* anim0;
  Unk_022a7c5c* gfx;
  const char* anim1;
  const char* anim2;
  const char* armyFile;
  const char* palette;
  int slot;
  const char* texB;

  gfx = data_022a7c5c;
  /* both come from the record before the tileset's */
  texA = (&data_02167e30[-1])[tileset & 0x7f].texImage1;
  texB = (&data_02167e30[-1])[tileset & 0x7f].texImage2;

  anim0 = data_0216a1a8;
  anim1 = data_0216a1ac;
  anim2 = data_0216a1b0;
  palette = MapGfx_GetPaletteFile(weather, tileset);
  MapGfx_GetWeatherPaletteFile(weather, tileset); /* result unused */
  if (GAME == &data_02183b08 && GAME->unk_1c4->mapId == 0xf8) {
    anim0 = data_0216a1b8;
    anim1 = data_0216a1bc;
    anim2 = data_0216a1c0;
  }

  FS_ChangeDir(data_0216a1a0);
  Gfx_LoadTexImageFile(texA, gfx->mapTexOffset);
  Gfx_LoadTexImageFile(texB, gfx->mapTex2Offset);
  Gfx_LoadTexImageFile(anim0, (u32)gfx->animDst);

  for (slot = 1; slot < 5; slot++) {
    if (GAME->players[slot].controller != PLAYER_CONTROLLER_NONE) {
      switch (Player_GetArmyStyle(slot)) {
        default:
          armyFile = data_0216a1c4;
          break;
        case 2:
          armyFile = data_0216a1c8;
          break;
        case 3:
          armyFile = data_0216a1cc;
          break;
        case 4:
          armyFile = data_0216a1d0;
          break;
        case 5:
          armyFile = data_0216a1d4;
          break;
      }
      Gfx_LoadTexImageFile(armyFile,
                           gfx->mapTexOffset + 0xa00 + (slot - 1) * 0x100);
    }
  }

  File_UnpackTo(gfx->animFrames[0], anim0);
  File_UnpackTo(gfx->animFrames[1], anim1);
  File_UnpackTo(gfx->animFrames[2], anim2);
  File_UnpackTo(gfx->animFrames[3], data_0216a1b4);
  Gfx_LoadTexPaletteFile(palette, gfx->texPaletteOffset, 0x200);
}
