TWI_Master32 662 Web


**
21.3.10
DATATASK auf C0 gesetzt

(WebTXStartdaten >= DATATASK) anstatt ==, Vorbereitung fuer unterscheiden von SOLARTASK und DATATASK

**

5.2.10
BUS_Status |=(1<<TWI_CONTROLBIT); Kontrollieren, wie TWI wieder eingeschaltet wird
Linie 1600
Timer fuer TWI zuruecksetzen fuer EEPROMWRITETASK

Test: 
BUS_Status |=(1<<TWI_CONTROLBIT); in ISR: eventuell zu frueh. 

		
in Talk() und Listen():
// Timer fuer TWI-Schlaufe zuruecksetzen
		TCNT0 = 0x00;
		SIGNAL_Count=0;


***
10.12.09
setReadyToSend von Bedingung Daten[2,3,4] nicht Null abhängig gemacht

25.10.09
count durch SIGNAL__count ersetzt

2.2.09
line 2215
Eingefügt: 
i2c_stop();
TWI zutuecksetzen;
TWBR =0;
delay_ms(10);

//	TWI neu starten
i2c_init();


1.2.
line 873
Anfangswert von startdelay in ODER eingefuegt: Kontrolle laeft auch so
startdelay == 

Pins fuer TWI zuerst als Ausgaenge und HI
Erst nach Ablauf Startdelay zu Eingaengen gemacht

line 1046
i2c_stop eingefuegt

line 1424
Ende von if (DCF77-erfolg==0) nach 2144 verschoben: Abfrage der Slaves nur wenn Uhr gelesen werden kann



line 916
TWBR =0; ersetzt durch
TWCR =0;
Quelle: file:///Users/sysadmin/Documents/Elektronik/AVR/I2C/TWI-Error/TWI%20-%20AVR%20stürzt%20ab%20-%20mikrocontroller.net.webarchive

BUS_Status:



31.1.09
line 2070 
i2c_stop() eingesetzt



