#include <LPC21xx.H>
#include "header.h"
#include <string.h>

#define THRE ((U0LSR >> 5) & 1)
#define RDR ((U0LSR) & 1)

int m[4] = {15, 60, 30, 0};

void delay_sec(unsigned int sec)
{
    T0PC = 0;
    T0PR = m[VPBDIV] * 1000000 - 1;
    T0TC = 0;
    T0TCR = 1;
    while (T0TC < sec)
        ;
    T0TCR = 0;
}

void delay_ms(unsigned int ms)
{
    T0PC = 0;
    T0PR = m[VPBDIV] * 1000 - 1;
    T0TC = 0;
    T0TCR = 1;
    while (T0TC < ms);
    T0TCR = 0;
}

void lcd_8bitinit(void)
{
    IODIR0 = 0X7FF;     // SETTING 0.0 TO 0.10 IN OUT DIR
    IOCLR0 = (1 << 10); // CLR THE EN PIN
    lcd_cmd_8(0X38);    // double line enable in 8 bit mode
    lcd_cmd_8(0X0E);    // enable cursor
    lcd_cmd_8(0x0F);
    lcd_cmd_8(0X01); // clr the screen and set cursor to home
}

void lcd_cmd_8(unsigned char cmd)
{
    IOCLR0 = 0X7FF;
    IOSET0 = cmd;       // writing cmd on pins
    IOCLR0 = (1 << 8);  // RS=0 FOR CMS MODE
    IOCLR0 = (1 << 9);  // LCD IN WRITE MODE
    IOSET0 = (1 << 10); // EN PIN SET
    delay_ms(2);        // LCD PROCESSING
    IOCLR0 = (1 << 10); // EN PIN CLR
}

void lcd_data_8bits(unsigned char data)
{
    IOCLR0 = 0X7FF;
    IOSET0 = data;      // writing cmd on pins
    IOSET0 = (1 << 8);  // RS=1 FOR CMS MODE
    IOCLR0 = (1 << 9);  // LCD IN WRITE MODE
    IOSET0 = (1 << 10); // EN PIN SET
    delay_ms(2);        // LCD PROCESSING
    IOCLR0 = (1 << 10); // EN PIN CLR
}

void lcd_stringprint(char *p)
{

    for (; *p; p++)
    {
        lcd_data_8bits(*p);
    }
}

int strleng(char *p)
{
    int i;
    for (i = 0; p[i]; i++);
    return i;
}

void lcd_stringcircle(char *p)
{
    int i, m;
    while (1)
    {
        if (i == 16)
            i = 0;
        for (i = 0; i < 16; i++)
        {

            if (i + strleng(p) < 16)
            {

                lcd_cmd_8(0X80 + i);
                lcd_stringprint(p);
                delay_ms(300);
                IOCLR0 = 0X7FF;

                lcd_cmd_8(0X01);
            }
            else if (i + strleng(p) > 16)
            {

                lcd_cmd_8(0X80 + i);
                lcd_stringprint(p);
                m = (i + strleng(p) - 16);
                lcd_cmd_8(0x80);
                lcd_stringprint(p + strleng(p) - m);
                delay_ms(300);
                IOCLR0 = 0X7FF;
                lcd_cmd_8(0X01);
            }
        }
    }
}

void lcd_2digitprint(int num)
{
    lcd_cmd_8(0XC6);
    lcd_data_8bits((num / 10) + 48);
    lcd_data_8bits((num % 10) + 48);
}

void lcd_digitprint_2ND(int num)
{
    lcd_cmd_8(0XC0);
    lcd_data_8bits((num / 10) + 48);
    lcd_data_8bits((num % 10) + 48);
}

void lcd_4bitinit(void)
{
    IODIR1 = 0XFE0000;
    PINSEL2 = 0;
    IOCLR1 = (1 << 19); // EN IS 1.19 RS IS 1.17 RW IS 1.18
    lcd_4bit_cmd(0X02);
    lcd_4bit_cmd(0X03);
    lcd_4bit_cmd(0X28);
    lcd_4bit_cmd(0X0E);
    lcd_4bit_cmd(0X01);
}

void lcd_4bit_cmd(unsigned char cmd)
{
    IOCLR1 = 0XFE0000;
    IOSET1 = ((cmd & 0XF0) << 16);
    IOCLR1 = (1 << 17); // RS=0 FOR CMD MODE
    IOCLR1 = (1 << 18); // LCD IN WRITE MODE
    IOSET1 = (1 << 19); // EN PIN SET
    delay_ms(2);        // LCD PROCESSING
    IOCLR1 = (1 << 19); // EN PIN CLR

    IOCLR1 = 0XFE0000;
    IOSET1 = ((cmd & 0X0F) << 20);
    IOCLR1 = (1 << 17); // RS=0 FOR CMD MODE
    IOCLR1 = (1 << 18); // LCD IN WRITE MODE
    IOSET1 = (1 << 19); // EN PIN SET
    delay_ms(2);        // LCD PROCESSING
    IOCLR1 = (1 << 19); // EN PIN CLR
}

void lcd_4bit_data(unsigned char data)
{

    IOCLR1 = 0XFE0000;
    IOSET1 = ((data & 0XF0) << 16);
    IOSET1 = (1 << 17); // RS=0 FOR CMD MODE
    IOCLR1 = (1 << 18); // LCD IN WRITE MODE
    IOSET1 = (1 << 19); // EN PIN SET
    delay_ms(2);        // LCD PROCESSING
    IOCLR1 = (1 << 19); // EN PIN CLR

    IOCLR1 = 0XFE0000;
    IOSET1 = ((data & 0X0F) << 20);
    IOSET1 = (1 << 17); // RS=0 FOR CMD MODE
    IOCLR1 = (1 << 18); // LCD IN WRITE MODE
    IOSET1 = (1 << 19); // EN PIN SET
    delay_ms(2);        // LCD PROCESSING
    IOCLR1 = (1 << 19); // EN PIN CLR
}

void lcd_stringprint_4bitmode(char *p)
{

    for (; *p; p++)
    {
        lcd_4bit_data(*p);
    }
}

void lcd_digitprint_2ND_4bit(int num)
{

    lcd_4bit_data((num / 0x10) + 48);
    lcd_4bit_data((num % 0x10) + 48);
}

void lcd_digitprint(int num)
{
    lcd_4bit_data((num / 10) + 48);
    lcd_4bit_data((num % 10) + 48);
}
void lcd_printday(int day)
{
    switch (day)
    {

    case 1:
        lcd_stringprint_4bitmode("SUN");
        break;
    case 2:
        lcd_stringprint_4bitmode("MON");
        break;
    case 3:
        lcd_stringprint_4bitmode("TUE");
        break;
    case 4:
        lcd_stringprint_4bitmode("WED");
        break;
    case 5:
        lcd_stringprint_4bitmode("THU");
        break;
    case 6:
        lcd_stringprint_4bitmode("FRI");
        break;
    case 7:
        lcd_stringprint_4bitmode("SAT");
        break;
    }
}

void lcd_stringcircle4(char *p)
{
    int i, m;
    while (1)
    {
        if (i == 16)
            i = 0;
        for (i = 0; i < 16; i++)
        {

            if (i + strleng4(p) < 16)
            {

                lcd_4bit_cmd(0X80 + i);
                lcd_stringprint4(p);
                delay_ms(300);
                IOCLR0 = 0XFE0000;

                lcd_4bit_cmd(0X01);
            }
            else if (i + strleng4(p) > 16)
            {

                lcd_4bit_cmd(0X80 + i);
                lcd_stringprint4(p);
                m = (i + strleng4(p) - 16);
                lcd_4bit_cmd(0x80);
                lcd_stringprint4(p + strleng4(p) - m);
                delay_ms(300);
                IOCLR0 = 0XFE0000;
                lcd_4bit_cmd(0X01);
            }
        }
    }
}

int lcd_stringprint4(char *p)
{

    for (; *p; p++)
    {
        lcd_4bit_data(*p);
    }
    return 0;
}

int strleng4(char *p)
{
    int i;
    for (i = 0; p[i]; i++);
    return i;
}

void floatprint(int num)
{

    int r, num1 = num;
    num = 0;

    while (num1 > 0)
    {
        r = num1 % 10;
        num = num * 10 + r;
        num1 = num1 / 10;
    }

    while (num > 0)
    {
        r = num % 10;
        r = r + 48;
        lcd_data_8bits(r);
        num = num / 10;
    }
}

void uart0_init(unsigned int baud)
{
    int m[4] = {15, 60, 30, 0};
    int num;
    num = (m[VPBDIV] * 1000000) / (16 * baud);
    PINSEL0 |= 0X05; // 0.0 TO TXD 0.1 TO RXD
    U0LCR = 0X83;    // DLAB=1 ENABLING BAUD MODIFICATION

    U0DLL = (num & 0xff);
    U0DLM = ((num >> 8) & (0xff));
    U0LCR = 0X03; // DISABLING BAUD MODIFICATION
}

void uart0_tx(unsigned char data)
{
    U0THR = data;
    while (THRE == 0);
}
char uart0_rx(void)
{
    char temp;
    while (RDR == 0);
    temp = U0RBR;
    return temp;
}

void uart_tx_string(char *ptr)
{
    while (*ptr)
    {
        uart0_tx(*ptr);
        ptr++;
    }
}

char *uart0_rx_string(char *temp, unsigned char maxbytes)
{
    char i = 0;
    while (i < maxbytes)
    {
        while (RDR == 0);
        temp[i] = U0RBR;
        if (U0RBR == '\r')
            break;
        i++;
    }
    temp[i] = '\0';
}

int mystrcmp(char *arr, char *temp)
{
    int i = 0;
    for (i = 0; arr[i]; i++)
    {
        if (arr[i] != temp[i])
            return (arr[i] - temp[i]);
    }
    return 0;
}

void passwordauthenticator(void)
{
    int i = 0;
    char temp[10];
    char *arr = "abcd";
    while (1)
    {
        uart_tx_string("enter password:");
        for (i = 0; i < 4; i++)
        {
            temp[i] = uart0_rx();
            if (temp[i] == '\b')
            {
                if (i != 0)
                {
                    temp[i - 1] = 0;
                    uart0_tx('\b');
                    uart0_tx(' ');
                    uart0_tx('\b');
                    i = i - 2;
                }

                else
                {
                    temp[0] = 0;
                    i = -1;
                }
            }

            else
                uart0_tx('*');
        }

        temp[i] = '\0';
        uart_tx_string("\r\n");
        uart_tx_string("result:");
        if (mystrcmp(arr, temp) == 0)
            uart_tx_string("correct\r\n");
        else
            uart_tx_string("wrong\r\n");
    }
}

void basic_calci(void)
{
L0:
    while (1)
    {
        signed int res = 0;
        int arr[20], b[10], i, j, c = 0, r, sum = 0, flag = 100, flag1 = 100, s, k = 0, temp;
        uart_tx_string("enter exp:");

        for (i = 0; i < 20; i++) // initialise arrays to 0
            arr[i] = 0;
        for (i = 0; i < 10; i++)
            b[i] = 0;

        for (i = 0;;) // i/p loop start
        {
            temp = uart0_rx();
            uart0_tx(temp);
            if (temp == '\r') // enter button breaks the input loop
                break;

            else if ((temp < '0') || (temp > '9')) // for spl characters
            {
                if (temp != '\b')
                {
                    i++;
                    b[k] = temp;
                    k++;
                    flag1 = 0; // whenever arithmetic op is entered
                }
            }

            else if ((temp <= '9') && (temp >= '0')) // for normal numbers
            {
                arr[i] = (arr[i] * 10) + (temp - 48);
                flag1 = 1; // when numeric input is entered
            }

            if (temp == '\b') // if backspace is entered
            {
                if (i <= 0) // when enter exp line is about to delete
                {
                    for (i = 0; i < 10; i++)
                        uart0_tx('\b');
                    goto L0; // begin again
                }

                if (flag1 == 0) // means if the last entered input is a operator
                {
                    k--;
                    b[k] = 0;
                    i--;
                    flag1 = 1; // next time delete a number if operator is already deleted
                }

                else if (flag1 == 1) // means if the last entered input is a number
                {

                    arr[i] = arr[i] / 10;
                    if (arr[i] == 0)
                    {
                        flag1 = 0; // next time delete a operator if a number  is already deleted
                    }
                }
            }
        } //  taking input is finally completed here
          ///////////////////////////////////////////////////////////////////////////////
        /*for(i=0;i<20;i++)
        uart0_tx(arr[i]+48);
        uart_tx_string("\r\n"); // these lines are to check whether input taken is properly written into arrays or not
        for(k=0;k<10;k++)
        uart0_tx(b[k]);
        uart_tx_string("\r\n");*/
        /////////////////////////////////////////////////////////////////////////////////
        for (i = 0; i < 10;)
        {
            flag = 100; // any random number bcoz when flag is 0 or 1 theres some task to do
            for (s = 0; s < 10; s++)
            {

                if ((b[s] == '*') || (b[s] == '/') || (b[s] == '%')) // first check / or % or *
                {
                    i = s;
                    goto L1; // goto L1 function if present
                }
            }

            for (s = 0; s < 10; s++)
            {
                if ((b[s] == '+') || (b[s] == '-')) // check where is + or -
                {
                    i = s;
                    goto L2; // goto L2 fun if present
                }
            }

        L1:           // for / * %
            flag = 0; // if L1 is executed we have to skip L2 thats y
            if (b[i] == '*')
            {
                j = i;
                res = arr[j] * arr[j + 1];
                arr[j] = res;

                for (j = i; j < 20; j++)
                {
                    arr[j + 1] = arr[j + 2]; // to arrange numbers array
                }
            }
            else if (b[i] == '/')
            {
                j = i;
                res = arr[j] / arr[j + 1];
                arr[j] = res;
                for (j = i; j < 20; j++)
                {
                    arr[j + 1] = arr[j + 2];
                }
            }
            else if (b[i] == '%')
            {
                j = i;
                res = arr[j] % arr[j + 1];
                arr[j] = res;
                for (j = i; j < 20; j++)
                {
                    arr[j + 1] = arr[j + 2];
                }
            }
            for (j = i; j < 10; j++) // this loop is to arrange operators array
                b[j] = b[j + 1];
        L2:
            if (flag == 0) // if at all L1 is executed then directly jump to L3
                goto L3;
            if (b[i] == '+')
            {
                j = i;
                res = arr[j] + arr[j + 1];
                arr[j] = res;
                for (j = i; j < 20; j++)
                {
                    arr[j + 1] = arr[j + 2];
                }
            }
            else if (b[i] == '-')
            {
                j = i;
                res = arr[j] - arr[j + 1];
                arr[j] = res;
                for (j = i; j < 20; j++)
                {
                    arr[j + 1] = arr[j + 2];
                }
            }

            for (j = i; j < 10; j++)
                b[j] = b[j + 1];

        L3:
            if (b[0] == 0) // if operator array gets empty break the loop
                break;

            i = 0; // every time check from the starting
        }
        uart_tx_string("************************");
        uart_tx_string("\r\n");
        uart_tx_string("\r\nresult = ");
        if (res < 0)
        {
            uart0_tx('-'); // if res is -ve transmit -ve sign and also multiply with -1
            arr[0] = (-1) * arr[0];
        }

        res = arr[0];
        if (res == 0)
        {
            uart0_tx(48);
            goto L4;
        }
        for (; res; res = res / 10) // reversing our result so that we can extract digits one by one
        {
            r = res % 10;
            sum = sum * 10 + r;
            c++;
        }
        for (; c > 0; sum = sum / 10) // printing our result digit by digit
        {
            c--;
            uart0_tx((sum % 10) + 48);
        }
    L4:
        uart_tx_string("\r\n\r\n************************");
        uart_tx_string("\r\n");
    }
}

#define DONE ((ADDR >> 31) & 1)
void adc_init(void)
{
    PINSEL1 |= 0x15400000;
    ADCR = 0X00200400;
}

unsigned int adc_read(unsigned char ch_num)
{
    ADCR |= (1 << ch_num);
    ADCR |= (1 << 24);
    while (DONE == 0);
    ADCR ^= (1 << 24);
    ADCR ^= (1 << ch_num);
    return ((ADDR >> 6) & 0X3FF);
}
#include <stdio.h>

void adc_float(unsigned int f)
{
    char buf[20];
    float res = 0;
    res = (f * 3.3) / 1023;
    sprintf(buf, "%f", res);
    uart_tx_string(buf);
}
void adc_int(int f)
{
    char buf[10];
    sprintf(buf, "%d", f);
    uart_tx_string(buf);
}
void adc_char(char f)
{
    char buf[10];
    sprintf(buf, "%d", f);
    uart_tx_string(buf);
}
void adc_test(void)
{
    float res = 0;
    unsigned int temp = 0;
    while (1)
    {
        res = 0;
        temp = 0;
        uart0_init(9600);
        adc_init();
        temp = adc_read(2);
        adc_int(temp);
        uart_tx_string("    ");

        adc_float(temp);
        uart_tx_string("v\r\n");

        delay_ms(1);
    }
}
void adc_assignment1(void)
{
    uart0_init(9600);

    while (1)
    {
        unsigned int temp = 0;
        unsigned char ch_num = 0;

        adc_init();
        uart_tx_string("CH NUM=");
        temp = adc_read(2);
        ch_num = ((ADDR >> 24) & 7);
        uart0_tx(ch_num + 48);
        uart_tx_string("\r\n");
        uart_tx_string("DEC=");
        adc_int(temp);
        uart_tx_string(" V=");

        char buf[10];
        float vout = 0;
        vout = ((temp * 3.3) / 1023);

        sprintf(buf, "%f", vout);
        uart_tx_string(buf);
        uart_tx_string("v\r\n");
        delay_ms(100);
    }
}

float res;

void adc_float_temparature(unsigned int temp)
{
    char buf[10];
    float vout = 0, temparature = 0;
    vout = ((temp * 3.3) / 1023);
    temparature = ((vout - 0.5) / 0.01);
    res = temp;
    sprintf(buf, "%f", temparature);
    uart_tx_string(buf);
    uart_tx_string("degree C");
}

extern char arr[100];
extern int f;

#define SPIF ((S0SPSR >> 7) & 1)
/*spi0_driver*/
void spi0_init(void)
{
    PINSEL0 = 0X1500;
    IODIR0 |= (1 << 7); // p0.7 o/p dir cs pin
    IOSET0 |= (1 << 7); // disselecting slave
    S0SPCR = 0x20;      // mastermode,MSB first,CPOL=0,CPHA=0
    S0SPCCR = 150;      // SPIO speed = 100kbps
}

char spi0_read(char data)
{
    S0SPDR = data;
    while (SPIF == 0);
    return S0SPDR;
}

int mcp3204_read(int ch_num)
{
    int res = 0, channel = (ch_num << 6);
    char byteh = 0, bytel = 0;
    IOCLR0 |= (1 << 7);
    spi0_read(0x06);
    byteh = spi0_read(channel);
    bytel = spi0_read(0);
    IOSET0 |= (1 << 7);
    byteh &= 0x0f;
    res = byteh << 8;
    res |= bytel;
    return res;
}

char mcp25aa010_read(char address)
{
    char res;
    IOCLR0 |= (1 << 7);

    spi0_read(0x03);
    spi0_read(address);
    res = spi0_read(0);
    IOSET0 |= (1 << 7);
    return res;
}

void mcp25aa010_write(char address, char data)
{
    IOCLR0 |= (1 << 7);
    spi0_read(0x06);

    spi0_read(0x02);
    spi0_read(address);
    spi0_read(data);

    spi0_read(0x04);

    IOSET0 |= (1 << 7);
}

void i2c_init(void)
{
    PINSEL0 |= 0X50;
    I2SCLH = I2SCLL = 75; // 100 KBPS
    I2CONSET = (1 << 6);  // I2EN=1 ,MASTER MODE
}

#define SI ((I2CONSET >> 3) & 1)
void i2c_byte_write_frame(unsigned char sa, unsigned char mr, unsigned char data)
{
    I2CONSET = (1 << 5); // START COND
    I2CONCLR = (1 << 3); // SI=0
    while (SI == 0);
    I2CONCLR = (1 << 5); // STA=0;
    I2DAT = sa;
    I2CONCLR = (1 << 3); // SI=0;
    while (SI == 0);
    I2DAT = mr;
    I2CONCLR = (1 << 3);
    while (SI == 0);
    I2DAT = data;
    I2CONCLR = (1 << 3);
    while (SI == 0);
    I2CONSET = (1 << 4); // STOP COND
    I2CONCLR = (1 << 3); // SI=0
}

unsigned char i2c_byte_read_frame(unsigned char sa, unsigned char mr)
{
    unsigned char temp = 0;
    I2CONSET = (1 << 5); // start
    I2CONCLR = (1 << 3); // SI=0
    while (SI == 0);
    I2DAT = sa;
    I2CONCLR = (1 << 3);
    while (SI == 0);
    I2DAT = mr;
    I2CONCLR = (1 << 3);
    while (SI == 0);
    I2CONSET = (1 << 5); // RESTART
    I2CONCLR = (1 << 3); // SI=0
    while (SI == 0);
    I2CONCLR = (1 << 5); // STA=0
    I2DAT = sa | 1; // SA+R
    I2CONCLR = (1 << 3);
    while (SI == 0);
    I2CONCLR = (1 << 3); // OPERATION READ DATA START
    while (SI == 0);
    temp = I2DAT;
    I2CONSET = (1 << 4); // stop
    I2CONCLR = (1 << 3); // SI=0
    return temp;
}
