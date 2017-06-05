
//////////////////////////////////////////////// IO PINS

#define MOTA_DIRA  2 /* left  motor direction a Connection to motordriver IN1 */
#define MOTA_DIRB  3 /* left  motor direction b Connection to motordriver IN2 */
#define MOTB_DIRA  4 /* right motor direction a Connection to motordriver IN3 */
#define MOTB_DIRB  5 /* right motor direction b Connection to motordriver IN4 */
#define ECHO 8       /* get ECHO from distance sensor */
#define TRIGGER 9    /* TRIGGER distance sensor */
unsigned long drive = 0;
//////////////////////////////////////////////// SETUP

void setup() {
  pinMode(MOTA_DIRA, OUTPUT); // left motor direction a
  pinMode(MOTA_DIRB, OUTPUT); // left motor direction b
  pinMode(MOTB_DIRA, OUTPUT); // right motor direction a
  pinMode(MOTB_DIRB, OUTPUT); // right motor direction b

  pinMode(TRIGGER, OUTPUT); // triggers pulse on distance sensor
  pinMode(ECHO, INPUT);     // returns an echo from the distance sensor

  Serial.begin(9600);
}

//////////////////////////////////////////////// LOOP

//robot drives forward and stops if there is an obstacle closer than 30cm

void loop() {

  while (millis() < drive) {
    int distance = measureDistance();                     // OBSTACLE DETECTION: measure distance via Ultrasonic range finder (returns cm to obstacle or 0 for infinite)
    if (distance < 50 && distance > 0) {                  // if measured distance is shorter than 30cm:
      moveBackward();
    } else {
      moveForward();                                      // move motors
      delay(300);
    }
  }
  stopMoving();
  delay(random(500,10000));
  drive=millis() + random(500,10000);

}



//////////////////////////////////////////////// FUNCTIONS

//---------------------------------------------- Read Sensor

int measureDistance() { // measures distance with distance sensor and returns cm to obstacle
  unsigned long distance;
  digitalWrite(TRIGGER, HIGH); // send 10uS Trigger pulse
  delayMicroseconds(12);
  digitalWrite(TRIGGER, LOW);

  while  (digitalRead(ECHO) == LOW) {}                        // do nothing until ECHO rises to HIGH
  unsigned long startTime = micros();                         // ECHO is HIGH: get current time
  unsigned long timeOut = startTime + 20000;                  // set timeOut
  while (digitalRead(ECHO) == HIGH && micros() < timeOut) {}  // wait until ECHO becomes LOW again or until time out is reached
  distance = micros() - startTime;                            // calculate how long the HIGH pulse was
  return int(distance / 58);                                  // convert time to cm and return
}

//---------------------------------------------- Move Vehicle

void moveForward() {
  digitalWrite(MOTA_DIRA, LOW);    // left backwards
  digitalWrite(MOTA_DIRB, HIGH);   // left forward
  digitalWrite(MOTB_DIRA, LOW);    // right backwards
  digitalWrite(MOTB_DIRB, HIGH);   // right forward
}


void moveBackward() {
  digitalWrite(MOTA_DIRA, HIGH);    // left backwards
  digitalWrite(MOTA_DIRB, LOW);   // left forward
  digitalWrite(MOTB_DIRA, HIGH);    // right backwards
  digitalWrite(MOTB_DIRB, LOW);   // right forward
}

void turnLeft() {
  digitalWrite(MOTA_DIRA, HIGH);  // left backwards
  digitalWrite(MOTA_DIRB, LOW);   // left forward stop
  digitalWrite(MOTB_DIRA, LOW);   // right backwards stop
  digitalWrite(MOTB_DIRB, HIGH);  // right forward
}

void turnRight() {
  digitalWrite(MOTA_DIRA, LOW);   // left backwards stop
  digitalWrite(MOTA_DIRB, HIGH);  // left forward
  digitalWrite(MOTB_DIRA, HIGH);  // right backwards
  digitalWrite(MOTB_DIRB, LOW);   // right forward stop
}

void stopMoving() {
  digitalWrite(MOTA_DIRA, LOW);   // left backwards stop
  digitalWrite(MOTA_DIRB, LOW);   // left forward stop
  digitalWrite(MOTB_DIRA, LOW);   // right backwards stop
  digitalWrite(MOTB_DIRB, LOW);   // right forward stop
}




