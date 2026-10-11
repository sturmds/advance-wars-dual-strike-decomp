#include "types.h"
#include "game/map_select.h"

void func_02064774(void)
{
    int list;
    int page;
    int tab;

    TABLES.unk_108 = 0;
    for (tab = 0; tab < 4; tab++) {
        for (list = 0; list < 13; list++) {
            TABLES.listOfTab[tab] = 0;
            for (page = 0; page < 4; page++) {
                TABLES.unk_000[tab][list][page] = 0;
            }
            TABLES.unk_0d0[tab][list] = 0;
        }
    }
}
