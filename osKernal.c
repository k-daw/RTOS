#include "osKernal.h"

#define NUM_OF_THREADS 					3
#define STACK_SIZE 						100
#define BUS_FREQ							16000000
#define SYSPRI13 (*((volatile uint32_t *)0xE000ED20))
uint32_t MILLIS_PRESCALER;
struct tcb{ 										//Thread Control Block 
	int32_t *StackPointer;
	struct tcb * next;
};


typedef struct tcb tcbType;

tcbType tcbs[NUM_OF_THREADS];
tcbType *currentPt;
int32_t TCB_STACK[NUM_OF_THREADS][STACK_SIZE];

void osSchedulerLaunch(void);
void osKernalStackInit(int i){
		tcbs[i].StackPointer = &TCB_STACK[i][STACK_SIZE-16];  // Stack pointer points at the address of the last stored element. Thus, the last one
		TCB_STACK[i][STACK_SIZE-1] = 0x01000000;  // Specifiy the Thumb mode fore Processor Operation 
		
}

uint8_t osKernalAddThreads(void(*task0)(void),void(*task1)(void), void(*task2)(void))
{
	__disable_irq();  // we want this process to be un-interrupted
	tcbs[0].next = &tcbs[1];
	tcbs[1].next = &tcbs[2];
	tcbs[2].next = &tcbs[0];
	osKernalStackInit(0);
	TCB_STACK[0][STACK_SIZE-2] = (int32_t) (task0);  // we pointed pc to a function of task0
	osKernalStackInit(1);
	TCB_STACK[1][STACK_SIZE-2] = (int32_t) (task1);  // we pointed pc to a function of task1
	osKernalStackInit(2);
	TCB_STACK[2][STACK_SIZE-2] = (int32_t) (task2);  // we pointed pc to a function of task0
	currentPt = &tcbs[0];
	__enable_irq();
	return 1;
}


void osKernalInit(void)
{
	__disable_irq();
	MILLIS_PRESCALER = BUS_FREQ / 1000;
}

void osKernalLanuch(uint32_t quanta)
{
	SysTick->CTRL = 0;
	SysTick->VAL = 0;
	SYSPRI13 = (SYSPRI13&0x00FFFFFF) | 0xE0000000;  // Priority 7, which is the lowest of them all
	SysTick->LOAD = (quanta * MILLIS_PRESCALER);
	SysTick->CTRL = 0x00000007;
	osSchedulerLaunch();
	
}

// Scheduler 
// TCB
// Stack

