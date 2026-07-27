#include"header.h"
#define SW1 ((IOPIN0>>14)&1)
#define SW2 ((IOPIN0>>15)&1)
u8 flag;
u8 sec,min,hrs,dw,dd,mm,yy,y1=20;


int main(){
	lcd_init();
	i2c_init();
	config_vic_eint0();
	while(1){
		yy=i2c_read(0xD1,0x6);//read year
		mm=i2c_read(0xD1,0x5);//read month
		dd=i2c_read(0xD1,0x4);//read date
		dw=i2c_read(0xD1,0x3);//read day of week
		hrs=i2c_read(0xD1,0x2);//read hrs
		min=i2c_read(0xD1,0x1);//read min
		sec=i2c_read(0xD1,0x0);//read sec
		
		lcd_cmd(0x80);
			lcd_cmd(0x0F);

			lcd_data((hrs/16)+48);
			lcd_data((hrs%16)+48);
			lcd_data(':');

			lcd_data((min/16)+48);
			lcd_data((min%16)+48);
			lcd_data(':');

			lcd_data((sec/16)+48);
			lcd_data((sec%16)+48);
				
			lcd_cmd(0xc0);
			lcd_data(dd/16+48);
			lcd_data(dd%16+48);
			lcd_data(':');
					
			lcd_data(mm/16+48);
			lcd_data(mm%16+48);
			lcd_data(':');
					
			lcd_data(y1/10+48);
			lcd_data(y1%10+48);
			lcd_data(yy/16+48);
			lcd_data(yy%16+48);
					
			lcd_cmd(0xCD);
			display_dow(dw);	
			if(flag==1){
				//set hours///
				hrs=bcd_dec(hrs);
				min=bcd_dec(min);
				sec=bcd_dec(sec);
				dd=bcd_dec(dd);
				mm=bcd_dec(mm);
				dw=bcd_dec(dw);
				
				while(1){
					lcd_cmd(0x80);
					lcd_cmd(0x0F);
					if(SW1==0){
						while(SW1==0);
							hrs++;
							if(hrs>23){
								hrs=0;
								dd++;
								dw++;
							}
							lcd_cmd(0x80);
						lcd_data(hrs/10+48);
						lcd_data(hrs%10+48);
						lcd_cmd(0xC0);
						lcd_data(dd/10+48);
						lcd_data(dd%10+48);
					}
					if(SW2==0){
						while(SW2==0);
						break;
					}
				}
				
				//set min///
				while(1){
					lcd_cmd(0x83);
					lcd_cmd(0x0F);
					if(SW1==0){
						while(SW1==0);
							min++;
							if(min>59){
								min=0;
								hrs++;
								if(hrs>23){
									hrs=0;
									dd++;
									dw++;
								}
							}
						lcd_cmd(0x80);
						lcd_data(hrs/10+48);
						lcd_data(hrs%10+48);
							
						lcd_cmd(0x83);
						lcd_data(min/10+48);
						lcd_data(min%10+48);
							
						lcd_cmd(0xC0);
						lcd_data(dd/10+48);
						lcd_data(dd%10+48);
					}
					if(SW2==0){
						while(SW2==0);
						break;
					}
				}
				
				
				///set sec///
				while(1){
					lcd_cmd(0x86);
					lcd_cmd(0x0F);
					if(SW1==0){
						while(SW1==0);
							sec++;
							if(sec>59){
								sec=0;
								min++;
								if(min>59){
									min=0;
									hrs++;
									if(hrs>23){
										hrs=0;
										dd++;
										dw++;
									}
								}
							}
						lcd_data(hrs/10+48);
						lcd_data(hrs%10+48);
							
						lcd_cmd(0x83);
						lcd_data(min/10+48);
						lcd_data(min%10+48);
							
						lcd_cmd(0x86);
						lcd_data(sec/10+48);
						lcd_data(sec%10+48);
							
						lcd_cmd(0xC0);
						lcd_data(dd/10+48);
						lcd_data(dd%10+48);
					}
					if(SW2==0){
						while(SW2==0);
						break;
					}
				}
				
				///set date///
				while(1){
					lcd_cmd(0xC0);
					lcd_cmd(0x0F);
					if(SW1==0){
						while(SW1==0){
							dd++;
							lcd_cmd(0xC0);
							lcd_data(dd/10+48);
							lcd_data(dd%10+48);
						}
					}
					if(SW2==0){
						while(SW2==0);
						break;
					}
				}
				
				//set month///
				while(1){
					lcd_cmd(0xC3);
					lcd_cmd(0x0F);
					if(SW1==0){
						while(SW1==0);
							mm++;
							if(mm>12){
								yy++;
								if(yy>99){
									y1++;
									lcd_cmd(0xC6);
									lcd_data(y1/10+48);
									lcd_data(y1%10+48);
								}
							}
							lcd_cmd(0xC3);
							lcd_data(mm/10+48);
							lcd_data(mm%10+48);
							
							lcd_cmd(0xC8);
							lcd_data(yy/10+48);
							lcd_data(yy%10+48);
					}
					if(SW2==0){
						while(SW2==0);
						break;
					}
				}
				
				
			///set year////
				while(1){
					lcd_cmd(0xC8);
					lcd_cmd(0x0F);
					if(SW1==0){
						while(SW1==0){
							yy++;
							if(yy>99){
								y1++;
									lcd_cmd(0xC6);
									lcd_data(y1/10+48);
									lcd_data(y1%10+48);
							}
							lcd_cmd(0xC8);
							lcd_data(yy/10+48);
							lcd_data(yy%10+48);
						}
					}
					if(SW2==0){
						while(SW2==0);
						break;
					}
				}
				
			}/////////END OF INTERRUPT/////////
	}
}