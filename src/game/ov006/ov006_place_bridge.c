#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"
#include "game/design_map_edit.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

int Ov006_GetNeighborMask(int x, int y);
int func_ov006_02360308(int x, int y);
int func_ov006_023600e8(int x, int y);
int Ov006_IsPlainRiver(int x, int y);
void func_ov006_02365eec(int x, int y);
void func_ov006_0235ddac(int x, int y);
void func_ov006_02351714(int x, int y, int terrain);
void func_ov006_02351828(int x, int y, int cell);
void func_ov006_0235d7dc(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) (STATE->mapTiles->tiles[(x) + STATE->mapTiles->rowStart[y]])

#define PUT_SEA_BRIDGE(cell)                         \
    func_ov006_02365eec(x, y);                       \
    if (terrain == TERRAIN_SHOAL) {                  \
        func_ov006_0235ddac(x, y);                   \
    }                                                \
    func_ov006_02351714(x, y, TERRAIN_BRIDGE);       \
    func_ov006_02351828(x, y, cell);                 \
    func_ov006_0235d7dc(x, y)

/* Puts a bridge on tile x, y where Ov006_CanPlaceBridge allows one, with the
 * piece that fits the neighbours. Sea, shoal and reef tiles: the piece runs
 * between the neighbours Ov006_GetNeighborMask reports, and with only one of
 * them, along the neighbours func_ov006_02360308 reports; a shoal goes
 * through func_ov006_0235ddac first. Other shapes take the cell
 * func_ov006_023600e8 picks. River tiles: the piece runs across the river's
 * flow. */
void Ov006_PlaceBridge(int x, int y)
{
    int terrain = TILE(x, y);
    int cell;

    if (terrain == TERRAIN_SEA || terrain == TERRAIN_SHOAL || terrain == TERRAIN_REEF) {
        switch (Ov006_GetNeighborMask(x, y)) {
        case DESIGN_NB_DOWN:
        case DESIGN_NB_UP:
            switch (func_ov006_02360308(x, y)) {
            case DESIGN_NB_RIGHT:
            case DESIGN_NB_LEFT:
            case DESIGN_NB_LEFT | DESIGN_NB_RIGHT:
                PUT_SEA_BRIDGE(MAP_CELL_BRIDGE_HORIZONTAL);
                return;
            }
        case DESIGN_NB_UP | DESIGN_NB_DOWN:
            PUT_SEA_BRIDGE(MAP_CELL_BRIDGE_VERTICAL);
            return;
        case DESIGN_NB_RIGHT:
        case DESIGN_NB_LEFT:
            switch (func_ov006_02360308(x, y)) {
            case DESIGN_NB_DOWN:
            case DESIGN_NB_UP:
            case DESIGN_NB_UP | DESIGN_NB_DOWN:
                PUT_SEA_BRIDGE(MAP_CELL_BRIDGE_VERTICAL);
                return;
            }
        case DESIGN_NB_LEFT | DESIGN_NB_RIGHT:
            PUT_SEA_BRIDGE(MAP_CELL_BRIDGE_HORIZONTAL);
            return;
        case DESIGN_NB_UP | DESIGN_NB_RIGHT | DESIGN_NB_DOWN:
        case DESIGN_NB_UP | DESIGN_NB_LEFT | DESIGN_NB_DOWN:
            PUT_SEA_BRIDGE(MAP_CELL_BRIDGE_VERTICAL);
            return;
        case DESIGN_NB_LEFT | DESIGN_NB_RIGHT | DESIGN_NB_DOWN:
        case DESIGN_NB_UP | DESIGN_NB_LEFT | DESIGN_NB_RIGHT:
            PUT_SEA_BRIDGE(MAP_CELL_BRIDGE_HORIZONTAL);
            return;
        case 0:
        default:
            if (terrain == TERRAIN_SHOAL) {
                func_ov006_02365eec(x, y);
                func_ov006_0235ddac(x, y);
            }
            cell = func_ov006_023600e8(x, y);
            if (cell > 0) {
                func_ov006_02365eec(x, y);
                func_ov006_02351714(x, y, TERRAIN_BRIDGE);
                func_ov006_02351828(x, y, cell);
            }
            return;
        case DESIGN_NB_RIGHT | DESIGN_NB_DOWN:
        case DESIGN_NB_LEFT | DESIGN_NB_DOWN:
        case DESIGN_NB_UP | DESIGN_NB_RIGHT:
        case DESIGN_NB_UP | DESIGN_NB_LEFT:
            return;
        }
    }
    if (Ov006_IsPlainRiver(x, y) == 0) {
        return;
    }
    cell = 0;
    switch (func_ov006_02360308(x, y)) {
    case 0:
    case DESIGN_NB_UP:
    case DESIGN_NB_UP | DESIGN_NB_DOWN:
        if (Ov006_GetNeighborMask(x, y) & (DESIGN_NB_LEFT | DESIGN_NB_RIGHT)) {
            cell = MAP_CELL_BRIDGE_HORIZONTAL;
        } else if ((x > 0 && TILE(x - 1, y) == TERRAIN_BRIDGE)
                   || (x < STATE->unk_1c4->mapWidth - 1 && TILE(x + 1, y) == TERRAIN_BRIDGE)) {
            cell = MAP_CELL_BRIDGE_HORIZONTAL;
        }
        break;
    case DESIGN_NB_RIGHT:
    case DESIGN_NB_LEFT:
    case DESIGN_NB_LEFT | DESIGN_NB_RIGHT:
        if (Ov006_GetNeighborMask(x, y) & (DESIGN_NB_UP | DESIGN_NB_DOWN)) {
            cell = MAP_CELL_BRIDGE_VERTICAL;
        } else if ((y > 0 && TILE(x, y - 1) == TERRAIN_BRIDGE)
                   || (y < STATE->unk_1c4->mapHeight - 1 && TILE(x, y + 1) == TERRAIN_BRIDGE)) {
            cell = MAP_CELL_BRIDGE_VERTICAL;
        }
        break;
    }
    if (cell > 0) {
        func_ov006_02365eec(x, y);
        func_ov006_02351714(x, y, TERRAIN_BRIDGE);
        func_ov006_02351828(x, y, cell);
    }
}
