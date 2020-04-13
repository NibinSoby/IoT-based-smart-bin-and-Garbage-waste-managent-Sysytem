#include <stdio.h>     // these are standard c program libraries 
#include <wiringPi.h> // to access gpio pins
#include <wiringPiI2C.h> // to control i2c lines
#include <fstream> // to store updated file
#include <iostream> // input output stream
#include <thread> //
#include <chrono>
//#include <wiringSerial.h>

#define DEVICE_ID 0x53 //  these c program header files are used for the I2C connection of raspberry pi and Accelerometer. 
#define REG_POWER_CTL   0x2D
#define REG_DATA_X_LOW  0x32
#define REG_DATA_X_HIGH 0x33
#define REG_DATA_Y_LOW  0x34
#define REG_DATA_Y_HIGH 0x35
#define REG_DATA_Z_LOW  0x36
#define REG_DATA_Z_HIGH 0x37



const int led = 16;  // these are pin connection of components with pi, such as led, buzzer etc.
const int proximity = 23;
const int buzzer = 12;
uint8_t LastStatus = 1;
uint8_t EmergencyStop= 21;


int main(void) // beginning of main function
{
 
    int fd = wiringPiI2CSetup(DEVICE_ID); // Setup I2C communication
    if (fd == -1) // fd means final data
  {
        std::cout << "Failed to init I2C communication.\n";
        return -1;
      }
    std::cout << "IoT Based Smart Garbage Monitoring Enabled\n";
    wiringPiI2CWriteReg8(fd, REG_POWER_CTL, 0b00001000); // Switch device to measurement mode
 




  wiringPiSetupGpio(); // Initializes wiringPi using GPIO pin
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);  
  pullUpDnControl(proximity,PUD_DOWN);
  pullUpDnControl(EmergencyStop,PUD_DOWN);
 
       for(int i=0; i<2; i++) // for loop to manage low and high of led and buzzer
    {
      digitalWrite(buzzer, HIGH);
      digitalWrite(led, HIGH);
      delay(100); // give delay of 100ms to distinguish Low and High
      digitalWrite(led, LOW);
      digitalWrite(buzzer, LOW);
      delay(100);
    }
   
    while(1)
  {
   
  int dataX = wiringPiI2CReadReg16(fd, REG_DATA_X_LOW); // defining x axis
        dataX = -(~(int16_t)dataX + 1); // converting in to decimal v alue
        int dataY = wiringPiI2CReadReg16(fd, REG_DATA_Y_LOW); //defining Y axis
        dataY = -(~(int16_t)dataY + 1);
        int dataZ = wiringPiI2CReadReg16(fd, REG_DATA_Z_LOW); // defining Z axis
        dataZ = -(~(int16_t)dataZ + 1);
       
std::cout << "\n Data X = "<<dataX; // output the X axis value
delay(300); // delay 
std::cout << "\n Data y = "<<dataY;  //output the Y axis value
delay(300); // delay
std::cout << "\n Data z = "<<dataZ; // output the Z  axis value 
delay(300); // delay

           
  if(dataX>200) // compare the value of x with 200
  {
   std::cout << "\n Data X is more than 200";
    for(int i=0; i<2; i++)
    {
       digitalWrite(led, HIGH);  // write led and buzzer value to high and low
      digitalWrite(buzzer, HIGH);
      delay(250);
      digitalWrite(led, LOW);
      digitalWrite(buzzer, LOW);
      delay(250);
    }
  }
  if(dataY>200)  // compare the value of y with 200
  {
    std::cout << "\n Data Y is more than 200";
    for(int i=0; i<2; i++)
    {
       digitalWrite(led, HIGH);  // write led and buzzer value to high and low
      digitalWrite(buzzer, HIGH);
      delay(250);
      digitalWrite(led, LOW);
      digitalWrite(buzzer, LOW);
      delay(250);
    }
  }
  if(dataZ>200) // compare the value of Z with 200 
  {
    std::cout << "\n Data Z is more than 200";
    for(int i=0; i<2; i++)
    {
       digitalWrite(led, HIGH);  // write led and buzzer value to high and low
      digitalWrite(buzzer, HIGH);
      delay(250);
      digitalWrite(led, LOW);
      digitalWrite(buzzer, LOW);
      delay(250);
    }
  }  
   
 

  if(digitalRead(EmergencyStop))
  {
     break;
  }
     
   }// end of while loop
}
