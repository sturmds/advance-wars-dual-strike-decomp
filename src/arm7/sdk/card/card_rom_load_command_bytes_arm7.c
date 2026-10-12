#include "types.h"
#include "sdk/card_arm7.h"

/* Waits for the ROM port, then loads the 8 command bytes, most significant first. */
void CardRom_LoadCommandBytes_Arm7(u32 commandHigh, u32 commandLow)
{
    while (IO_GAMECARD_ROM_CTRL & CARD_START) {
    }
    IO_GAMECARD_MASTER = CARDMST_ENABLE | CARDMST_IRQ;
    IO_GAMECARD_CMD_BYTES[0] = commandHigh >> 24;
    IO_GAMECARD_CMD_BYTES[1] = commandHigh >> 16;
    IO_GAMECARD_CMD_BYTES[2] = commandHigh >> 8;
    IO_GAMECARD_CMD_BYTES[3] = commandHigh;
    IO_GAMECARD_CMD_BYTES[4] = commandLow >> 24;
    IO_GAMECARD_CMD_BYTES[5] = commandLow >> 16;
    IO_GAMECARD_CMD_BYTES[6] = commandLow >> 8;
    IO_GAMECARD_CMD_BYTES[7] = commandLow;
}
