
enum ledStateEnum {
  LED_unknown,
  LED_OFF,
  LED_SLOW,
  LED_FAST,
  LED_ON
};

enum senseoStateEnum {
  SENSEO_unknown,
  SENSEO_OFF,
  SENSEO_HEATING,
  SENSEO_READY,
  SENSEO_BREWING,
  SENSEO_NOWATER
};


// senseo sm stuff
int senseoStatePrev;
int senseoState;
bool hasChanged = false;
unsigned long lastStateChangeMillis = 0;
unsigned long timeInLastState;

// led sm stuff
int ocSenseLedPin;
bool ledChanged = false;
unsigned long ledChangeMillis;
unsigned long prevLedChangeMillis = 0;
ledStateEnum ledState = LED_unknown;
ledStateEnum ledStatePrev = LED_unknown;

// LED state machine
// Measure your timings by activating debugging in ledChangedRoutine()
static const int pulseDurLedSlow = 1000;                   // duration of one pulse when LED is blinking slow in milliseconds (state LED_SLOW)
static const int pulseDurLedFast = 100;                    // duration of one pulse when LED is blinking fast in milliseconds (state LED_FAST)
static const int pulseDurTolerance = 10;                   // tolerance for pulse duration. With the tested senseo, tolerance was not more than +-1ms
static const int pulseContThreshold = 2 * pulseDurLedSlow; // time before switching to continuous LED state (state LED_ON and state LED_OFF)

// Senseo state machine (in seconds)
static const int HeatingTime = 70;
static const int HeatingTimeTol = 15;
static const int Brew1CupSeconds = 21;
static const int Brew2CupSeconds = 41;
static const int BrewHeat1CupSeconds = 55;
static const int BrewHeat2CupSeconds = 85;

static const int LedIgnoreChangeDuration = 5;

void updateSenseoState(int ledState) {
  senseoStatePrev = senseoState;
  hasChanged = false;

  switch (senseoState) {
   case SENSEO_unknown:
      if (ledState == LED_OFF) senseoState = SENSEO_OFF;
      else if (ledState == LED_SLOW) senseoState = SENSEO_HEATING;
      else if (ledState == LED_FAST) senseoState = SENSEO_NOWATER;
      else if (ledState == LED_ON) senseoState = SENSEO_READY;
      break;
    case SENSEO_OFF:
      if (ledState == LED_OFF) senseoState = SENSEO_OFF;
      else if (ledState == LED_SLOW) senseoState = SENSEO_HEATING;
      else if (ledState == LED_FAST) senseoState = SENSEO_NOWATER;
      else if (ledState == LED_ON) senseoState = SENSEO_READY;
      break;
    case SENSEO_HEATING:
      if (ledState == LED_OFF) senseoState = SENSEO_OFF;
      else if (ledState == LED_FAST) senseoState = SENSEO_NOWATER;
      else if (ledState == LED_ON) senseoState = SENSEO_READY;
      else if ((millis() - lastStateChangeMillis) > (1000 * (HeatingTime + HeatingTimeTol))) {
        // Heating takes more time then expected, assume immediate brew.
        senseoState = SENSEO_BREWING;
      }
      break;
    case SENSEO_READY:
      if (ledState == LED_ON) senseoState = SENSEO_READY;
      else if (ledState == LED_OFF) senseoState = SENSEO_OFF;
      else if (ledState == LED_SLOW) senseoState = SENSEO_BREWING;
      else if (ledState == LED_FAST) senseoState = SENSEO_NOWATER;
      break;
    case SENSEO_BREWING:
      if (ledState == LED_OFF) senseoState = SENSEO_OFF;
      else if (ledState == LED_FAST) {
        //cup was brewed
        senseoState = SENSEO_NOWATER;
      } else if (ledState == LED_ON) {
        //cup was brewed
        senseoState = SENSEO_READY;
      }
      break;
    case SENSEO_NOWATER:
      if (ledState == LED_FAST) senseoState = SENSEO_NOWATER;
      else if (ledState == LED_SLOW) senseoState = SENSEO_HEATING;
      else if (ledState == LED_ON) senseoState = SENSEO_READY;
      else if (ledState == LED_OFF) senseoState = SENSEO_OFF;
      break;
  }
  if (senseoStatePrev != senseoState) {
    hasChanged = true;
    unsigned long now = millis();
    timeInLastState = (unsigned long)(now - lastStateChangeMillis);
    lastStateChangeMillis = now;
  }
}

void pinStateToggled() {
  unsigned long now = millis();
  if (now - ledChangeMillis <= LedIgnoreChangeDuration) return; // simple debouncer
  prevLedChangeMillis = ledChangeMillis;
  ledChangeMillis = now;
  ledChanged = true;
}

void updateLEDState() {
  ledStatePrev = ledState;
  if (ledChanged) {
    int pulseDuration = ledChangeMillis - prevLedChangeMillis;

    if (abs(pulseDuration - pulseDurLedFast) < pulseDurTolerance) {
        ledState = LED_FAST;
    } else if (abs(pulseDuration - pulseDurLedSlow) < pulseDurTolerance) {
        ledState = LED_SLOW;
    } else {
      // Nothing to do here.
      // pulseDuration could be below (user interaction) or above (end of a continuous state) the known times.
      // No actions needed.
    }
    ledChanged = false;
  }

  // decide if LED is not blinking but in a continuous state
  int t = (unsigned long)(millis() - ledChangeMillis);
  if (( t > pulseContThreshold) && (t < 3 * pulseContThreshold)) {
    ledState = !digitalRead(ocSenseLedPin) ? LED_ON : LED_OFF;
  }
}

String getSenseoStateAsString() {
  if (senseoState == SENSEO_OFF) return "SENSEO_OFF";
  else if (senseoState == SENSEO_HEATING) return "SENSEO_HEATING";
  else if (senseoState == SENSEO_READY) return "SENSEO_READY";
  else if (senseoState == SENSEO_BREWING) return "SENSEO_BREWING";
  else if (senseoState == SENSEO_NOWATER) return "SENSEO_NOWATER";
  else return "SENSEO_unknown";
}

String getSenseoStatePrevAsString() {
  if (senseoStatePrev == SENSEO_OFF) return "SENSEO_OFF";
  else if (senseoStatePrev == SENSEO_HEATING) return "SENSEO_HEATING";
  else if (senseoStatePrev == SENSEO_READY) return "SENSEO_READY";
  else if (senseoStatePrev == SENSEO_BREWING) return "SENSEO_BREWING";
  else if (senseoStatePrev == SENSEO_NOWATER) return "SENSEO_NOWATER";
  else return "SENSEO_unknown";
}

String getLEDStateAsString() {
  if (ledState == LED_OFF) return "LED_OFF";
  else if (ledState == LED_SLOW) return "LED_SLOW";
  else if (ledState == LED_FAST) return "LED_FAST";
  else if (ledState == LED_ON) return "LED_ON";
  else return "LED_unknown";
}