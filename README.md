# espLightMeter
The circuit uses an ESP8266 wireless module and a IR-transistor to measure the light level. The value 
is then sent to a account on Emoncms.org. The ESP-module is an ESP-01 and is programmed via the Arduino
IDE, the code is in LightMeter.ino.
There's also a script called emoncms_light.py. This script fetches the light value from emoncms.org which then 
is used to decide if it's dark in the room. It will then send a code via codesend and 433Utils on a Raspberry Pi 
to switch on or off a lamp.