#include "types.h"
#include "game/task.h"
#include "game/map_unit.h"
#include "game/player.h"
#include "game/terrain.h"
#include "game/Unk_02080440.h"
#include "game/play_records.h"
#include "game/visual_mode.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u32 gLinkPlay;
extern const TaskScriptCmd data_02168d88[];

s32 HwDiv32(s32 numerator, s32 denominator);
int Player_GetCaptureRate(int playerIndex);
int CaptureIncrease(int player, int slot);
void ActingUnit_ResetCaptureIfMoved(void);
void ActingUnit_StartAtDest(void);
void Game_EnterState18(void);
int Capture_PlayCompleteSound(int owner, int tile);
void Capture_StartDisplay(int x, int y, int oldProgress, int newProgress,
                          int owner);
void Tile_SetOwner(int x, int y);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* HP as shown on the map: 1-10, rounded up. */
#define SHOWN_HP(unit) ((unit)->hp != 0 ? HwDiv32((unit)->hp - 1, 10) + 1 : 0)

/* Index into a per-tile array of MapTiles. */
#define TILE_INDEX(x, y) ((x) + GAME->mapTiles->rowStart[y])

/* MAP_UNIT_OWNER, written as the original masks the slot before shifting. */
#define ACTING_UNIT_OWNER ((((GAME->actingUnit - GAME->units) & 0xc0) >> 6) + 1)

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

  shownHp = SHOWN_HP(GAME->actingUnit);
  progress =
      HwDiv32(shownHp * Player_GetCaptureRate(GAME->currentPlayer), 100) +
      CaptureIncrease(GAME->currentPlayer, 0);
  tile = GAME->mapTiles
             ->tiles[TILE_INDEX(GAME->actingUnitDestX, GAME->actingUnitDestY)];
  ActingUnit_ResetCaptureIfMoved();
  ActingUnit_StartAtDest();

  oldProgress = GAME->actingUnit->captureProgress;
  progress += GAME->actingUnit->captureProgress;
  if (progress >= MAP_UNIT_CAPTURE_POINTS) {
    progress = MAP_UNIT_CAPTURE_POINTS;
  }

  if (GAME->players[GAME->currentPlayer].controller ==
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

  if ((GAME->unk_1c4->fog == 0 && GAME->unk_1c4->weather != WEATHER_RAIN) ||
      (GAME->players[GAME->currentPlayer].visionGrids & 2) ||
      GAME->mapTiles->unk_2338[TILE_INDEX(GAME->actingUnitDestX,
                                          GAME->actingUnitDestY)] != 0) {
    if (GAME->visualMode == VISUAL_A || GAME->visualMode == VISUAL_B ||
        MAP_TILE_TERRAIN(tile) == TERRAIN_HQ ||
        MAP_TILE_TERRAIN(tile) == TERRAIN_LAB) {
      Capture_StartDisplay(GAME->actingUnitDestX, GAME->actingUnitDestY,
                           GAME->actingUnit->captureProgress, progress,
                           ACTING_UNIT_OWNER);
    } else if (progress >= MAP_UNIT_CAPTURE_POINTS) {
      Capture_PlayCompleteSound(ACTING_UNIT_OWNER, MAP_TILE_TERRAIN(tile));
    }
  } else if (gLinkPlay != 0 &&
             (GAME->visualMode == VISUAL_A || GAME->visualMode == VISUAL_B) &&
             GAME->players[GAME->currentPlayer].controller ==
                 PLAYER_CONTROLLER_AI) {
    /* the tile is out of sight: no capture scene, but link play still needs the task */
    Task_Create(data_02168d88, TASK_LIST(3));
  }

  GAME->actingUnit->captureProgress = progress;
  if (GAME->actingUnit->captureProgress >= MAP_UNIT_CAPTURE_POINTS) {
    GAME->actingUnit->captureProgress = 0;
    if (MAP_TILE_TERRAIN(tile) == TERRAIN_HQ ||
        MAP_TILE_TERRAIN(tile) == TERRAIN_LAB) {
      GAME->players[MAP_TILE_OWNER(tile)].unk_34 = 1;
    }
    Tile_SetOwner(GAME->actingUnitDestX, GAME->actingUnitDestY);
  }
  Game_EnterState18();
}
