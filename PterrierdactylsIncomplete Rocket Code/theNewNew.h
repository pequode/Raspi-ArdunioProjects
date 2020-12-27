#ifndef MAADLORA
#define MAADLORA

#include <Wire.h>
#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>


#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
#define RF95_FREQ 915.0

class MAADLORA{
private: 
	bool isTrans;
	RH_RF95 radObject(RFM95_CS, RFM95_INT);

public:
	MAADLORA();
	MAADLORA(bool transmit);
	void sendData(String inp);
	//string receiveData();
	//vector<char[20]> convertToPackets(vector<float> data, vector<int> sigFigs);

};


#endif
