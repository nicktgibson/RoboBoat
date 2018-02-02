/*
 Example sketch for the Xbox Wireless Reciver library - developed by Kristian Lauszus
 It supports up to four controllers wirelessly
 For more information see the blog post: http://blog.tkjelectronics.dk/2012/12/xbox-360-receiver-added-to-the-usb-host-library/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <XBOXRECV.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
XBOXRECV Xbox(&Usb);


const byte numChars = 64;
char receivedChars[numChars];

boolean error=0;
boolean newData = false;
boolean debug = 1;

const int lowerPWM=100;

int motorOneOutputDesiredOld = 0;
int motorTwoOutputDesiredOld = 0;
int motorThreeOutputDesiredOld = 0;
int motorFourOutputDesiredOld = 0;


void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nXbox Wireless Receiver Library Started"));
}






void loop() {
  recvWithXbox();
  useData();
  
}

// XBOX CONTROLLER STUFF
void recvWithXbox() {
  int leftInput; int rightInput;
  Usb.Task();
  if (Xbox.XboxReceiverConnected && newData== false) {
    for (uint8_t i = 0; i < 4; i++) {
      if (Xbox.Xbox360Connected[i]) {
        if (Xbox.getButtonPress(L2, i) || Xbox.getButtonPress(R2, i)) {
          Serial.print("L2: ");
          Serial.print(Xbox.getButtonPress(L2, i));
          Serial.print("\tR2: ");
          Serial.println(Xbox.getButtonPress(R2, i));
          Xbox.setRumbleOn(Xbox.getButtonPress(L2, i), Xbox.getButtonPress(R2, i), i);
        }

        if (Xbox.getAnalogHat(LeftHatY, i) > 7500 || Xbox.getAnalogHat(LeftHatY, i) < -7500 || Xbox.getAnalogHat(RightHatY, i) > 7500 || Xbox.getAnalogHat(RightHatY, i) < -7500) {
          leftInput = Xbox.getAnalogHat(LeftHatY, i)* 0.00778;
          rightInput = Xbox.getAnalogHat(RightHatY, i)* 0.00778;
          if ((Xbox.getAnalogHat(LeftHatY, i) > 7500 || Xbox.getAnalogHat(LeftHatY, i) < -7500) || (Xbox.getAnalogHat(RightHatY, i) > 7500 || Xbox.getAnalogHat(RightHatY, i) < -7500)) {
            //Motors 1 & 2
            if (leftInput > -1)
            receivedChars[0] = '+';
            else
            receivedChars[0] = '-';
            
            receivedChars[1] = (abs(leftInput)/100)+48;    
            receivedChars[2] = (abs(leftInput)%100/10)+48;    
            receivedChars[3] = (abs(leftInput)%100%10)+48; 
            // Duplicating the first motor to the second motor so that both left motors act the same.   
            receivedChars[4] = receivedChars[0];        
            receivedChars[5] = receivedChars[1];
            receivedChars[6] = receivedChars[2];
            receivedChars[7] = receivedChars[3];

            //Motors 3 & 4
            if (rightInput > -1)
            receivedChars[8] = '+';
            else
            receivedChars[8] = '-';
            
            receivedChars[9] = (abs(rightInput)/100)+48;    
            receivedChars[10] = (abs(rightInput)%100/10)+48;    
            receivedChars[11] = (abs(rightInput)%100%10)+48; 
            // Duplicating the third motor to the fourth motor so that both left motors act the same.   
            receivedChars[12] = receivedChars[8];        
            receivedChars[13] = receivedChars[9];
            receivedChars[14] = receivedChars[10];
            receivedChars[15] = receivedChars[11];
            newData = true;
            }
            if (debug == 1)   
          Serial.println(receivedChars);
        }

        if (Xbox.getButtonClick(UP, i)) {
          Xbox.setLedOn(LED1, i);
          Serial.println(F("Up"));
        }
        if (Xbox.getButtonClick(DOWN, i)) {
          Xbox.setLedOn(LED4, i);
          Serial.println(F("Down"));
        }
        if (Xbox.getButtonClick(LEFT, i)) {
          Xbox.setLedOn(LED3, i);
          Serial.println(F("Left"));
        }
        if (Xbox.getButtonClick(RIGHT, i)) {
          Xbox.setLedOn(LED2, i);
          Serial.println(F("Right"));
        }

        if (Xbox.getButtonClick(START, i)) {
          Xbox.setLedMode(ALTERNATING, i);
          Serial.println(F("Start"));
        }
        if (Xbox.getButtonClick(BACK, i)) {
          Xbox.setLedBlink(ALL, i);
          Serial.println(F("Back"));
        }
        if (Xbox.getButtonClick(L3, i))
          Serial.println(F("L3"));
        if (Xbox.getButtonClick(R3, i))
          Serial.println(F("R3"));

        if (Xbox.getButtonClick(L1, i))
          Serial.println(F("L1"));
        if (Xbox.getButtonClick(R1, i))
          Serial.println(F("R1"));
        if (Xbox.getButtonClick(XBOX, i)) {
          Xbox.setLedMode(ROTATING, i);
          Serial.print(F("Xbox (Battery: "));
          Serial.print(Xbox.getBatteryLevel(i)); // The battery level in the range 0-3
          Serial.println(F(")"));
        }
        if (Xbox.getButtonClick(SYNC, i)) {
          Serial.println(F("Sync"));
          Xbox.disconnect(i);
        }

        if (Xbox.getButtonClick(A, i))
          Serial.println(F("A"));
        if (Xbox.getButtonClick(B, i))
          Serial.println(F("B"));
        if (Xbox.getButtonClick(X, i))
          Serial.println(F("X"));
        if (Xbox.getButtonClick(Y, i))
          Serial.println(F("Y"));
      }
    }
  }
}

// ALL OF THIS IS MOTOR DRIVER STUFF  - DO NOT EDIT
void useData() {
  int lenFromEnd=16; //Takes only the last +XXX+XXX+XXX+XXX
   //Change +255 to -255 to 510 values, apply a low pass filter, and set a warning flag if it tries to switch too much,
    if (newData == true) {        
        int len = strlen(receivedChars);
        if (len>15){ //Because we take the last 16, having less than 15 causes issues.
        //Serial.println(&receivedChars[len - lenFromEnd]); //This displays the whole motor driver command
        
        //interpreting the data into indivual bits.
        
        // motor one input
        char direction1 = receivedChars[0];
        int motorOneInput = ((receivedChars[1]-48)*100)+((receivedChars[2]-48)*10)+((receivedChars[3]-48)*1); //What the arduino recieves as the desired PWM
        if (abs(motorOneInput)>255)
        motorOneInput = 255;
        if (abs(motorOneInput)<lowerPWM)
        motorOneInput=0;
        // motor one output
        int motorOneOutputDesired=motorOneInput; 
        if (direction1=='-')
          motorOneOutputDesired=(-1*motorOneInput);   
        // motor two input
        char direction2 = receivedChars[4];
        int motorTwoInput = ((receivedChars[5]-48)*100)+((receivedChars[6]-48)*10)+((receivedChars[7]-48)*1);
        if (abs(motorTwoInput)>255)
        motorTwoInput = 255;
        if (abs(motorTwoInput)<lowerPWM)
        motorTwoInput=0;
        // motor two output
        int motorTwoOutputDesired=motorTwoInput; 
        if (direction2=='-')
          motorTwoOutputDesired=(-1*motorTwoInput);
        // motor three input
        char direction3 = receivedChars[8];
        int motorThreeInput = ((receivedChars[9]-48)*100)+((receivedChars[10]-48)*10)+((receivedChars[11]-48)*1);
        if (abs(motorThreeInput)>255)
        motorThreeInput=255;
        if (abs(motorThreeInput)<lowerPWM)
        motorThreeInput=0;
        // motor three output
        int motorThreeOutputDesired = motorThreeInput; 
        if (direction1=='-')
          motorThreeOutputDesired = (-1*motorThreeInput);
        // motor four input
        char direction4 = receivedChars[12];
        int motorFourInput = ((receivedChars[13]-48)*100)+((receivedChars[14]-48)*10)+((receivedChars[15]-48)*1);
        if (abs(motorFourInput)>255)
        motorFourInput=255;
        if (abs(motorFourInput)<lowerPWM)
        motorFourInput=0;
        //motor four output
        int motorFourOutputDesired = motorFourInput; 
        if (direction1=='-')
          motorFourOutputDesired = (-1*motorFourInput);



        // low pass filters

         motorOneOutputDesired = ((6*motorOneOutputDesiredOld)+(4*motorOneOutputDesired))/10;
         motorTwoOutputDesired = ((6*motorTwoOutputDesiredOld)+(4*motorTwoOutputDesired))/10;
         motorThreeOutputDesired = ((6*motorTwoOutputDesiredOld)+(4*motorThreeOutputDesired))/10;
         motorFourOutputDesired = ((6*motorTwoOutputDesiredOld)+(4*motorFourOutputDesired))/10;

         motorOneOutputDesiredOld = motorOneOutputDesired;
         motorTwoOutputDesiredOld = motorTwoOutputDesired;
         motorThreeOutputDesiredOld = motorThreeOutputDesired;
         motorFourOutputDesiredOld = motorFourOutputDesired;

         int motorOneOutput=motorOneOutputDesired;
         int motorTwoOutput=motorTwoOutputDesired;
         int motorThreeOutput=motorThreeOutputDesired;
         int motorFourOutput=motorFourOutputDesired;

        if (abs(motorOneOutput)>255) //This section is important because after the low pass filter you can go back under the low PWM limit (ie send it +200 and -190)
        motorOneOutput = 255;        //This can burn out the motor or the motor driver because you're dumping current because the motor is under its starting limit.
        if (abs(motorOneOutput)<lowerPWM)
        motorOneOutput=0;

        if (abs(motorTwoOutput)>255)
        motorTwoOutput = 255;
        if (abs(motorTwoOutput)<lowerPWM)
        motorTwoOutput=0;

        if (abs(motorThreeOutput)>255)
        motorThreeOutput =255;
        if (abs(motorThreeOutput)<lowerPWM)
        motorThreeOutput=0;

        if (abs(motorFourOutput)>255)
        motorFourOutput=255;
        if (abs(motorFourOutput)<lowerPWM)
        motorFourOutput=0;
        

        //debug to print the outputs

        if (debug == 1){
          Serial.print("Motor one is going ");
          Serial.print(motorOneOutput);
          Serial.print(", desired is");
          Serial.println(motorOneOutputDesired);
          
          Serial.print("Motor two is going ");
          Serial.print(motorTwoOutput);
          Serial.print(", desired is");
          Serial.println(motorTwoOutputDesired);
          
          Serial.print("Motor three is going ");
          Serial.print(motorThreeOutput);
          Serial.print(", desired is");
          Serial.println(motorThreeOutputDesired);

          Serial.print("Motor four is going ");
          Serial.print(motorFourOutput);
          Serial.print(", desired is");
          Serial.println(motorTwoOutputDesired);

          Serial.println("--------------------------------------");
          
        }

        // Actually output signals to the motors
           // Driver 1, motor 1
        if (direction1=='+'){
        digitalWrite(23,HIGH); //Enable
        digitalWrite(25,HIGH); //M1In2
        digitalWrite(27,LOW); //M1In1
        analogWrite(5,abs(motorOneOutput)); //Speed control
        }
        else if (direction1=='-'){
         digitalWrite(23,HIGH); //Enable
         digitalWrite(25,LOW); //M1In2
         digitalWrite(27,HIGH); //M1In1
         analogWrite(5,abs(motorOneOutput)); //Speed control
        }
        else {
         digitalWrite(23,HIGH); //Enable
         digitalWrite(25,LOW); //M1In2
         digitalWrite(27,LOW); //M1In1
         analogWrite(5,0); //Speed control
        
        }
        
        // Driver 1, motor 2
        if (direction2=='+'){
        digitalWrite(22,HIGH); //Enable
        digitalWrite(24,HIGH); //M1In2
        digitalWrite(26,LOW); //M1In1
        analogWrite(6,abs(motorTwoOutput)); //Speed control
        }
        else if (direction2=='-'){
         digitalWrite(22,HIGH); //Enable
         digitalWrite(24,LOW); //M1In2
         digitalWrite(26,HIGH); //M1In1
         analogWrite(6,abs(motorTwoOutput)); //Speed control
        }
        else {
         digitalWrite(22,HIGH); //Enable
         digitalWrite(24,LOW); //M1In2
         digitalWrite(26,LOW); //M1In1
         analogWrite(6,0); //Speed control
        
        }

        // Driver 2, motor 1
        if (direction3=='+'){
        digitalWrite(53,HIGH); //Enable
        digitalWrite(51,HIGH); //M1In2
        digitalWrite(49,LOW); //M1In1
        analogWrite(9,abs(motorThreeOutput)); //Speed control
        }
        else if (direction3=='-'){
         digitalWrite(53,HIGH); //Enable
         digitalWrite(51,LOW); //M1In2
         digitalWrite(49,HIGH); //M1In1
         analogWrite(9,abs(motorThreeOutput)); //Speed control
        }
        else {
         digitalWrite(53,HIGH); //Enable
         digitalWrite(51,LOW); //M1In2
         digitalWrite(49,LOW); //M1In1
         analogWrite(9,0); //Speed control
        
        }

        // Driver 2, motor 2
        if (direction4=='+'){
        digitalWrite(52,HIGH); //Enable
        digitalWrite(50,HIGH); //M1In2
        digitalWrite(48,LOW); //M1In1
        analogWrite(10,abs(motorFourOutput)); //Speed control
        }
        else if (direction4=='-'){
         digitalWrite(52,HIGH); //Enable
         digitalWrite(50,LOW); //M1In2
         digitalWrite(48,HIGH); //M1In1
         analogWrite(10,abs(motorFourOutput)); //Speed control
        }
        else {
         digitalWrite(52,HIGH); //Enable
         digitalWrite(50,LOW); //M1In2
         digitalWrite(48,LOW); //M1In1
         analogWrite(10,0); //Speed control
        
        }


        
    }
    else{
      Serial.println("The command is too short");
      error=true;
    }
    }
       newData = false;
       Serial.flush();
}
