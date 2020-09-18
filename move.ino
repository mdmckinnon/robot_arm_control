#include <Servo.h>
#include <math.h>

// initalize vars
int angle1;
int angle2;
int angle3;
int angle4;

float arm1 = 100.0;
float arm2 = 100.0;
float endEffector = 0; // Update with the length from the grapsing point to the pivot
int servoSpeed = 50; // Change this to incease or decrease speed of movement

//Servos to be used
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

void setup() {
	// attach the servos to specified pins on arduino
	servo1.attach(6);
	servo2.attach(9);
	servo3.attach(10);
	servo4.attach(11);

	servo1.write(90);
	servo2.write(90);
	servo3.write(90);
}

void loop() {
	// put your main code here, to run repeatedly:
	//getCoordinates();
	calculateMove(0, 175, -25);
	moveArm();
	
	delay(1000);
	
	calculateMove(175, 175, 0);
	moveArm();
	delay(1000);
}

// Uses updated values for theta 1 through 4 and moves servos accordingly
// Borref gave me the inspiration for this approach
void moveArm() {
  bool complete1 = false;
  bool complete2 = false;
  bool complete3 = false;
  bool complete4 = true; //Change to false once end effector is installed
  long prevTime = millis();
  
	//Loops until all the servos have reached thier desired position
	while (!complete1 && !complete2 && !complete3){
		if (millis() - prevTime > servoSpeed) {
			prevTime = millis();
			
			// Moving servo 1
			if (angle1 > servo1.read()) {
				servo1.write(servo1.read() + 1);
			}
			else if (angle1 < servo1.read()) {
				servo1.write(servo1.read() - 1);
			}
			else {
				complete1 = true;
			}
			
			// Moving servo 2
			if (angle2 > servo2.read()) {
				servo2.write(servo2.read() + 1);
			}
			else if (angle2 < servo2.read()) {
				servo2.write(servo2.read() - 1);
			}
			else {
				complete2 = true;
			}
			
			// Moving servo 3
			if (angle3 > servo3.read()) {
				servo3.write(servo3.read() + 1);
			}
			else if (angle3 < servo3.read()) {
				servo3.write(servo3.read() - 1);
			}
			else {
				complete3 = true;
			}
			
			//Moving servo 4
			if (angle4 > servo4.read()) {
				servo4.write(servo4.read() + 1);
			}
			else if (angle4 < servo4.read()) {
				servo4.write(servo4.read() - 1);
			}
			else {
				complete4 = true;
			}
		}
	}
}

// Accepts x, y, z coordinates and returns values of theta for each servo
void calculateMove(int x, int y, int z) {
  //Initialize vars
	float theta1 = 0;
	float theta2 = 0;
	float theta3 = 0;
	float theta4 = 0;
	float vLength = sqrt(x * x + y * y + z * z);

	// Calc theta 1
	if (x >= 0) {
		theta1 = 90.0 - (atan(x/y) * 180.0 / M_PI_2);
	}
	else {
		theta1 = 90.0 + (atan(abs(x)/y) * 180.0 / M_PI_2);
	}

	// Calc theta 2
	float phi1 = acos((arm1 * arm1 + vLength * vLength - arm2 * arm2)/ (2 * arm1 * vLength));
	float phi2 = atan(z / sqrt(x * x + y * y));
	theta2 = (phi1  + phi2) * 180.0 / M_PI_2;

	// Calc theta 3
	phi1 = acos((arm1 * arm1 + arm2 * arm2 - vLength * vLength) / (2 * arm1 * arm2));
	theta3 = phi1 * 180.0 / M_PI_2;

	//Updating angles of the servos
	angle1 = round(180 - theta1);
	angle2 = round(theta2 * 2);
	angle3 = round((theta3 - 52.5) * 180 / 127.5);
	angle4 = 0;
}
