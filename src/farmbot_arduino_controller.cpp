// Do not remove the include below
#include "farmbot_arduino_controller.h"
#include "pins.h"
#include "Config.h"

#include "ServoControl.h"

static char commandEndChar = 0x0A;
static GCodeProcessor* gCodeProcessor = new GCodeProcessor();

unsigned long lastAction;
unsigned long currentTime;

//The setup function is called once at startup of the sketch
void setup() {
	pinMode(X_STEP_PIN  , OUTPUT);
	pinMode(X_DIR_PIN   , OUTPUT);
	pinMode(X_ENABLE_PIN, OUTPUT);
	pinMode(X_MIN_PIN   , INPUT );
	pinMode(X_MAX_PIN   , INPUT );

	pinMode(Y_STEP_PIN  , OUTPUT);
	pinMode(Y_DIR_PIN   , OUTPUT);
	pinMode(Y_ENABLE_PIN, OUTPUT);
	pinMode(Y_MIN_PIN   , INPUT );
	pinMode(Y_MAX_PIN   , INPUT );

	pinMode(Z_STEP_PIN  , OUTPUT);
	pinMode(Z_DIR_PIN   , OUTPUT);
	pinMode(Z_ENABLE_PIN, OUTPUT);
	pinMode(Z_MIN_PIN   , INPUT );
	pinMode(Z_MAX_PIN   , INPUT );

	pinMode(HEATER_0_PIN, OUTPUT);
	pinMode(HEATER_1_PIN, OUTPUT);
	pinMode(FAN_PIN     , OUTPUT);
	pinMode(LED_PIN     , OUTPUT);

	//pinMode(SERVO_0_PIN , OUTPUT);
	//pinMode(SERVO_1_PIN , OUTPUT);

	digitalWrite(X_ENABLE_PIN, HIGH);
	digitalWrite(Y_ENABLE_PIN, HIGH);
	digitalWrite(Z_ENABLE_PIN, HIGH);

	Serial.begin(115200);

	ServoControl::getInstance()->attach();

	lastAction = millis();

}

// The loop function is called in an endless loop
void loop() {



	if (Serial.available()) {

		String commandString = Serial.readStringUntil(commandEndChar);
		if (commandString && commandString.length() > 0) {

			lastAction = millis();

			Command* command = new Command(commandString);
			if(LOGGING) {
				command->print();
			}
			gCodeProcessor->execute(command);
			free(command);
		}
	}
	delay(10);

	currentTime = millis();
	if (currentTime < lastAction) {
		// If the device timer overruns, reset the idle counter
		lastAction = millis();
	}
	else {

		if ((currentTime - lastAction) > 5000) {
			// After an idle time, send the idle message
			Serial.print("R00\r\n");
			lastAction = millis();
		}
	}

}
