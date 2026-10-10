#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

int Ov006_GetNeighborMask(int x, int y);
int func_ov006_02360308(int x, int y);
int func_ov006_023600e8(int x, int y);
int Ov006_IsPlainRiver(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) (STATE->mapTiles->tiles[(x) + STATE->mapTiles->rowStart[y]])

/* Neighbour bits of Ov006_GetNeighborMask and func_ov006_02360308. */
#define NB_DOWN  1
#define NB_RIGHT 2
#define NB_LEFT  4
#define NB_UP    8

/* Bridge pieces func_ov006_0235fb54 places: across a river that runs up and
 * down, or across one that runs left and right. */
#define BRIDGE_PIECE_ACROSS_VERTICAL   21
#define BRIDGE_PIECE_ACROSS_HORIZONTAL 22

/* TRUE when tile x, y can hold a bridge: it already is one; it is sea, shoal
 * or reef with a fitting set of neighbours (Ov006_GetNeighborMask); or it is
 * a river tile that Ov006_IsPlainRiver accepts, with a neighbour or a bridge
 * on either side across the river's flow (func_ov006_02360308 gives the
 * flow). The inner switches that return TRUE either way are needed to
 * match. */
int Ov006_CanPlaceBridge(int x, int y)
{
    int terrain = TILE(x, y);
    int piece;

    if (terrain == TERRAIN_BRIDGE) {
        return TRUE;
    }
    if (terrain == TERRAIN_SEA || terrain == TERRAIN_SHOAL || terrain == TERRAIN_REEF) {
        switch (Ov006_GetNeighborMask(x, y)) {
        case NB_DOWN:
        case NB_UP:
            switch (func_ov006_02360308(x, y)) {
            case NB_RIGHT:
            case NB_LEFT:
            case NB_LEFT | NB_RIGHT:
                return TRUE;
            }
        case NB_UP | NB_DOWN:
            return TRUE;
        case NB_RIGHT:
        case NB_LEFT:
            switch (func_ov006_02360308(x, y)) {
            case NB_DOWN:
            case NB_UP:
            case NB_UP | NB_DOWN:
                return TRUE;
            }
        case NB_LEFT | NB_RIGHT:
            return TRUE;
        case NB_UP | NB_RIGHT | NB_DOWN:
        case NB_UP | NB_LEFT | NB_DOWN:
            return TRUE;
        case NB_LEFT | NB_RIGHT | NB_DOWN:
        case NB_UP | NB_LEFT | NB_RIGHT:
            return TRUE;
        case 0:
        default:
            if (func_ov006_023600e8(x, y) > 0) {
                return TRUE;
            }
        case NB_RIGHT | NB_DOWN:
        case NB_LEFT | NB_DOWN:
        case NB_UP | NB_RIGHT:
        case NB_UP | NB_LEFT:
            return FALSE;
        }
    }
    if (Ov006_IsPlainRiver(x, y) == 0) {
        return FALSE;
    }
    piece = 0;
    switch (func_ov006_02360308(x, y)) {
    case 0:
    case NB_UP:
    case NB_UP | NB_DOWN:
        if (Ov006_GetNeighborMask(x, y) & (NB_LEFT | NB_RIGHT)) {
            piece = BRIDGE_PIECE_ACROSS_VERTICAL;
        } else if ((x > 0 && TILE(x - 1, y) == TERRAIN_BRIDGE)
                   || (x < STATE->unk_1c4->mapWidth - 1 && TILE(x + 1, y) == TERRAIN_BRIDGE)) {
            piece = BRIDGE_PIECE_ACROSS_VERTICAL;
        }
        break;
    case NB_RIGHT:
    case NB_LEFT:
    case NB_LEFT | NB_RIGHT:
        if (Ov006_GetNeighborMask(x, y) & (NB_UP | NB_DOWN)) {
            piece = BRIDGE_PIECE_ACROSS_HORIZONTAL;
        } else if ((y > 0 && TILE(x, y - 1) == TERRAIN_BRIDGE)
                   || (y < STATE->unk_1c4->mapHeight - 1 && TILE(x, y + 1) == TERRAIN_BRIDGE)) {
            piece = BRIDGE_PIECE_ACROSS_HORIZONTAL;
        }
        break;
    }
    return piece > 0;
}
