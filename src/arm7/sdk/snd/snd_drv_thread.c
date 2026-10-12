#include "sdk/snd_drv.h"

void SndArm7_InitAlarms(void);
u16 SndDrv_NextRandom(void);
void SndHw_SetOutputControl(int leftOutput, int rightOutput, int ch1Direct, int ch3Direct);
void func_arm7_037fe534(int volume);   /* writes the master volume register */
int func_arm7_037fe654(void);         /* sets the master enable bit */
void func_arm7_037ff9e8(BOOL periodic);
void func_arm7_037ffcf4(void);
void func_arm7_037ffee0(void);
void SndArm7_SeqMain(BOOL periodic);
void SndArm7_InitSeq(void);
void SndArm7_UpdateSharedWork(void);
void func_arm7_03802408(void);

#define SOUND_MAIN_LOUDNESS_FULL 127

/* Main loop of the sound driver: runs once per interval alarm or command message. */
void SndDrv_Thread(void *arg)
{
    SND_InitIntervalTimer();
    func_arm7_037ffee0();
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
        func_arm7_037ffcf4();
        func_arm7_03802408();
        SndArm7_SeqMain(periodic);
        func_arm7_037ff9e8(periodic);
        SndArm7_UpdateSharedWork();
        SndDrv_NextRandom();
    }
}
