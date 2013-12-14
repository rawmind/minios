#include "rtc.h"

uint8 daysInMonth [12] = {31,28,31,30,31,31, 31,30 ,31 ,30, 31};

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
	current_rtc_time.second = from_rtc_port(0);
	current_rtc_time.minute = from_rtc_port(2);
	current_rtc_time.hour = from_rtc_port(0x4);
	current_rtc_time.day_of_month = from_rtc_port(7);
	current_rtc_time.month = from_rtc_port(8);
	current_rtc_time.year = from_rtc_port(9);
	return current_rtc_time;
}


RTC_Time get_utc_time(uint8 UTC){
// todo: 
	RTC_Time current_rtc_time = get_time();
	uint8 mod_hours = (current_rtc_time.hour+UTC)%0x24;
	if( mod_hours != 0 ){
		current_rtc_time.hour=mod_hours;
		current_rtc_time.day_of_month++;
		uint8 maxDayInMonth =  daysInMonth[current_rtc_time.month];

		if(isBissextile(current_rtc_time.year) && (current_rtc_time.month == 2)){
			maxDayInMonth++;
		}
		uint8 modDays = maxDayInMonth % current_rtc_time.day_of_month;
		if(modDays != 0){
			current_rtc_time.day_of_month=modDays;
		}

	}
	return current_rtc_time;
}

bool isBissextile(uint8 year){
	return (year % 4) == 0;
}





