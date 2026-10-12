#include "types.h"
#include "sdk/pm.h"
#include "sdk/spi.h"

int func_arm7_03805678(void);           /* the current LED pattern */

/* Takes the power chip words from the SPI PXI handler: collects the words
 * of a request, then answers it at once or queues it for the SPI thread. */
void PM_AnalyzeCommand(u32 data)
{
    u16 command;

    if (data & SERIAL_MSG_FIRST_WORD) {
        int i;
        for (i = 0; i < POWER_MSG_MAX_HALFWORDS; i++) {
            data_arm7_03809ffc.words[i] = 0;
        }
    }

    data_arm7_03809ffc.words[(data & SERIAL_MSG_SLOT_BITS) >> SERIAL_MSG_SLOT_POS] = data;

    if (!(data & SPI_PXI_END_BIT)) {
        return;
    }

    command = (data_arm7_03809ffc.words[0] & 0xff00) >> 8;
    switch (command) {
    case POWER_MSG_HANDSHAKE:
        SPIi_ReturnResult(POWER_MSG_HANDSHAKE, SPI_PXI_RESULT_SUCCESS);
        break;
    case POWER_MSG_PREPARE_SLEEP:
        if (!SPIi_SetEntry(SERIAL_OWNER_POWER_CHIP, command, 2, data_arm7_03809ffc.words[0] & 0xff,
                           data_arm7_03809ffc.words[1] & 0xffff)) {
            SPIi_ReturnResult(command, SPI_PXI_RESULT_EXCLUSIVE);
        }
        break;
    case POWER_MSG_STORE_REGISTER:
        if (!SPIi_SetEntry(SERIAL_OWNER_POWER_CHIP, command, 2, data_arm7_03809ffc.words[0] & 0xff,
                           data_arm7_03809ffc.words[1] & 0xffff)) {
            SPIi_ReturnResult(command, SPI_PXI_RESULT_EXCLUSIVE);
        }
        break;
    case POWER_MSG_FETCH_REGISTER:
        if (!SPIi_SetEntry(SERIAL_OWNER_POWER_CHIP, command, 1, data_arm7_03809ffc.words[0] & 0xffff)) {
            SPIi_ReturnResult(command, SPI_PXI_RESULT_EXCLUSIVE);
        }
        break;
    case POWER_MSG_APPLY_ORDER:
        if (!SPIi_SetEntry(SERIAL_OWNER_POWER_CHIP, command, 1,
                           ((data_arm7_03809ffc.words[0] & 0xff) << 16) | (data_arm7_03809ffc.words[1] & 0xffff))) {
            SPIi_ReturnResult(command, SPI_PXI_RESULT_EXCLUSIVE);
        }
        break;
    case POWER_MSG_LED_STATE:
        PowerLed_SelectPattern(data_arm7_03809ffc.words[0] & 0xff);
        SPIi_ReturnResult(POWER_MSG_LED_STATE, SPI_PXI_RESULT_SUCCESS);
        break;
    case POWER_MSG_QUERY_BLINK_SCRIPT:
        SPIi_ReturnResult(POWER_MSG_QUERY_BLINK_SCRIPT, func_arm7_03805678());
        break;
    default:
        SPIi_ReturnResult(command, SPI_PXI_RESULT_INVALID_COMMAND);
        break;
    }
}
