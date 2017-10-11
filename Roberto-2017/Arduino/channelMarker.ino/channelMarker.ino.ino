
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



const int bufferSize=256;
char dataBuffer[bufferSize] = "#"; //hash mark needed or else you have to reset arduino each run
char cmdBuffer[bufferSize];
const int PWM_MAX = 255;
const int FRAME_CENTER = 320;
const float PIX2PWM = 0.78125;
int intData = 0;
int delta;
int motorPWM = 0;
bool isAutonomous = true;
bool isRunning;
int motorL_PWM=0;
int motorR_PWM=0;


void setup() {


 Serial.begin(9600);  //so far runs best at 9600, 19200 allows gaps in PWM frequency
 pinMode(2,OUTPUT);
 pinMode(3,OUTPUT);
 pinMode(4,OUTPUT);
 pinMode(6,OUTPUT);
 pinMode(7,OUTPUT);
 pinMode(8,OUTPUT);
 

}



void loop() 
{

  if (isAutonomous)
    autonomousMode();
  else
    manualMode();
   
}


void autonomousMode()
{

  //Serial.println("autonomous");

    if (Serial.available()>0) ///if data is available
     { 
        while(Serial.available()>0)  //while data is available
         {
            Serial.readBytesUntil('x',dataBuffer, bufferSize); //BEST method so far
          } //endwhile
     } //endif
    
       
Serial.println(dataBuffer);
    
    while(!Serial.available()>0){} //needed for some weird reason
        
     intData = atoi(dataBuffer);  //thse 3 statements must be together
     motorPWM = (PWM_MAX-abs(intData-FRAME_CENTER)*PIX2PWM);  
     intData = (intData-FRAME_CENTER);

     
      if(intData==(-1-FRAME_CENTER)) //or (String)dataBuffer=="-1", 
      {
         //if no buoys are in camera FOV, spin the boat until both are visible
         intData=0;
         motorPWM = PWM_MAX;
         Serial.println("SEARCHING FOR BUOYS");
         digitalWrite(4,LOW);
         digitalWrite(2,HIGH);
         analogWrite(3,150);   //150 @ 4.5V just catches the buoys
         digitalWrite(7,HIGH);
         digitalWrite(8,LOW);
         analogWrite(6,150);   //150 @ 4.5V just catches the buoys
      }
      else if(dataBuffer[0]=='#')
      {
          //if the boat recieves the #, cut the power to the motors
          Serial.println("POWER OFF");
          digitalWrite(4,LOW);
          digitalWrite(2,LOW);
          analogWrite(3,0);
          digitalWrite(7,LOW);
          digitalWrite(8,LOW);
          analogWrite(6,0);
      }
      else if(dataBuffer[0]=='M')
      {
          //if the boat receives the M, enter the manual contorl mode
          isAutonomous = false;
          Serial.println("Manual Mode");
      }
      else if (intData>0) //right motor slows down
      {
          //if the channel center is greater than 320px, left fullPWM, right proportionalPWM
          Serial.print("RIGHT: ");
          Serial.print("\t");
          Serial.print(motorPWM);
          Serial.println();
          digitalWrite(4,HIGH);
          digitalWrite(2,LOW);
          analogWrite(3,PWM_MAX);
          digitalWrite(7,HIGH);
          digitalWrite(8,LOW);
          analogWrite(6,motorPWM);
       }
       else if(intData<0) //left motor slows down
       {
          //if the channel center is less than 320px, left proportionalPWM, right fullPWM 
          Serial.print("LEFT: ");
          Serial.print("\t");
          Serial.print(motorPWM);
          Serial.println();
          digitalWrite(4,HIGH);
          digitalWrite(2,LOW);
          analogWrite(3,motorPWM);
          digitalWrite(7,HIGH);
          digitalWrite(8,LOW);
          analogWrite(6,PWM_MAX);
         }
       else if(intData==0)
       {

          //if the channel center is 320px, both motors fullPWM
          Serial.print("FULL POWER!");
          digitalWrite(4,HIGH);
          digitalWrite(2,LOW);
          analogWrite(3,PWM_MAX);
          digitalWrite(7,HIGH);
          digitalWrite(8,LOW);
          analogWrite(6,PWM_MAX);
        }

     //do we really need these?
    Serial.flush(); //improves accuracy response
    memset(dataBuffer, '\0', sizeof(dataBuffer)); ///clear buffers so strange value dont occur durng testing
    delay(5);  //need this so arduino can catch up to zotac
}


//////////////////// MANUAL ///////////////////////////////////////


void manualMode()
{
 Serial.println("manual");

  int delim; 
  String data;
  int dataLength;
  
  if (Serial.available()>0) ///if data is available
      { 
         while(Serial.available()>0)  //while data is available
          {
             
              Serial.readBytesUntil('x',dataBuffer, bufferSize); //BEST method so far is to use a delimmeter
            
            } //endwhile
        
      } //endif


    
data = dataBuffer;
dataLength = data.length();
delim = data.indexOf(',');


//having the conversions out of the loop help stop delays between PWM setting
motorL_PWM = atoi(data.substring(0,delim).c_str());
motorR_PWM = atoi(data.substring(delim+1, dataLength).c_str());

    if(data[0]=='A')
    {
   
        isAutonomous=true;
        Serial.println("Autonomous Mode");
   
    }
    else if(dataBuffer[0]=='R')
    {
        motorL_PWM = atoi(data.substring(1,delim).c_str());
        motorR_PWM = atoi(data.substring(delim+1, dataLength).c_str());
        digitalWrite(4,LOW);
        digitalWrite(2,HIGH);
        analogWrite(3,motorL_PWM);
        digitalWrite(7,LOW);
        digitalWrite(8,HIGH);
        analogWrite(6,motorR_PWM);
        Serial.print("R"); Serial.print(motorL_PWM); Serial.print("   "); Serial.print(motorR_PWM); Serial.println();
  
    }
    else
    {
  
        digitalWrite(4,HIGH);
        digitalWrite(2,LOW);
        analogWrite(3,motorL_PWM);
        digitalWrite(7,HIGH);
        digitalWrite(8,LOW);
        analogWrite(6,motorR_PWM);
        Serial.print(motorL_PWM); Serial.print("   "); Serial.print(motorR_PWM); Serial.println(); 
    }

          
    //do we really need these?      
    Serial.flush(); //improves accuracy response
    memset(dataBuffer, '\0', sizeof(dataBuffer)); ///clear buffer so strange values dont occur during tesitng
    delay(5); //need this so arduino can catchup to zotac
}



