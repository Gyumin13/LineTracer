// Made with Arduino IDE interface. You can use it with car that you made. 

int LEFTA = 6;
int LEFTB = 5;
int RIGHTA = 11;
int RIGHTB = 10;
int IRLEFT = A0; // Changed to pin number
int IRRIGHT = A1; // Changed to pin number
int RED = 7;
int BLUE = 8;
int LSPEED;
int RSPEED;
int sensorThreshold = 430; //임계값 : 자신이 지정한 검정색의 측정 최솟값
int sensorDifference = 200;
int MAX_DISTANCE = 10; 
int SLOW_DISTANCE = 20;



void setup() {
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  pinMode(LEFTA, OUTPUT);
  pinMode(LEFTB, OUTPUT);
  pinMode(RIGHTA, OUTPUT);
  pinMode(RIGHTB, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(IRRIGHT, INPUT);
  pinMode(IRLEFT, INPUT);
  Serial.begin(9600);
}

float measureDistance() {
  digitalWrite(3, LOW);
  delayMicroseconds(2);
  digitalWrite(3, HIGH);
  delayMicroseconds(10);
  digitalWrite(3, LOW);

  float duration = pulseIn(2, HIGH);
  float distance = duration * 0.034 / 2;

  return distance;
}

void run_forward() {
  LSPEED = 255;
  RSPEED = 255; 
}

void run_left() {
  LSPEED = 20;
  RSPEED = 190;
}

void run_right() {
  LSPEED = 190;
  RSPEED = 20;
}

void stop() {
  LSPEED = 0;
  RSPEED = 0;
}

void slow(){
  LSPEED = LSPEED - 50;
  RSPEED = RSPEED - 50;
}

void tr() {
  int valIRRight = analogRead(IRRIGHT);
  int valIRLeft = analogRead(IRLEFT); 
  if (valIRRight > sensorThreshold and valIRLeft > sensorThreshold) {
    LSPEED = 255;
    RSPEED = 255;
  } 
  else if (valIRRight - valIRLeft > sensorDifference) {
    run_left();
  } 
  else if (valIRLeft - valIRRight > sensorDifference) {
    run_right();
  } 
  else {
    run_forward();
  }
}


void loop() {
  float distance = measureDistance();

  if (distance <= MAX_DISTANCE) {
    stop();
    digitalWrite(RED, HIGH);
    digitalWrite(BLUE, LOW);
  } else if (distance > MAX_DISTANCE and distance <= SLOW_DISTANCE) {
    slow();
    digitalWrite(RED, LOW);
    digitalWrite(BLUE, HIGH);
  } else {
    tr();
    digitalWrite(RED, LOW);
    digitalWrite(BLUE, HIGH);
  }
  

  analogWrite(LEFTA, LSPEED);
  analogWrite(LEFTB, 0);
  analogWrite(RIGHTA, RSPEED);
  analogWrite(RIGHTB, 0);
  Serial.print("LEFT: ");
  Serial.print(analogRead(IRLEFT));
  Serial.print("  RIGHT: ");
  Serial.println(analogRead(IRRIGHT));
  delay(10);
}