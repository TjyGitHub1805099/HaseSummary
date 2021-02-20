#ifndef _TRAVELTIME_H_
#define _TRAVELTIME_H_



/**the time part of hour**/
#define TIME_MAX_HOUR	100

/**the time part of minue**/
#define TIME_MAX_MINUM	60

/**the time part of second**/
#define TIME_MAX_SECOND	60


#define TIMECNT_MAX	360000

typedef struct
{
	unsigned int 	Timecnt;
	unsigned char	Time_h;
	unsigned char	Time_m;
	unsigned char	Time_s;
	unsigned char	TimeValid;

}TRAVEL_TIMEInfor;


void TravelTime_Init(void);
void ResetTime(void);

void TravelTime_VMAIN(void);



#endif