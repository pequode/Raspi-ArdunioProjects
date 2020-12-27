#include <math.h>
using namespace std;
float currentTime = 0,lastTime = 0,
Altitude = 0,Appoge =0, expectedAppoge =0,
MaxImpulse = 0, MaxV =0,LastV;

void appocgeCalc(){
	float a = 0;//getAccel();
	if( (a+9.8) > 0 ){
		float currentImpulse = a + 9.8;// rough estimate of impulse not including drag 
		if (currentImpulse>MaxImpulse) MaxImpulse =currentImpulse; // saves max force 
	}

	currentTime = 0; //millis()/1000;
	Altitude = 0;//getAlt();//+ LastV*(currentTime-lastTime) + 0.5*a*(currentTime-lastTime)^2;
	LastV = a*(currentTime-lastTime);
	if (LastV>MaxV) MaxV = LastV;
	lastTime = currentTime;
	if(Altitude > Appoge) Appoge = Altitude;
	expectedAppoge = -sqrt(MaxV)/(2*a);
}
int main(){



	appocgeCalc();
	return 0;
}