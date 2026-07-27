#include"header.h"
extern u8 flag;

void EINT0_HANDLER(void)__irq{
	flag=1;
	EXTINT=1;
	VICVectAddr=0;
}

void config_vic_eint0(void){
	PINSEL1|=1;
	EXTMODE=1;
	EXTPOLAR=0;
	
	VICIntSelect=0;
	VICVectCntl0=14|(1<<5);
	VICVectAddr0=(u32)EINT0_HANDLER;
	VICIntEnable=1<<14;
}
