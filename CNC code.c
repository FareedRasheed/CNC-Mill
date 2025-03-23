/********************************************
LEGO CNC CODE
********************************************/
#include"PC_FileIO.c"



/*manipulate xyzSpeed and drillSpeed
Recommended xyzSpeed of 5-10. slower speeds are safer and better for deeper cuts
Recommended drillSpeed of 40-50. slower speeds are safer and better for deeper cuts
*/
void selectSpeed(int &xyzSpeed, int &drillSpeed) {

	while(!getButtonPress(buttonUp)) {
		displayTextLine(5,"LEFT and RIGHT for xyz speed");
		displayTextLine(7, "Press UP button to confirm");
		displayTextLine(8,"xyz speed is %d", xyzSpeed);

		//gets button input from controller to change xyzSpeed
		if(getButtonPress(buttonRight) && xyzSpeed < 100) {
			while(getButtonPress(buttonAny)){
			}
			xyzSpeed=xyzSpeed+5;
			} else if(getButtonPress(buttonLeft) && xyzSpeed > 0) {
			while(getButtonPress(buttonAny)){
			}
			xyzSpeed=xyzSpeed-5;

		}
	}
	displayTextLine(5, "The xyz Speed is %d", xyzSpeed);
	displayTextLine(7,"");
	displayTextLine(8,"");
	wait1Msec(1000);



	while(!getButtonPress(buttonRight)) {
		displayTextLine(5,"UP/DOWN to set drill speed");
		displayTextLine(7, "Press RIGHT button to confirm");
		displayTextLine(8,"drill speed is %d", drillSpeed);

		//gets button input from controller to change drillSpeed
		if(getButtonPress(buttonUp) && drillSpeed < 100) {
			while(getButtonPress(buttonAny)){
			}
			drillSpeed= drillSpeed+5;
			} else if(getButtonPress(buttonDown)&& drillSpeed > 0) {
			while(getButtonPress(buttonAny)){
			}
			drillSpeed=drillSpeed-5;

		}
	}
	displayTextLine(5, "The drillSpeed is %d", drillSpeed);
	displayTextLine(7,"");
	displayTextLine(8,"");
	wait1Msec(1000);
	displayTextLine(5, "");



}


//Zero all axes before beginning operation
void startUp(int &xyzSpeed, int &drillSpeed) {

	//X
	SensorType[S1] = sensorEV3_Touch;
	//Y
	SensorType[S2] = sensorEV3_Touch;
	//Z
	SensorType[S3] = sensorEV3_Touch;

	//waits for button press
	while(!getButtonPress(buttonUp)) {
		displayTextLine(5,"UP BUTTON TO CALIBRATE");
	}
	displayTextLine(5,"ZEROING AXIS");

	//Zeros X axis
	displayTextLine(5,"ZERO X");
	while(SensorValue[S1]==0) {
		motor[motorA] = -5;
	}
	motor[motorA]=0;
	displayTextLine(5,"X Complete");
	wait1Msec(1000);

	//Zeros Y axis
	displayTextLine(5,"ZERO Y");
	while(SensorValue[S2]==0) {
		motor[motorB] = 5;
	}
	motor[motorB]=0;
	displayTextLine(5,"Y Complete");
	wait1Msec(1000);

	//Zeros Z Axis
	displayTextLine(5,"ZERO Z");
	while(SensorValue[S3]==0) {
		motor[motorC] = -5;
	}
	motor[motorC]=0;
	displayTextLine(5,"Z Complete");
	wait1Msec(1000);
	displayTextLine(5,"");
	nMotorEncoder[motorA]=	nMotorEncoder[motorB]=	nMotorEncoder[motorC]=0;

	selectSpeed(xyzSpeed, drillSpeed);



	clearTimer(T1);


}
//Checks that textfile instructions are within the bounds
int checkLimit(char axis,int &encoder) {

	//tested range of structure
	const int XMAX = 300; //motorA
	const int YMAX = -450; //motorB
	const int ZMAX = 130; //motorC

	if(axis == 'X') {
		if(nMotorEncoder[motorA]+encoder>XMAX) {
			//if instruction moves cutter beyond allowed range, display warning
			displayTextLine(5, "Cannot move to %d", nMotorEncoder[motorA]+encoder);
			displayTextLine(6, "Moved to max range.");

			encoder = XMAX - nMotorEncoder[motorA];
			wait1Msec(2500);
			displayTextLine(5,"");
			displayTextLine(6,"");
		}
		else if(nMotorEncoder[motorA]+encoder < 0 ) {
			//if instruction moves cutter beyond allowed range, display warning
			displayTextLine(5, "Cannot move to %d", nMotorEncoder[motorA]+encoder);
			displayTextLine(6, "Moved to max range.");
			encoder = -nMotorEncoder[motorA];
			wait1Msec(2500);
			displayTextLine(5,"");
			displayTextLine(6,"");
		}

		} else if(axis == 'Y') {
		if(nMotorEncoder[motorB]-encoder<YMAX) {
			//if instruction moves cutter beyond allowed range, display warning
			displayTextLine(5, "Cannot move to %d", nMotorEncoder[motorB]-encoder);
			displayTextLine(6, "Moved to MAX range.");
			encoder =  abs(YMAX) - abs(nMotorEncoder[motorB]);
			encoder = abs(encoder);
			wait1Msec(2500);
			displayTextLine(5,"");
			displayTextLine(6,"");
		}
		else if(nMotorEncoder[motorB]-encoder > 0 ) {
			//if instruction moves cutter beyond allowed range, display warning
			displayTextLine(5, "Cannot move to %d", nMotorEncoder[motorB]-encoder);
			displayTextLine(6, "Moved to MAX range.");
			encoder = nMotorEncoder[motorB];
			wait1Msec(2500);
			displayTextLine(5,"");
			displayTextLine(6,"");
		}
		} else {
		if(nMotorEncoder[motorC]+encoder>ZMAX) {
			//if instruction moves cutter beyond allowed range, display warning
			displayTextLine(5, "Cannot move to %d", nMotorEncoder[motorC]+encoder);
			displayTextLine(6, "Moved to MAX range.");
			encoder = ZMAX - nMotorEncoder[motorC];
			wait1Msec(2500);
			displayTextLine(5,"");
			displayTextLine(6,"");
		}
		else if(nMotorEncoder[motorC]+encoder < 0 ) {
			//if instruction moves cutter beyond allowed range, display warning
			displayTextLine(5, "Cannot move to %d", nMotorEncoder[motorC]+encoder);
			displayTextLine(6, "Moved to MIN range.");
			encoder = -nMotorEncoder[motorC];
			wait1Msec(2500);
			displayTextLine(5,"");
			displayTextLine(6,"");
		}
	}
	return encoder;
}


//Controlls movement of machine. Takes input from textfile and instructs motors
void movement(char axis, int &encoder, int &xyzSpeed) {

	//xAxis movement
	if (axis == 'X') {
		int tempX = nMotorEncoder[motorA];

		if(encoder>= 0) {
			while(nMotorEncoder[motorA] <= tempX + encoder) {
				displayTextLine(5,"X %d", nMotorEncoder[motorA]);
				motor[motorA] = xyzSpeed;
			}

			} else {
			while(nMotorEncoder[motorA] >= tempX + encoder) {
				displayTextLine(5,"X %d", nMotorEncoder[motorA]);
				motor[motorA] = -xyzSpeed;
			}


		}
		motor[motorA] = 0;

		wait1Msec(1000);
	}

	//yAxis movement
	else if (axis == 'Y') {
		int tempY = nMotorEncoder[motorB];
		if(encoder>=0) {
			while(nMotorEncoder[motorB] >= tempY - encoder) {
				motor[motorB] = -xyzSpeed;
				displayTextLine(5, "Y %d", nMotorEncoder[motorB]);
			}
			} else {
			while(nMotorEncoder[motorB] <= tempY - encoder) {
				motor[motorB] = xyzSpeed;
				displayTextLine(5, "Y %d", nMotorEncoder[motorB]);
			}
		}
		motor[motorB] = 0;
		wait1Msec(1000);

	}
	//zAxis movement
	else {
		int tempZ = nMotorEncoder[motorC];
		if(encoder >= 0) {
			while(nMotorEncoder[motorC] <= tempZ + encoder) {
				motor[motorC] = xyzSpeed;
				displayTextLine(5, "Z %d", nMotorEncoder[motorC]);
			}
			} else {
			while(nMotorEncoder[motorC] >= tempZ + encoder) {
				motor[motorC] = -xyzSpeed;
				displayTextLine(5, "Z %d", nMotorEncoder[motorC]);
			}

		}
		motor[motorC] = 0;
		wait1Msec(1000);

		//limit switches
		if(SensorValue[S1]==1) {
			nMotorEncoder[motorA]=0;
			} else if(SensorValue[S2]==1) {
			nMotorEncoder[motorB]=0;
			} else if(SensorValue[S3]==1) {
			nMotorEncoder[motorC]=0;
		}

	}
}

//reads the textfile for instructions
void readFile(TFileHandle &instructions, int &xyzSpeed) {
	char axis;
	int encoder;

	while(readCharPC(instructions,axis)&&readIntPC(instructions,encoder)) {
		displayTextLine(1,"Axis %c",axis);
		displayTextLine(2,"Encoder %d",encoder);
		encoder = checkLimit(axis,encoder);
		movement(axis, encoder, xyzSpeed);
	}
}

//ends operations after cut is complete
void shutOff(){

	//Z
	while(SensorValue[S3]==0) {
		motor[motorC] = -5;
	}
	motor[motorC]=0;

	motor[motorD] = 0;

	//X
	while(SensorValue[S1]==0) {
		motor[motorA] = -5;
	}
	motor[motorA]=0;

	//Y
	while(SensorValue[S2]==0) {
		motor[motorB] = 5;
	}
	motor[motorB]=0;


	stopAllTasks();
}

task millOperation() {

	int xyzSpeed = 5;
	int drillSpeed = 40;

	startUp(xyzSpeed, drillSpeed);

	displayTextLine(5,"RIGHT BUTTON TO START CUT");
	while(!getButtonPress(buttonRight)) {
	}
	motor[motorD]=drillSpeed;

	TFileHandle instructions;
	if(openReadPC(instructions,"INS.txt")) {
		displayTextLine(5,"FILE OPENED");
		wait1Msec(1000);
		readFile(instructions, xyzSpeed);
		} else {
		displayBigTextLine(5,"ERROR");
	}
	closeFilePC(instructions);
	shutOff();
}




//contains emergency stop
task main() {
	//All other program tasks need to be started.
	startTask(millOperation);

	while(true) {

		//Using CENTER button of EV3 brick as an emergency stop button
		//that can interupt code and immediately set all motor speeds to 0.
		while(!getButtonPress(buttonEnter)) {
		}
		if(getButtonPress(buttonEnter)) {
			while(getButtonPress(buttonEnter)) {}
			//All tasks need to be stopped when e-stop is pressed.
			stopTask(millOperation);

			motor[motorA] = motor[motorB] = motor[motorC] = motor[motorD] = 0;

			displayTextLine(5, "");
			displayTextLine(7,"");
			displayTextLine(8,"");

			displayBigTextLine(1, "ERROR");
			displayTextLine(5,"EMERGENCY STOP ACTIVATED");
			displayTextLine(7,"Press CENTER KEY to restart.");

			while(!getButtonPress(buttonEnter)) {}
			while(getButtonPress(buttonEnter)) {}
			displayBigTextLine(1,"");
			displayTextLine(5,"");
			displayTextLine(7,"");


			//restart tasks
			startTask(millOperation);


		}

		wait1Msec(50);
	}
}

//////////////////////////////
