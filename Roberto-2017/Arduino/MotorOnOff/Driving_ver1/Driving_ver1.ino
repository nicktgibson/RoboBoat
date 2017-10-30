void setup() {
  // put your setup code here, to run once:

char ch[16];

pinMode(2, OUTPUT);
pinMode(4, OUTPUT);
pinMode(3, OUTPUT);
pinMode(6, OUTPUT);
pinMode(5, OUTPUT);
pinMode(7, OUTPUT);
pinMode(9, OUTPUT);
pinMode(8, OUTPUT);
pinMode(10, OUTPUT);
pinMode(12, OUTPUT);
pinMode(11, OUTPUT);
pinMode(13, OUTPUT);

Serial.begin(9600);
 //   Serial.println(ch);
Serial.println(" w forward \n a left\n d right \n s reverse \n z stop ");

}



void loop() {
  // put your main code here, to run repeatedly:

  if(Serial.available())
  {

    //ch = Serial.read();
    ch = '+111+111+111+111';
    
    // Motor One
    if (ch[0] == 'f')      // serial command for the first motor +=forward
          digitalWrite(2, HIGH); //Motor 1 FWD
          digitalWrite(4, LOW);
    else if (ch[0] == '-') //- =backwards
          digitalWrite(2, LOW); //Motor 1 REV
          digitalWrite(4, HIGH);
    //ch(1,2,3) are pwm
    
   // Motor Two
    if (ch[4] == '+')      // serial command for the second motor +=forward
          digitalWrite(6, HIGH); //Motor 2 FWD
          digitalWrite(7, LOW);
       else if (ch[4] == '-') //- =backwards
          digitalWrite(6, LOW); //Motor 2 REV
          digitalWrite(7, HIGH);
    //ch(5,6,7) are pwm
    
    // Motor Three
    if (ch[8] == '+')      // serial command for third motor +=forward
          digitalWrite(8, HIGH); //Motor 3 FWD
          digitalWrite(9, LOW);
       else if (ch[8] == '-') //- =backwards
          digitalWrite(8, LOW); //Motor 3 REV
          digitalWrite(9, HIGH);
    //ch(9,10,11) are pwm
    
    // Motor Four
    if (ch[12] == '+')      // serial command 's' REVERSE
          digitalWrite(12, HIGH); //Motor 4 FWD
          digitalWrite(13, LOW);
          else if (ch[12] == '-') //-=backwards   
          digitalWrite(12, LOW); //Motor 4 REV
          digitalWrite(13, HIGH);
    //ch(13,14,15)are pwm


    
  }else{
    //Serial.println("Serial not detected");
  }

}
