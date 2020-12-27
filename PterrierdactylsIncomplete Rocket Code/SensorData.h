#ifndef SENSORDATA
#define SENSORDATA
#include <Wire.h>
#include <Arduino.h>
#include <theNewNew.h>

// list of sensors 
// accelerometer
//	X 5
//  Y 5
//	Z 5
//	Yaw 5
//	Pitch 5 
//	Roll 5
// altimiter
// 		- Barameter/ meters to 2 dec 7   
//		- Temp / cent to 2 dec  5 
// gps
//		Lat 20 
//		Long 20 
class SensorJayz(){
	private:
		float val[3];
		int length;
		int packetNum
	public:
		SensorJayz();
		void updateVal();
		char[20] convertToPacket();
		void sendData();

}
#endif
