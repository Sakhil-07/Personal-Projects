#include <LPC21xx.H>
#include "header.h"

// SW1 and SW2 are for control and SW3 is to apply
#define sw1 ((IOPIN0 >> 14) & 1) 
#define sw2 ((IOPIN0 >> 15) & 1)
#define sw3 ((IOPIN0 >> 16) & 1)

int main()
{
    unsigned char h = 0, m = 0, s = 0, f = 0, c = 0, today, date = 0, day = 1, year = 0, month = 0, count = 0, ampm;
    i2c_init();
    lcd_4bitinit();
    uart0_init(9600);

    while (1)
    {
        lcd_4bit_cmd(0x80);
        lcd_stringprint_4bitmode("AM:PRESS SW1 \r\n");
        lcd_4bit_cmd(0xC0);
        lcd_stringprint_4bitmode("PM:PRESS SW2 ");
        while (1)
        {
            if (sw1 == 0)
            {
                while (sw1 == 0);
                ampm = 0; // am selected
                break;
            }
            else if (sw2 == 0)
            {
                while (sw2 == 0);
                ampm = 1; // pm selected
                break;
            }
        }
        lcd_4bit_cmd(0x01);
        lcd_stringprint_4bitmode("ENTER HRS:\r\n");
        while (1)
        {
            lcd_4bit_cmd(0xc0);
            if (ampm == 0)
            {
                if (sw2 == 0)
                {
                    while (sw2 == 0);
                    c = (c > 0) ? c - 1 : 11;
                    lcd_digitprint(c);
                }
                else if (sw1 == 0)
                {
                    while (sw1 == 0);
                    c = (c < 11) ? c + 1 : 0;
                    lcd_digitprint(c);
                }
                else if (sw3 == 0)
                {
                    while (sw3 == 0);
                    h = (((c / 10) << 4) | (c % 10)) | (ampm << 5) | (1 << 6);
                    c = 0;
                    i2c_byte_write_frame(0xd0, 0x02, h);
                    break;
                }
            }
            else if (ampm == 1)
            {
                if (f == 0)
                {
                    c = 1;
                    f = 1;
                }

                if (sw2 == 0)
                {
                    while (sw2 == 0);
                    c = (c > 1) ? c - 1 : 12;
                    lcd_digitprint(c);
                }
                else if (sw1 == 0)
                {
                    while (sw1 == 0);
                    c = (c < 12) ? c + 1 : 1;
                    lcd_digitprint(c);
                }
                else if (sw3 == 0)
                {
                    while (sw3 == 0);
                    h = (((c / 10) << 4) | (c % 10)) | (ampm << 5) | (1 << 6);
                    c = 0;
                    i2c_byte_write_frame(0xd0, 0x02, h);
                    break;
                }
            }
        }
        while (sw3 == 1);
        if (sw3 == 0 && count == 0)
        {
            count = 1;
            while (sw3 == 0);
            lcd_4bit_cmd(0x01);
            lcd_stringprint_4bitmode("ENTER MINS:\r\n");

            while (1)
            {
                lcd_4bit_cmd(0xc0);
                if (sw2 == 0)
                {
                    while (sw2 == 0);
                    c = (c > 0) ? c - 1 : 59;
                    lcd_digitprint(c);
                }
                else if (sw1 == 0)
                {
                    while (sw1 == 0);
                    c = (c < 59) ? c + 1 : 0;
                    lcd_digitprint(c);
                }
                else if (sw3 == 0)
                {
                    while (sw3 == 0);
                    m = (((c / 10) << 4) | (c % 10));
                    c = 0;
                    i2c_byte_write_frame(0xd0, 0x1, m);
                    break;
                }
            }
        }
        while (sw3 == 1);
        if (sw3 == 0 && count == 1)
        {
            count = 2;
            while (sw3 == 0);
            lcd_4bit_cmd(0x01);
            lcd_stringprint_4bitmode("ENTER SECS:\r\n");

            while (1)
            {
                lcd_4bit_cmd(0xc0);
                if (sw2 == 0)
                {
                    while (sw2 == 0);
                    c = (c > 0) ? c - 1 : 59;
                    lcd_digitprint(c);
                }
                else if (sw1 == 0)
                {
                    while (sw1 == 0);
                    c = (c < 59) ? c + 1 : 0;
                    lcd_digitprint(c);
                }
                else if (sw3 == 0)
                {
                    while (sw3 == 0);
                    s = (((c / 10) << 4) | (c % 10));
                    c = 0;
                    i2c_byte_write_frame(0xd0, 0x0, s);
                    break;
                }
            }
        }
        while (sw3 == 1);
        if (sw3 == 0 && count == 2)
        {
            count = 3;
            while (sw3 == 0);
            lcd_4bit_cmd(0x01);
            lcd_stringprint_4bitmode("ENTER DATE:\r\n");

            while (1)
            {
                lcd_4bit_cmd(0xc0);
                if (sw2 == 0)
                {
                    while (sw2 == 0);
                    c = (c > 1) ? c - 1 : 31;
                    lcd_digitprint(c);
                }
                else if (sw1 == 0)
                {
                    while (sw1 == 0);
                    c = (c < 31) ? c + 1 : 1;
                    lcd_digitprint(c);
                }
                else if (sw3 == 0)
                {
                    while (sw3 == 0);
                    date = (((c / 10) << 4) | (c % 10));
                    c = 0;
                    i2c_byte_write_frame(0xd0, 0x04, date);
                    break;
                }
            }
        }
        while (sw3 == 1);
        if (sw3 == 0 && count == 3)
        {
            count = 4;
            while (sw3 == 0);
            lcd_4bit_cmd(0x01);
            lcd_stringprint_4bitmode("ENTER MONTH:\r\n");
            while (1)
            {
                lcd_4bit_cmd(0xc0);
                if (sw2 == 0)
                {
                    while (sw2 == 0);
                    c = (c > 1 ) ? c - 1 : 12;
                    lcd_digitprint(c);
                }
                else if (sw1 == 0)
                {
                    while (sw1 == 0);
                    c = (c < 12) ? c + 1 : 0;
                    lcd_digitprint(c);
                }
                else if (sw3 == 0)
                {
                    while (sw3 == 0);
                    month = (((c / 10) << 4) | (c % 10));
                    c = 0;
                    i2c_byte_write_frame(0xd0, 0x05, month);
                    break;
                }
            }
        }
        while (sw3 == 1);
        if (sw3 == 0 && count == 4)
        {
            count = 5;
            while (sw3 == 0);
            lcd_4bit_cmd(0x01);
            lcd_stringprint_4bitmode("ENTER YEAR:\r\n");
            while (1)
            {
                lcd_4bit_cmd(0xc0);
                if (sw2 == 0)
                {
                    while (sw2 == 0);
                    c--;
                    lcd_digitprint(c);
                }
                else if (sw1 == 0)
                {
                    while (sw1 == 0);
                    c++;
                    lcd_digitprint(c);
                }
                else if (sw3 == 0)
                {
                    while (sw3 == 0);
                    year = (((c / 10) << 4) | (c % 10));
                    c = 0;
                    i2c_byte_write_frame(0xd0, 0x06, year);
                    break;
                }
            }
        }

        while (sw3 == 1);
        if (sw3 == 0 && count == 5)
        {
            count = 6;
            while (sw3 == 0);
            lcd_4bit_cmd(0x01);
            lcd_stringprint_4bitmode("ENTER DAY(1=SUN):\r\n");

            while (1)
            {
                lcd_4bit_cmd(0xc0);
                if (sw2 == 0)
                {
                    while (sw2 == 0);
                    c = (c > 1) ? c - 1 : 7;
                    lcd_digitprint(c);
                }
                else if (sw1 == 0)
                {
                    while (sw1 == 0);
                    c = (c < 7) ? c + 1 : 1;
                    lcd_digitprint(c);
                }
                else if (sw3 == 0)
                {
                    while (sw3 == 0);
                    day = (((c / 10) << 4) | (c % 10));
                    c = 0;
                    i2c_byte_write_frame(0xd0, 0x03, day);
                    break;
                }
            }
        }
        if (count == 6) break;
    }
    while (1)
    {
        s = i2c_byte_read_frame(0xd0, 0x0);
        m = i2c_byte_read_frame(0xd0, 0x01);
        h = i2c_byte_read_frame(0xd0, 0x02);
        day = i2c_byte_read_frame(0xd0, 0x03);
        if (((h >> 6) & 1) == 1)
        {
            if (((h >> 5) & 1) == 1)    ampm = 1;
            else                        ampm = 0;
            h = h & 0x1f;
        }
        
        date    = i2c_byte_read_frame(0xd0, 0x04);
        month   = i2c_byte_read_frame(0xd0, 0x05);
        year    = i2c_byte_read_frame(0xd0, 0x06);

        lcd_4bit_cmd(0x01);
        lcd_digitprint_2ND_4bit(h);
        lcd_4bit_data(':');
        lcd_digitprint_2ND_4bit(m);
        lcd_4bit_data(':');
        lcd_digitprint_2ND_4bit(s);
        if (ampm == 0)      lcd_stringprint_4bitmode(" AM");
        else if (ampm == 1) lcd_stringprint_4bitmode(" PM");
        lcd_4bit_cmd(0xC0);
        lcd_digitprint_2ND_4bit(date);
        lcd_4bit_data('/');
        lcd_digitprint_2ND_4bit(month);
        lcd_4bit_data('/');
        lcd_digitprint_2ND_4bit(year);
        lcd_4bit_data(' ');
        lcd_printday(day);
        delay_ms(1000);
    }
}

