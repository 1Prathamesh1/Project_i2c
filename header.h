#include<lpc21xx.h>
typedef unsigned int u32;
typedef unsigned char u8;

//////////LCD FUNCTIONS////////////
extern void delay_ms(unsigned int);
extern void delay_sec(unsigned int);
extern void lcd_data(unsigned char);
extern void lcd_cmd(unsigned char);
extern void lcd_init(void);
extern void lcd_str(char *);
extern void lcd_int(int);
extern void lcd_char_int(char);


//////////UART0 FUNCTIONS////////////
extern void uart0_init(unsigned int);
extern void uart0_tx(unsigned char);
extern unsigned char uart0_rx(void);
extern void uart0_str(char *);
extern void uart0_str_rx(char *);
extern void uart0_int(int);
extern void uart0_float(float);


////////EXTERNAL INTERRUPT/////////
extern void config_vic_eint0(void);
extern void config_vic_eint1(void);
extern void config_vic_eint2(void);
extern void config_vic_eint3(void);


//////////UART0 INTERRUPT////////////
extern void config_vic_uart0(void);



////////ADC///////////////////////
extern void adc_init(void);
extern u32 adc_read(u8);


///////////SPI/////////////
extern void spi0_init(void);
extern u8 spi0(u8);
extern u32 mcp3204_read(u8);


/////////I2C//////////////
extern void i2c_init(void);
extern void i2c_write(u8,u8,u8);
extern u8 i2c_read(u8,u8);
extern u8 bcd_dec(u8);
extern u8 dec_bcd(u8);
extern void display_dow(u8);
extern void mode12(void);
extern void mode24(void);
extern void date(void);

