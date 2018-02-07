/*
 * Nick Hussey (NKH15@my.fsu.edu)
 * 2017-2018 Roboboat
 * 1/10/18
 */

 /*      vvvvv This is the desired input from serial to control the motors, just copy and paste it into the serial monitor (ctrl+shift+m)
  *      <+111+222+333+444>
  */
// How to control the motor https://www.robotshop.com/letsmakerobots/files/userpics/u19659/20131105_-_KoBot25.jpg from https://www.robotshop.com/letsmakerobots/dual-mc33926-motor-driver-control-pins  

  const byte numChars = 64;
  char receivedChars[numChars];
  boolean error=0;
  boolean newData = false;
  boolean debug = 0;
  const int lowerPWM=100;
  
  int motorOneOutputDesiredOld = 0;
  int motorTwoOutputDesiredOld = 0;
  int motorThreeOutputDesiredOld = 0;
  int motorFourOutputDesiredOld = 0;

//Pins

void setup() {
    Serial.begin(9600);
    Serial.println("<FAMU-FSU Robo-boat Motor Driver is ready>");
    Serial.println("Version 1.2. Fixed the low pass filter.");
    if (debug == 1){
    Serial.print("The lowest PWM that motors will activate at is: ");  
    Serial.println(lowerPWM);
    Serial.println("We need to go back and see what the new lowest PWM that causes motion in the water is.");
    }
    Serial.println("--------------------------------------");
     pinMode(23, OUTPUT);  //Driver 1 M1 Enable        
     pinMode(25, OUTPUT);  //Driver 1 M1In2        
     pinMode(27, OUTPUT);  //Driver 1 M1In1        
     pinMode(5, OUTPUT);  //Driver 1 M1 PWM
     
     pinMode(22, OUTPUT);  //Driver 1 M2 Enable **Important note**: On the motor driver we use both M1 and M2 are enabled by 1 pin, this is currently unused.
     pinMode(24, OUTPUT);  //Driver 1 M2 In2
     pinMode(26, OUTPUT);  //Driver 1 M2In1
     pinMode(6, OUTPUT);  //Driver 1 M2PWM

     pinMode(53, OUTPUT);  //Driver 2 M1 Enable  
     pinMode(51, OUTPUT);  //Driver 2 M2 In2
     pinMode(49, OUTPUT);  //Driver 2 M2In1
     pinMode(9, OUTPUT);  //Driver 2 M2PWM

     pinMode(52, OUTPUT);  //Driver 2 M2 Enable **Important note**: On the motor driver we use both M1 and M2 are enabled by 1 pin, this is currently unused.
     pinMode(50, OUTPUT);  //Driver 2 M2 In2
     pinMode(48, OUTPUT);  //Driver 2 M2In1
     pinMode(10, OUTPUT);  //Driver 2 M2PWM
     
     digitalWrite(23,LOW);  //Set all the pins to low on startup
     digitalWrite(25,LOW); 
     digitalWrite(27,LOW); 
     digitalWrite(5,LOW); 

     digitalWrite(22,LOW); 
     digitalWrite(24,LOW); 
     digitalWrite(26,LOW); 
     digitalWrite(6,LOW);

     digitalWrite(53,LOW); 
     digitalWrite(51,LOW); 
     digitalWrite(49,LOW); 
     digitalWrite(9,LOW);

     digitalWrite(52,LOW); 
     digitalWrite(50,LOW); 
     digitalWrite(48,LOW); 
     digitalWrite(10,LOW);
     
    
}

void loop() {
  // Serial Communication help from https://forum.arduino.cc/index.php?topic=288234.0
    recvWithStartEndMarkers();
    useData();
    
}


void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;
 
 // if (Serial.available() > 0) {
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void useData() {
  int lenFromEnd=16; //Takes only the last +XXX+XXX+XXX+XXX
   //Change +255 to -255 to 510 values, apply a low pass filter, and set a warning flag if it tries to switch too much,
    if (newData == true) {        
        int len = strlen(receivedChars);
        if (debug == 1){
          Serial.println(receivedChars);
        }
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
        if (direction3=='-')
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
        if (direction4=='-')
          motorFourOutputDesired = (-1*motorFourInput);



        // low pass filters

         motorOneOutputDesired = ((6*motorOneOutputDesiredOld)+(4*motorOneOutputDesired))/10;
         motorTwoOutputDesired = ((6*motorTwoOutputDesiredOld)+(4*motorTwoOutputDesired))/10;
         motorThreeOutputDesired = ((6*motorThreeOutputDesiredOld)+(4*motorThreeOutputDesired))/10;
         motorFourOutputDesired = ((6*motorFourOutputDesiredOld)+(4*motorFourOutputDesired))/10;

         motorOneOutputDesiredOld = motorOneOutputDesired;
         motorTwoOutputDesiredOld = motorTwoOutputDesired;
         motorThreeOutputDesiredOld = motorThreeOutputDesired;
         motorFourOutputDesiredOld = motorFourOutputDesired;

         int motorOneOutput=motorOneOutputDesired;
         int motorTwoOutput=motorTwoOutputDesired;
         int motorThreeOutput=motorThreeOutputDesired;
         int motorFourOutput=motorFourOutputDesired;

         //set the 255 and lowerPWM limits again

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
          Serial.println(motorFourOutputDesired);

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
        digitalWrite(24,HIGH); //M2In2
        digitalWrite(26,LOW); //M2In1
        analogWrite(6,abs(motorTwoOutput)); //Speed control
        }
        else if (direction2=='-'){
         digitalWrite(22,HIGH); //Enable
         digitalWrite(24,LOW); //M2In2
         digitalWrite(26,HIGH); //M2In1
         analogWrite(6,abs(motorTwoOutput)); //Speed control
        }
        else {
         digitalWrite(22,HIGH); //Enable
         digitalWrite(24,LOW); //M12n2
         digitalWrite(26,LOW); //M12n1
         analogWrite(6,0); //Speed control
        
        }

        // Driver 2, motor 1
        if (direction3=='+'){
        digitalWrite(53,HIGH); //Enable
        digitalWrite(51,HIGH); //D2 M1In2
        digitalWrite(49,LOW); //D2 M1In1
        analogWrite(9,abs(motorThreeOutput)); //Speed control
        }
        else if (direction3=='-'){
         digitalWrite(53,HIGH); //Enable
         digitalWrite(51,LOW); //D2 M1In2
         digitalWrite(49,HIGH); //D2 M1In1
         analogWrite(9,abs(motorThreeOutput)); //Speed control
        }
        else {
         digitalWrite(53,HIGH); //Enable
         digitalWrite(51,LOW); //D2 M1In2
         digitalWrite(49,LOW); //D2 M1In1
         analogWrite(9,0); //Speed control
        
        }

        // Driver 2, motor 2
        if (direction4=='+'){
        digitalWrite(52,HIGH); //Enable
        digitalWrite(50,HIGH); //D2 M1In2
        digitalWrite(48,LOW); //D2 M1In1
        analogWrite(10,abs(motorFourOutput)); //Speed control
        }
        else if (direction4=='-'){
         digitalWrite(52,HIGH); //Enable
         digitalWrite(50,LOW); //D2 M1In2
         digitalWrite(48,HIGH); //D2 M1In1
         analogWrite(10,abs(motorFourOutput)); //Speed control
        }
        else {
         digitalWrite(52,HIGH); //Enable
         digitalWrite(50,LOW); //D2 M1In2
         digitalWrite(48,LOW); //D2 M1In1
         analogWrite(10,0); //Speed control
        
        }


        
    } //if >15 character long
    else if ((receivedChars[0]=='z') || (receivedChars[0]=='Z') || (receivedChars[0]=='?' )) { //stop command
      digitalWrite(23,HIGH); //Enable
      digitalWrite(25,LOW); //M1In2
      digitalWrite(27,LOW); //M1In1
      analogWrite(5,0); //Speed control

      digitalWrite(22,HIGH); //Enable
      digitalWrite(24,LOW); //M22n2
      digitalWrite(26,LOW); //M22n1
      analogWrite(6,0); //Speed control

      digitalWrite(53,HIGH); //Enable
      digitalWrite(51,LOW); //D2 M1In2
      digitalWrite(49,LOW); //D2 M1In1
      analogWrite(9,0); //Speed control

      digitalWrite(52,HIGH); //Enable
      digitalWrite(50,LOW); //D2 M1In2
      digitalWrite(48,LOW); //D2 M1In1
      analogWrite(10,0); //Speed control
    } //end of stop command

    
    else{ //less than 15 char and unknown command
      Serial.println("The command is too short");
      error=true;
    }
    newData = false;
    }
}

