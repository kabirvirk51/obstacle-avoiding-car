#include <Servo.h>

#define TRIG_PIN 6
#define ECHO_PIN 7
#define SERVO_PIN 4
#define SPEAKER_PIN 9
#define LEFT_LED 8
#define RIGHT_LED 10
#define LEFT_MOTOR_PIN1 12
#define LEFT_MOTOR_PIN2 11
#define RIGHT_MOTOR_PIN1 3
#define RIGHT_MOTOR_PIN2 2

Servo servo;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(LEFT_LED, OUTPUT);
  pinMode(RIGHT_LED, OUTPUT);
  pinMode(LEFT_MOTOR_PIN1, OUTPUT);
  pinMode(LEFT_MOTOR_PIN2, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN1, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN2, OUTPUT);

  servo.attach(SERVO_PIN);

  Serial.begin(9600);
}

void loop() {
  long duration, distance;

  // Trigger ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the echo duration
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance in centimeters
  distance = duration / 29 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance <= 5) {
    // Obstacle detected, activate the siren and lights, back up slightly, and then turn the servo left and right to find the direction with more distance

    activateHonk();
    backup();
    digitalWrite(LEFT_LED, LOW);
    digitalWrite(RIGHT_LED, LOW);    
   
    servo.write(85); // Center the servo position
    delay(500);

    int leftDistance = measureDistance(0, 90); // Measure distance turning left
    int rightDistance = measureDistance(180, 90); // Measure distance turning right

    if (leftDistance > rightDistance) {
      // Turn left
      digitalWrite(LEFT_LED, HIGH);
      turnLeft();
      digitalWrite(LEFT_LED, LOW);
    } else {
      // Turn right
      digitalWrite(RIGHT_LED, HIGH);
      turnRight();
      digitalWrite(RIGHT_LED, LOW);
    }
   
  } else {
    // No obstacle, drive forward
    driveForward();
  }
}

int measureDistance(int angle, int delayTime) {
  servo.write(angle);
  delay(delayTime);
 
  long duration, distance;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration / 29 / 2;

  delay(1000);

  servo.write(105);

  return distance;
}

void driveForward() {
  digitalWrite(LEFT_MOTOR_PIN1, HIGH);
  digitalWrite(LEFT_MOTOR_PIN2, LOW);

  digitalWrite(RIGHT_MOTOR_PIN1, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN2, LOW);
}

void backup() {
  digitalWrite(LEFT_MOTOR_PIN1, LOW);
  digitalWrite(LEFT_MOTOR_PIN2, HIGH);

  digitalWrite(RIGHT_MOTOR_PIN1, LOW);
  digitalWrite(RIGHT_MOTOR_PIN2, HIGH);

  delay(500);
  stopMotors();
}

void stopMotors() {
  digitalWrite(LEFT_MOTOR_PIN1, LOW);
  digitalWrite(LEFT_MOTOR_PIN2, LOW);

  digitalWrite(RIGHT_MOTOR_PIN1, LOW);
  digitalWrite(RIGHT_MOTOR_PIN2, LOW);

  delay(500);
}

void turnLeft() {
  stopMotors();
  delay(200);
 
  digitalWrite(LEFT_MOTOR_PIN1, LOW);
  digitalWrite(LEFT_MOTOR_PIN2, HIGH);

  digitalWrite(RIGHT_MOTOR_PIN1, HIGH);
  digitalWrite(RIGHT_MOTOR_PIN2, LOW);

  delay(500);
  stopMotors();
}

void turnRight() {
  stopMotors();
  delay(200);
 
  digitalWrite(LEFT_MOTOR_PIN1, HIGH);
  digitalWrite(LEFT_MOTOR_PIN2, LOW);

  digitalWrite(RIGHT_MOTOR_PIN1, LOW);
  digitalWrite(RIGHT_MOTOR_PIN2, HIGH);

  delay(500);
  stopMotors();
}

void activateHonk() {

 tone(SPEAKER_PIN, 1000, 1000);
 digitalWrite(RIGHT_LED, HIGH);
 digitalWrite(LEFT_LED, HIGH);
 
}