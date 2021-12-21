#include <theNewNew.h>

MAADLORA::MAADLORA(){
Serial.println("Error: Did not define object type");
}

// used to define if the module is a the ground station or the rocket
MAADLORA::MAADLORA(bool transmit){
	isTrans = transmit;
	RH_RF95 rf95(RFM95_CS, RFM95_INT);
	radObject = rf95;
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

	while (!rf95.init()) {
	  Serial.println("LoRa radio init failed");
	  while (1);
	}
	Serial.print("LoRa radio init OK!, \t");

	  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
	if (!rf95.setFrequency(RF95_FREQ)) {
	  Serial.println("setFrequency failed");
	  while (1);
	}
	Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
	rf95.setTxPower(23, false);
	if (isTrans){
		Serial.println("Rocket Setup");

	}
	else {
		Serial.println("Ground Setup");
	}
}
// used to send data, receiving data is far eaiser
void MAADLORA::sendData(vector<float> data, vector<int> sigFigs){
	delay(1000); // Wait 1 second between transmits, could also 'sleep' here!
  Serial.println("Transmitting..."); // Send a message to rf95_server

  vector<char[20]> dataPackets = convertToPackets(data, sigFigs);
  for (i=0;i<dataPackets.size();i++){
  	char radiopacket[20] = dataPackets[i];
	itoa(packetnum++, radiopacket+13, 10);
	Serial.print("Sending "); Serial.println(radiopacket);
	radiopacket[19] = 0;
	Serial.println("Sending...");
  	delay(10);
  	rf95.send((uint8_t *)radiopacket, 20);
  	Serial.println("Waiting for packet to complete...");
  	delay(10);
  	rf95.waitPacketSent();
	// Now wait for a reply
	uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
	uint8_t len = sizeof(buf);
	}
	Serial.println("Waiting for reply...");
	if (rf95.waitAvailableTimeout(1000))
	{
	  // Should be a reply message for us now
		if (rf95.recv(buf, &len))
		{
			Serial.print("Got reply: ");
			Serial.println((char*)buf);
			Serial.print("RSSI: ");
			Serial.println(rf95.lastRssi(), DEC);
		}
		else Serial.println("Receive failed");
	}
	else Serial.println("No reply, is there a listener around?");
}



string MAADLORA::receiveData();
vector<char[20]> MAADLORA::convertToPackets(vector<float> data, vector<int> sigFigs){
return 0;
}
