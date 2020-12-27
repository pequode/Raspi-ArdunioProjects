#include "test.h"
TEFXL::TEFXL(){ 
	numcool = -1;
}
TEFXL::TEFXL(int num){
	numcool = num;

}
void TEFXL::serialPrnt(){
	Serial.println(numcool);
}
