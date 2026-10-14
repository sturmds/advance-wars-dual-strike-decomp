#include "types.h"
#include "game/ov003_unit.h"

extern Ov003Unit data_ov003_02369538[];
extern u8
    data_ov003_02365d10[]; /* per tile code: bits 5-7 row, bits 0-4 terrain */
extern Ov003TileInfo data_ov003_02364d88[];

#define STATE data_ov003_02364928
#define MAP data_ov003_02364a88

void Sound_PlaySe(int id);
int Ov003_IsTerrainSpecial(int terrain);
int Ov003Unit_CompareOnTile(Ov003Unit* unit);
int func_ov003_0235d4c0(int row);
void Ov003_CaptureProperty(int tile, int terrain, int row);
int func_ov003_02362098(int row);
void Ov003Row_AppendCell(int row, int tile, int flag);
void Ov003_PointTaskCreate(int x, int y);

/* Per frame, for each unit that is in use: a unit standing on a tile that
 * belongs to another row counts frames in unk_04 (reset when it moves to a
 * new tile). Once the count reaches the tile's claimFrames + 30,
 * Ov003_CaptureProperty hands the tile to the unit's row (guess from the
 * arguments) and the count restarts; Ov003Unit_CompareOnTile decides whether the
 * count grows (0), waits at 30 (1) or resets (2). A claim by another row
 * than Ov003State.row plays sound 248 for the current slot's unit.
 * Sound 247 plays at most once per call, when a unit of another row has
 * counted past 30 frames on the current slot's row, on a frame where the
 * state counter unk_1c8 is a multiple of 32 (terrain 8) or 64 (other terrain).
 */
void Ov003_UpdateTileClaims(void) {
  Ov003Unit* unit;
  int i;
  int tileIndex;
  u8 terrain;
  u8 soundPlayed;
  u32 row;
  int result;

  unit = data_ov003_02369538;
  soundPlayed = 0;

  for (i = 0; i < 16; i++, unit++) {
    if (unit->hp == 0) {
      continue;
    }
    if (unit->unk_17_6) {
      continue;
    }
    tileIndex = (unit->y >> 7) * MAP->width + (unit->x >> 7);
    terrain = data_ov003_02365d10[MAP->cells[tileIndex]];
    if (Ov003_IsTerrainSpecial(terrain) == 0) {
      unit->unk_04 = 0;
      continue;
    }
    row = (terrain >> 5) & 7;
    if (row == unit->row) {
      unit->unk_04 = 0;
      continue;
    }
    if (unit->lastTile != tileIndex) {
      unit->unk_04 = 0;
    }
    unit->lastTile = tileIndex;
    result = Ov003Unit_CompareOnTile(unit);
    switch (result) {
      case 0:
        if (unit->unk_11 == 0) {
          unit->unk_04++;
          if (unit->unk_04 >= 30) {
            if (func_ov003_0235d4c0(unit->row) == 1) {
              unit->unk_04++;
            }
          }
          if (unit->unk_04 >= data_ov003_02364d88[unit->kind].unk_00 + 30) {
            if (unit->row != STATE->row &&
                unit - data_ov003_02369538 == STATE->currentSlot) {
              Sound_PlaySe(248);
            }
            Ov003_CaptureProperty(tileIndex, terrain, unit->row);
            unit->unk_04 = 0;
            if (func_ov003_02362098(unit->row) == 1 && row == 0 &&
                (terrain & 31) == 14) {
              Ov003Row_AppendCell(unit->row, unit->kind, 1);
            }
            Ov003_PointTaskCreate((unit->x & ~127) >> 3, (unit->y & ~127) >> 3);
          }
        }
        break;
      case 1:
        if (unit->unk_11 == 0 && unit->unk_04 < 30) {
          unit->unk_04++;
        }
        break;
      case 2:
        unit->unk_04 = 0;
        break;
    }
    if (unit->unk_04 > 30 && row == STATE->slots[STATE->currentSlot].row &&
        !soundPlayed) {
      if ((terrain & 31) == 8) {
        if ((STATE->frameCount & 31) == 0) {
          Sound_PlaySe(247);
          soundPlayed = 1;
        }
      } else if ((STATE->frameCount & 63) == 0) {
        Sound_PlaySe(247);
        soundPlayed = 1;
      }
    }
  }
}
