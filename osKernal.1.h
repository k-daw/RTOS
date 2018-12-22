#include<stdint.h>
#include "TM4C123GH6PM.h"                   // Device header

#ifndef _OS_KERNEL_H
#define _OS_KERNEL_H

void osKernalLanuch(uint32_t quanta);

void osKernalInit(void);

uint8_t osKernalAddThreads(void(*task0)(void),
													void(*task1)(void),
													void(*task2)(void));

#endif
			
													