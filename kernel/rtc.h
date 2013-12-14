#ifndef RTC_H_
#define RTC_H_

#include "stdlib.h"

#define RTC_ADDRESS_PORT  0x70
#define RTC_DATA_PORT	0x71

typedef struct{
	uint8 second;
	uint8 minute;
	uint8 hour;
	uint8 day_of_month;
	uint8 month;
	uint8 year;
} RTC_Time;


RTC_Time get_time();
uint8 from_rtc_port(uint8 offset);
RTC_Time get_utc_time(uint8 UTC);
bool isBissextile(uint8 year);


#endif /* RTC_H_ */
