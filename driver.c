#include <LPC21xx.H>
#include"header.h"
#define THRE ((U0LSR>>5)&1)
#define RDR (U0LSR&1)
void delay_ms(unsigned int ms){
	T0PR=15000-1;
	T0TC=0;
	T0PC=0;
	T0TCR=1;
	while(T0TC<ms);
	T0TCR=0;
}

void delay_sec(unsigned int sec){
	T0PR=15000000-1;
	T0TC=0;
	T0PC=0;
	T0TCR=1;
	while(T0TC<sec);
	T0TCR=0;
}


void lcd_data(unsigned char data){
	IOCLR1=0XFE<<16;
	
	IOSET1=(data & 0XF0)<<16;
	IOSET1=1<<17;
	IOCLR1=1<<18;
	IOSET1=1<<19;
	delay_ms(2);
	IOCLR1=1<<19;
	
	IOCLR1=0XFE<<16;
	
	IOSET1=(data & 0X0F)<<20;
	IOSET1=1<<17;
	IOCLR1=1<<18;
	IOSET1=1<<19;
	delay_ms(2);
	IOCLR1=1<<19;
}

void lcd_cmd(unsigned char cmd){
	IOCLR1=0XFE<<16;
	
	IOSET1=(cmd & 0XF0)<<16;
	IOCLR1=1<<17;
	IOCLR1=1<<18;
	IOSET1=1<<19;
	delay_ms(2);
	IOCLR1=1<<19;
	
	IOCLR1=0XFE<<16;
	
	IOSET1=(cmd & 0X0F)<<20;
	IOCLR1=1<<17;
	IOCLR1=1<<18;
	IOSET1=1<<19;
	delay_ms(2);
	IOCLR1=1<<19;
}

void lcd_init(void){
	IODIR1=0XFE<<16;
	PINSEL2=0;
	lcd_cmd(0x02);
	lcd_cmd(0x28);
	lcd_cmd(0x0E);
	lcd_cmd(0x01);
	
}

void lcd_str(char *ptr){
	while(*ptr){
		lcd_data(*ptr);
		ptr++;
	}
}

void lcd_int(int n){
	int a[10],i=0;
	if(n==0)
		lcd_data('0');
	
	if(n<0){
		n=-n;
		lcd_data('-');
	}else{
		while(n>0){
			a[i++]=n%10;
			n/=10;
		}
	}
	
	for(int i=i-1;i>=0;i--){
		lcd_data(a[i]);
	}	
}


void lcd_char_int(char ch){
	lcd_data(ch/10+48);
	lcd_data(ch%10+48);
}

void uart0_init(unsigned int baud){
	unsigned int result=0,pclk;
	
	if(VPBDIV==0){
		pclk=15000000;
	}else if(VPBDIV==1){
		pclk=60000000;
	}else if(VPBDIV==2){
		pclk=30000000;
	}
	
	result=pclk/(16*baud);
	
	PINSEL0|=0X05;
	U0LCR=0X83;
	U0DLL=(result & 0XFF);
	U0DLM=((result>>8)&0XFF);
	U0LCR=0X03;
	
}

void uart0_tx(unsigned char data){
	U0THR=data;
	while(THRE==0);
}

unsigned char uart0_rx(void){
	while(RDR==0);
	return U0RBR;
}

void uart0_str(char *ptr){
	while(*ptr){
		uart0_tx(*ptr);
		ptr++;
	}
}

void uart0_int(int num){
	if(num==0){
		uart0_str("0.00");
		return;
	}
	
	if(num<0){
		uart0_tx('-');
		num=-num;
	}
	int rev=0;
	while(num){
		rev=rev*10+num%10;
		num/=10;
	}
	
	while(rev){
		uart0_tx(rev%10+48);
		rev/=10;
	}
}


void uart0_float(float f){
	int num;
	if(f==0){
		uart0_str("0.00");
		return;
	}
	
	if(f<0){
		uart0_tx('-');
		f=-f;
	}
	
	num=f;
	uart0_int(num);
	num=(f-num)*100000;
	uart0_int(num);
}

u8 bcd_dec(u8 bcd){
	u8 res=0;
	res=((bcd>>4)*10)+(bcd&0xF);
	return res;
}

u8 dec_bcd(u8 dec){
	u8 res=0;
	res=((dec/10)<<4) | (dec%10);
	return res;
}

void display_dow(u8 ch){
	switch(ch){
			case 1:lcd_str("SUN");
						break;
			case 2:lcd_str("MON");
						break;
			case 3:lcd_str("TUE");
						break;
			case 4:lcd_str("WED");
						break;
			case 5:lcd_str("THU");
						break;
			case 6:lcd_str("FRI");
						break;
			case 7:lcd_str("SAT");
						break;
		}
}
