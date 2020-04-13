#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
//#include <wiringSerial.h>

#define DEVICE_ID 0x53
#define REG_POWER_CTL   0x2D
#define REG_DATA_X_LOW  0x32
#define REG_DATA_X_HIGH 0x33
#define REG_DATA_Y_LOW  0x34
#define REG_DATA_Y_HIGH 0x35
#define REG_DATA_Z_LOW  0x36
#define REG_DATA_Z_HIGH 0x37


std::ofstream output_file("/var/www/html/index.html");	 //Will overwrite an existing file

void UpdateServer(char Status[7])
{
std::ofstream output_file("/var/www/html/index.html");	 //update html page
output_file << "<!DOCTYPE html> <html> <head> <style> table {   font-family: arial, sans-serif;   border-collapse: collapse;   width: 100%; }  td, th {   border: 1px solid #dddddd;   text-align: left;   padding: 8px; }  tr:nth-child(even) {   background-color: #dddddd; } </style> </head> <body>  <h2>IoT BASED SMART GARBAGE BIN STATUS</h2>  <table>   <tr>     <th>S. No.</th>     <th>Bin Number</th>     <th>Status</th>   </tr>   <tr>     <td>1</td>     <td>234654</td>     <td>";
output_file <<Status;
output_file <<"</td>   </tr>   <tr>     <td>2</td>     <td>574565</td>     <td>Empty</td>   </tr>   <tr>     <td>3</td>     <td>748561</td>     <td>Empty</td>   </tr>   <tr>     <td>4</td>     <td>457912</td>     <td>Empty</td>   </tr>   <tr>     <td>5</td>     <td>354185</td>     <td>Empty</td>   </tr>   <tr>     <td>6</td>     <td>476212</td>     <td>Empty</td>   </tr>     <tr>     <td>7</td>     <td>471698</td>     <td>Empty</td>   </tr>     <tr>     <td>8</td>     <td>984157</td>     <td>Empty</td>   </tr>     <tr>     <td>9</td>     <td>174852</td>     <td>Empty</td>   </tr>     <tr>     <td>10</td>     <td>015876</td>     <td>Empty</td>   </tr> </table>  </body> </html> ";
output_file.close();	
}	


const int led = 16;
const int proximity = 23;
const int servo = 18;
const int WaterSensor = 24;
const int buzzer = 12;
uint8_t LastStatus = 1;
uint8_t EmergencyStop= 21;

char NotFull[] = {'N','o','t','F','u','l','l'};
char Full[] =  {'F','u','l','l',' ',' ',' '};

int main(void)
{
	
    int fd = wiringPiI2CSetup(DEVICE_ID); // Setup I2C communication
    if (fd == -1) 
	{
        std::cout << "Failed to init I2C communication.\n";
        return -1;
    	}
    std::cout << "IoT Based Smart Garbage Monitoring Enabled\n";
    wiringPiI2CWriteReg8(fd, REG_POWER_CTL, 0b00001000); // Switch device to measurement mode
	
	wiringPiSetupGpio();
	pinMode(led, OUTPUT);
	pinMode(buzzer, OUTPUT);	
	pullUpDnControl(proximity,PUD_DOWN);
	pullUpDnControl(EmergencyStop,PUD_DOWN);
	pullUpDnControl(WaterSensor,PUD_UP);
	pinMode (servo, PWM_OUTPUT) ;
  	pwmSetMode (PWM_MODE_MS);
  	pwmSetRange (1024);
   	pwmSetClock (100);
   
    while(1)
	{
		
	int dataX = wiringPiI2CReadReg16(fd, REG_DATA_X_LOW);
        dataX = -(~(int16_t)dataX + 1);
        int dataY = wiringPiI2CReadReg16(fd, REG_DATA_Y_LOW);
        dataY = -(~(int16_t)dataY + 1);
        int dataZ = wiringPiI2CReadReg16(fd, REG_DATA_Z_LOW);
        dataZ = -(~(int16_t)dataZ + 1);
        

	if(digitalRead(proximity) && LastStatus==0)
	{
		UpdateServer(Full);	
		digitalWrite(led, HIGH);
	        LastStatus = 1;
	}
		
	
    	else if(!digitalRead(proximity) && LastStatus==1)
	{
	     UpdateServer(NotFull);
	     digitalWrite(led, LOW);
	     LastStatus = 0;
        }
           
	if(dataY>240)
	{
		for(int i=0; i<2; i++)
		{
		  digitalWrite(buzzer, HIGH);
		  delay(250);
		  digitalWrite(buzzer, LOW);
		  delay(250);
		}
	}
		
	if(dataZ>240)
	{
		for(int i=0; i<2; i++)
		{
		  digitalWrite(buzzer, HIGH);
		  delay(250);
		  digitalWrite(buzzer, LOW);
		  delay(250);
		}
	}
	if(dataX>240)
	{
		for(int i=0; i<2; i++)
		{
		  digitalWrite(buzzer, HIGH);
		  delay(250);
		  digitalWrite(buzzer, LOW);
		  delay(250);
		}
	}		
		
	if(!digitalRead(WaterSensor))
	{
	   pwmWrite(servo,100);
           delay(2500);
           pwmWrite(servo,600);
   	   delay(1000);
	}

	if(digitalRead(EmergencyStop))
	{
	   break;
	}
	    
   }// end of while loop
}
