#include "theNewNew.h"

MAADLORA::MAADLORA(){Serial.println("Error: Did not define object type");}
MAADLORA::MAADLORA(bool transmit){
	isTrans = transmit;
	//This might be fucked ^^
	pinMode(RFM95_RST, OUTPUT);
	digitalWrite(RFM95_RST, HIGH);

	Serial.begin(115200); //check for serial port
	while (!Serial) {
	  delay(1);
	}

	delay(100);

	Serial.print("Successfully initialized,\t");

	  // manual reset
	digitalWrite(RFM95_RST, LOW);
	delay(10);
	digitalWrite(RFM95_RST, HIGH);
	delay(10);

	Serial.print("Reset, \t");

	while (!this->radObject.init()) {
	  Serial.println("LoRa radio init failed");
	  while (1);
	}
	Serial.print("LoRa radio init OK!, \t");

	  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
	if (!this->radObject.setFrequency(RF95_FREQ)) {
	  Serial.println("setFrequency failed");
	  while (1);
	}
	Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
	this->radObject.setTxPower(23, false);
	if (isTrans){
		Serial.println("Rocket Setup");

	}
	else {
		Serial.println("Ground Setup");
	}
}
void MAADLORA::sendData(String inp){
	delay(1000); // Wait 1 second between transmits, could also 'sleep' here!
  	Serial.println("Transmitting..."); // Send a message to rf95_server

  	char radiopacket[20] = "hepC";
	itoa(packetnum++, radiopacket+13, 10); 
	Serial.print("Sending "); Serial.println(radiopacket); 
	radiopacket[19] = 0;
	Serial.println("Sending...");
  	delay(10);
  	this->radObject.send((uint8_t *)radiopacket, 20);
  	Serial.println("Waiting for packet to complete..."); 
  	delay(10);
  	this->radObject.waitPacketSent();
	// Now wait for a reply
	uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
	uint8_t len = sizeof(buf);
	Serial.println("Waiting for reply...");
	if (this->radObject.waitAvailableTimeout(1000))
	{ 
	  // Should be a reply message for us now   
		if (this->radObject.recv(buf, &len))
		{
			Serial.print("Got reply: ");
			Serial.println((char*)buf);
			Serial.print("RSSI: ");
			Serial.println(this->radObject.lastRssi(), DEC);    
		}
		else Serial.println("Receive failed");
	}
	else Serial.println("No reply, is there a listener around?");
}

	

//string MAADLORA::receiveData();
/*vector<char[20]> MAADLORA::convertToPackets(vector<float> data, vector<int> sigFigs){
return 0;
}*/