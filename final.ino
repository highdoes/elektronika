#include <SoftwareSerial.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
SoftwareSerial SIM900(7, 8);

float t;
char incomingChar;

void setup() {
	Serial.begin(19200);
	SIM900.begin(19200);
	sensors.begin();

	delay(20000);
	Serial.print("SIM900 ready...");
	SIM900.print("AT+CMGF=1\r");
	delay(100);
	SIM900.print("AT+CNMI=2,2,0,0,0\r");
	delay(100);
}

void loop() {
	if (SMSRequest()) {
		sensors.requestTemperatures();
		t = sensors.getTempCByIndex(0);
		delay(10);
		SIM900.println("AT + CMGS = \"+48796455469\"");
		delay(100);
		String dataMessage = ("Actual temperature is  " + String(t) + "*C ");
		SIM900.print(dataMessage);
		delay(100);
		SIM900.println((char)26);
		delay(100);
		SIM900.println();
		delay(5000);
	}
	delay(10);
}


boolean SMSRequest() {
	if (SIM900.available() >0) {
		incomingChar = SIM900.read();
		if (incomingChar == 'T') {
			delay(10);
			Serial.print(incomingChar);
			incomingChar = SIM900.read();
			if (incomingChar == 'E') {
				delay(10);
				Serial.print(incomingChar);
				incomingChar = SIM900.read();
				if (incomingChar == 'M') {
					delay(10);
					Serial.print(incomingChar);
					incomingChar = SIM900.read();
					if (incomingChar == 'P') {
						delay(10);
						Serial.print(incomingChar);
						Serial.print("...Request Received \n");
						return true;
					}
				}
			}
		}
	}
	return false;
}
