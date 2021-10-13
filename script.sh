#!/bin/bash
#sudo chmod -v 777 /dev/ttyACM0
bazel run app:_mainUpload --platforms @AvrToolchain//platforms:ArduinoUno
#bazel run app:main_upload --platforms @AvrToolchain//platforms:ArduinoUno