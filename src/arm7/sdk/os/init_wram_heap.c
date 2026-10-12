#include "types.h"
#include "sdk/os_arm7.h"

/* The last of the nine arena slots (low ends at 0x027ffda0, high ends at
   0x027ffdc4): ARM7 private WRAM in arena id order. */
#define ARENA_WRAM_SUB_PRIVATE 8

/* Arena and heap calls, roles guessed from their arguments and the SDK
   layout; their own files still take and return addresses as int. */
void func_arm7_037fc6fc(int arena, int lo);     /* set arena low end */
int func_arm7_037fc7ec(int arena);              /* arena low end */
int func_arm7_037fc800(int arena);              /* arena high end */
void Heap_VerifyAndCountFree_Arm7(int arena, int heap);
int Heap_CarveFromRange_Arm7(int arena, int lo, int hi);              /* create heap */
int Heap_SetUpArenaTable_Arm7(int arena, int lo, int hi, int maxHeaps); /* init allocator */
void Heap_SelectDefault_Arm7(int arena, int heap);   /* set current heap */
void func_arm7_037fded4(void *dest, u32 data, u32 size);        /* fill bytes */

/*
 * Sets up the allocator in the ARM7 private WRAM arena with room for one
 * heap, clears the rest of the arena, makes one heap over it and selects
 * it. Halts if the heap cannot be made. Returns the heap handle.
 */
int InitWramHeap(void)
{
    int hi;
    int lo;
    int heap;

    hi = func_arm7_037fc800(ARENA_WRAM_SUB_PRIVATE);
    lo = Heap_SetUpArenaTable_Arm7(ARENA_WRAM_SUB_PRIVATE, func_arm7_037fc7ec(ARENA_WRAM_SUB_PRIVATE), hi, 1);
    func_arm7_037fded4((void *)lo, 0, func_arm7_037fc800(ARENA_WRAM_SUB_PRIVATE) - lo);
    func_arm7_037fc6fc(ARENA_WRAM_SUB_PRIVATE, lo);
    hi = func_arm7_037fc800(ARENA_WRAM_SUB_PRIVATE);
    heap = Heap_CarveFromRange_Arm7(ARENA_WRAM_SUB_PRIVATE, func_arm7_037fc7ec(ARENA_WRAM_SUB_PRIVATE), hi);
    if (heap < 0) {
        Cpu_HaltForever_Arm7();
    }
    Heap_SelectDefault_Arm7(ARENA_WRAM_SUB_PRIVATE, heap);
    Heap_VerifyAndCountFree_Arm7(ARENA_WRAM_SUB_PRIVATE, heap);
    return heap;
}
