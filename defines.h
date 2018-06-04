/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <joerg@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.        Joerg Wunsch
 * ----------------------------------------------------------------------------
 *
 * General stdiodemo defines
 *
 * $Id: defines.h,v 1.1.2.2 2006/10/08 21:51:14 joerg_wunsch Exp $
 */

/* CPU frequency */
#define F_CPU 1000000UL

/* UART baud rate */
#define UART_BAUD  9600

/* HD44780 LCD port connections */
#define HD44780_PORT A
#define HD44780_RS PORT6
#define HD44780_RW PORT4
#define HD44780_E  PORT5
/* The data bits have to be in ascending order. */
#define HD44780_D4 PORT0
#define HD44780_D5 PORT1
#define HD44780_D6 PORT2
#define HD44780_D7 PORT3


// TWITASK 


#define SCLPIN      0
#define SDAPIN      1

#define WEB_ON      1
#define IOW_TYP      24

#define PRELL 2

#define HEIZUNG      0
#define WERKSTATT      1
#define WOZI         2
#define BUERO         3
#define LABOR         4
#define OG1            5
#define OG2            6
#define ESTRICH      7

#define FEHLERBYTE   24      //Beginn der Fehlermeldungen


#define buffer_size         8

/*
 #define NULLTASK            0xB0   // Nichts tun
 #define STATUSTASK         0xB1   // Status des TWI aendern
 #define STATUSCONFIRMTASK   0xB2   // Statusaendern bestaetigen
 #define STATUSWAITTASK     0xB3  // Warten bis TWI zuende
 #define EEPROMREPORTTASK   0xB4   // Daten von EEPROM an HomeServer schicken
 #define EEPROMCONFIRMTASK   0xB5  // Quittung an HomeCentral senden
 #define EEPROMRECEIVETASK   0xB6   // Start-Adresse von EEPROM empfangen
 #define EEPROMWRITETASK      0xB7   // auf EEPROM schreiben
 #define EEPROMREADTASK      0xB8   // von EEPROM lesen
 #define EEPROMSENDTASK      0xB9   // von EEPROM lesen
 #define EEPROMWAITTASK      0xBE   // auf schreiben von EEPROM warten
 
 #define PWMREADTASK        0xBA  // PWM-Daten vom EEPROM lesen und an Zielraum schicken (synchronisierung)
 
 #define SYNCWAITTASK       0xBB // Sync im gang
 #define SYNCOKTASK         0xBD // Sync OK
 
 #define DATATASK            0xC0   // Normale Loop im Webserver
 
 #define SOLARTASK            0xC1   // Data von solar
 #define MASTERTASK         0xC2 // default bei fehlendem SPI
 
 #define MASTERERRTASK      0xC7   // Fehlermeldung vom Master senden
 */
#define ERRTASK   0xA0   // Fehlertask an Webserver schicken, soll Eintrag ins Log veranlassen
#define ERR_UHR   0xA1   // 
#define NULLTASK   0xB0   // Nichts tun
#define STATUSTASK   0xB1   // Status des TWI aendern
#define STATUSCONFIRMTASK   0xB2   // Statusaenderung des TWI bestaetigen
#define STATUSRETURNTASK   0xB3   // Status ON des TWI bestaetigen
#define EEPROMREPORTTASK   0xB4   // Daten vom EEPROM an HomeServer senden
#define EEPROMCONFIRMTASK   0xB5   // Quittung an HomeCentral senden
#define EEPROMRECEIVETASK   0xB6   // Adresse fuer EEPROM-Write empfangen
#define EEPROMWRITETASK   0xB7   // auf EEPROM schreiben    
#define EEPROMREADTASK   0xB8   // von EEPROM lesen
#define EEPROMSENDTASK   0xB9   // Daten vom HomeServer an HomeCentral senden
#define EEPROMREADWOCHEATASK   0xBA   //
#define PWMREADTASK   0xBA   // PWM-Daten vom EEPROM lesen und an Zielraum schicken (synchronisierung)        
#define EEPROMREADWOCHEBTASK   0xBB   //         
#define SYNCWAITTASK   0xBB   // Sync im gang        
#define EEPROMREADPWMTASK   0xBC   // Daten fuer PWM-Array im EEPROM holen
#define SYNCOKTASK   0xBD   // Sync OK        
#define RESETTASK   0xBF   // HomeCentral reseten     

#define DATATASK   0xC0   // Normale Loop im Webserver        
#define SOLARTASK   0xC1   // Daten von solar        
#define MASTERTASK   0xC2   // default bei fehlendem SPI        
#define MASTERERRTASK   0xC7   // Fehlermeldung vom Master senden        
#define RAMWRITEDAYTASK   0xD7   // Daten ins RAM schreiben, nur an einem Tag gueltig



// defines fuer Alarm
#define TIEFKUEHLALARM        3
#define WASSERALARMKELLER     4
#define WASSERALARMESTRICH    1

// defines fuer spistatus





// defines fuer BUS-Status
#define SPI_SENDBIT            0
#define TWI_CONTROLBIT         1            // Statusbit fuer TWI
#define WEB_CONTROLBIT         2            // Statusbit fuer Web

// Uhr


// defines fuer uhrstatus
#define SYNC_OK         0   // Uhr ist synchronisiert
#define SYNC_WAIT         1   // Uhr ist  wartet auf Synchronisation
#define SYNC_READY      2   // DCF77 hat gueltiges Datum 
#define SYNC_CHECK      3   // DCF77 soll Datum bereitstellen, Anzahl korrekte Daten abwarten
#define SYNC_NULL         4   // Uhr ist undefiniert, wartet auf Synchronisation (nach restart)
#define SYNC_NEW         5   // erste Synchrinisation nach Reset, noch keine gueltige Zeit
#define SYNC_WEBTIME    6  // Zeit wurde vom Webserver uebernommen
#define SYNC_FIRSTRUN   7 // Erste Runde nach Start


#define MIN_SYNC      2   // Anzahl gueltige Daten fuer Synchronisation


// defines fuer EEPROMstatus
#define PWM_READ     0  // Daten fuer PWM lesen


#define TASTE1 38
#define TASTE2 46
#define TASTE3 54
#define TASTE4 72
#define TASTE5 95
#define TASTE6 115
#define TASTE7 155
#define TASTE8 186
#define TASTE9 205
#define TASTEL 225
#define TASTE0 235
#define TASTER 245

#define STARTDELAY 0xAFFF

//#define WOCHENPLANBREITE 0x40

#define START_BYTE_DELAY   12      // Timerwert fuer Start-Byte
#define BYTE_DELAY         12      // Timerwert fuer Data-Byte


// defines fuer PWM
#define PWM_CODEBIT        0     //Es folgt ein TWI-Paket mit den Positionsdaten fuer den PWM. Diese werden in den Array Servoposition geladen
#define PWM_SCHALTERBIT    1     // Byte 7 enthaelt Schalterposition, die eingestellt werden soll
#define PWM_POSITIONBIT    2     // Byte 7 enthaelt Impulslaenge fuer PWM
