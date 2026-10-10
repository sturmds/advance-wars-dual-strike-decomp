#include "types.h"

BOOL func_ov006_02365e30(int hqCell, int *x, int *y);   /* the army's HQ position, if it has one */
int Ov006_CountPropertyTilesMatching(int cell);                        /* count of tiles with that cell */

/* Cell codes of armies 1-4: each army's cells are 32 codes apart. */
#define CELL_HQ_ARMY1 40
#define CELL_HQ_ARMY2 72
#define CELL_HQ_ARMY3 104
#define CELL_HQ_ARMY4 136

/* Number of production buildings (cells HQ + 2, + 3 and + 6; likely base,
 * airport and port, which fits Ov006_IsMapPlayable) of the army
 * whose HQ cell is hqCell, or -1 when that army has no HQ on the map. */
int Ov006_CountArmyFactories(int hqCell)
{
    int x;
    int y;
    int count;

    if (!func_ov006_02365e30(hqCell, &x, &y)) {
        return -1;
    }
    count = 0;
    switch (hqCell) {
    case CELL_HQ_ARMY1:
        count += Ov006_CountPropertyTilesMatching(CELL_HQ_ARMY1 + 6);
        count += Ov006_CountPropertyTilesMatching(CELL_HQ_ARMY1 + 2);
        count += Ov006_CountPropertyTilesMatching(CELL_HQ_ARMY1 + 3);
        break;
    case CELL_HQ_ARMY2:
        count += Ov006_CountPropertyTilesMatching(CELL_HQ_ARMY2 + 6);
        count += Ov006_CountPropertyTilesMatching(CELL_HQ_ARMY2 + 2);
        count += Ov006_CountPropertyTilesMatching(CELL_HQ_ARMY2 + 3);
        break;
    case CELL_HQ_ARMY3:
        count += Ov006_CountPropertyTilesMatching(CELL_HQ_ARMY3 + 6);
        count += Ov006_CountPropertyTilesMatching(CELL_HQ_ARMY3 + 2);
        count += Ov006_CountPropertyTilesMatching(CELL_HQ_ARMY3 + 3);
        break;
    case CELL_HQ_ARMY4:
        count += Ov006_CountPropertyTilesMatching(CELL_HQ_ARMY4 + 6);
        count += Ov006_CountPropertyTilesMatching(CELL_HQ_ARMY4 + 2);
        count += Ov006_CountPropertyTilesMatching(CELL_HQ_ARMY4 + 3);
        break;
    }
    return count;
}
