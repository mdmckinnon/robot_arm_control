#include <Servo.h>
#include <math.h>

// initalize vars
int angles[] = {90, 90, 90, 90}; // Edit these if you want to change the starting position of the arm

float arm1 = 100.0;
float arm2 = 100.0;
float endEffector = 0; // Update with the length from the grapsing point to the pivot
int servoSpeed = 50; // Change this to incease or decrease speed of movement


//Servos to be used
Servo servos[4];
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
	// put your main code here, to run repeatedly:
	//getCoordinates();
	calculateMove(-500, 200, -50);
	moveArm();
	
	delay(1000);
	
	calculateMove(150, 100, 0);
	moveArm();
	delay(1000);
}

// Gets the position data for the x,y,z points
// Y >= 0; 
//void getXYZ() {
//}

// Uses updated values for theta 1 through 4 and moves servos accordingly
// Borref gave me the inspiration for this approach
void moveArm() {
	bool completed[] = {false, false, false, false}; // bool to check if all servos are in position
	long prevTime = millis();
	int pos;
  
	//Loops until all the servos have reached thier desired position
	while (!completed[0] && !completed[1] && !completed[2]){
		if (millis() - prevTime > servoSpeed) {
			prevTime = millis();
			for (byte i = 0; i < 4; i++) {
				pos = servos[i].read();
				if (angles[i] > pos) {
					servos[i].write(pos + 1);
				}
				else if (angles[i] < pos) {
					servos[i].write(pos - 1);
				}
				else {
					completed[i] = true;
				}
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
	angles[0] = round(180 - theta1);
	angles[1] = round(theta2 * 2);
	angles[2] = round((theta3 - 52.5) * 180 / 127.5);
	angles[3] = 0;

 //Checks to ensure robot doesn't extend past limits, other checking is implemented in get xyz
 for (byte i = 0; i < 4; i++) {
	if (angles[i] >= 180) {
		angles[i] = 179;
	}
	else if (angles[i] <= 0) {
		angles[i] = 1;
	}
 }
}
