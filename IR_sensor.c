#include<stdio.h>
#include <wiringPi.h>     // to access gpio pins
#include <wiringPiI2C.h>   // to control I2c lines
#include <fstream>        // to store updated file
#include <iostream>      // basic input output
#include <thread> 
#include <chrono>       

const int led = 16;       // pin number of led
const int proximity = 23; // pin number for proximity
const int buzzer = 12;
uint8_t LastStatus = 1;
uint8_t EmergencyStop= 21;// to stop in case of error


int main(void)
{
  
  
  wiringPiSetupGpio();    // function to set gpio 
  pinMode(led, OUTPUT);   // led pin as output
  pinMode(buzzer, OUTPUT);  // set as output
  pullUpDnControl(proximity,PUD_DOWN); // as sensor gives high signal when it sense any surface ,when sensor is in off state some times it disconnected . so to avoid the floating state of pin and to stabilized functionality of the input pin for proximity sensor. We set to pull down internally using this pullupdncontrol() function. This sets input pin to ground via 10k resistor if we write PUD_DOWN in the argument. So pin always get ground signal until high signal comes from the sensor .
  pullUpDnControl(EmergencyStop,PUD_DOWN);
   
       for(int i=0; i<2; i++)
    {
      digitalWrite(buzzer, HIGH); // sets output pin to +5v
      digitalWrite(led, HIGH);
      delay(100);                //Set a time delay before going off
      digitalWrite(led, LOW);
      digitalWrite(buzzer, LOW);
      delay(100);
    } 
   
    while(1)
  {
    
  if(digitalRead(proximity) && LastStatus==0) // && this " AND " operator and reads the and value from input

 
  {
    
    digitalWrite(led, HIGH);
    digitalWrite(buzzer, HIGH);
          LastStatus = 1;
  }
    
  
      else if(!digitalRead(proximity) && LastStatus==1) // digitalRead() = reads the value from input pin
  {
       
       digitalWrite(led, LOW);
        digitalWrite(buzzer, LOW);
       LastStatus = 0;
        }
           
   
  if(digitalRead(EmergencyStop))
  {
     break;
  }
      
   }// end of while loop
}
