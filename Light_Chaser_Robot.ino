const int fw1 = 5;
const int bk1 = 6;
const int fw2 = 7;
const int bk2 = 8;
const int am1 = 9;
const int am2 = 10; //motor pins, fw=forward, bk=back, am=enable
const int lightSensor = A0; //pin for light sensor
const int lightThreshold = 60;
const int pingPin = 4;
const int trigPin = 3 ;
const int minimumDist = 10;
const int timeoutChangeDir = 1000;
int changeDir = 0;

void setup() {
  Serial.begin(19200);
  Serial.println("Ready");
  pinMode(fw1,OUTPUT);
  pinMode(bk1,OUTPUT);
  pinMode(fw2,OUTPUT);
  pinMode(bk2,OUTPUT);
  pinMode(am1,OUTPUT);
  pinMode(am2,OUTPUT);
  pinMode(pingPin, INPUT);
  pinMode(trigPin, OUTPUT);
}

void motorAction(int fwm, int bkm,int anm,int motspeed) {
      if (motspeed > 0 ) {
        digitalWrite(fwm,HIGH);
        digitalWrite(bkm,LOW);
      } else if (motspeed < 0) {
        digitalWrite(fwm,LOW);
        digitalWrite(bkm,HIGH);
      } else {
        digitalWrite(fwm,LOW);
        digitalWrite(bkm,LOW);
      }
      analogWrite(anm,abs(motspeed));
}

void straight(int spd) {
  motorAction(fw1,bk1,am1,spd);
  motorAction(fw2,bk2,am2,spd);
}
  
void right(int spd) {
  motorAction(fw1,bk1,am1,spd);
  motorAction(fw2,bk2,am2,0);
}

void left(int spd) {
  motorAction(fw1,bk1,am1,0);
  motorAction(fw2,bk2,am2,spd);  
}

void halt() {
  motorAction(fw1,bk1,am1,0);
  motorAction(fw1,bk1,am2,0);
}

bool proximity(){
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(pingPin, HIGH);
  if (duration/29/2 < minimumDist) {
    return true;
  } else {
    return false;
  }
}

void loop(){
  if (analogRead(lightSensor) < lightThreshold) {
    Serial.println("FORWARD");
    straight(100);
    changeDir ++;
    if (changeDir > timeoutChangeDir) {
      right(150);
      left(-150);
      delay(500);
      changeDir = 0;
    }
  } else {
    Serial.println("HALT");
    halt();
  }
  while(proximity()) {
    Serial.println("PROXIMITY");
    right(-150);
    left(150);
  }
}
