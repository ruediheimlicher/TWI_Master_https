
#define TRUE 1
#define FALSE 0
	//Clear CH bit of RTC
#define CH 7
#define DELAY (2)

/*
struct time 
{
  uint8_t sekunde;
  uint8_t minute;
  uint8_t stunde;
  uint8_t kalendertag;
  uint8_t wochentag;
  uint8_t kalendermonat;
  uint8_t kalenderjahr;
};
*/

uint8_t bcd2dec(uint8_t bcd);
uint8_t dec2bcd(uint8_t dec);
void rtc_init(void);
uint8_t rtc_start(void);
uint8_t rtc_stop(void);
uint8_t rtc_writeByte(uint8_t data);
uint8_t rtc_readByte(uint8_t *data,uint8_t ack);	

uint8_t DS1307Read(uint8_t address,uint8_t *data);
uint8_t read_Zeit(uint8_t *sekunden, uint8_t *minuten, uint8_t *stunden);


uint8_t DS1307Write(uint8_t address,uint8_t data);
uint8_t rtc_write_Control(uint8_t frequenz);
uint8_t rtc_write_Stunde(uint8_t stunde);
uint8_t rtc_write_Minute(uint8_t minute);
uint8_t rtc_write_Sekunde(uint8_t sekunde);
uint8_t rtc_write_Zeit(uint8_t stunde, uint8_t minute, uint8_t sekunde);
uint8_t rtc_write_Datum(uint8_t wochentag, uint8_t tagdesmonats, uint8_t monat, uint8_t jahr);


void rtc_get_time(unsigned char *hour,unsigned char *min,unsigned char *sec);
void rtc_set_time(unsigned char hour,unsigned char min,unsigned char sec);
void rtc_get_date(unsigned char *date,unsigned char *month,unsigned char *year);
void rtc_set_date(unsigned char date,unsigned char month,unsigned char year)
;