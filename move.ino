#include <Servo.h>
#include <math.h>

// initalize vars
int angles[] = {90, 90, 90, 90}; // Edit these if you want to change the starting position of the arm
float arm1 = 100.0;
float arm2 = 100.0;
float endEffector = 0; // Change to the length from the grapsing point to the pivot
int servoSpeed = 2; // Change this to incease or decrease speed of movement


//Servos to be used
Servo servos[4];
int servopos[] = {90, 90, 90, 90};
// servos[0] Base rotation
// servos[1] Main arm pivot
// servos[2] Middle joint 
// servos[3] End effector orientation

void setup() {
	// attach the servos to specified pins on arduino
	servos[0].attach(6);
	servos[1].attach(9);
	servos[2].attach(10);
	servos[3].attach(11);
  
	moveArm(); // Initializes arm into starting position
}

void loop() {
	calculateMove(-150, 75, -50);
	moveArm();
	delay(2000);
	
	calculateMove(75, 50, 50);
	moveArm();
	delay(2000);

	calculateMove(100, 150, -50);
	moveArm();
	delay(2000);
}

// Gets the position data for the x,y,z points 
//void getXYZ() {
//}

// Uses stored values for angles 1 through 4 and moves servos to position
void moveArm() {
	bool completed[] = {false, false, false, true}; // bool to check if all servos are in position
	int pos;
  
	// Loops until all the servos have reached thier desired position
	while (!completed[0] || !completed[1] || !completed[2] || !completed[3]){
      
		// Loops through each servos, changes position 1 degree at a time
		for (byte i = 0; i < 4; i++) {
			pos = servos[i].read();
			if (angles[i] > pos && completed[i] == false) {
				servos[i].write(pos + 1);
			}
			else if (angles[i] < pos && completed[i] == false) {
				servos[i].write(pos - 1);
			}
			else {
				completed[i] = true;
			}
		delay(servoSpeed);
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
	float vLength = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

	// Calc theta 1
	if (x > 0) {
		theta1 = (atan2(y, x) * 180.0 / M_PI);
	}
	else {
		theta1 = (atan2(-x, y) * 180.0 / M_PI) + 90;
	}

	// Calc theta 2
	theta2 = acos((pow(arm1, 2) + pow(vLength, 2) - pow(arm2, 2))/ (2 * arm1 * vLength));
	theta2 = (theta2 + asin(z / vLength)) * 180.0 / M_PI;

	// Calc theta 3
	theta3 = acos((pow(arm1, 2) + pow(arm2, 2) - pow(vLength, 2)) / (2 * arm1 * arm2));
	theta3 = theta3 * 180.0 / M_PI;
	
	// Calc theta 4
	//theta4 = 0;
	
	// Updating angles of the servos
	angles[0] = round(180 - theta1);
	angles[1] = round(180 - (theta2 * 2));
	angles[2] = round((theta3 - 52.5) * 180 / 127.5);
	angles[3] = 0;
  
 // Checks to ensure robot doesn't extend past limits
 // Need to update to better handle errors, send message back
 for (byte i = 0; i < 4; i++) {
	if (angles[i] >= 180) {
		angles[i] = 179;
	}
	else if (angles[i] <= 0) {
		angles[i] = 1;
	}
 }
}
