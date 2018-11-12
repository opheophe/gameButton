int cable_1_led = 11;
int cable_1_button = 12;
int cable_2_led = 10;
int cable_2_button = 9;

int set_button = A5;
int reset_button = A3;
int status = 1;
unsigned long previous_time;
int blink_status = 0;
int winner = 0;

void setup() {
  Serial.begin(9600);

  pinMode(cable_1_button, INPUT_PULLUP);
  pinMode(cable_1_led, OUTPUT);
  pinMode(cable_2_button, INPUT_PULLUP);
  pinMode(cable_2_led, OUTPUT);
  pinMode(set_button, INPUT_PULLUP);
  pinMode(reset_button, INPUT_PULLUP);
}


/*

  LOGIK

  1. Nollställd
  Inga lampor lyser
  Ingen kan trycka

  2. Primad
  Alla lampor blinkar
  Alla kan trycka
  Den som trycker winner

  3. Vinst
  Lampan som tryckte är tänd
  Alla andra släkta
  Ingen kan trycka


  SET
  Sätter alltid till primad

  RESET
  Sätter alltid till nollställd
*/

void loop() {

  switch (status) {
    case 1:
      // RESET, do nothing ontill SET is pressed
      break;
    case 2:
      if (millis() - 100 > previous_time) {
        previous_time = millis();
        if (blink_status == 0) {
          blink_status = 1;
          digitalWrite(cable_1_led, HIGH);
          digitalWrite(cable_2_led, HIGH);
        } else {
          blink_status = 0;
          digitalWrite(cable_1_led, LOW);
          digitalWrite(cable_2_led, LOW);
        }
        Serial.println("Blink");
      }

      break;
    case 3:
      switch (winner) {
        case 1:
          digitalWrite(cable_1_led, HIGH);
          digitalWrite(cable_2_led, LOW);
          break;
        case 2:
          digitalWrite(cable_1_led, LOW);
          digitalWrite(cable_2_led, HIGH);
          break;
        default:

          break;
      }



      break;
    default:

      break;
  }

  int buttonVal_1 = digitalRead(cable_1_button);
  if (buttonVal_1 == LOW) {
    Serial.println("Button 1 pressed");
    if (status == 2) {
      winner = 1;
      status = 3;
    }
  }

  int buttonVal_2 = digitalRead(cable_2_button);
  if (buttonVal_2 == LOW) {
    Serial.println("Button 2 pressed");
    if (status == 2) {
      winner = 2;
      status = 3;
    }
  }


  
  int buttonVal_set = digitalRead(set_button);
  if (buttonVal_set == LOW) {
    Serial.println("Button set pressed");
    Serial.println("Status = 2");
    status = 2;
    blink_status = 1;
    previous_time = millis();
    digitalWrite(cable_1_led, HIGH);
    digitalWrite(cable_2_led, HIGH);
    winner = 0;
  }

  int buttonVal_reset = digitalRead(reset_button);
  if (buttonVal_reset == LOW) {
    Serial.println("Button reset pressed");
    Serial.println("Status = 1");
    status = 1;
    digitalWrite(cable_1_led, LOW);
    digitalWrite(cable_2_led, LOW);
    blink_status = 0;
    winner = 0;
  }
}
