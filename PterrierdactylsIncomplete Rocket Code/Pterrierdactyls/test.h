#ifndef TEST
#define TEST
#include <Arduino.h>
class TEFXL{
private: 
	int numcool;
public:
	TEFXL();
	TEFXL(int k);
	void serialPrnt();
};
#endif 