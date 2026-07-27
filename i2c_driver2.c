#include"header.h"
#define SI ((I2CONSET>>3)&1)
#define STA (1<<5)
#define STO (1<<4)

void i2c_init(){
	PINSEL0|=0X50;
	I2CONSET=(1<<6);
	I2CSCLL=I2CSCLH=75;
}

void i2c_write(u8 sa,u8 sa,u8 data){
	I2CONSET=STA;
	I2CONCLR=1<<3;
	while(SI==0);
	I2CONCLR=STO;
	
	I2DAT=sa;
	I2CONSET=1<<3;
	while(SI==0);
	if(I2STAT==0X30){
		uart0_str("ERR:SA");
		goto exit;
	}
	I2CONCLR=
}