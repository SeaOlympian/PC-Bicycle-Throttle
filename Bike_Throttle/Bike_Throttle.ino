//These variables can be changed based on user need and preference.
int Hall_effect_pin = 14;   //Stores the pin number that the hall effect sensor is plugged into
float maximumRPM = 90;  //Set to the maximum RPM that you want to pedal at for full throttle
float minimumRPM = 50;  //Set to the RPM that will be the starting point for throttle input (NOTE: this input is the highest RPM that will be equal to ZERO throttle input)

float RPMRange = maximumRPM - minimumRPM;

//Variable to cout passes of the magnet
volatile byte count = 0;

int rpm = 0;

float throttle = 0;

unsigned long previousTime = 0;

//----------FUNCTIONS----------

void count_function () {
  count++;
}

//--------------------

void rpm_calc () {
  delay(1000);

  //Serial.println(count);

   
  //Turns off the interrupt command
  detachInterrupt(0);

  //RPM Calculation NOTE: millis counts the milliseconds since the program started
  rpm = 60*1000/(millis() - previousTime)*count;

  //Calculate previous time
  previousTime = millis();

  //Reset the count to prevent growing rpm reading
  count = 0;

  Serial.print("RPM = ");
  Serial.println(rpm);
  
  attachInterrupt(digitalPinToInterrupt(Hall_effect_pin), count_function, RISING);
}

//--------------------

void throttle_calc () {
  //Thanks to Wheatly_Inc, Wolfhaus, and FyronFD
  throttle = (rpm - minimumRPM)/RPMRange;

  Serial.print("Throttle %: ");
  Serial.println(throttle);
}

//----------MAIN----------

void setup() {
  // put your setup code here, to run once:
  pinMode(Hall_effect_pin, INPUT_PULLUP);

  //Begin serial console
  Serial.begin(9600);

  //Sets up the interupt pin and triggered command
  attachInterrupt(digitalPinToInterrupt(Hall_effect_pin), count_function, RISING);
}

//--------------------

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(digitalRead(Hall_effect_pin));
  rpm_calc();

  throttle_calc();
}
