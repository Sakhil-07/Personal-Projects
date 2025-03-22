#include <LPC21xx.H>

extern void delay_sec(unsigned int sec);
extern void delay_ms(unsigned int ms);
extern void switchpress(void);
extern void lcd_cmd_8(unsigned char cmd);
extern void lcd_data_8bits(unsigned char data);
extern void lcd_8bitinit(void);
extern void lcd_digitprint_2ND(int num);
extern void lcd_2digitprint(int num);
extern void lcd_stringcircle(char *p);
extern void lcd_stringprint(char *p);
extern void lcd_4bitinit(void);
extern void lcd_4bit_cmd(unsigned char cmd);
extern void lcd_4bit_data(unsigned char data);
extern void lcd_stringprint_4bitmode(char *p);
extern void lcd_digitprint_2ND_4bit(int num);
extern void lcd_stringcircle4(char *p);
extern int strleng4(char *p);
extern int lcd_stringprint4(char *p);
extern void uart_tx_string(char *ptr);
extern char uart0_rx(void);
extern void uart0_tx(unsigned char data);
extern void uart0_init(unsigned int baud);
extern void passwordauthenticator(void);
extern void basic_calci(void);
extern 	void adc_int(int f);
extern 	void adc_char(char f);
extern void spi0_init(void);
extern void lcd_digitprint(int num);
extern char spi0_read(char data);
extern int mcp3204_read(int ch_num);
extern char mcp25aa010_read(char address);
extern void mcp25aa010_write(char address,char data);
extern void adc_init(void);
extern 	void adc_float(unsigned int f);
extern 	unsigned int adc_read(unsigned char ch_num);
extern void adc_float_temparature(unsigned int temp);
extern void adc_assignment1(void);
extern void i2c_init(void);
extern void i2c_byte_write_frame(unsigned char sa,unsigned char mr,unsigned char data);
extern unsigned char i2c_byte_read_frame(unsigned char sa,unsigned char mr);

typedef struct can_msg
{
	unsigned int id;
	unsigned char rtr;
	unsigned char dlc;
	unsigned int byteA;
    	unsigned int byteB;
}can1;
extern void can1_rx(can1*);
extern void can1_tx(can1 );
extern void can1_init(void);
