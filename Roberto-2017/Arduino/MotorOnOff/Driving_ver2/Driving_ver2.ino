void setup() {
  // put your setup code here, to run once:

char ch;

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

    char ch = Serial.read();
    if (ch == 's')      // serial command 's' REVERSE
     {

    digitalWrite(2, LOW); //Motor 1 REV
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);

    digitalWrite(6, LOW); //Motor 2 REV
    digitalWrite(5, HIGH);
    digitalWrite(7, HIGH);

    digitalWrite(8, LOW); //Motor 3 REV
    digitalWrite(10, HIGH);
    digitalWrite(9, HIGH);

    digitalWrite(12, LOW); //Motor 4 REV
    digitalWrite(11, HIGH);
    digitalWrite(13, HIGH);

    }else if(ch == 'd')  // serial command 'd' CW (right)
    {

    digitalWrite(2, LOW); //Motor 1 REV
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);

    digitalWrite(6, LOW); //Motor 2 REV
    digitalWrite(5, HIGH);
    digitalWrite(7, HIGH);

    digitalWrite(8, HIGH); //Motor 3 FWD
    digitalWrite(10, HIGH);
    digitalWrite(9, LOW);

    digitalWrite(12, HIGH); //Motor 4 FWD
    digitalWrite(11, HIGH);
    digitalWrite(13, LOW);
      
      
    }else if(ch == 'a')  // serial command 'a' CCW (left)
    {

    digitalWrite(2, HIGH); //Motor 1 FWD
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);

    digitalWrite(6, HIGH); //Motor 2 FWD
    digitalWrite(5, HIGH);
    digitalWrite(7, LOW);

    digitalWrite(8, LOW); //Motor 3 REV
    digitalWrite(10, HIGH);
    digitalWrite(9, HIGH);

    digitalWrite(12, LOW); //Motor 4 REV
    digitalWrite(11, HIGH);
    digitalWrite(13, HIGH);
      
      
    }else if(ch == 'w')  // serial command 'w' Forward
    {
    Serial.println("FORWARD!");  

    digitalWrite(2, HIGH); //Motor 1 FWD
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);

    digitalWrite(6, HIGH); //Motor 2 FWD
    digitalWrite(5, HIGH);
    digitalWrite(7, LOW);

    digitalWrite(8, HIGH); //Motor 3 REV
    digitalWrite(10, HIGH);
    digitalWrite(9, LOW);

    digitalWrite(12, HIGH); //Motor 4 REV
    digitalWrite(11, HIGH);
    digitalWrite(13, LOW);
      
      
    }else if(ch == 'z')  // serial command 'z' STOP
    {

    digitalWrite(2, LOW); //Motor 1 OFF
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);

    digitalWrite(6, LOW); //Motor 2 OFF
    digitalWrite(5, LOW);
    digitalWrite(7, LOW);

    digitalWrite(8, LOW); //Motor 3 OFF
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);

    digitalWrite(12, LOW); //Motor 4 OFF
    digitalWrite(11, LOW);
    digitalWrite(13, LOW);
      
      
    }
  
  }else{
    //Serial.println("Serial not detected");
  }


  /*
  digitalWrite(2, LOW); //Motor 1 Fwd
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);

  digitalWrite(6, LOW); //Motor 2 Fwd
  digitalWrite(5, HIGH);
  digitalWrite(7, HIGH);

  digitalWrite(8, LOW); //Motor 3 Fwd
  digitalWrite(10, HIGH);
  digitalWrite(9, HIGH);

  digitalWrite(12, LOW); //Motor 4 Fwd
  digitalWrite(11, HIGH);
  digitalWrite(13, HIGH);
  */
}
