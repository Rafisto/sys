#ifndef DEV_RTC_H
#define DEV_RTC_H

#include "../../kernel/types.h"
#include "../../kernel/x86.h"

typedef struct rtc_time
{
    uint8_t seconds; // 00-59, 0x00
    uint8_t minutes; // 00-59, 0x02
    uint8_t hours;   // 00-23, 0x04
    uint8_t weekday; // 01-07, 0x06
    uint8_t day;     // 01-31, 0x07
    uint8_t month;   // 01-12, 0x08
    uint8_t year;    // 00-99, 0x09
} rtc_time;

#define RTC_CMOS_PORT 0x70 // RTC CMOS PORT - send the requests
#define RTC_CMOS_DATA 0x71 // RTC CMOS DATA - read the data

static uint8_t bcd_to_bin(uint8_t bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

void rtc_get_time(rtc_time* time)
{
    outb(RTC_CMOS_PORT, 0x00);
    time->seconds = bcd_to_bin(inb(RTC_CMOS_DATA));
    outb(RTC_CMOS_PORT, 0x02);
    time->minutes = bcd_to_bin(inb(RTC_CMOS_DATA));
    outb(RTC_CMOS_PORT, 0x04);
    time->hours = bcd_to_bin(inb(RTC_CMOS_DATA));
    outb(RTC_CMOS_PORT, 0x06);
    time->weekday = bcd_to_bin(inb(RTC_CMOS_DATA));
    outb(RTC_CMOS_PORT, 0x07);
    time->day = bcd_to_bin(inb(RTC_CMOS_DATA));
    outb(RTC_CMOS_PORT, 0x08);
    time->month = bcd_to_bin(inb(RTC_CMOS_DATA));
    outb(RTC_CMOS_PORT, 0x09);
    time->year = bcd_to_bin(inb(RTC_CMOS_DATA));
}


#endif // DEV_RTC_H