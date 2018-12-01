#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

// First value is amount of elements
int melody_1[] = {7, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_A5, NOTE_E5, NOTE_A5};
int duration_1[] =   {0, 250,    250,    250,    500,    250,    500};

int melody_start[] = {7, NOTE_C4,   NOTE_C4,   NOTE_C4,   NOTE_C4, 0,   NOTE_E5};
int duration_start[] =   {0, 1000,   500,      500,       250,     100, 750};

int melody_error[] = {2, NOTE_E3};
int duration_error[] =   {0, 250};

int cable_1_led = 11;
int cable_1_button = 12;
int cable_2_led = 10;
int cable_2_button = 9;
int cable_3_led = 7;
int cable_3_button = 8;
int buzzer = 13;
int set_button = A5;
int reset_button = A3;
int status = 1;

void play_melody(int melody[], int duration[]) {
  for (int i = 1; i < melody[0]; i++) {
    if (melody[i] != 0) {
      tone(buzzer, melody[i]);
    }
    delay(duration[i]);
    noTone(buzzer);
    delay(duration[i] * 0.3);

  }
}

class button {
    int button_number;
    int led_pin;
    int button_pin;
    boolean too_fast = false;
    unsigned long too_fast_time = millis();
    int button_blink = 0;
    unsigned long previous_time = millis();
    boolean is_winner = false;
    boolean is_pressed = false;

  public:
    button(int in_button_number, int in_pin_led, int in_pin_button) {
      led_pin = in_pin_led;
      button_pin = in_pin_button;
      button_number = in_button_number;
      pinMode(led_pin, OUTPUT);
      pinMode(button_pin, INPUT_PULLUP);

    }
    void reset() {
      button_blink = 0;
      is_winner = false;
      previous_time = millis();
      light_off();
      too_fast = false;
      is_pressed = false;
    }

    void do_blink() {



    }

    void light_on() {
      digitalWrite(led_pin, HIGH);
    }
    void light_off() {
      digitalWrite(led_pin, LOW);
    }

    void light_winner() {
      if (is_winner) {
        light_on();
      } else {
        light_off();
      }
    }

    void check_pressed() {
      if (digitalRead(button_pin) == LOW) {
        Serial.print("Button ");
        Serial.print(button_number);
        Serial.println(" pressed");
        if (too_fast) {
          // Reset timer again for being too fast.
          too_fast_time = millis();
          light_off();
        } else if (status == 1) {
          // pressing too early
          light_off();
          too_fast_time = millis();
          too_fast = true;
          Serial.println("Too fast");
        } else if (status == 2) {
          is_winner = true;
          play_melody( melody_1, duration_1 );
          status = 3;
        } else {
          play_melody( melody_error, duration_error );
        }
      }
    }

    void update() {
      check_pressed();

      switch (status) {
        case 1:
          break;
        case 2:
          if (millis() < too_fast_time + 5000) {
            button_blink = 0;
          } else if (millis() - 100 > previous_time) {
            too_fast = false;
            previous_time = millis();
            if ( button_blink == 0) {
              button_blink = 1;
              light_on();
            } else {
              button_blink = 0;
              light_off();
            }
          }
          break;
        case 3:
          // A winner is found
          light_winner();
          break;
        default:

          break;
      }

    }

};

button button_1(1, cable_1_led, cable_1_button);
button button_2(2, cable_2_led, cable_2_button);
button button_3(3, cable_3_led, cable_3_button);




void setup() {
  Serial.begin(9600);


  pinMode(set_button, INPUT_PULLUP);
  pinMode(reset_button, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
}


/*

  LOGIK

  1. Nollställd
  Inga lampor lyser
  Ingen kan trycka
  Om någon trycker inaktiveras knappen i 5 sekunder

  2. Primad
  Alla lampor blinkar
  Om någon har en inaktiverad knapp gör ett tryck att man får vänta 5 sek till
  Den som trycker först winner

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

  button_1.update();
  button_2.update();
  button_3.update();

  int buttonVal_set = digitalRead(set_button);
  if (buttonVal_set == LOW) {
    if (status == 1) {
      Serial.println("Button set pressed");

      button_1.light_on();
      button_2.light_on();
      button_3.light_on();
      play_melody( melody_start, duration_start );
      // Check for cheating by pressing the button to early
      button_1.update();
      button_2.update();
      button_3.update();

      status = 2;
    } else {
      play_melody( melody_error, duration_error );
    }
  }

  int buttonVal_reset = digitalRead(reset_button);
  if (buttonVal_reset == LOW) {
    Serial.println("Button reset pressed");
    status = 1;
    button_1.reset();
    button_2.reset();
    button_3.reset();
  }
}
