#include <LiquidCrystal.h>

const int button1_Pin = 6;   // Toggle LED on/off
const int button2_Pin = 7;   // Toggle blink mode
const int ledPin = 13;       // LED output
const int analog_Pin = A0;   // Analog input for adjustable blink speed

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

bool ledState = LOW;         // Stores the on/off state of the LED
bool blinkMode = false;      // Stores whether blink mode is active
int blinkSpeed = 500;        // Default blink speed
unsigned long lastLCDUpdate = 0; // Timestamp for the last LCD update
const unsigned long lcdUpdateInterval = 500; // LCD update interval in milliseconds
unsigned long lastBlinkUpdate = 0; // Timestamp for the last LED blink update
unsigned long blinkLCDUpdateInterval = 50; // Faster LCD update interval when blink mode is on

void setup() {
  Serial.begin(9600);  
  lcd.begin(16, 2);           // Initialize a 16x2 LCD
  lcd.clear();
  pinMode(ledPin, OUTPUT);
  pinMode(button1_Pin, INPUT);
  pinMode(button2_Pin, INPUT);
}

void loop() {
  // Check if Button 1 is pressed to toggle LED on/off
  if (digitalRead(button1_Pin) == HIGH) {
    ledState = !ledState;   // Toggle LED state
    if (!ledState) blinkMode = false; // Turn off blink mode if LED is off
    updateLED();
    delay(300);              // Debounce delay
  }

  // Check if Button 2 is pressed to enable/disable blink mode, only if LED is on
  if (digitalRead(button2_Pin) == HIGH && ledState) {
    blinkMode = !blinkMode;  // Toggle blink mode
    delay(300);              // Debounce delay
  }

  // Control LED behavior based on the state of blink mode and LED state
  if (blinkMode && ledState) {
    blinkSpeed = analogRead(analog_Pin);           // Read analog value for speed
    blinkSpeed = map(blinkSpeed, 0, 1023, 100, 2000); // Map to reduced delay range (100 to 2000 ms)
    unsigned long currentMillis = millis();
    if (currentMillis - lastBlinkUpdate >= blinkSpeed) {
      lastBlinkUpdate = currentMillis;
      digitalWrite(ledPin, !digitalRead(ledPin));  // Toggle LED state
    }
  } else if (ledState) {
    // If blink mode is off and LED is toggled on, keep the LED on
    digitalWrite(ledPin, HIGH);
  } else {
    // If blink mode is off and LED is toggled off, turn the LED off
    digitalWrite(ledPin, LOW);
  }

  // Update LCD at a different refresh rate depending on blink mode
  unsigned long currentLCDInterval = blinkMode ? blinkLCDUpdateInterval : lcdUpdateInterval;
  if (millis() - lastLCDUpdate >= currentLCDInterval) {
    updateLCD();
    lastLCDUpdate = millis();
  }
}

void updateLED() {
  // Update LED based on the ledState variable when blink mode is disabled
  if (ledState) {
    digitalWrite(ledPin, HIGH);  // Turn LED on
    Serial.println("LED ON");    // Print LED state as ON
  } else {
    digitalWrite(ledPin, LOW);   // Turn LED off
    Serial.println("LED OFF");   // Print LED state as OFF
  }
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  if (ledState) {
    lcd.print("LED ON");
    lcd.setCursor(7, 0);
    if (blinkMode) {
      float frequency = 1000.0 / blinkSpeed;  // Calculate frequency in Hz
      lcd.print("Blink ON ");
      lcd.setCursor(0, 1);
      lcd.print("Speed: ");
      lcd.print(frequency, 2);                // Display speed with 2 decimals
      lcd.print(" Hz");
    } else {
      lcd.print("Blink OFF");
    }
  } else {
    lcd.print("LED OFF");
  }
}
