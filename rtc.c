#include <avr/io.h>
#include <util/delay.h>

#include "rtc.h"

#define WHILEMAX  0xFFFF

#define RTCWRITEERR			0xB0
#define RTCSTARTERR			0xB1
#define RTCADDRESSERR		0xB2
#define RTCWRITESTATUSERR	0xB3
#define RTCSTOPERR			0xB4

#define RTCREADSTATUSERR	0xB6
#define RTCREADERR         0xD0

uint8_t bcd2dec(uint8_t bcd)
{
  return (((((bcd)>>4) & 0x0F) * 10) + ((bcd) & 0x0F));
}

uint8_t dec2bcd(uint8_t dec)
{
  return ((((dec)/10)<<4)|((dec)%10));
}

void rtc_init()
{
	//Clear CH bit of RTC
	#define CH 7

	uint8_t temp;
	DS1307Read(0x00,&temp);

	//Clear CH Bit
	temp&=(~(1<<CH));

	DS1307Write(0x00,temp);
	
	//Set 12 Hour Mode
	DS1307Read(0x02,&temp);
	
	//CLEAR 12Hour BIT // BIT 6
	temp|=(0b00000000);

	//Write Back to DS1307
	DS1307Write(0x02,temp);
	
	
	

}

/*
 while(!(TWCR & (1<<TWINT)) && whilecounter)
 {
 whilecounter--;
 }
 
 if (whilecounter==0) // kein Erfolg
 {
 TWI_Flag=STARTERR;
 //OSZIAHI;
 return STARTERR;
 }

 */

uint8_t rtc_start()
{
   uint16_t whilecounter = WHILEMAX; // 5 ms
	//Put Start Condition on Bus
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTA);

	//Poll Till Done
//	while(!(TWCR & (1<<TWINT)));
   while(!(TWCR & (1<<TWINT)) && whilecounter)
   {
      whilecounter--;
   }
   
   if (whilecounter==0) // kein Erfolg
   {
      TWI_Flag=RTCSTARTERR;
      //OSZIAHI;
      return RTCSTARTERR;
   }
   return 0;
}

uint8_t rtc_stop()
{
   uint16_t whilecounter = WHILEMAX; // 5 ms
	//Put Stop Condition on bus
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	
	//Wait for STOP to finish
	//while(TWCR & (1<<TWSTO));
   while(!(TWCR & (1<<TWINT)) && whilecounter)
   {
      whilecounter--;
   }
   
   if (whilecounter==0) // kein Erfolg
   {
      TWI_Flag=RTCSTOPERR;
      //OSZIAHI;
      return RTCSTOPERR;
   }

   return 0;
}

uint8_t rtc_writeByte(uint8_t data)
{
	uint16_t whilecounter = WHILEMAX; // 5 ms
	TWDR=data;	

	//Initiate Transfer
	TWCR=(1<<TWEN)|(1<<TWINT);

	//Poll Till Done
	//while(!(TWCR & (1<<TWINT)));

   while(!(TWCR & (1<<TWINT)) && whilecounter)
   {
      whilecounter--;
   }
   
   if (whilecounter==0) // kein Erfolg
   {
      TWI_Flag=RTCWRITEERR;
      //OSZIAHI;
      
      
      return RTCWRITEERR;
   }

   
	//Check Status
	if((TWSR & 0xF8) == 0x18 || (TWSR & 0xF8) == 0x28 || (TWSR & 0xF8) == 0x40)
	{
		//SLA+W Transmitted and ACK received
		//or
		//SLA+R Transmitted and ACK received
		//or
		//DATA Transmitted and ACK recived

		return 0;
	}
	else
		return RTCWRITESTATUSERR;	//Error
}

uint8_t rtc_readByte(uint8_t *data,uint8_t ack)
{
   uint16_t whilecounter = WHILEMAX; // 5 ms
	//Set up ACK
	if(ack)
	{
		//return ACK after reception
		TWCR|=(1<<TWEA);
	}
	else
	{
		//return NACK after reception
		//Signals slave to stop giving more data
		//usually used for last byte read.
		TWCR &= (~(1<<TWEA));
	}

	//Now enable Reception of data by clearing TWINT
	TWCR|=(1<<TWINT);

	//Wait till done
	//while(!(TWCR & (1<<TWINT)));
   while(!(TWCR & (1<<TWINT)) && whilecounter)
   {
      whilecounter--;
   }
   
   if (whilecounter==0) // kein Erfolg
   {
      TWI_Flag=RTCREADERR;
      //OSZIAHI;
      return RTCREADERR;
   }

	
   
   //Check status
	if((TWSR & 0xF8) == 0x58 || (TWSR & 0xF8) == 0x50)
	{
		//Data received and ACK returned
		//	or
		//Data received and NACK returned

		//Read the data

		*data=TWDR;
		return 0;
	}
	else
		return RTCREADSTATUSERR;	//Error
	
}


/***************************************************

Function To Read Internal Registers of DS1307
---------------------------------------------

address : Address of the register
data: value of register is copied to this.


Returns:
>0= Failure
0= Success
***************************************************/

uint8_t DS1307Read(uint8_t address,uint8_t *data)
{
	uint8_t res;	//result
	//rtc_stop();
	//Start
	rtc_start();

	//SLA+W (for dummy write to set register pointer)
	res=rtc_writeByte(0xD0);	//DS1307 address + W
	
	//Error
	if(res)	return res+1	;
	
		
	//Now send the address of required register
	res=rtc_writeByte(address);
	
	//Error
	if(res)	return res+2;
	
	//Repeat Start
	rtc_start();
	
	//SLA + R
	res=rtc_writeByte(0xD1);	//DS1307 Address + R
	
	//Error
	if(res)	return res+3;
	
	//Now read the value with NACK
	res=rtc_readByte(data,0);
	
	//Error
	if(res)	return res+4;
	
	//STOP
	rtc_stop();
	
	return 0;
}

/***************************************************

Function To Read Internal Registers of DS1307
---------------------------------------------

address : Address of the register
data: value of register is copied to this.


Returns:
>0= Failure
0= Success
***************************************************/

uint8_t Read_Datum(uint8_t* wochentag, uint8_t* tagdesmonats, uint8_t* monat, uint8_t* jahr)
{	
	//uint8_t address;
	uint8_t data;
	uint8_t res=0;	//result
	//rtc_stop();
	//Start
	rtc_start();

	//SLA+W (for dummy write to set register pointer)
	res=rtc_writeByte(0xD0);	//DS1307 address + W
	
	//Error
	if(res)	return res	;
	
		
	//Now send the address of required register
	res=rtc_writeByte(0x03);
	
	//Error
	if(res)	return res+1;
	
	//Repeat Start
	rtc_start();
	
	//SLA + R
	res=rtc_writeByte(0xD1);	//DS1307 Address + R
	
	//Error
	if(res)	return res+2;
	
	
	// Wochentag lesen mit ACK
	data=0;
	res=rtc_readByte(&data,1);
	*wochentag = (data & 0x07); // Bit 0-2
	//Error
	if(res)	return res+3;

	// Tag des Monats lesen mit ACK
	data=0;
	res=rtc_readByte(&data,1);
	*tagdesmonats=((data & 0xF0)>>4)*10 + (data & 0x0F); //Bit 0-5
	//Error
	if(res)	return res+4;

	// Monat lesen mit ACK
	data=0;
	res=rtc_readByte(&data,1);
	*monat=((data & 0x10)>>4)*10 + (data & 0x0F); // Bit 0-4
	//Error
	if(res)	return res+5;

	// Jahr lesen mit NACK
	data=0;
	res=rtc_readByte(&data,0);
	*jahr = ((data & 0xF0)>>4)*10 + (data & 0x0F);
	//Error
	if(res)	return res+6;
	
	//STOP
	rtc_stop();
	
	return 0;
}

/***************************************************

Function To Write Internal Registers of DS1307
---------------------------------------------

address : Address of the register
data: value to write.


Returns:
>0= Failure
0= Success
***************************************************/

uint8_t DS1307Write(uint8_t address,uint8_t data)
{
	uint8_t res;	//result
	
	//Start
	rtc_start();
	
	//SLA+W
	res=rtc_writeByte(0xD0);	//DS1307 address + W
	
	//Error
	if(res)	return res;
	
	//Now send the address of required register
	res=rtc_writeByte(address);
	
	//Error
	if(res)	return res+1;
	
	//Now write the value
	res=rtc_writeByte(data);
	
	//Error
	if(res)	return res+2;
	
	//STOP
	rtc_stop();
	
	return 0;
}

uint8_t read_Zeit(uint8_t *sekunden, uint8_t *minuten, uint8_t *stunden)
{
	uint8_t res=0;	//result
	uint8_t data;
	//Start
	rtc_start();
	
	//SLA+R
	res=rtc_writeByte(0xD0);	//DS1307 address + W
	
	//Error
	if(res)	return res;
	
	//Now send the address of required register
	res=rtc_writeByte(0x00);
	
	//Error
	if(res)	return res+1;
	
	//Repeat Start
	rtc_start();
	
	//SLA + R
	res=rtc_writeByte(0xD1);	//DS1307 Address + R
	
	//Error
	if(res)	return res+2;
	
	
	// Sekunde lesen mit ACK
	data=0;
	res=rtc_readByte(&data,1);
	*sekunden = (((uint8_t)data & 0x70)>>4)*10 + (data & 0x0F);
	//Error
	if(res)	return res+3;

	// Minute lesen mit ACK
	data=0;
	res=rtc_readByte(&data,1);
	*minuten=((data & 0xF0)>>4)*10 + (data & 0x0F);
	//Error
	if(res)	return res+4;

	// Stunde lesen mit NACK
	data=0;
//	res=rtc_readByte(stunden,1);
	res=rtc_readByte(&data,0);
//	uint8_t h=((data & 0xF0)>>4)*10 + (data & 0x0F);
	*stunden = ((data & 0xF0)>>4)*10 + (data & 0x0F);
//	*stunden = h;
//	stunden=(((uint8_t)data & 0xF0)>>4)*10 + (data & 0x0F);
	//Error
	if(res)	return res+5;

	//STOP
	rtc_stop();

	return 0;
}

uint8_t rtc_write_Control(uint8_t frequenz)
{
	uint8_t res;	//result
	
	//Start
	rtc_start();
	
	//SLA+W
	res=rtc_writeByte(0xD0);	//DS1307 address + W
	
	//Error
	if(res)	return 1;
	
	//Now send the address of required register
	res=rtc_writeByte(0x07);
	
	//Error
	if(res)	return 2;
	
	//Now write the value
	uint8_t wert=0;
	switch (frequenz)
	{
		case 1:
		{
			wert= 0; // 1 Hz
		}break;
		case 4:
		{
			wert=1;
		}break;
		case 8:
		{
			wert=2;
		}break;
		case 32:
		{
			wert=3;
		}break;
			
			
	}
	wert |= 0x10; // Bits 4: SQWE enable
	
	res=rtc_writeByte(wert);

	//res=rtc_writeByte(stunde);
	
	//Error
	if(res)	return 3;
	
	//STOP
	rtc_stop();
	
	return 0;
}

uint8_t rtc_setOszillator(uint8_t status)
{
	uint8_t res;	//result
	
	//Start
	rtc_start();
	
	//SLA+W
	res=rtc_writeByte(0xD0);	//DS1307 address + W
	
	//Error
	if(res)	return 1;
	
	//Now send the address of required register
	res=rtc_writeByte(0x07);
	
	//Error
	if(res)	return 2;
	
	//Now write the value
	//uint8_t wert=0;

	//STOP
	rtc_stop();
	
	return 0;


}


uint8_t rtc_write_Stunde(uint8_t stunde)
{
	uint8_t res;	//result
	
	//Start
	rtc_start();
	
	//SLA+W
	res=rtc_writeByte(0xD0);	//DS1307 address + W
	delay_ms(DELAY);
	//Error
	if(res)	return 1;
	
	//Now send the address of required register
	res=rtc_writeByte(0x02);
	delay_ms(DELAY);
	//Error
	if(res)	return 2;
	
	//Now write the value
	uint8_t wert=0;
	wert=(((stunde/10)& 0x3F) << 4) + (stunde%10); // Bits 0-5: Stunden Bit 6:0->24h, 1->12h
	
	res=rtc_writeByte(wert);
	delay_ms(DELAY);
	//res=rtc_writeByte(stunde);
	
	//Error
	if(res)	return 3;
	
	//STOP
	rtc_stop();
	
	return 0;
}

uint8_t rtc_write_Minute(uint8_t minute)
{
	uint8_t res;	//result
	
	//Start
	rtc_start();
	
	//SLA+W
	res=rtc_writeByte(0xD0);	//DS1307 address + W
	delay_ms(DELAY);
	//Error
	if(res)	return 1;
	
	//Now send the address of required register
	res=rtc_writeByte(0x01);
	delay_ms(DELAY);
	//Error
	if(res)	return 2;
	
	//Now write the value
	uint8_t wert=0;
	wert=((minute/10) << 4) + (minute%10);
	
	res=rtc_writeByte(wert);
	delay_ms(DELAY);
	//res=rtc_writeByte(minute);
	
	//Error
	if(res)	return 3;
	
	//STOP
	rtc_stop();
	delay_ms(DELAY);
	return 0;
}

uint8_t rtc_write_Sekunde(uint8_t sekunde)
{
	uint8_t res;	//result
	
	//Start
	rtc_start();
	
	//SLA+W
	res=rtc_writeByte(0xD0);	//DS1307 address + W
	delay_ms(DELAY);
	//Error
	if(res)	return 1;
	
	//Now send the address of required register
	res=rtc_writeByte(0x00);
	delay_ms(DELAY);
	//Error
	if(res)	return 2;
	
	//Oszillator abschalten
//	res=rtc_writeByte(0x80);
	
	//Now write the value
	uint8_t wert=0;
	wert=(((sekunde/10)& 0x7F) << 4) + (sekunde%10); // Bits 0-5: Stunden Bit 6:0->24h, 1->12h
	res=rtc_writeByte(wert);
	delay_ms(DELAY);
	
	//Error
	if(res)	return 3;
	
	//STOP
	rtc_stop();
	
	return 0;
}


uint8_t rtc_write_Zeit(uint8_t stunde, uint8_t minute, uint8_t sekunde)
{
	
	uint8_t res;	//result
	uint8_t wert;
	//Start
	rtc_start();
	
	//SLA+W
	res=rtc_writeByte(0xD0);	//DS1307 address + W
	
	//Error
	if(res)	return 1;
	
	//Adresse des Registers schicken
	res=rtc_writeByte(0x00);// sekunde
	//Error
	if(res)	return 2;

	//Value schreiben
	wert=0;
	wert=(((sekunde/10)& 0x7F) << 4) + (sekunde%10);
	
	res=rtc_writeByte(wert);
	//Error
	if(res)	return 3;
	
	//Value schreiben
	wert=0;
	wert=((minute/10) << 4) + (minute%10);
	res=rtc_writeByte(wert);
	//Error
	if(res)	return 4;
	
	
	//Value schreiben
	wert=0;
	wert=(((stunde/10)& 0x3F) << 4) + (stunde%10); // Bits 0-5: Stunden Bit 6:0->24h, 1->12h
	res=rtc_writeByte(wert);

	//Error
	if(res)	return 5;
	
	//STOP
	rtc_stop();
	
	return 0;
}

uint8_t rtc_write_Datum(uint8_t wochentag, uint8_t tagdesmonats, uint8_t monat, uint8_t jahr)
{
	
	uint8_t res;	//result
	uint8_t wert;
	//Start
	rtc_start();
	
	//SLA+W
	res=rtc_writeByte(0xD0);	//DS1307 address + W
	
	//Error
	if(res)	return 1;
	
	//Adresse des Registers schicken
	res=rtc_writeByte(0x03);// wochentag
	//Error
	if(res)	return 2;

	// Wochentag
	wert=0;
	res=rtc_writeByte(wochentag & 0x07);
	//Error
	if(res)	return 3;
	delay_ms(1);
	//Tag des Monats
	wert=0;
	wert=(((tagdesmonats/10)& 0x3F) << 4) + (tagdesmonats%10); // Bits 0-5
	res=rtc_writeByte(wert);
	//Error
	if(res)	return 4;
		
	//Monat
	wert=0;
	wert=(((monat/10)& 0x1F) << 4) + (monat%10); // Bits 0-4
	res=rtc_writeByte(wert);

	//Error
	if(res)	return 5;

	//Jahr
	wert=0;
	wert=(((jahr/10)) << 4) + (jahr%10); // Bits 0-4
	res=rtc_writeByte(wert);

	//Error
	if(res)	return 6;
	
	//STOP
	rtc_stop();
	
	return 0;
}



//
/*
void rtc_get_time(unsigned char *hour,unsigned char *min,unsigned char *sec)
{
rtc_start();
rtc_writeByte(0xd0);
rtc_writeByte(0);
rtc_start();
rtc_writeByte(0xd1);
*sec=bcd2bin(rtc_readByte(1,1));
*min=bcd2bin(rtc_readByte(1,1));
*hour=bcd2bin(rtc_readByte(0,0));
rtc_stop();
}
void rtc_set_time(unsigned char hour,unsigned char min,unsigned char sec)
{
rtc_start();
rtc_writeByte(0xd0);
rtc_writeByte(0);
rtc_writeByte(bin2bcd(sec));
rtc_writeByte(bin2bcd(min));
rtc_writeByte(bin2bcd(hour));
rtc_stop();
}
void rtc_get_date(unsigned char *date,unsigned char *month,unsigned char *year)
{
rtc_start();
rtc_writeByte(0xd0);
rtc_writeByte(4);
rtc_start();
rtc_writeByte(0xd1);
*date=bcd2bin(rtc_readByte(1,1));
*month=bcd2bin(rtc_readByte(1,1));
*year=bcd2bin(rtc_readByte(0,0));
rtc_stop();
}
void rtc_set_date(unsigned char date,unsigned char month,unsigned char year)
{
rtc_start();
rtc_writeByte(0xd0);
rtc_writeByte(4);
rtc_writeByte(bin2bcd(date));
rtc_writeByte(bin2bcd(month));
rtc_writeByte(bin2bcd(year));
rtc_stop();
}


*/
//	
	
	
	
/*
Anthony Says: 
Hi Avinash
This is my favourite Chip (DS1307)
I know it pretty Good
I give a Lib for my friend
for BCD->BIN
*sec = ((*sec & 0xF0) >> 4)*10 + (*sec & 0x0F);
*mnt = ((*mnt & 0xF0) >> 4)*10 + (*mnt & 0x0F);
*hr = ((*hr & 0xF0) >> 4)*10 + (*hr & 0x0F);
*day = ((*day & 0xF0) >> 4)*10 + (*day & 0x0F);
*year = ((*year & 0xF0) >> 4)*10 + (*year & 0x0F);
*date = ((*date & 0xF0) >> 4)*10 + (*date & 0x0F);
*mn = ((*mn & 0xF0) >> 4)*10 + (*mn & 0x0F);
for BIN->BCD
rtc_writeByte(((sec/10)<<4) + (sec%10));
rtc_writeByte(((min/10)<<4) + (min%10));
rtc_writeByte(((hour/10)<<4) + (hour%10));
///////////////////////////////////////////////////////////
////Read Ram
unsigned char rtc_read(unsigned char address_)
{
unsigned char data_;
rtc_start();
rtc_writeByte(0xd0);
rtc_writeByte(address_);
rtc_start();
rtc_writeByte(0xd1);
data=rtc_readByte(0);
rtc_stop();
return data_;
}
///
void rtc_write(unsigned char address,unsigned char data)
{
rtc_start();
rtc_writeByte(0xd0);
rtc_writeByte(address);
rtc_writeByte(data);
rtc_stop();
}
void rtc_init(unsigned char rs,unsigned char sqwe,unsigned char out)
{
rs&=3;
if (sqwe) rs|=0×10;
if (out) rs|=0×80;
rtc_start();
rtc_writeByte(0xd0);
rtc_writeByte(7);
rtc_writeByte(rs);
rtc_stop();
}
void rtc_get_time(unsigned char *hour,unsigned char *min,unsigned char *sec)
{
rtc_start();
rtc_writeByte(0xd0);
rtc_writeByte(0);
rtc_start();
rtc_writeByte(0xd1);
*sec=bcd2bin(rtc_readByte(1));
*min=bcd2bin(rtc_readByte(1));
*hour=bcd2bin(rtc_readByte(0));
rtc_stop();
}
void rtc_set_time(unsigned char hour,unsigned char min,unsigned char sec)
{
rtc_start();
rtc_writeByte(0xd0);
rtc_writeByte(0);
rtc_writeByte(bin2bcd(sec));
rtc_writeByte(bin2bcd(min));
rtc_writeByte(bin2bcd(hour));
rtc_stop();
}
void rtc_get_date(unsigned char *date,unsigned char *month,unsigned char *year)
{
rtc_start();
rtc_writeByte(0xd0);
rtc_writeByte(4);
rtc_start();
rtc_writeByte(0xd1);
*date=bcd2bin(rtc_readByte(1));
*month=bcd2bin(rtc_readByte(1));
*year=bcd2bin(rtc_readByte(0));
rtc_stop();
}
void rtc_set_date(unsigned char date,unsigned char month,unsigned char year)
{
rtc_start();
rtc_writeByte(0xd0);
rtc_writeByte(4);
rtc_writeByte(bin2bcd(date));
rtc_writeByte(bin2bcd(month));
rtc_writeByte(bin2bcd(year));
rtc_stop();
}
*/
