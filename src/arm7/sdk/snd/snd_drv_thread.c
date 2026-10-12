#include "sdk/os_arm7.h"
#include "sdk/snd.h"
#include "sdk/snd_hw.h"

void func_arm7_037fe534(int volume);   /* writes the master volume register */
int func_arm7_037fe654(void);         /* sets the master enable bit */

#define SOUND_MAIN_LOUDNESS_FULL 127

/* Main loop of the sound driver: runs once per interval alarm or command message. */
void SndDrv_Thread(void *arg)
{
    SND_InitIntervalTimer();
    SndExChannel_InitAll();
    SndArm7_InitSeq();
    SndArm7_InitAlarms();
    func_arm7_037fe654();
    SndHw_SetOutputControl(0, 0, 0, 0);
    func_arm7_037fe534(SOUND_MAIN_LOUDNESS_FULL);
    SND_StartIntervalTimer();

    while (TRUE) {
        BOOL periodic = FALSE;

        switch ((u32)SND_WaitForIntervalTimer()) {
        case SOUND_THREAD_MSG_TICK:
            periodic = TRUE;
            break;
        case SOUND_THREAD_MSG_NEW_REQUESTS:
            break;
        }
        SndExChannel_Update();
        SndArm7_ProcessCommands();
        SndArm7_SeqMain(periodic);
        SndExChannel_Main(periodic);
        SndArm7_UpdateSharedWork();
        SndDrv_NextRandom();
    }
}
