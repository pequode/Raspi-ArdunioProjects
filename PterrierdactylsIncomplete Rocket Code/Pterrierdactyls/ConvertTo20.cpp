; PlatformIO Project Configuration File
;
;   Build options: build flags, source filter
;   Upload options: custom upload port, speed and extra flags
;   Library options: dependencies, extra library storages
;   Advanced options: extra scripting
;
; Please visit documentation for the other options and examples
; https://docs.platformio.org/page/projectconf.html

[env:adafruit_feather_m0]
platform = atmelsam
board = adafruit_feather_m0
framework = arduino
lib_deps = 
    RadioHead
//beginning of code we wrote
#include <string>
//testing shit goes here shout out to you int main
char *out20point;
char out20[];
out20point = &out20;

void ConvertTo20(string);

void ConvertTo20(string inString){
    for (int i=0; i<=strlen(inString);i++){
            for (int j=0; j<=20; j++){   
                out20point[j] = inString[j];

            }
        }
    }
}
