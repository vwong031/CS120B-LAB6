# CS120B-LAB6

## Wiring Diagram
![image](https://user-images.githubusercontent.com/74090811/206876158-9556f763-c6f8-4618-822c-d59ecc54458e.png)

## Exercise 1:
In this system, you are going to make a mock-up for an automatic ice maker. 

The system has 2 inputs and 5 outputs. We will use the two photoresistors to simulate sensors and three of the five LEDs to simulate pumps. The pumps require different pulse width modulation (PWM) signals to operate. 

The system inputs are as follows:
* Photoresistor A0 simulates a water level sensor
* Photoresistor A1 simulates an ice level sensor

The system outputs are as follows:
* Yellow LED (pin 2) is lit up to indicate that the ice tray is full
* Blue LED (pin 3) simulates a water pump
* Red LED (pin 4) is lit up to indicate that the water level is too low to make ice
* White LED (pin 5) simulates a liquid nitrogen pump
* Green LED (pin 6) simulates the ice dispenser

The system samples the two photoresistors at different rates
* The water level sensor (the left photoresistor connected to A0) is sampled every 50 ms to check if there is enough water to make ice 
  * If this photoresistor is covered, the system pauses and the red LED (pin 4) turns on
  * The red LED (pin 4) turns off whenever the photoresistor is uncovered
* The ice level sensor (the right photoresistor connected to A1) is sampled every 200 ms to check if the ice tray is full
  * If this photoresistor is covered, the system pauses and the yellow LED (pin 2) turns on
  * The yellow LED (pin 2) turns off whenever the photoresistor is uncovered

The system is initially Off
* The user turns the system On/Off by simultaneously covering both photoresistors
* The system cycles through the following three steps infinitely until one or both of the sensors is tripped (i.e., either photoresistor is covered). The system then resumes when both sensors are cleared (i.e., both photoresistors are uncovered)
* Place your finger on top of the photoresistor to trip the sensor
* Remove your finger from the photoresistor to clear the sensor
 

1) The system initializes by turning on the water pump for 3 seconds to move water to the cooling station
    * The water pump is the blue LED on pin 3
    * The water pump uses a PWM signal with a duty cycle of 20% and a period of 300 ms
2) The system turns off the water pump and turns on the liquid nitrogen pump for 5 seconds
    * The liquid nitrogen pump is the white LED on pin 5
    * The liquid nitrogen pump uses a PWM signal with a duty cycle of 70% and a period of 500 ms
3) The system turns off the water pump and turns on the dispenser for 2 seconds to move the newly formed ice cubes into the holding tray
    * The dispenser is the green LED on pin 6
    * The dispenser motor uses a PWM signal with a duty cycle of 10% and a period of 100 ms
    
Demo Link: https://youtu.be/i3nY3y5vkJs 
