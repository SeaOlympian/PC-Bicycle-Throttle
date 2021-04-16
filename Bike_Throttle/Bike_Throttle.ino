#include <XInput.h>

//These variables can be changed based on user need and preference.
int Hall_effect_pin = 18;   //Stores the pin number that the hall effect sensor is plugged into.
float maximumRPM = 90;  //Set to the maximum RPM that you want to pedal at for full throttle.
float minimumRPM = 50;  //Set to the RPM that will be the starting point for throttle input (NOTE: this input is the highest RPM that will be equal to ZERO throttle input).
int debug = 4;  //Outputs debung information to the serial monitor. Set to 0 for no debug options, 1 for hall effect sensor testing, 2 for RPM calculation testing, and 3 for throttle calculation testing.
const int Pin_LeftJoyX = A7;
const int Pin_LeftJoyY = A8;

const int ADC_Max = 1023;
float RPMRange = maximumRPM - minimumRPM;

//Variable to cout passes of the magnet
volatile byte count = 0;

float rpm = 0;

float throttle = 0;

unsigned long previousTime = 0;

//----------FUNCTIONS----------

void count_function () {
  count++;
}

//--------------------

int rpm_calc () {
  delay(1000);
   
  //Turns off the interrupt command
  detachInterrupt(0);

  //RPM Calculation NOTE: millis counts the milliseconds since the program started
  rpm = 60*1000/(millis() - previousTime)*count;

  //Calculate previous time
  previousTime = millis();

  //Reset the count to prevent growing rpm reading
  count = 0;

  if (debug == 2 || debug == 4){
    Serial.print("RPM = ");
    Serial.println(rpm);
  }
  
  attachInterrupt(digitalPinToInterrupt(Hall_effect_pin), count_function, RISING);

  return rpm;
}

//----------MAIN----------

void setup() {
  // put your setup code here, to run once:
  pinMode(Hall_effect_pin, INPUT_PULLUP);

  if (debug != 0)
    //Begin serial console
    Serial.begin(9600);

  XInput.setJoystickRange(0, ADC_Max);
  XInput.setTriggerRange(0.5, 0.9);

  //Sets up the interupt pin and triggered command
  attachInterrupt(digitalPinToInterrupt(Hall_effect_pin), count_function, RISING);
  
  XInput.begin();
}

//--------------------

void loop() {
  if (debug == 1 || debug == 4)
    Serial.println(digitalRead(Hall_effect_pin));
    
  float throttle_input = rpm_calc();
  
  int leftJoyX = analogRead(Pin_LeftJoyX);
  int leftJoyY = analogRead(Pin_LeftJoyY);

  XInput.setTrigger(TRIGGER_LEFT, throttle_input);  
  //XInput.setJoystickX(JOY_LEFT, leftJoyX);
  //XInput.setJoystickY(JOY_LEFT, leftJoyY);
}
