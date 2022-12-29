int p1 = 12;
int p2 = 11;
int p3 = 10;
int p4 = 9;
float deg = 0;
int dir;
int spd;
int dur;
int input1;
int input2;
int input3;
bool inputComplete = false;
String mode;
bool Run = true;
bool homeSwitch = false;
int homePin = 3;
int lastStep = 1; //default
float curStep = 10000; //will be defined in setup
long stepDest = 1;

#include <DMXSerial.h>

void step1(){
  digitalWrite(p1, HIGH);
  digitalWrite(p2, LOW);
  digitalWrite(p3, HIGH);
  digitalWrite(p4, LOW);
  lastStep = 1;
}


void step2(){
  digitalWrite(p1, LOW);
  digitalWrite(p2, HIGH);
  digitalWrite(p3, HIGH);
  digitalWrite(p4, LOW);
  lastStep = 2;
  
}

void step3(){
  digitalWrite(p1, LOW);
  digitalWrite(p2, HIGH);
  digitalWrite(p3, LOW);
  digitalWrite(p4, HIGH);
  lastStep = 3;
}

void step4(){
  digitalWrite(p1, HIGH);
  digitalWrite(p2, LOW);
  digitalWrite(p3, LOW);
  digitalWrite(p4, HIGH);
  lastStep = 4;
}

void stepCCW(){
  if(lastStep==1){
    step2();
    
  }
  else if(lastStep==2){
    step3();
    
  }
  else if(lastStep==3){
    step4();
    
  }
  else if(lastStep==4){
    step1();
    
  }
  curStep=curStep-1;
  delayMicroseconds(2000);
}

void stepCW(){
  if(lastStep==1){
    step4();
    
  }
  else if(lastStep==2){
    step1();
    
  }
  else if(lastStep==3){
    step2();
    
  }
  else if(lastStep==4){
    step3();
    
  }
  curStep=curStep+1;
  delayMicroseconds(2000);
}

void stepToDeg(float deg){
  stepDest = float((deg/360))*256*8;
  while(curStep != stepDest){
    if(curStep<stepDest){
      stepCW();
    }
    if(curStep>stepDest){
      stepCCW();
    }
    deg = int(((float)curStep/((float)(256*8)))*(float)360);
    //Serial.println(String(curStep)+"\t"+String(stepDest)+"\t"+String(deg));
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(p1, OUTPUT);
  pinMode(p2, OUTPUT);
  pinMode(p3, OUTPUT);
  pinMode(p4, OUTPUT);
  //Serial.begin(9600);
  pinMode(homePin, INPUT);
  while(digitalRead(homePin)==HIGH){
    stepCCW();
  }
  curStep = 0;
  DMXSerial.init(DMXReceiver);
  
  // set some default values
  DMXSerial.write(1, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  //stepCW();
  unsigned long lastPacket = DMXSerial.noDataSince();
  // read recent DMX values and set pwm levels 
  float degree = map(DMXSerial.read(1),0,255,0,90);
  stepToDeg(degree);
}
