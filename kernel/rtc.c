#include "rtc.h"


uint8 from_rtc_port(uint8 offset) {
	outb(RTC_ADDRESS_PORT, offset);
	uint8 value = inb(RTC_DATA_PORT);
	return value;
}

void rtc_init(){
	uint8 flag = from_rtc_port(10);
}


RTC_Time get_time(){
	RTC_Time current_rtc_time;
	uint8 UTC = (+4);
	current_rtc_time.second = from_rtc_port(0);
	current_rtc_time.minute = from_rtc_port(2);
	current_rtc_time.hour = from_rtc_port(0x4)+UTC;
	current_rtc_time.day_of_month = from_rtc_port(7);
	current_rtc_time.month = from_rtc_port(8);
	current_rtc_time.year = from_rtc_port(9);
	return current_rtc_time;

}



