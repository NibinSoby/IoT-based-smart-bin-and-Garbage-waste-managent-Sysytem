#include <stdio.h> // these are standard c program libraries
#include <wiringPi.h> //to access gpio pins
#include <wiringPiI2C.h> // to control i2c lines
#include <fstream> //  to store updated file
#include <iostream> // input output stream
#include <thread> 
#include <chrono>


const int led = 16; // these are pin connection of components with pi, such as led, buzzer, water sensor etc.
const int servo = 18;
const int WaterSensor = 24;
const int buzzer = 12;
uint8_t LastStatus = 1;



int main(void) // main program start here
{
	
	wiringPiSetupGpio(); // Setup I2C communication
	pinMode(led, OUTPUT); // 
	pinMode(buzzer, OUTPUT); 	
	pullUpDnControl(WaterSensor,PUD_UP);
  	pinMode (servo, PWM_OUTPUT) ;
  	pwmSetMode (PWM_MODE_MS);
  	pwmSetRange (1024); // set the range in pwm generator
   	pwmSetClock (100); // set the divisor for pwm clock.  servos are controlled by sending an electrical pulse of variable width
	     for(int i=0; i<2; i++) // for loop for changing between two position
		{
		  digitalWrite(buzzer, HIGH);
		  digitalWrite(led, HIGH);
		  delay(100);  // delay between low and high
		  digitalWrite(led, LOW);
		  digitalWrite(buzzer, LOW);
		  delay(100);
		} 
	while (1) // infinite loop which will run until break statement
	{
	if(!digitalRead(WaterSensor)) // if water sensor read
	{
	   pwmWrite(servo,100); // servo pwm 100
           delay(2500); // after this delay servo shaft will go to the original location
           pwmWrite(servo,600);
   	   delay(1000); // again delay
	}

	
	
	   break; // break statement end the loop.
	
}    // end of while loop
}
