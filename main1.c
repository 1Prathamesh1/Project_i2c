#include"header.h"
#define SW1 ((IOPIN0>>14)&1)
#define SW2 ((IOPIN0>>15)&1)
u8 sec,min,hrs,dw,dd,mm,yy,y1=20,c;
u8 flag;
u8 hr_reg,am_pm,mode;

void read_rtc(void);
void write_rtc(void);
void lcd_display(void);
void convert_dec(void);
void convert_bcd(void);
void day_calc(void);
void display_mode(void);
void select_ampm(void);

int main(){
	lcd_init();
	config_vic_eint0();
	i2c_init();
	
	while(1){
		read_rtc();
		convert_dec();
		lcd_display();
		
		if(flag==1){
			lcd_cmd(0x80);
			lcd_cmd(0x0F);
			while(1){
				if(SW1==0){
					while(SW1==0);
					delay_ms(50);
					hrs++;
					c=hrs;
					if(mode==1){
						if(hrs>12){
							if(hrs==12){
								hrs=1;
								am_pm=!am_pm;
							}else{
								hrs=1;
							}
							am_pm=!am_pm;
						}
					}else{
							if(hrs>23)
							hrs=0;
					}
					lcd_display();
				}//esw1
				if(SW2==0){
					while(SW2==0);
					delay_ms(50);
					break;
				}//esw2
			}//ehrswhile
			
			lcd_cmd(0x83);
			lcd_cmd(0x0F);
			while(1){
				if(SW1==0){
					while(SW1==0);
					delay_ms(50);
					min++;
					lcd_display();
				}//endsw1
				if(SW2==0){
					while(SW2==0);
					delay_ms(50);
					break;
				}//endsw2
			}//endminwhile
			
			
			lcd_cmd(0x86);
			lcd_cmd(0x0F);
			while(1){
				if(SW1==0){
					while(SW1==0);
					delay_ms(50);
					sec++;
					if(sec>59)
						sec=0;
					lcd_display();
				}//endsw1
				if(SW2==0){
					while(SW2==0);
					delay_ms(50);
					break;
				}//endsw2
			}//endsecwhile
			
			lcd_cmd(0xC0);
			lcd_cmd(0x0F);
			while(1){
				if(SW1==0){
					while(SW1==0);
					delay_ms(50);
					day_calc();
					lcd_display();
				}//endsw1
				if(SW2==0){
					while(SW2==0);
					delay_ms(50);
					break;
				}
			}//endddwhile
			
			lcd_cmd(0xC3);
			lcd_cmd(0x0F);
			while(1){
				if(SW1==0){
					while(SW1==0);
					delay_ms(50);
					mm++;
					if(mm>12)
						mm=1;
					lcd_display();
				}//endsw1
				if(SW2==0){
					while(SW2==0);
					delay_ms(50);
					break;
				}//endsw2
			}//endmmwhile	
			lcd_cmd(0xC8);
			lcd_cmd(0x0F);
			while(1){
				if(SW1==0){
					while(SW1==0);
					delay_ms(50);
					yy++;
					if(yy>99){
						yy=0;
						y1++;
					}
					lcd_display();
				}//endsw1
				if(SW2==0){
					while(SW2==0);
					delay_ms(50);
					break;
				}
			}//endyywhile
			lcd_cmd(0xCD);
			lcd_cmd(0x0F);
			while(1){
				if(SW1==0){
					while(SW1==0);
					delay_ms(50);
					dw++;
					if(dw>7)
						dw=1;
					lcd_cmd(0xCD);
					display_dow(dw);
				}//endsw1
				if(SW2==0){
					while(SW2==0);
					delay_ms(50);
					break;
				}//endsw2
			}//enddwwhile
			lcd_cmd(0x01);
			lcd_str("Select mode");
			display_mode();
			while(1){
				if(SW1==0){
					while(SW1==0);
					delay_ms(50);
					mode=!mode;
					display_mode();
				}//endsw1
				if(SW2==0){
					while(SW2==0);
					delay_ms(50);
					lcd_cmd(0x01);
					break;
				}
			}//endmodewhile
			if(mode==1)
				select_ampm();
			else{
				if(am_pm==1){
					if(hrs!=12)
						hrs=hrs+12;
				}else{
						if(hrs==12)
						hrs=0;
				}
			}
			flag=0;
			convert_bcd();
			write_rtc();
		}//eintloop
	}//emainwhile
}

void read_rtc(void){
	yy=i2c_read(0xD1,0x6);//read year
	mm=i2c_read(0xD1,0x5);//read month
	dd=i2c_read(0xD1,0x4);//read date
	dw=i2c_read(0xD1,0x3);//read day of week
	hrs=i2c_read(0xD1,0x2);//read hrs
	min=i2c_read(0xD1,0x1);//read min
	sec=i2c_read(0xD1,0x0);//read sec
}

void convert_dec(){
	sec=bcd_dec(sec);
	min=bcd_dec(min);
	hr_reg = hrs;          

  if(hr_reg&(1<<6)){
		mode=1;
    am_pm=(hr_reg>>5)&1;
    hrs=bcd_dec(hr_reg & 0x1F);
  }else{
		mode=0;
    hrs=bcd_dec(hr_reg & 0x3F);
  }
	dw=bcd_dec(dw);
	dd=bcd_dec(dd);
	mm=bcd_dec(mm);
	yy=bcd_dec(yy);
}

void lcd_display(){
	//lcd_cmd(0x01);
	lcd_cmd(0x80);
	
	lcd_data(hrs/10+48);
	lcd_data(hrs%10+48);
	lcd_data(':');
	
	lcd_data(min/10+48);
	lcd_data(min%10+48);
	lcd_data(':');
	
	lcd_data(sec/10+48);
	lcd_data(sec%10+48);
	
	if(mode==1){
		lcd_cmd(0x8D);
		if(am_pm)
			lcd_str("PM");
		else
			lcd_str("AM");
	}
	
	lcd_cmd(0xC0);
	lcd_data(dd/10+48);
	lcd_data(dd%10+48);
	lcd_data(':');
	
	lcd_data(mm/10+48);
	lcd_data(mm%10+48);
	lcd_data(':');
	
	lcd_data(y1/10+48);
	lcd_data(y1%10+48);
	lcd_data(yy/10+48);
	lcd_data(yy%10+48);
	
	lcd_cmd(0xCD);
	display_dow(dw);
	
	
}

void day_calc(void){
	dd++;
	if((mm==1) || (mm==3) || (mm==5) || (mm==7) || (mm==8) || (mm==10) || (mm==12)){
			if(dd>31){
				dd=1;
			}
	}else if(mm==4 || mm==6 || mm==9 || mm==11){
			if(dd>30){
				dd=1;
			}
	}else if(mm==2){
			int year = 2000+yy;
			if((year%400==0) || ((year%4==0)&&(year%100!=0))){
				if(dd>29){
					dd=1;
				}
			}else{
					if(dd>28){
						dd=1;
					}
			}
	}
}

void convert_bcd(void){
	sec=dec_bcd(sec);
	min=dec_bcd(min);
	if(mode==0){
		hrs=dec_bcd(hrs);

    hrs&=~(1<<6);      // 24-hour mode
    hrs&=~(1<<5);      // clear AM/PM bit  
	}else{
		hrs=dec_bcd(hrs);
    hrs |= (1<<6);       // 12-hour mode
    if(am_pm)
      hrs|=(1<<5);
    else
      hrs&=~(1<<5);
	}
	dw=dec_bcd(dw);
	dd=dec_bcd(dd);
	mm=dec_bcd(mm);
	yy=dec_bcd(yy);
}

void write_rtc(void){
	i2c_write(0xD0,0,sec);
	i2c_write(0xD0,1,min);
	i2c_write(0xD0,2,hrs);
	i2c_write(0xD0,3,dw);
	i2c_write(0xD0,4,dd);
	i2c_write(0xD0,5,mm);
	i2c_write(0xD0,6,yy);
}
void display_mode(void){
		if(mode==0){
			lcd_cmd(0xC0);
			lcd_str("Current:24 hrs");
		}else{
			lcd_cmd(0xC0);
			lcd_str("Current:12 hrs");
		}
}
void select_ampm(void){
    if(hrs==0){
      hrs=12;
      am_pm=0;
    }else if(hrs<12){
      am_pm=0;
    }else if(hrs==12){
      am_pm=1;
    }else{
			hrs=hrs-12;
      am_pm=1;
    }
		
    lcd_cmd(0x01);
    lcd_str("Select AM/PM");

    while(1){
        lcd_cmd(0xC0);
        if(am_pm==0)
            lcd_str("AM ");
        else
            lcd_str("PM ");

        if(SW1==0){
					while(SW1==0);
          delay_ms(50);
          am_pm=!am_pm;
        }

        if(SW2==0){
					while(SW2==0);
          delay_ms(50);
          lcd_cmd(0x01);
          break;
        }
    }
}
