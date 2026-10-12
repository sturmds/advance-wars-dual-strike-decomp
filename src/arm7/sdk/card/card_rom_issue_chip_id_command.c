#include "types.h"
#include "sdk/card_arm7.h"

void CardRom_LoadCommandBytes_Arm7(u32 commandHigh, u32 commandLow);

/* Sends the read ID command to the ROM and returns the 4 byte answer. */
u32 CardRom_IssueChipIdCommand(void)
{
    CardRom_LoadCommandBytes_Arm7((u32)GAMECARD_OP_GET_CHIP_ID << 24, 0);
    IO_GAMECARD_ROM_CTRL = ((BOOT_HEADER_CARD_TIMING & ~CARD_COMMAND_MASK) | (CARD_START | CARD_RESET_HI | GAMECARD_REPLY_LEN_4)) & ~GAMECARD_GAP_CLOCK_BITS;
    while ((IO_GAMECARD_ROM_CTRL & CARD_DATA_READY) == 0) {
    }
    return IO_GAMECARD_READ_PORT;
}
