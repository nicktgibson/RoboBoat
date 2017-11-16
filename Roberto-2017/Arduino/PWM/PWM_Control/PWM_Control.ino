// Used some code from
// http://forum.arduino.cc/index.php?topic=396450
// Visit there for help
// Written by Nick Hussey
// 10-20-2017

//#include<string>

const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data

boolean newData = false;
int PWM1;
int PWM2;
int PWM3;
int PWM4;

void setup() {
    Serial.begin(9600);
    Serial.println("Please send +XXX+XXX+XXX+XXX where each + is a '+' or '-' and each XXX is 0-255");

    
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);

  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  

  

}

void loop() {
    recvWithEndMarker();
    ControlMotorDirection();
}

void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();
        if (rc != '>') {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
       // receivedChars = itoa((receivedChars [1]))// + string(receivedChars [2]);
      //  receivedChars = receivedChars[(sizeof(receivedChars)-17)]+receivedChars[(sizeof(receivedChars)-16)]+receivedChars[(sizeof(receivedChars)-15)]+receivedChars[(sizeof(receivedChars)-14)]+receivedChars[(sizeof(receivedChars)-13)]+receivedChars[(sizeof(receivedChars)-12)]+receivedChars[(sizeof(receivedChars)-11)]+receivedChars[(sizeof(receivedChars)-10)]+receivedChars[(sizeof(receivedChars)-9)]+receivedChars[(sizeof(receivedChars)-8)]+receivedChars[(sizeof(receivedChars)-7)]+receivedChars[(sizeof(receivedChars)-6)]+receivedChars[(sizeof(receivedChars)-5)]+receivedChars[(sizeof(receivedChars)-4)]+receivedChars[(sizeof(receivedChars)-3)]+receivedChars[(sizeof(receivedChars)-2)]+receivedChars[(sizeof(receivedChars)-1)];
        //receivedChars = receivedChars.substr((sizeof(receivedChars)-16),sizeof(receivedChars))
        //receivedChars = receivedChars[(sizeof(receivedChars)-16):sizeof(receivedChars)]
        //else if(rc = 'z'){
        //    digitalWrite(3, LOW);
        //   digitalWrite(5, LOW);
        //    digitalWrite(10, LOW);
        //    digitalWrite(11, LOW);
        //}
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
        
    }  
}

void ControlMotorDirection() {
    if (newData == true) {
      if (receivedChars[0]=='+'){            //Motor One (0,1,2,3)
        Serial.println("Motor one forward");
        digitalWrite(2, HIGH);
        digitalWrite(4, LOW);}
      else if (receivedChars[0]=='-'){
        Serial.println("Motor one reverse");
        digitalWrite(2, LOW);
        digitalWrite(4, HIGH);}
       else{
        Serial.println("Motor one error");}
        
      if (receivedChars[4]=='+'){            //Motor Two (4,5,6,7)
        Serial.println("Motor two forward");
        digitalWrite(6, HIGH);
        digitalWrite(7, LOW); // previously 7
        }
      else if (receivedChars[4]=='-'){
        Serial.println("Motor two reverse");
        digitalWrite(6, LOW);
        digitalWrite(7, HIGH);  // previously 7
        }
       else{
        Serial.println("Motor two error");}
        
      if (receivedChars[8]=='+'){            //Motor Three (8,9,10,11)
        Serial.println("Motor three forward");
        digitalWrite(8, HIGH);
        digitalWrite(9, LOW);}
      else if (receivedChars[8]=='-'){
        Serial.println("Motor three reverse");
        digitalWrite(8, LOW);
        digitalWrite(9, HIGH);}
       else{
        Serial.println("Motor three error");}
      
      if (receivedChars[12]=='+'){            //Motor Four (12,13,14,15)
        Serial.println("Motor four forward");
        digitalWrite(12, HIGH);
        digitalWrite(13, LOW);}
      else if (receivedChars[12]=='-'){
        Serial.println("Motor four reverse");
        digitalWrite(12, LOW);
        digitalWrite(13, HIGH);}
       else{
        Serial.println("Motor four error");}
      
        newData = false;
        
        PWM1=((100*(receivedChars[1]-48))+(10*(receivedChars[2]-48))+(1*(receivedChars[3]-48))); //Reads the XXX into a standard number
        if (PWM1==0){
          digitalWrite(3,LOW);
        }
        else if (PWM1>255){ //PWM can only accept 0-255
          PWM1=255;
          Serial.println(PWM4);
          analogWrite(3, PWM4);
          }
        else{
          Serial.println(PWM1); //On an UNO Pins labled with a ~ are PWM pins. On the mega the entire row is PWM enable
          analogWrite(3, PWM1); //Current;y Pins 2,6,10,14 are the PWM 
        }

        
        PWM2=((100*(receivedChars[5]-48))+(10*(receivedChars[6]-48))+(1*(receivedChars[7]-48)));
        if (PWM2==0){
          digitalWrite(5,LOW);
        }
        else if (PWM2>255){
          PWM2=255;
          Serial.println(PWM2 );
          analogWrite(5, PWM2);
        }
        else {
          Serial.println(PWM2);
          analogWrite(5, PWM2);
        }
        
        PWM3=((100*(receivedChars[9]-48))+(10*(receivedChars[10]-48))+(1*(receivedChars[11]-48)));
        if (PWM3==0){
          digitalWrite(10,LOW);
        }
        else if (PWM3>255){
          PWM3=255;
          Serial.println(PWM3);
          analogWrite(10, PWM3);
        }
        else {
          Serial.println(PWM3);
          analogWrite(10, PWM3);
        }
        
        PWM4=((100*(receivedChars[13]-48))+(10*(receivedChars[14]-48))+(1*(receivedChars[15]-48)));
        if (PWM4==0){
          digitalWrite(11,LOW);
        }
        else if (PWM4>255){
          PWM4=255;
          Serial.println(PWM4 );
          analogWrite(11, PWM4);
        }
        else {
          Serial.println(PWM4 );
          analogWrite(11, PWM4);
        }
        }
    }
