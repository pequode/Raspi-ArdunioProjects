# Installation
This program should work with the following libraries for python:
- flask
- pygame

and these libraries for cpp:
- Windows
- wiringPi

There should be a bash command added to the RaspberryPi startup file that looks something like this: `./path/GardenRun&;python3 path/app.py`
Additionally port 5000 should be opened on the firewall.

Addionally the GPIO pins should be enabled on the RaspberryPi.

The code should be run on an RaspberryPi 3 b+.
