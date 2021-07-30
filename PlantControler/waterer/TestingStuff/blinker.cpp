#include <wiringPi.h>
int main (void)
{
  wiringPiSetup () ;
  int pin= 1;
  pinMode (pin, OUTPUT) ;
  int s = 0;
  for (s=0;s<5;s++)
  {
    digitalWrite (pin, HIGH) ; delay (500) ;
    digitalWrite (pin,  LOW) ; delay (500) ;
  }
  return 0 ;
}
