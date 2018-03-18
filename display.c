/*
 *  display.c
 *  TWI_Master
 *
 *  Created by Sysadmin on 22.03.08.
 *  Copyright 2008 Ruedi Heimlicher. All rights reserved.
 *
 */

#include "display.h"
#include <avr/io.h>
#include <string.h>

//#define EEPROM_WOCHENPLAN_ADRESSE 0xA0; //in TWI-Maser ebenfall definiert
//extern volatile uint8_t DCF77daten[];
extern uint8_t DCF77daten[];

//extern volatile uint8_t DCF77daten[];

extern PGM_P const P_MenuTable[];
extern volatile uint8_t HeizungRXdaten[];
//extern   uint8_t DCF77daten[];
extern  volatile uint8_t LaborRXdaten[];
extern  volatile uint8_t LaborTXdaten[];
extern  volatile uint8_t BueroRXdaten[];
extern  volatile uint8_t BueroTXdaten[];
extern  volatile uint8_t WerkstattRXdaten[];
extern  volatile uint8_t WerkstattTXdaten[];
extern PGM_P const RaumTable[];
extern uint16_t Brennerzeit;
//extern volatile uint8_t MenuEbene;
//volatile uint8_t Raum_Thema;		//	Bit 4-7: Thema		Bit 0-3: Raum
//volatile uint8_t Objekt_Wochentag;	//	Bit 4-7: Objekt		Bit 0-3: Wochentag
//extern char* wochentagstring[];
//char menubuffer[16];
//char titelbuffer[16];
//extern volatile struct time Zeit;
extern void lcd_CGRAMInit_Titel(void);

void display_line(uint8_t line)
{

}

void displayRaum(const uint8_t RaumThema, const uint8_t ObjektWochentag, const uint8_t Stunde, const uint8_t MenuEbene)
{
	uint8_t i=0;
	char menubuffer[16];
	char titelbuffer[16];
	//char* wochentag[] = {"MO","DI","MI","DO","FR","SA","SO"};
	//	err_gotoxy(0,1);
	//	err_puts("dR: \0");
	//	err_puthex(RaumThema);
	//	err_puthex(ObjektWochentag);
	//	err_puthex(MenuEbene);
	//	delay_ms(1000);
	
	//lcd_clr_part(0,0,11);
	//	RaumTable: Namen der Räume
	strcpy_P(titelbuffer, (PGM_P)pgm_read_word(&(RaumTable[RaumThema>>4])));//Bit 7 - 4
	
   //	Raum anzeigen:
	
	//	lcd_gotoxy(0,0);
	//	lcd_puts(titelbuffer);
	
	lcd_gotoxy(15,0);
	//lcd_put_zeit(DCF77daten[0],DCF77daten[1]);
	lcd_put_zeit(Zeit.minute,Zeit.stunde);
	
	//	delay_ms(1000);
	//	(RaumThema>>4):			Bit 7-4, Nummer des Raumes 
	//	*8:							Adresse des ersten Themas des Raumes in MenuTable
	//	(RaumThema & 0x0F):		Bit 3-0, Nummer des Themas 
	uint8_t adr=0;
	if (MenuEbene)
	{
		adr=8*(RaumThema>>4)+(RaumThema & 0x0F)+((MenuEbene & 0xF0)>>4)-1;	
	}
	//err_gotoxy(15,0);
	//err_puts("A:\0");
	//err_puthex(adr);
	
	//delay_ms(500);
	
	strcpy_P(menubuffer, (PGM_P)pgm_read_word(&(P_MenuTable[adr])));//Bit 3 - 0	Untermenu im PROGMEM
	lcd_clr_line(1);
	lcd_gotoxy(0,1);
	//lcd_puthex((uint8_t)&adr);
	//Thema anzeigen:
	//lcd_puts(menubuffer);
	uint8_t raum=RaumThema >>4;
	uint8_t objekt=(ObjektWochentag>>4);
	uint8_t Wochentag=(ObjektWochentag & 0x0F);//DCF-Wochentag ist 1-basiert
	//err_gotoxy(0,0);
	//err_puts("r\0");
	//err_puthex(raum);
	//err_puts("o\0");
	//err_puthex(objekt);
	//err_puts("t\0");
	//err_puthex(Wochentag);
	
	if (MenuEbene & 0xF0)	// nicht oberste Ebene
	{
		lcd_clr_part(0,0,11);
		lcd_puts(titelbuffer);
		lcd_gotoxy(12,0);
		lcd_put_wochentag(Wochentag);// DCF77 ist 1-basiert
		
		lcd_clr_line(1);
		lcd_puts(menubuffer);
		
		switch(RaumThema >>4)	//	Bits 7-4, Raum
		{
			case 0:	//Heizung
			{
				//			lcd_gotoxy(10,1);
				//			lcd_puts("dR H:\0");
				//			delay_ms(800);
				//			uint8_t adr=8*(RaumThema>>4)+(RaumThema & 0x0F);
				//			strcpy_P(menubuffer, (PGM_P)pgm_read_word(&(P_MenuTable[adr])));//Bit 3 - 0	Untermenu im PROGMEM
				
				switch (MenuEbene & 0xF0) //Menu auf Ebene 1: Thema
				{
					case 0x00://oberste Ebene
					{
						
					}break;
						
					case 0x10://erste Ebene: Status
					{
						lcd_gotoxy(0,1);
						lcd_puts(menubuffer);
						
						//err_gotoxy(0,0);
						//err_puthex(adr);
						//delay_ms(400);
						lcd_clr_line(2);
						lcd_gotoxy(0,2);
						//	Vorlauftemperatur:
						lcd_puts("H:\0");
						lcd_put_tempbis99(HeizungRXdaten[0]);
						lcd_gotoxy(10,2); //dritte Zeile
						//	Aussentemperatur
						lcd_puts("R:\0");
						lcd_put_tempbis99(HeizungRXdaten[1]);
						
						lcd_gotoxy(10,1); //zweite Zeile
						
						lcd_puts("A:\0");//	Aussentemperatur
						lcd_put_tempbis99(HeizungRXdaten[2]-20);
						
						//	Brennerstatus:
						uint16_t ACEingang=(HeizungRXdaten[3]);
						lcd_clr_line(3);
						lcd_gotoxy(0,3); //zweite Zeile
						lcd_puts("B: \0");
						if (ACEingang)
						{
							lcd_puts("OFF");
						}
						else
						{
							lcd_puts("ON ");
						}
						//	Akkumulierte Brennerzeit
						lcd_gotoxy(10,3);
						lcd_puts("LT: \0");
						lcd_put_zeit(Brennerzeit % 60,Brennerzeit /60);
						
					}break;
						
					case 0x20://	ObjektWochentag
					{
						/*
						 err_clr_line(1);
						 err_puts("DCF77buf:\0");
						 err_gotoxy(10,1);
						 err_putint2(DCF77daten[1]);
						 err_gotoxy(13,1);
						 err_putint2(DCF77daten[0]);
						 err_gotoxy(16,1);
						 err_putint2(DCF77daten[5]);
						 delay_ms(1800);
						 */
						//lcd_clr_line(2);
						//lcd_puts("Tag:\0");
						//lcd_putint1(DCF77daten[5]);
						//lcd_puts("TgPlan:\0");
						lcd_clr_line(1);
						lcd_puts(menubuffer);
						lcd_clr_line(3);
						lcd_gotoxy(0,3);
						//void TagZeigen(uint8_t Daten[], uint8_t Tag)
						// Wochentag von Uhr:	DCF77daten[5]
						// Stunde von Uhr	:	DCF77daten[1]
						//	Tagplan lesen
						uint8_t tagblock[buffer_size];
						
						//lcd_puts("Raum:\0");
						//lcd_putint1(RaumThema >>4);
						//lcd_puts(" Tag:\0");
						//lcd_puthex(DCF77daten[5]);
						//lcd_puthex(ObjektWochentag & 0x0F);
						
						//err_clr_line(0);
						//err_gotoxy(0,0);
						//err_puts("O_WD:\0");
						//err_puthex(ObjektWochentag);
						//err_puthex(DCF77daten[5]);
						//err_puthex(Zeit.wochentag);
						
						uint8_t taglesenerfolg=WochentagLesen(EEPROM_WOCHENPLAN_ADRESSE, tagblock, raum, objekt, Wochentag);
						//						err_clr_line(0);
						
						/*
						 uint8_t i=0;
						 for (i=0;i<6;i++)
						 {
						 err_puthex(tagblock[5]);
						 err_putc(' ');
						 }
						 */
						//err_putint2(taglesenerfolg);
						//delay_ms(800);
						
						WochentagZeigen(tagblock,Wochentag, Stunde);
						
						
					}break;
						
						
				}//switch
				
				
			}break; //	Heizung
				
			case 1: //Werkstattt
			{
				switch (RaumThema & 0x0F) //Menu auf Ebene 1
				{
					case 0x00://Status
					{
						lcd_clr_line(3);
						lcd_gotoxy(0,1);
						lcd_puts(menubuffer);
						
						lcd_clr_line(2);
						lcd_gotoxy(0,2);
						//	Vorlauftemperatur:
						lcd_puts("T:\0");
						lcd_put_tempbis99(WerkstattRXdaten[0]);
						
						//	Akkumulierte Ofenzeit
						//lcd_gotoxy(10,3);
						//lcd_puts("LT: \0");
						//lcd_put_zeit(Brennerzeit % 60,Brennerzeit /60);
						
						
					}break;
						
					case 0x01://Tagplan
					{
						lcd_clr_line(3);
						lcd_gotoxy(0,3);
						//void TagZeigen(uint8_t Daten[], uint8_t Tag)
						// Wochentag:	DCF77daten[5]
						// Stunde:		DCF77daten[1]
						uint8_t tagblock[buffer_size];
						uint8_t taglesenerfolg=TagLesen(EEPROM_WOCHENPLAN_ADRESSE, tagblock, raum, Wochentag);
						TagZeigen(tagblock,Wochentag);
						
						
					}break;
						
						
				}//switch
				
				
			}	break;	// Werkstatt
				
			case 2: //WoZi
			{
				switch (RaumThema & 0x0F) //Menu auf Ebene 1
				{
					case 0x00://Status
					{
						lcd_clr_line(2);
						lcd_clr_line(3);
						
					}break;
						
					case 0x01://Tagplan
					{
						lcd_clr_line(3);
						lcd_gotoxy(0,3);
						//void TagZeigen(uint8_t Daten[], uint8_t Tag)
						// Wochentag:	DCF77daten[5]
						// Stunde:		DCF77daten[1]
						uint8_t tagblock[buffer_size];
						uint8_t taglesenerfolg=TagLesen(EEPROM_WOCHENPLAN_ADRESSE, tagblock, raum, Wochentag);
						TagZeigen(tagblock,Wochentag);
						
						
					}break;
						
						
				}//switch
				
				
			}	break;
				
			case 3: //Buero
			{
				switch (MenuEbene & 0xF0) //Menu auf Ebene 1: Thema
				{
					case 0x00://oberste Ebene
					{
						
					}break;
						
					case 0x10://Status
					{
						
						lcd_gotoxy(0,1);
						lcd_puts(menubuffer);
						
						//err_gotoxy(0,0);
						//err_puthex(adr);
						//delay_ms(400);
						lcd_clr_line(2);
						lcd_gotoxy(0,2);
						lcd_puts("H:\0");
						lcd_clr_line(3);
					}break;
						
					case 0x20://	ObjektWochentag
					{
						/*
						 err_clr_line(1);
						 err_puts("DCF77buf:\0");
						 err_gotoxy(10,1);
						 err_putint2(DCF77daten[1]);
						 err_gotoxy(13,1);
						 err_putint2(DCF77daten[0]);
						 err_gotoxy(16,1);
						 err_putint2(DCF77daten[5]);
						 delay_ms(1800);
						 */
						//lcd_clr_line(2);
						//lcd_puts("Tag:\0");
						//lcd_putint1(DCF77daten[5]);
						//lcd_puts("TgPlan:\0");
						lcd_clr_line(1);
						lcd_puts(menubuffer);
						lcd_clr_line(3);
						lcd_gotoxy(0,3);
						//void TagZeigen(uint8_t Daten[], uint8_t Tag)
						// Wochentag von Uhr:	DCF77daten[5]
						// Stunde von Uhr	:	DCF77daten[1]
						//	Tagplan lesen
						uint8_t tagblock[buffer_size];
						
						//lcd_puts("Raum:\0");
						//lcd_putint1(RaumThema >>4);
						//lcd_puts(" Tag:\0");
						//lcd_puthex(DCF77daten[5]);
						//lcd_puthex(ObjektWochentag & 0x0F);
						
						//err_clr_line(0);
						//err_gotoxy(0,0);
						//err_puts("O_WD:\0");
						//err_puthex(ObjektWochentag);
						//err_puthex(DCF77daten[5]);
						//err_puthex(Zeit.wochentag);
						
						
						
						//uint8_t taglesenerfolg=WochentagLesen(EEPROM_WOCHENPLAN_ADRESSE, tagblock, raum, objekt, DCF77daten[5]);
						
						uint8_t taglesenerfolg=WochentagLesen(EEPROM_WOCHENPLAN_ADRESSE, tagblock, raum, objekt, Wochentag);
						
						//err_putint2(taglesenerfolg);
						//delay_ms(800);
						
						//TagZeigen(tagblock,DCF77daten[5]);
						WochentagZeigen(tagblock,Wochentag, Stunde);
						
						
					}						
						
				}//switch
				
				
			}	break;
				
			case 4: //Labor
			{
				switch (RaumThema & 0x0F) //Menu auf Ebene 1
				{
					case 0x00://Status
					{
						lcd_clr_line(3);
						lcd_gotoxy(0,1);
						lcd_puts(menubuffer);
						
						lcd_clr_line(2);
						lcd_gotoxy(0,2);
						//	Vorlauftemperatur:
						lcd_puts("L0:\0");
						lcd_put_tempbis99(LaborRXdaten[0]);
						lcd_putc(' ');
						lcd_puts("LA:\0");
						
						lcd_put_tempAbMinus20(LaborRXdaten[1]);
						
						//	Akkumulierte Ofenzeit
						//lcd_gotoxy(10,3);
						//lcd_puts("LT: \0");
						//lcd_put_zeit(Brennerzeit % 60,Brennerzeit /60);
						
						
					}break;
						
					case 0x01://Tagplan
					{
						lcd_clr_line(3);
						lcd_gotoxy(0,3);
						//void TagZeigen(uint8_t Daten[], uint8_t Tag)
						// Wochentag:	DCF77daten[5]
						// Stunde:		DCF77daten[1]
						uint8_t tagblock[buffer_size];
						uint8_t taglesenerfolg=TagLesen(EEPROM_WOCHENPLAN_ADRESSE, tagblock, raum, Wochentag);
						TagZeigen(tagblock,Wochentag);
						
						
					}break;
						
						
				}//switch
				
				
			}	break;
				
			case 5: //OG 1
			{
				switch (RaumThema & 0x0F) //Menu auf Ebene 1
				{
					case 0x00://Status
					{
						lcd_clr_line(2);
						lcd_clr_line(3);
						
					}break;
						
					case 0x01://Tagplan
					{
						lcd_clr_line(3);
						lcd_gotoxy(0,3);
						//void TagZeigen(uint8_t Daten[], uint8_t Tag)
						// Wochentag:	DCF77daten[5]
						// Stunde:		DCF77daten[1]
						uint8_t tagblock[buffer_size];
						uint8_t taglesenerfolg=TagLesen(EEPROM_WOCHENPLAN_ADRESSE, tagblock, raum, Wochentag);
						TagZeigen(tagblock,Wochentag);
						
						
					}break;
						
						
				}//switch
				
				
			}	break;
				
			case 6: //OG 2
			{
				switch (RaumThema & 0x0F) //Menu auf Ebene 1
				{
					case 0x00://Status
					{
						lcd_clr_line(2);
						lcd_clr_line(3);
						
					}break;
						
					case 0x01://Tagplan
					{
						lcd_clr_line(3);
						lcd_gotoxy(0,3);
						//void TagZeigen(uint8_t Daten[], uint8_t Tag)
						// Wochentag:	DCF77daten[5]
						// Stunde:		DCF77daten[1]
						uint8_t tagblock[buffer_size];
						uint8_t taglesenerfolg=TagLesen(EEPROM_WOCHENPLAN_ADRESSE, tagblock, raum, Wochentag);
						TagZeigen(tagblock,Wochentag);
						
						
					}break;
						
						
				}//switch
				
				
			}	break;
				
			case 7: //Estrich
			{
				switch (RaumThema & 0x0F) //Menu auf Ebene 1
				{
					case 0x00://Status
					{
						lcd_clr_line(2);
						lcd_clr_line(3);
						
					}break;
						
					case 0x01://Tagplan
					{
						lcd_clr_line(3);
						lcd_gotoxy(0,3);
						//void TagZeigen(uint8_t Daten[], uint8_t Tag)
						// Wochentag:	DCF77daten[5]
						// Stunde:		DCF77daten[1]
						uint8_t tagblock[buffer_size];
						uint8_t taglesenerfolg=TagLesen(EEPROM_WOCHENPLAN_ADRESSE, tagblock, raum, Wochentag);
						TagZeigen(tagblock,Wochentag);
						
						
					}break;
						
						
				}//switch
				
				
			}	break;
				
				
		}//switch RaumThema
		//		err_gotoxy(15,1);
		//		err_puts("M\0");
		//		err_puthex(MenuEbene);
		//		delay_ms(1000);
		//		err_gotoxy(15,1);
		//		err_puts("   \0");
		
	}	// if MenuEbene
	else
	{
		lcd_CGRAMInit_Titel();
		//		lcd_clr_line(0);
		//		lcd_put_wochentag(Wochentag);
		
		//err_clr_line(0);
		//err_putint2(Zeit.wochentag);
		//err_putint2(Zeit.stunde);
		//err_putint2(Zeit.minute);
		//delay_ms(600);
		
		//lcd_put_zeit(DCF77daten[0],DCF77daten[1]);
		lcd_clr_part(0,0,14);
		lcd_put_wochentag(Zeit.wochentag);
		lcd_putc(' ');
		lcd_putint2(Zeit.kalendertag);
		lcd_putc('.');
		lcd_putint2(Zeit.kalendermonat);
		lcd_putc('.');
		lcd_putint2(Zeit.kalenderjahr);
		lcd_gotoxy(15,0);
		lcd_put_zeit(Zeit.minute,Zeit.stunde);
		lcd_clr_line(3);
		lcd_clr_line(2);
		lcd_clr_line(1);
		
		lcd_gotoxy(1,1);
		lcd_putc(2);	//	Ecke links oben
		
		//	Obere Linie
		
		//
		for (i=0;i<7;i++)
		{
			lcd_putc(0xB0);	//Linie mitte
		}
		
		if ((RaumThema & 0xF0)==0x70)	//Ende der Liste
		{
			lcd_putc(0xB0);	//Linie mitte
		}
		else
		{
			lcd_putc(7);	//Pfeil nach oben
		}
		for (i=0;i<8;i++)
		{
			lcd_putc(0xB0);	//Linie mitte
		}
		//
		
		
		lcd_putc(5);	//	Ecke rechts oben
		uint8_t l=strlen(titelbuffer);
		lcd_gotoxy(1,2);
		//lcd_puts("+++ \0");
		lcd_putc(0x7C);
		//		lcd_putc(1);
		lcd_gotoxy(10-l/2-1,2);
		lcd_puts(titelbuffer);
		lcd_gotoxy(18,2);
		lcd_putc(0x7C);
		
		lcd_gotoxy(1,3);
		lcd_putc(3);	//	Ecke links oben
		
		// Untere Linie
		
		//
		for (i=0;i<7;i++)
		{
			lcd_putc(0xB0);	//Linie mitte
		}
		
		if (RaumThema & 0xF0)
		{
			lcd_putc(8);	//Nicht erster Raum, Pfeil nach unten
		}
		else
		{
			lcd_putc(0xB0);	//Erster Raum, Linie mitte
		}
		for (i=0;i<8;i++)
		{
			lcd_putc(0xB0);	//Linie mitte
		}
		//
		
		lcd_putc(6);	//	Ecke rechts oben
		
		//lcd_puts("+++\0");
		//		lcd_clr_line(1);
		//		lcd_puts(menubuffer);
	}
}
