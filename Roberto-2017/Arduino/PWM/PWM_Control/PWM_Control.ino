// Used some code from
// http://forum.arduino.cc/index.php?topic=396450
// Visit there for help
// Written by Nick Hussey
// 10-20-2017

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
        digitalWrite(7, LOW);
        }
      else if (receivedChars[4]=='-'){
        Serial.println("Motor two reverse");
        }
       else{
        Serial.println("Motor two error");}
        
      if (receivedChars[8]=='+'){            //Motor Three (8,9,10,11)
        Serial.println("Motor three forward");}
      else if (receivedChars[8]=='-'){
        Serial.println("Motor three reverse");}
       else{
        Serial.println("Motor three error");}
      
      if (receivedChars[12]=='+'){            //Motor Four (12,13,14,15)
        Serial.println("Motor four forward");}
      else if (receivedChars[12]=='-'){
        Serial.println("Motor four reverse");}
       else{
        Serial.println("Motor four error");}
      
        newData = false;
        
        PWM1=((100*(receivedChars[1]-48))+(10*(receivedChars[2]-48))+(1*(receivedChars[3]-48))); //Reads the XXX into a standard number
        if (PWM1>255){ //PWM can only accept 0-255
        PWM1=255;}
        Serial.println(PWM1); //On an UNO Pins labled with a ~ are PWM pins. On the mega the entire row is PWM enabled
        analogWrite(3, PWM1); //Current;y Pins 2,6,10,14 are the PWM 
        
        PWM2=((100*(receivedChars[5]-48))+(10*(receivedChars[6]-48))+(1*(receivedChars[7]-48)));
        if (PWM2>255){
        PWM2=255;}
        Serial.println(PWM2);
        analogWrite(5, PWM2);
        
        PWM3=((100*(receivedChars[9]-48))+(10*(receivedChars[10]-48))+(1*(receivedChars[11]-48)));
        if (PWM3>255){
        PWM3=255;}
        Serial.println(PWM3);
        analogWrite(10, PWM2);
        
        PWM4=((100*(receivedChars[13]-48))+(10*(receivedChars[14]-48))+(1*(receivedChars[15]-48)));
        if (PWM4>255){
        PWM4=255;}
        Serial.println(PWM4 );
        analogWrite(11, PWM4);
    }
    }
