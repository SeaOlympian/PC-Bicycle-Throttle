//Variables that can be changed
int maximumRPM = 90;
float minimumRPM = 50;
int RPMRange = maximumRPM - minimumRPM;


//Variable to store pin number
int input = 14;

//Variable to cout passes of the magnet
volatile byte count = 0;

int rpm = 0;

float throttle = 0;

unsigned long previousTime = 0;



void count_function () {
  count++;
}



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
  
  attachInterrupt(digitalPinToInterrupt(input), count_function, RISING);
}



void throttle_calc () {
  //Thanks to Wheatly_Inc, Wolfhaus, and FyronFD
  throttle = (rpm - minimumRPM)/RPMRange;

  Serial.print("Throttle %: ");
  Serial.println(throttle);
}



void setup() {
  // put your setup code here, to run once:
  pinMode(input, INPUT_PULLUP);

  //Begin serial console
  Serial.begin(9600);

  //Sets up the interupt pin and triggered command
  attachInterrupt(digitalPinToInterrupt(input), count_function, RISING);
}



void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(digitalRead(input));
  rpm_calc();

  throttle_calc();
}
