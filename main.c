#include "TM4C123GH6PM.h"
#include "osKernal.h"


#define QUANTA 10
uint32_t counter1, counter2, counter3;

void Thread0(void)
{
	while(1){counter1++; }
}

void Thread1(void)
{
	while(1){counter2++;}
}

void Thread2(void)
{
	while(1){counter3++;}
}
int main(void)
{
	osKernalInit();
	osKernalAddThreads(&Thread0, &Thread1, &Thread2);
	osKernalLanuch(QUANTA);
	
}
