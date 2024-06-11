# DataPulse
Connecting to a microcontroller and recording its data through Bluetooth

The Arduino microcontroller must support BLE (Bluetooth Low Energy), otherwise the arduino script would not work. Adjust the sensor reading methods as it suits your application.

Note: The python script will overwrite the existing csv file if ran more than once in a day. Increase or decrease the sleep duration to change the increments between each data collection.
