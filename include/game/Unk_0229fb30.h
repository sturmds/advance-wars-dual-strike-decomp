#ifndef GAME_UNK_0229FB30_H
#define GAME_UNK_0229FB30_H

#include "types.h"

/* data_0229fb30 (main bss). */
typedef struct Unk_0229fb30 {
    u8 pad_00[2];
    u8 unk_02;                         /* Battle_ShowQuote sets it to 1 */
    u8 unk_03;                         /* 1-3: which of three frame graphics the battle screen loads */
    u8 pad_04[2];
    u8 unk_06[2];                      /* per side; nonzero: BattleZoomTask_Update draws a sprite pair for the side */
    u16 unk_08;                        /* the battle screen ends when unk_08 == unk_0c and unk_0a == unk_0e */
    u16 unk_0a;
    u16 unk_0c;
    u16 unk_0e;
    u32 unk_10;                        /* frame counter of the battle screen */
    u8 pad_14[0x45a - 0x14];
    u8 unk_45a;                        /* index into MenuState.combat.players */
    s8 unk_45b;                        /* menu entry 0-4 whose help text LinkMenu_DrawHelpText shows */
    u8 pad_45c[0x630 - 0x45c];
    u8 unk_630;                        /* stage of the two-player link menu, 0-3 */
    u8 unk_631;                        /* link player compared with func_0207a260() to tell the two sides apart */
    u8 unk_632;                        /* button id picked by the player unk_631 names */
    u8 unk_633;                        /* button id picked by the other player */
} Unk_0229fb30;

extern Unk_0229fb30 data_0229fb30;

#endif
