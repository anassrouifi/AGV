#include <Arduino.h>
#include <AFMotor.h>

AF_DCMotor motor1(1); // Motor 1
AF_DCMotor motor2(2); // Motor 2
AF_DCMotor motor3(3); // Motor 3
AF_DCMotor motor4(4); // Motor 4

const int weightButtonPin = 10;
const int lineSensorPin = A0;
const int roadButtonPinA = 11;
const int roadButtonPinB = 12;

int selectedWeight = 0; // 0 = None, 1 = A, 2 = B, 3 = C
bool isOnLine = false;

void setup() {
    Serial.begin(115200);

    pinMode(weightButtonPin, INPUT_PULLUP);
    pinMode(lineSensorPin, INPUT);
    pinMode(roadButtonPinA, INPUT_PULLUP);
    pinMode(roadButtonPinB, INPUT_PULLUP);

    motor1.setSpeed(255); // Adjust motor speed here
    motor2.setSpeed(255);
    motor3.setSpeed(255);
    motor4.setSpeed(255);
}

int readWeightButton() {
    int buttonState = digitalRead(weightButtonPin);

    if (buttonState == LOW) {
        selectedWeight++;
        if (selectedWeight > 3) {
            selectedWeight = 0;
        }
        delay(300); // Debounce delay
    }

    return selectedWeight;
}

void setMotorSpeeds(int weight) {
    int speed;

    if (weight == 1) {
        speed = 150; // Set speed for weight A
    } else if (weight == 2) {
        speed = 100; // Set speed for weight B
    } else if (weight == 3) {
        speed = 50;  // Set speed for weight C
    } else {
        speed = 0;   // No weight selected
    }

    motor1.setSpeed(speed);
    motor2.setSpeed(speed);
    motor3.setSpeed(speed);
    motor4.setSpeed(speed);
}

void followLine() {
        int lineThreshold = 500; // Adjust this threshold value based on your sensors
        int sensorValue = analogRead(lineSensorPin);

        if (sensorValue > lineThreshold) {
            // Turn right
            motor1.run(FORWARD);
            motor2.run(BACKWARD);
            motor3.run(BACKWARD);
            motor4.run(FORWARD);
        } else {
            // Turn left
            motor1.run(BACKWARD);
            motor2.run(FORWARD);
            motor3.run(FORWARD);
            motor4.run(BACKWARD);
        }
    }

}

int selectRoad() {
    int roadSelected = 0;

    if (digitalRead(roadButtonPinA) == LOW) {
        roadSelected = 1;
    } else if (digitalRead(roadButtonPinB) == LOW) {
        roadSelected = 2;
    }

    return roadSelected;
}

void loop() {
    int weight = readWeightButton();
    setMotorSpeeds(weight);

    isOnLine = digitalRead(lineSensorPin);
    if (isOnLine) {
        followLine();
    }

    int selectedRoad = selectRoad();
    if (selectedRoad == 1) {
        motor1.run(FORWARD);
        motor2.run(BACKWARD);
        motor3.run(BACKWARD);
        motor4.run(FORWARD);

    } else if (selectedRoad == 2) {
        motor1.run(BACKWARD);
        motor2.run(FORWARD);
        motor3.run(FORWARD);
        motor4.run(BACKWARD);    }
}