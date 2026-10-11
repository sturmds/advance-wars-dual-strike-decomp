#include "types.h"
#include "game/terrain.h"
#include "game/Unk_ov006_0235f690.h"

int Ov006_GetTilePropertyKind(int x, int y);   /* 0: no property */
void Ov006_SetTerrain(int x, int y, int terrain);
void Ov006_StoreMapCell(int x, int y, int cell);   /* guess: sets the cell code of tile x, y */
void func_ov006_02366064(int x, int y, int cell);
void func_020c4448(void);
int Ov006_GetPipeSeamCell(int x, int y);

extern Unk_ov006_0235f690 *data_ov006_0236d640;

/* unk_2b holds the number of property tiles; read as signed because the code
 * loads it with ldrsb (see ov006_remove_property.c). */
#define PROPERTY_COUNT (*(s8 *)&data_ov006_0236d640->unk_2b)

#define PROPERTY_LIMIT 60

/* Puts a pipe seam on tile x, y with the cell Ov006_GetPipeSeamCell picks.
 * With 60 property tiles on the map already, it only goes ahead when the
 * tile is itself a property. */
void Ov006_PlacePipeSeam(int x, int y)
{
    int cell;

    if (PROPERTY_COUNT >= PROPERTY_LIMIT && Ov006_GetTilePropertyKind(x, y) == 0) {
        return;
    }
    Ov006_SetTerrain(x, y, TERRAIN_PIPE_SEAM);
    cell = Ov006_GetPipeSeamCell(x, y);
    Ov006_StoreMapCell(x, y, cell);
    func_ov006_02366064(x, y, cell);
    func_020c4448();
}
