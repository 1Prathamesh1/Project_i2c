#include"header.h"
#define SW1 ((IOPIN0>>14)&1)
#define SW2 ((IOPIN0>>15)&1)
u8 flag,mode=0;
u8 sec,min,hrs,dw,dd,mm,yy,y1=20,c;
u8 am_pm=0,hr_reg;

void read_rtc(void);
void lcd_display(void);

int main(){
	lcd_init();
	i2c_init();
	config_vic_eint0();
	
	while(1){
		read_rtc();
		lcd_display();
		if(flag==1){
			lcd_display();
			/////SET MODE//////
			lcd_cmd(0x01);
			lcd_str("Set Mode");
			lcd_cmd(0xc0);
			if((hrs>>6)&1)
			lcd_str("CURRENT:12 Hrs");
			else
			lcd_str("CURRENT:24 Hrs");
			while(1){
				if(SW1==0){
					while(SW1==0);
					mode=!mode;
					if(mode==0){
						lcd_cmd(0xC0);
						lcd_str("24 Hrs");
					}else{
						lcd_cmd(0x01);
						lcd_str("Set Mode");
						lcd_cmd(0xC0);
						lcd_str("12 Hrs");
					}
				}
				if(SW2==0){
					while(SW2==0);
					break;
				}
			}
				
			///////////////////////
			if(mode==0){
				mode24();
			}else{
				mode12();
			}
			date();
			/////////////////////////////
			
			///END OF INTERRUPT LOOP/////
			flag=0;
		}
		//END OF MAIN WHILE LOOP/////////
	}
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



void lcd_display(void){ 
			u8 temp,hr;
			lcd_cmd(0x80);
			lcd_cmd(0x0F);
			if(mode==0){// Position after time
				lcd_data((hrs/16)+48);
				lcd_data((hrs%16)+48);
				lcd_data(':');
			}else{
				if((hrs>>6)&1){
					temp=((hrs>>5)&1);
					hr=bcd_dec(hrs&0x1F);
				}else{
					hr=bcd_dec(hrs&0x3F);
					
					if(hr==0){
                hr=12;
                temp=0;
            }else if(hr<12){
                temp=0;
            }else if(hr==12){
                temp=1;
            }else{
                hr-=12;
                temp=1;
            }
				}
				lcd_data(hr/10+48);
        lcd_data(hr%10+48);
        lcd_data(':');

        lcd_cmd(0x8D);

        if(temp)
            lcd_str("PM");
        else
            lcd_str("AM");
			
			}
		
		lcd_cmd(0x83);			
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
}



void mode12(void){
		lcd_display();
    sec=i2c_read(0xD1,0x00);
    min=i2c_read(0xD1,0x01);
    hrs=i2c_read(0xD1,0x02);
    dw=i2c_read(0xD1,0x03);
    dd=i2c_read(0xD1,0x04);
	
	
			///convert to dec
		if((hrs>>6)&1){
			am_pm=((hrs>>5)&1);
			hrs=bcd_dec(hrs&0x1F);
		}else{
			 hrs=bcd_dec(hrs&0x3F);
			 if(hrs==0){
					hrs=12;
					am_pm=0;
			}else if(hrs<12){
					am_pm=0;
			}else if(hrs==12){
					am_pm=1;
			}else{
					hrs-=12;
					am_pm=1;
			}
		}
		
		min=bcd_dec(min);
		sec=bcd_dec(sec);
		dd=bcd_dec(dd);
		dw=bcd_dec(dw);
		
		lcd_cmd(0x80);
		lcd_data(hrs/10+48);
		lcd_data(hrs%10+48);

		lcd_cmd(0x8D);

		if(am_pm)
				lcd_str("PM");
		else
				lcd_str("AM");
		
		while(1){
    	lcd_cmd(0x80);
			lcd_cmd(0x0F);
			if(SW1==0){
				while(SW1==0);
				delay_ms(150);

				hrs++;
			    if(hrs>12){
			        hrs=1;
			        am_pm=!am_pm;
			        
			        if(am_pm==0){
			            dd++;
			            dw++;
			            
			            if(dw>7)
			                dw=1;
			        }
			    }
					
					if(hrs>=12 && am_pm==1){
							dd++;
							dw++;
							am_pm=!am_pm;
					}
							
				lcd_cmd(0x80);
				lcd_cmd(0x0F);
				lcd_data(hrs/10+48);
				lcd_data(hrs%10+48);
				lcd_cmd(0x8D);
				if(am_pm==1)
					lcd_str("PM");
				else
					lcd_str("AM");
				lcd_cmd(0xC0);
				lcd_data(dd/10+48);
				lcd_data(dd%10+48);
				lcd_cmd(0xCD);
				display_dow(dw);
							
		}		
		if(SW2==0){
			while(SW2==0);
			delay_ms(150);
			break;
		}
	}
    
	///MIN//////////
    while(1){
			    lcd_cmd(0x83);
			    lcd_cmd(0x0F);
			    if(SW1==0){
			        while(SW1==0);
			        min++;
			        if(min>59){
                        min=0;
                        hrs++;

                        if(hrs>12){
                        hrs=1;
                        am_pm=!am_pm;
                        
                        if(am_pm==0){
                            dd++;
                            dw++;
                            if(dw>7)
                            dw=1;
                        }
                    }
                }
					lcd_cmd(0x80);
					lcd_data(hrs/10+48);
            		lcd_data((hrs%10)+48);
            		lcd_data(':');
            				
            		lcd_data((min/10)+48);
            		lcd_data((min%10)+48);
			    }
			    if(SW2==0){
			        while(SW2==0);
					delay_ms(100);
			        break;
			    }
			}

    ////SEC////////////////
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
			                if(hrs>12){
			                    hrs=1;
			                    am_pm=!am_pm;
			                    
			                    if(am_pm==0){
			                        dd++;
			                        dw++;
			                        
			                        if(dw>7)
			                        dw=1;
			                    }
			                }
			            }
			        }
			        lcd_cmd(0x80);
			        lcd_data(hrs/10+48);
            	lcd_data((hrs%10)+48);
            	lcd_data(':');
            				
            	lcd_data((min/10)+48);
            	lcd_data((min%10)+48);
            	lcd_data(':');
            				
            	lcd_data((sec/10)+48);
            	lcd_data((sec%10)+48);
			        
			    }
			    if(SW2==0){
			        while(SW2==0);
					delay_ms(100);
	            	break;
			    }
			}

    hr_reg=dec_bcd(hrs);
    hr_reg|=(1<<6);

    if(am_pm)
        hr_reg|=(1<<5);
    else
        hr_reg&=~(1<<5);

    sec=dec_bcd(sec);
    min=dec_bcd(min);
    dd=dec_bcd(dd);
    dw=dec_bcd(dw);

    i2c_write(0xD0,0x00,sec);
    i2c_write(0xD0,0x01,min);
    i2c_write(0xD0,0x02,hr_reg);
    i2c_write(0xD0,0x03,dw);
    i2c_write(0xD0,0x04,dd);
}

void mode24(){
				read_rtc();
				lcd_cmd(0x01);
				lcd_display();
				dd=bcd_dec(dd);
				dw=bcd_dec(dw);
				min=bcd_dec(min);	
	
				if(hrs&(1<<6)){
						am_pm=(hrs>>5)&1;
						hrs=bcd_dec(hrs&0x1F);

						if(am_pm){
								if(hrs!=12)
										hrs+=12;
						}else{
								if(hrs==12)
										hrs=0;
						}
				}else{
						hrs=bcd_dec(hrs&0x3F);
				}
	
				while(1){
						lcd_cmd(0x80);
						lcd_cmd(0x0F);
						if(SW1==0){
							while(SW1==0);
							delay_ms(100);
							hrs++;
							if(hrs>23){
								hrs=0;
								dd++;
								dw++;
								if(dw>7)
									dw=1;
							}
							lcd_data(hrs/10+48);
							lcd_data(hrs%10+48);
							lcd_cmd(0xC0);
							lcd_data(dd/10+48);
							lcd_data(dd%10+48);
							lcd_cmd(0xCD);
							display_dow(dw);
						}
						if(SW2==0){
							while(SW2==0);
							break;
						}
					}
				
					////MIN/////
		
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
										if(dw>7)
											dw=1;
									}
			        }
								lcd_cmd(0x80);
								lcd_data(hrs/10+48);
            		lcd_data((hrs%10)+48);
            		lcd_data(':');
            				
            		lcd_data((min/10)+48);
            		lcd_data((min%10)+48);
            		lcd_data(':');
							
								lcd_cmd(0xC0);
								lcd_data(dd/10+48);
								lcd_data(dd%10+48);
								lcd_cmd(0xCD);
								display_dow(dw);
			    }
			    if(SW2==0){
			        while(SW2==0);
			        break;
			    }
			}
			
			//SEC/////////////
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
													if(dw>7)
														dw=1;
			                }
			            }
			        }
			        lcd_cmd(0x80);
			        lcd_data(hrs/10+48);
            		lcd_data((hrs%10)+48);
            		lcd_data(':');
            				
            		lcd_data((min/10)+48);
            		lcd_data((min%10)+48);
            		lcd_data(':');
            				
            		lcd_data((sec/10)+48);
            		lcd_data((sec%10)+48);
			        
			    }
					if(SW2==0){
			        while(SW2==0);
			      //  hrs=dec_bcd(hrs);
			        min=dec_bcd(min);
			        sec=dec_bcd(sec);
							dd=dec_bcd(dd);
							dw=dec_bcd(dw);
			        i2c_write(0xD0,0,sec);
			        i2c_write(0xD0,1,min);
			       // i2c_write(0xD0,2,hrs);
							i2c_write(0xD0,4,dd);
							i2c_write(0xD0,3,dw);
							hrs=dec_bcd(hrs);
							hrs&=~(1<<6);      //24-hour mode
							hrs&=~(1<<5);      //PM bit cleared

							i2c_write(0xD0,2,hrs);
						break;
			    }
			}
}



void date(){
			///DD///
			dd=bcd_dec(dd);
			mm=bcd_dec(mm);
			yy=bcd_dec(yy);
			dw=bcd_dec(dw);
			while(1){
				lcd_cmd(0xC0);
				lcd_cmd(0x0F);
				if(SW1==0){
					while(SW1==0);
					delay_ms(100);
					dd++;
					dw++;
					if((mm==1) || (mm==3) || (mm==5) || (mm==7) || (mm==8) || (mm==10) || (mm==12)){
						if(dd>31){
							dd=1;
							mm++;
						}
					}else if(mm==4 || mm==6 || mm==9 || mm==11){
						if(dd>30){
							dd=1;
							mm++;
						}
					}else if(mm==2){
							int year = 2000+yy;
							if((year%400==0) || ((year%4==0)&&(year%100!=0))){
									if(dd>29){
											dd=1;
											mm++;
									}
							}else{
									if(dd>28){
											dd=1;
											mm++;
									}
							}
					}
						if(mm>12){
							mm=1;
							yy++;
							if(yy>99){
								yy=0;
								y1++;
							}
						}
						
						if(dw>7){
							dw=1;
						}
						
						lcd_cmd(0xC0);
						lcd_data(dd/10+'0');
						lcd_data(dd%10+'0');
						lcd_data(':');

						lcd_data(mm/10+'0');
						lcd_data(mm%10+'0');
						lcd_data(':');
						lcd_data(y1/10+48);
						lcd_data(y1%10+48);
						lcd_data((yy/10)+'0');
						lcd_data((yy%10)+'0');
						lcd_cmd(0xCD);
						display_dow(dw);
				}
				if(SW2==0){
					while(SW2==0);
					delay_ms(100);
					dd=dec_bcd(dd);
					mm=dec_bcd(mm);
					yy=dec_bcd(yy);
					dw=dec_bcd(dw);
					i2c_write(0xD0,0x3,dw);
					i2c_write(0xD0,0x4,dd);
					i2c_write(0xD0,0x5,mm);
					i2c_write(0xD0,0x6,yy);
					break;
				}
			}
			
			///MONTH////
			mm=i2c_read(0xD1,0x5);
			yy=i2c_read(0xD1,0X6);
			mm=bcd_dec(mm);
			yy=bcd_dec(yy);
			
			while(1){
				lcd_cmd(0xC3);
				lcd_cmd(0x0F);
				if(SW1==0){
					while(SW1==0);
					delay_ms(100);
					mm++;
					if(mm>12){
						mm=1;
						yy++;
						if(yy>99){
							yy=0;
							y1++;
						}
					}
					lcd_cmd(0xC3);
					lcd_data(mm/10+'0');
					lcd_data(mm%10+'0');
					lcd_data(':');
					lcd_data(y1/10+48);
					lcd_data(y1%10+48);
					lcd_data((yy/10)+'0');
					lcd_data((yy%10)+'0');
				}
				if(SW2==0){
					while(SW2==0);
					delay_ms(100);
					mm=dec_bcd(mm);
					yy=dec_bcd(yy);
					i2c_write(0xD0,0x5,mm);
					i2c_write(0xD0,0x6,yy);
					break;
				}
			}
			
			///YEAR////
			yy=i2c_read(0xD1,0X6);
			yy=bcd_dec(yy);
			
			while(1){
				lcd_cmd(0xC6);
				lcd_cmd(0x0F);
				if(SW1==0){
					while(SW1==0);
					delay_ms(100);
						yy++;
						if(yy>99){
							yy=0;
							y1++;
						}
					lcd_cmd(0xC6);
					lcd_data(y1/10+48);
					lcd_data(y1%10+48);
					lcd_data((yy/10)+'0');
					lcd_data((yy%10)+'0');
				}
				if(SW2==0){
					while(SW2==0);
					delay_ms(100);
					mm=dec_bcd(mm);
					yy=dec_bcd(yy);
					i2c_write(0xD0,0x5,mm);
					i2c_write(0xD0,0x6,yy);
					break;
				}
			}
			
			///DAY OF WEEK///
			dw=i2c_read(0xD1,03);
			while(1){
				lcd_cmd(0xCD);
				lcd_cmd(0x0F);
				if(SW1==0){
					while(SW1==0);
					delay_ms(100);
					dw++;
					if(dw>7)
						dw=1;
					lcd_cmd(0xCD);
					display_dow(dw);
				}
				if(SW2==0){
					while(SW2==0);
					delay_ms(100);
					dw=dec_bcd(dw);
					i2c_write(0xD0,0x3,dw);
					break;
				}
			}
}

