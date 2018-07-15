int pin_led_1=2;
int pin_button_1=5;
int pin_buzzer=3;

void setup() {
  //start serial connection
  Serial.begin(9600);
  //configure pin 2 as an input and enable the internal pull-up resistor
  pinMode(pin_button_1, INPUT_PULLUP);
  pinMode(pin_led_1, OUTPUT);
  pinMode(pin_buzzer, OUTPUT);  

}


void buzz(int targetPin, long frequency, long length) {
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }

}

void loop() {
  int sensorVal = digitalRead(pin_button_1);
  Serial.println(sensorVal);

  if (sensorVal == LOW) {
    digitalWrite(pin_led_1, LOW);
  } else {
    digitalWrite(pin_led_1, HIGH);
    buzz(pin_buzzer, 1000, 250);
    buzz(pin_buzzer, 3000, 250);
    buzz(pin_buzzer, 700, 250);
    buzz(pin_buzzer, 1000, 250);
  }
}
