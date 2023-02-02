const uint8_t BUTTON_SHORT = 4;
const uint8_t BUTTON_LONG = 3;
const uint8_t SWITCH_ON = 2;
const uint8_t TRIAC_GATE = 0;

const uint32_t LONG_ON  = 45 * 60 * 1000ul;
const uint32_t SHORT_ON = 10 * 60 * 1000ul;

void setup() {
  pinMode(BUTTON_SHORT, INPUT_PULLUP);
  pinMode(BUTTON_LONG, INPUT_PULLUP);
  pinMode(SWITCH_ON, INPUT_PULLUP);
  pinMode(TRIAC_GATE, OUTPUT);
  digitalWrite(TRIAC_GATE, HIGH);           // TRIAC gate is active LOW.
}

bool ventilationOn;
uint32_t ventilationTime;
uint32_t lastOnRequest;

void loop() {
  uint32_t timeOn = millis() - lastOnRequest;
  uint32_t timeRemaining = ventilationTime - timeOn;
  if (digitalRead(SWITCH_ON) == LOW) {
    ventilationOn = true;
  }
  else {
    if (ventilationOn) {
      if (timeOn > ventilationTime) {
        ventilationOn = false;
        timeRemaining = 0;
        ventilationTime = 0;
      }
    }
    if (digitalRead(BUTTON_SHORT) == LOW) {
      ventilationOn = true;
      if (timeRemaining < SHORT_ON) {
        ventilationTime = SHORT_ON;
        lastOnRequest = millis();
      }
    }
    if (digitalRead(BUTTON_LONG) == LOW) {
      ventilationOn = true;
      if (timeRemaining < LONG_ON) {
        ventilationTime = LONG_ON;
        lastOnRequest = millis();
      }
    }
  }
  digitalWrite(TRIAC_GATE, ventilationOn ? LOW : HIGH);
}
