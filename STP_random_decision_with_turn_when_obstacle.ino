
//////////////////////////////////////////////// IO PINS

#define MOTA_DIRA  2 /* left  motor direction a Connection to motordriver IN1 */
#define MOTA_DIRB  3 /* left  motor direction b Connection to motordriver IN2 */
#define MOTB_DIRA  4 /* right motor direction a Connection to motordriver IN3 */
#define MOTB_DIRB  5 /* right motor direction b Connection to motordriver IN4 */
#define ECHO 8       /* get ECHO from distance sensor */
#define TRIGGER 9    /* TRIGGER distance sensor */

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

void loop() {

  int duration;                                               // variable stores how long something is done in ms
  int action = random(1, 5);                                  // decide which action to do by random (assign random value 1...4 to 'action' variable)
  Serial.print("Action: ");
  Serial.println(action);
  
  switch (action) {                                           // switch to specific action
    

    case 1:                                                   // action 1: move forward
      moveForward();                                          // move motors
      duration = random(3000, 15000);                         // set random duration for this action
      unsigned long timeOut;                                  
      timeOut = millis() + duration;                          // set timeOut for this action based on current time plus duration
      while (millis() <  timeOut) {                           // do this action as long as timeOut is not reached
        int distance = measureDistance();                     // OBSTACLE DETECTION: measure distance via Ultrasonic range finder (returns cm to obstacle or 0 for infinite)
        Serial.println(distance);
        if (distance < 30 && distance > 0) {
          timeOut = millis();                                 // break out of while() if distance is shorter than threshold and larger than infinite
        }
      }
      break;                                                  // end this action

    case 2:                                                   // action 2: turn left
      Serial.println("Turn left");
      turnLeft();                                             // move motors
      duration = random(100, 3000);                           // set random duration for this action
      delay(duration);                                        // wait for some time
      break;                                                  // end this action

    case 3:                                                   // action 3: turn right
      Serial.println("Turn right");
      turnRight();                                            // move motors
      duration = random(100, 3000);                           // set random duration for this action
      delay(duration);                                        // wait for some time
      break;                                                  // end this action

    default:                                                  // default action: stop and wait
      Serial.println("Stop");
      stopMoving();                                           // stop motors
      duration = random(2000, 4000);                          // set random duration for this action
      delay(duration);                                        // wait for some time
      break;                                                  // end this action
  }
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




