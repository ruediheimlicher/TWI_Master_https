# include <avr/io.h>

//# include "i2cmaster.h"
# include "Funktionen.h"
#include <stdlib.h>
#include <avr/wdt.h>

#define LOOPLEDPINPORT			PORTC
#define LOOPLEDPIN				2	// Ausgang fuer LOOPLEDPIN
#define TWI_CONTROLPIN			3		// LED-Anzeige fuer TWI ON

#define TWICOUNTPIN				5


						

#define SERVOPORT					PORTC				// Ausgang fuer Servo
#define SERVOPIN0					7

#define SCHALTEREIN				0
#define SCHALTERAUS				1

#define ADCPORT					PORTA
#define TASTATURPIN				0	//	Eingang fuer Tastatur
#define THERMOMETERPIN			1	//	Eingang fuer Thermometer

#define SRPORT						PORTD
#define SRDDR						DDRD
#define SRPIN						PIND
#define SR_CLK_PIN				4
#define SR_DATA_PIN				5
#define SR_LOAD_PIN				6



#define WRITE_CONFIRM_BIT		1

#define RTC_ADRESSE						0xD0

#define SLAVE_ADRESSE					0x50
#define DCF77_ADRESSE					0x54
#define HEIZUNG_ADRESSE					0x56
#define ESTRICH_ADRESSE					0x58
#define LABOR_ADRESSE					0x60
#define BUERO_ADRESSE					0x62
#define WERKSTATT_ADRESSE				0x64
#define WOZI_ADRESSE						0x66
#define OG1_ADRESSE						0x68
#define OG2_ADRESSE						0x72
#define DATA_ADRESSE						0x70
#define WEB_ADRESSE						0x88
#define EEPROM_WOCHENPLAN_ADRESSE	0xA0

#define buffer_size			8 
#define tag_data_size		32
#define tag_start_adresse	0
#define tastatur_kanal		0


#define twi_buffer_size		8
//#define buffer_size			8
#define tag_data_size		32
#define eeprom_buffer_size 8

//#define data_buffer_size	8




#define PAKETE								1



// Fehler-defines
// Heizung:
#define A0	0xA0
#define A1	0xA1
#define A2	0xA2
#define A3	0xA3
#define A4	0xA4
#define A5	0xA5

#define INNEN			0	// Bit fuer Innentemperatur

// TWI
#define STATUS			3	// Byte fuer Status

// Strom
#define STROMHH      4 // Bytes fuer Stromdaten
#define STROMH       5
#define STROML       6




/*Der Buffer fuer die Daten der DCF-Uhr.*/
//volatile uint8_t DCF77daten[buffer_size];//
uint8_t DCF77daten[buffer_size];//

/*Der Buffer fuer die Daten der RTC.*/
volatile uint8_t RTCdaten[buffer_size];//

enum datenoffset // Offset der Daten auf dem Array, der zur WebCentral uebertragen wird.
{
	heizung, werkstatt, buero, wozi, labor, og1, og2, estrich=9
};

void masterinit(void);


//volatile int count=0;

//uint16_t			EEMEM Brennerzeit;	// Akkumulierte Laufzeit
//uint16_t			brennerzeit;

struct time {
  uint8_t sekunde;
  uint8_t minute;
  uint8_t stunde;
  uint8_t kalendertag;
  uint8_t wochentag;
  uint8_t kalendermonat;
  uint8_t kalenderjahr;
};



volatile struct time Zeit;

volatile uint8_t  	neueZeit=0;

uint8_t					segDelay=30; //Delay f�r Segmentanzeige
uint16_t					Anzeigewert=0;
uint16_t					zaehler=0;
uint16_t					dauer=0;
volatile uint8_t  	digitnummer=0;
volatile uint8_t  	maxdigit=5;
volatile uint8_t  	digitdelaycountL=0;
volatile uint8_t  	digitdelaycountH=0;

volatile uint8_t		PaketNummer=0;

volatile uint8_t      TWI_FLAG=0;

//uint16_t				brennerzeit=0;

// Data fuer temporaere Settings
/*
 raum: 8 
   objekt: 4
      wochentag: 8
         Data: 8
 zu gross fuer 644p
 */
// volatile uint8_t     daySettingArray[8][8][8][8] = {{{{0}}}}; // 1 Zeile pro raum, 8 Zeilen fuer tag, 4 Zeilen fuer objekt, 6 bytes Data 

// neue Version
volatile uint8_t     daySettingArray[16][16] = {{0}}; // 1 Zeile pro paket, 16 bytes Data 
/*
 byte0:  
 */
volatile uint8_t                        permanent;
void daySettingSchreiben(uint8_t wochentag, uint8_t raum, uint8_t data[]);

uint8_t daySettingLesen(uint8_t wochentag, uint8_t raum, uint8_t data[]);



/*Der Buffer fuer die Daten der DCF-Uhr.*/
//volatile uint8_t DCF77daten[buffer_size];//


uint8_t TagSchreiben(unsigned char ADRESSE, uint8_t Daten[], uint8_t Tag);
void readADCSlave (unsigned char address);
void readDCF77(unsigned char address);
void readEstrichSlave (unsigned char address);

uint8_t UhrAbrufen(void);

const char *byte2binary(int x);
uint8_t  SevenSegment(uint8_t count,uint8_t dp, uint8_t dec_hex);

