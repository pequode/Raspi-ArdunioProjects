#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Adafruit_ADS1015.h>
Adafruit_ADS1115 ads(0x48);
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


float Max = 0;
float Min = 1000;
float Time =0 ;
int TimeI = 0;

int FRW = 11; //pin for the Well plate fans 
int FRR = 12; //pin for the Heat Resuvior plate fans 
int HRR = 4; //pin for the Heat Resuvior Heating Elements
int SDR = 5; //data pin for the servo that lets air in from the outside 
int SDW = 6; //data pin for the servo that lets air into the well plates 

int t0 = -1, t1 = -2, t2 = -3, t3 = -4, t4 = 3, t5 = 2;// the analog inputs from the temperature senstors 
float tempsReadings[6][16] ;  
//For PID Control 
double kp = 2,ki = 5,kd = 1;
unsigned long currentTime, previousTime;
double elapsedTime;
double errors;
double lastError;
double input, output, setPoint;
double cumError, rateError;
double TargetTemp = 37.2;

unsigned long startTime;

void turnR(int pin){//works
  for(int i = 0;i<200;i++){
  digitalWrite(pin,HIGH);
  //delay(5);
  delayMicroseconds(750);
  digitalWrite(pin,LOW);
  delayMicroseconds(750);
  }
}
void turnL(int pin){//works
  for(int i = 0;i<200;i++){
    digitalWrite(pin,HIGH);
    //delay(5);
    delayMicroseconds(1300);
    digitalWrite(pin,LOW);
    delayMicroseconds(1300);
    }
  }

// somethings off
float wellToTempAn(float res,int sizeDataIn){
  //variable defs
  float THERMISTORNOMINAL = 5000;// resistance at 25 degrees C
  float TEMPERATURENOMINAL = 21.1111;// temp. for nominal resistance (almost always 25 C)
  float BCOEFFICIENT = 3270;// The beta coefficient of the thermistor (usually 3000-4000)
  float SERIESRESISTOR = 4600;
  res =  sizeDataIn/ res - 1;
  res = SERIESRESISTOR / res;
  
  float steinhart;
  steinhart = res / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;// convert to C
  float  TempC = steinhart-7.55;
  return TempC; 
  
  }//done//works
float resToTempAn(float res,int sizeDataIn){
  //variable defs
  float THERMISTORNOMINAL = 10000;// resistance at 25 degrees C
  float TEMPERATURENOMINAL = 21.1111;// temp. for nominal resistance (almost always 25 C)
  float BCOEFFICIENT = 3270;// The beta coefficient of the thermistor (usually 3000-4000)
  float SERIESRESISTOR = 4600;
  res =  sizeDataIn/ res - 1;
  res = SERIESRESISTOR / res;
  Serial.print(res);
  float steinhart;
  steinhart = res / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;// convert to C
  float  TempC = steinhart-7.55;
  return TempC; 
  
  }
float getTemp(int Ain){//works for well not for other
    
    float TempC =0;
    float adc0 =0;
    switch(Ain){
        case -1:
          adc0 = ads.readADC_SingleEnded(0);
          TempC = wellToTempAn(adc0,32768);
          break;
        case -2:
          adc0 = ads.readADC_SingleEnded(1);
          TempC = wellToTempAn(adc0,32768);
          break;
        case -3:
          adc0 = ads.readADC_SingleEnded(2);
          TempC = wellToTempAn(adc0,32768);
          break;
        case -4:
          adc0 = ads.readADC_SingleEnded(3);
          TempC = wellToTempAn(adc0,32768);
          break;
        case 3:
          adc0 = analogRead(3);
          TempC = resToTempAn(adc0,1023);
          break;
        case 2:
          adc0 = analogRead(3);//fudge
          TempC = resToTempAn(adc0,1023);
          break;
        default:
          TempC =-1;  
    }      
    return TempC;//working
}

float getCurrentTempWell(){//done
   // updates each well Temp
    Serial.print("Well Temps:\n");
    float currTemp0 = getTemp(t0);
    float currTemp1 = getTemp(t1);
    float currTemp2 = getTemp(t2);
    float currTemp3 = getTemp(t3);
    // for the temp sensors updates the array of past temps 
    for(int i = 0;i<4;i++){
      float temp = 0;
      for(int j = 0;j<16;j++){
         float temps = tempsReadings[i][j];
         tempsReadings[i][j] = temp;
         Serial.print(temp);
         Serial.print(" ");
         temp = temps;
      } 
      Serial.println(",");
    }
    
    tempsReadings[0][0] = currTemp0;
    tempsReadings[1][0] = currTemp1;
    tempsReadings[2][0] = currTemp2;
    tempsReadings[3][0] = currTemp3;
    
    float ave1 = 0, ave2 = 0, ave3 = 0, ave4 = 0 ; 
    float sum = 0;
    for(int j = 0;j<16;j++)sum += tempsReadings[0][j];
        ave1 = sum/16.0;
    sum = 0;

    for(int j = 0;j<16;j++)sum += tempsReadings[1][j];
    ave2 = sum/16.0;
    sum = 0;
    for(int j = 0;j<16;j++)sum += tempsReadings[2][j];
    ave3 = sum/16.0;
    sum = 0;
    for(int j = 0;j<16;j++)sum += tempsReadings[3][j];
    ave4 = sum/16.0;

    // put sensor check here 
    float ave = (ave1 + ave2 +ave3 +ave4)/4;
    Serial.println(ave);
    return ave;  
}

float getCurrentTempRes(){//still a black box for updateing temp sensors and then averaging the last 10 on all of them and then averaging the averages, maybe a contingency for broken sensor
    // updates each well Temp
    Serial.print("Res Temps:\n");
    
    
    float currTemp10 = getTemp(t4);
    float currTemp20 = getTemp(t5);
    Serial.print(currTemp10);
    Serial.print("\n");
    // for the temp sensors updates the array of past temps 
    for(int i = 4;i<6;i++){
      float temp = 0;
      for(int j = 0;j<16;j++){
         float temps = tempsReadings[i][j];
         Serial.print(temp);
         Serial.print(" ");
         tempsReadings[i][j] = temp;
         temp = temps;
      } 
      Serial.println(",");
    }
    
    tempsReadings[4][0] = currTemp10;
    tempsReadings[5][0] = currTemp20;
   
    float ave1 = 0, ave2 = 0 ;
    float sum = 0;
    for(int j = 0;j<16;j++)sum = sum + tempsReadings[4][j];
    ave1 = sum/16.0;
    sum = 0;
    for(int j = 0;j<16;j++)sum =sum + tempsReadings[5][j];
    ave2 = sum/16.0;

    // put sensor check here 
    float ave = (ave1 + ave2)/2;
    Serial.println(ave);
    return ave; 
}

void OpenOutside(bool isOpen){// for letting air into the resuvior
    if(isOpen){
        turnR(SDR);
    }else{
        turnL(SDR);
        
    } 
}
void OpenInside(bool isOpen){// for letting air into the well
    if(isOpen){
        turnL(SDW);
    }
    else{
        turnR(SDW);
    }  
}
void turnOnFanWell(bool isOn){//works
    if(isOn){
      analogWrite(FRW, 255);
    }else{
      analogWrite(FRW, 0);
    }
}
void turnOnFanRes(bool isOn){//works
    if(isOn){
      analogWrite(FRR, 255);
    }else{
      analogWrite(FRR, 0);
    }  
}
void turnOnHeat(bool isOn){//works
    if(isOn){
      digitalWrite(HRR, LOW);
    }else{
      digitalWrite(HRR, HIGH);
    }  
}

void Disp(float Current){
    if (Current > Max) Max = Current;
    if (Current < Min) Min = Current;
    float MaxR = ((int)(Max*100+0.5));
    float MaxRounded =  (float) MaxR /100.0;
    float MinR = ((int)(Min*100+0.5));
    float MinRounded = (float) MinR /100.0;
    float TempR =((int)Current*100+0.5);
    float TempRounded =(float) TempR /100.0;
    float Time = (millis()-startTime)/60000.0;
    float TimeR =(int) (Time*100+0.5);
    float TimeRounded =(float) TimeR /100.0;
    Serial.print("Time: Rounded");
    Serial.println(TimeRounded);
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Maxs: ");
    lcd.print(Max);
    lcd.print(" C");
    lcd.setCursor(0,1);
    lcd.print("Min: ");
    lcd.print(MinRounded);
    lcd.print(" C");
    delay(3000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temp: ");
    lcd.print(TempRounded);
    lcd.print(" C");
    lcd.setCursor(0,1);
    lcd.print("Time: ");
    lcd.print(TimeRounded);
    lcd.print(" Mn");
    delay(3000);
}


double  PID(double inp){     
    currentTime = millis();                //get current time
    elapsedTime = (double)(currentTime - previousTime);        //compute time elapsed from previous computation
    errors = TargetTemp - inp;                                // determine error
    cumError += errors * elapsedTime;                // compute integral
    rateError = (errors - lastError)/elapsedTime;   // compute derivative
    double out = kp*errors + ki*cumError + kd*rateError;                //PID output               
    lastError = errors;                                //remember current error
    previousTime = currentTime;                        //remember current time
    return out;                                        //have function return the PID output
}
void changeTemp(double tempC){
    double topTolerance = 1.0;
    if(tempC>TargetTemp+topTolerance){
          OpenOutside(true);
          OpenInside(true);
          turnOnFanWell(true);
          turnOnFanRes(true);
          turnOnHeat(false);
    }
    else if(tempC<TargetTemp-topTolerance){
          OpenOutside(false);
          OpenInside(true);
          turnOnFanWell(true);
          turnOnFanRes(true);
          turnOnHeat(true);
  
    }    
}

void setup()
{
  Serial.begin(9800);
  startTime = millis();
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  pinMode(FRW, OUTPUT);
  pinMode(FRR, OUTPUT);
  pinMode(HRR, OUTPUT);
  digitalWrite(HRR,LOW);
}
void loop()
{
  double TempC = getCurrentTempWell();
  //double TempR =TempC;// getCurrentTempRes();
  //double OutValue = PID(TempC);
  //double ResOutValue = PID(TempR);
  changeTemp(TempC);
  Disp(TempC);
  //float adc0 = ads.readADC_SingleEnded(0);
  //TempC = wellToTempAn(adc0,32768);
  float c = TempC;
  c = (c *(9.0/5.0))+32;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(c);
  //OpenOutside(true);
  //turnOnFanRes(true);
  delay(1000);
  
}