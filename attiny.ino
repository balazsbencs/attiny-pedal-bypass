#include <EEPROM.h>

const uint8_t relayPin = PIN_PA6;   // PA6
const uint8_t ledPin = PIN_PA3;     // PA3
const uint8_t switchPin = PIN_PA0;  // PA0

const int stateEEPROMAddress = 0;

bool relayState = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

bool lastButtonState = HIGH;
bool buttonState = HIGH;

void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);
  
  // Read the last saved state from EEPROM at the specified address.
  // EEPROM.read() returns a byte (0-255). We stored 0 for off and 1 for on.
  byte savedState = EEPROM.read(stateEEPROMAddress);

  // Set the relayState variable based on the saved state.
  // If savedState is 1, relayState becomes true. Otherwise, it's false.
  relayState = (savedState == 1);

  digitalWrite(relayPin, LOW);  // Start with relay and LED off
  digitalWrite(ledPin, LOW);
}

void loop() {
  bool reading = digitalRead(switchPin);

  // Debounce logic
  if (reading != lastButtonState) {
    lastDebounceTime = millis(); // Reset the debounce timer
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // If the button state has changed (after debounce)
    if (reading != buttonState) {
      buttonState = reading;

      // If the button was pressed (LOW state)
      if (buttonState == LOW) {
        // Toggle relay and LED state
        relayState = !relayState;
        digitalWrite(relayPin, relayState);
        digitalWrite(ledPin, relayState);
        EEPROM.update(stateEEPROMAddress, relayState ? 1 : 0);
      }
    }
  }

  lastButtonState = reading;
}
