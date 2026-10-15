#include "types.h"
#include "game/task.h"
#include "game/map_unit.h"
#include "game/player.h"
#include "game/terrain.h"
#include "game/Unk_02080440.h"
#include "game/play_records.h"
#include "game/visual_mode.h"
#include "game/math.h"

extern u32 gLinkPlay;
extern const TaskScriptCmd data_02168d88[];

int Player_GetCaptureRate(int playerIndex);
int CaptureIncrease(int player, int slot);
void ActingUnit_ResetCaptureIfMoved(void);
void ActingUnit_StartAtDest(void);
void Game_EnterState18(void);
int Capture_PlayCompleteSound(int owner, int tile);
void Capture_StartDisplay(int x, int y, int oldProgress, int newProgress,
                          int owner);
void Tile_SetOwner(int x, int y);

/* HP as shown on the map: 1-10, rounded up. */
#define SHOWN_HP(unit) ((unit)->hp != 0 ? HwDiv32((unit)->hp - 1, 10) + 1 : 0)

/* MAP_UNIT_OWNER, written as the original masks the slot before shifting. */
#define ACTING_UNIT_OWNER \
  ((((MAP_STATE->actingUnit - MAP_STATE->units) & 0xc0) >> 6) + 1)

/* Unit action 3: the acting unit captures the tile it ends its move on.
 * Capture points added are the shown HP times the player's capture rate
 * (percent), plus CaptureIncrease, up to MAP_UNIT_CAPTURE_POINTS. At the
 * limit the tile changes owner (Tile_SetOwner) and an HQ or Lab marks its
 * old owner. The scene functions show the progress; with fog or rain they
 * are skipped for a tile the current player cannot see. Human players
 * count captures begun and finished in gPlayRecords. */
void UnitAction_Capture(void) {
  int shownHp;
  int progress;
  u8 tile;
  int oldProgress;

  shownHp = SHOWN_HP(MAP_STATE->actingUnit);
  progress =
      HwDiv32(shownHp * Player_GetCaptureRate(MAP_STATE->currentPlayer), 100) +
      CaptureIncrease(MAP_STATE->currentPlayer, 0);
  tile = MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(MAP_STATE->actingUnitDestX,
                                                   MAP_STATE->actingUnitDestY)];
  ActingUnit_ResetCaptureIfMoved();
  ActingUnit_StartAtDest();

  oldProgress = MAP_STATE->actingUnit->captureProgress;
  progress += MAP_STATE->actingUnit->captureProgress;
  if (progress >= MAP_UNIT_CAPTURE_POINTS) {
    progress = MAP_UNIT_CAPTURE_POINTS;
  }

  if (MAP_STATE->players[MAP_STATE->currentPlayer].controller ==
      PLAYER_CONTROLLER_HUMAN) {
    if (oldProgress == 0) {
      if (gPlayRecords.capturesStarted < 999999) {
        gPlayRecords.capturesStarted++;
      }
    }
    if (progress == MAP_UNIT_CAPTURE_POINTS) {
      if (gPlayRecords.capturesCompleted < 999999) {
        gPlayRecords.capturesCompleted++;
      }
    }
  }

  if ((MAP_STATE->unk_1c4->fog == 0 &&
       MAP_STATE->unk_1c4->weather != WEATHER_RAIN) ||
      (MAP_STATE->players[MAP_STATE->currentPlayer].visionGrids & 2) ||
      MAP_STATE->mapTiles->unk_2338[MAP_TILE_INDEX(
          MAP_STATE->actingUnitDestX, MAP_STATE->actingUnitDestY)] != 0) {
    if (MAP_STATE->visualMode == VISUAL_A ||
        MAP_STATE->visualMode == VISUAL_B ||
        MAP_TILE_TERRAIN(tile) == TERRAIN_HQ ||
        MAP_TILE_TERRAIN(tile) == TERRAIN_LAB) {
      Capture_StartDisplay(
          MAP_STATE->actingUnitDestX, MAP_STATE->actingUnitDestY,
          MAP_STATE->actingUnit->captureProgress, progress, ACTING_UNIT_OWNER);
    } else if (progress >= MAP_UNIT_CAPTURE_POINTS) {
      Capture_PlayCompleteSound(ACTING_UNIT_OWNER, MAP_TILE_TERRAIN(tile));
    }
  } else if (gLinkPlay != 0 &&
             (MAP_STATE->visualMode == VISUAL_A ||
              MAP_STATE->visualMode == VISUAL_B) &&
             MAP_STATE->players[MAP_STATE->currentPlayer].controller ==
                 PLAYER_CONTROLLER_AI) {
    /* the tile is out of sight: no capture scene, but link play still needs the task */
    Task_Create(data_02168d88, TASK_LIST(3));
  }

  MAP_STATE->actingUnit->captureProgress = progress;
  if (MAP_STATE->actingUnit->captureProgress >= MAP_UNIT_CAPTURE_POINTS) {
    MAP_STATE->actingUnit->captureProgress = 0;
    if (MAP_TILE_TERRAIN(tile) == TERRAIN_HQ ||
        MAP_TILE_TERRAIN(tile) == TERRAIN_LAB) {
      MAP_STATE->players[MAP_TILE_OWNER(tile)].unk_34 = 1;
    }
    Tile_SetOwner(MAP_STATE->actingUnitDestX, MAP_STATE->actingUnitDestY);
  }
  Game_EnterState18();
}
