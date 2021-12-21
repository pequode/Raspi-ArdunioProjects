# RaspberryPi and Ardunio Projects
## Contents
This Repo contains a most of the source code for my various personal projects that Employ Micro-Controllers. For this Repo Each folder should be considered its own Mini Repo with a corresponding project.

Some of the code posted here requires additional library downloads. For those I will try to include sources for those supporting libraries.

### Projects
* **Incubator:**
  - As part of BU's EK210 I was put in a team of 3 and tasked with creating a Incubator that could keep human heart cell tissue at 38.0 &plusmn; 0.2&deg;C. I was in charge of the wiring of temperature sensors and heaters. I was also in charge of creating a control loop that managed the temperature given sensor info. The folder contains the control system code. This project used:
    - Ardunio Uno
    - 4x Thermistors rated for our temperature range
    - 3x 100 Ohm 100 watt resistors.
    - 4x fans to help regulate temperature
    - 2x servos to control airflow

* **Plant Controller**
  - I decided to build an automated system to garden for me. I live in an apartment and am forgetful when it comes to watering my plants. This is actually the second iteration of the project. The first used an ardunio and 4 relays to do the same thing but with a rote timing cycle. This version was used to develop my ability to chain together multiple applications to make a more impressive whole. I wanted to create a device that could:
    - Water my plants
    - Change the lighting on my plants
    - keep the humidity low.
    - be configured from a web app
    - allow for remote monitoring of the plants status.
    - keep accurate timing( an issue with the last iteration)  

   To accomplish this I created a threaded cpp script that could run each of the GPIO pins on my RaspberryPi given a dynamic control loop. I configured the script to save safe and to run on startup so that if the local network lost connectivity or power was interrupted the core watering functionality would not stop and the GUI would reconnect automatically. I wrote a python flask server to host the GUI via a web app.
   I wired the necessary components to make the relays change the state of the various actuators.
   This project used:
     - RaspberryPi 3b+
     - 4x Relays
     - A webcam plugged into the RaspberryPi (to provide the website with a picture of the plant)
     - 5x custom wired receptacles (so that the project could be re-purposed later)
* **Pterrierdactyls- Boston University Rocket Propulsion Group's High Powered Rocket Competition**
 - I participated in BURPG's HPR competition. I was responsible for getting the 2 adafruit feather micro-controllers to communicate with each other and the IMU to get data about its orientation. Additionally I was in charge of creating a method for the rocket to predict when it would be at it highest point. This folder holds all these programs. There would be more code but This project was during 2020 and the Pandemic led to the project being canceled. All of the posted code worked.
* **RaspberryPi Programs**
 - This folder contains different small projects with the RaspberryPi that aren't large enough to be their own folder. 
