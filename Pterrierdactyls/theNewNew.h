#ifndef MAADLORA
#define MAADLORA
#include <Wire.h>
#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <vector.h>
//parameters for feather lora board M0
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
#define RF95_FREQ 915.0

// used to define the MAADLORA class
class MAADLORA{
private:
	bool isTrans;
	RH_RF95 radObject;

public:
	MAADLORA();
	MAADLORA(bool transmit);
	void sendData(vector<float> data, vector<int> sigFigs);
	string receiveData();
	vector<char[20]> convertToPackets(vector<float> data, vector<int> sigFigs);

};


#endif
