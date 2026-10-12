#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL func_0209dd7c(void);
BOOL Player_HasHq(int player);
BOOL AiUnit_FindEnemyTarget(s16* tile);
BOOL func_020aac60(s16* tile);
BOOL Ai_FindCaptureSiteTile(s16* tile);
BOOL AiTransport_PickFootUnit(s16* tile);
BOOL Ai_FindNearestOwnPropertyTile(s16* tile);
BOOL Ai_FindOwnHqTile(s16* tile);
BOOL func_020a9f9c(s16* tile);
BOOL Ai_FindReachableAiTarget(s16* tile);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Counterpart of AiUnit_ActByOrder: runs the target tile search of the
 * acting unit's AI order and writes the tile to site. FALSE: the search
 * found none. Order 7 gives the unit's own tile. The site is where
 * AiUnit_RetreatTowardSite heads for. */
BOOL AiUnit_PickOrderSite(s16* site) {
  switch (STATE->actingUnit->aiOrder) {
    case 0:
    case 1:
      if (!AiUnit_FindEnemyTarget(site)) {
        return FALSE;
      }
      break;
    case 2:
      if (func_0209dd7c() == TRUE) {
        if (!func_020aac60(site)) {
          return FALSE;
        }
      } else {
        if (!AiUnit_FindEnemyTarget(site)) {
          return FALSE;
        }
      }
      break;
    case 3:
      if (!Ai_FindCaptureSiteTile(site)) {
        return FALSE;
      }
      break;
    case 4:
      if (!AiTransport_PickFootUnit(site)) {
        return FALSE;
      }
      break;
    case 5:
      if (!Ai_FindNearestOwnPropertyTile(site)) {
        return FALSE;
      }
      break;
    case 6:
      if (Player_HasHq(STATE->currentPlayer) == TRUE) {
        if (!Ai_FindOwnHqTile(site)) {
          return FALSE;
        }
      } else {
        if (!Ai_FindNearestOwnPropertyTile(site)) {
          return FALSE;
        }
      }
      break;
    case 8:
    case 10:
      if (!func_020a9f9c(site)) {
        return FALSE;
      }
      break;
    case 9:
      if (!Ai_FindReachableAiTarget(site)) {
        return FALSE;
      }
      break;
    case 7:
      site[0] = STATE->actingUnit->x;
      site[1] = STATE->actingUnit->y;
      break;
    default:
      return FALSE;
  }
  return TRUE;
}
