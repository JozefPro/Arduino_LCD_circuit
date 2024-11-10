#include <LiquidCrystal.h>

const int button1_Pin = 6;   // Toggle addition/subtraction
const int button2_Pin = 7;   // Toggle multiplication/division
const int button3_Pin = 8;   // Confirm operation and calculate
const int button4_Pin = 10;  // Clear/reset calculation
const int button5_Pin = 9;   // Show result when pressed
const int analog2_Pin = A0;  // Analog input for adjustable second operand
const int analog1_Pin = A1;  // Analog input for adjustable first operand
const int ledPin = 13;       // LED output

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int secondNumber = 0;
int previousAnalog2Value = 0;   // Store the previous analog value
int currentAnalog2Value = 0;    // Variable to store the current analog reading

int firstNumber = 0;
int previousAnalog1Value = 0;   // Store the previous analog value
int currentAnalog1Value = 0;    // Variable to store the current analog reading

char operation = '+';
bool showRez = false;
double result = 0;
bool introScreen = true;

void setup() {
  lcd.begin(16, 2);
  pinMode(button1_Pin, INPUT);
  pinMode(button2_Pin, INPUT);
  pinMode(button3_Pin, INPUT);
  pinMode(button4_Pin, INPUT);
  pinMode(button5_Pin, INPUT);
  pinMode(ledPin, OUTPUT);


  // Display intro screen
  lcd.setCursor(0, 0);
  lcd.print("Skaiciuotuvas");
  lcd.setCursor(0, 1);
  lcd.print("Tinka VBE");
}

void loop() {
  // Check if button2 and button4 are pressed to show outro screen
  if (digitalRead(button2_Pin) == HIGH && digitalRead(button4_Pin) == HIGH) {
    displayOutroScreen();
    return;
  }
  if (introScreen) {
    // Wait for button1 and button3 to be pressed together to proceed
    if (digitalRead(button1_Pin) == HIGH && digitalRead(button3_Pin) == HIGH) {
      introScreen = false;
      lcd.clear();
    }
    return;
  }
  // Check if button2 and button4 are pressed to show outro screen
  if (digitalRead(button2_Pin) == HIGH && digitalRead(button4_Pin) == HIGH) {
    displayOutroScreen();
    return;
  }
  if (introScreen) {
    // Wait for button1 and button3 to be pressed together to proceed
    if (digitalRead(button1_Pin) == HIGH && digitalRead(button3_Pin) == HIGH) {
      introScreen = false;
      lcd.clear();
    }
    return;
  }

  // Read current analog values
  currentAnalog1Value = analogRead(analog1_Pin);
  currentAnalog2Value = analogRead(analog2_Pin);

  // Check if the analog value has changed
  if (currentAnalog1Value != previousAnalog1Value || currentAnalog2Value != previousAnalog2Value) 
  {
    showRez = false; // Reset result display when analog changes
    lcd.clear(); // Clear the LCD screen
    previousAnalog1Value = currentAnalog1Value;  // Update previous value
    previousAnalog2Value = currentAnalog2Value;  // Update previous value
    //displayLCD_NoRez(); // Show default screen
  }

  // Map the analog value to a range and display it
  firstNumber = map(currentAnalog1Value, 0, 1023, 100, -100);
  secondNumber = map(currentAnalog2Value, 0, 1023, 100, -100);

  // Check buttons and set operation only if exactly one is pressed
  int b1St = digitalRead(button1_Pin);
  int b2St = digitalRead(button2_Pin);
  int b3St = digitalRead(button3_Pin);
  int b4St = digitalRead(button4_Pin);
  int b5St = digitalRead(button5_Pin);

  if (b1St == HIGH && b2St == LOW && b3St == LOW && b4St == LOW && b5St == LOW)
  {
    operation = '+';
    showRez = false; // Reset result display when analog changes
  }
  else if (b2St == HIGH && b1St == LOW && b3St == LOW && b4St == LOW && b5St == LOW) 
  {
    operation = '-';
    showRez = false; // Reset result display when analog changes
  }
  else if (b3St == HIGH && b1St == LOW && b2St == LOW && b4St == LOW && b5St == LOW)
  {
    operation = '*';
    showRez = false; // Reset result display when analog changes
  }
  else if (b4St == HIGH && b1St == LOW && b2St == LOW && b3St == LOW && b5St == LOW)
  {
    operation = '/';
    showRez = false; // Reset result display when analog changes
  }

  // Show result when Button 5 is pressed
  if (digitalRead(button5_Pin) == HIGH) 
  {
    calculate();              // Calculate result
    displayLCD_Rez();         // Display result              
  } 
  else if (!showRez) displayLCD_NoRez();

  // Show result in decimal and binary and blink binary if button1, button3, and button5 are pressed together
  if (b1St == HIGH && b3St == HIGH && b5St == HIGH) {
    displayLCD_DecimalAndBinary(result);
    blinkBinaryResult((int)result);
  }
}

void displayLCD_NoRez()
{
  lcd.setCursor(0, 0);
  lcd.print("A");
  lcd.print(operation);
  lcd.print("B:");
  lcd.setCursor(5, 0);
  lcd.print(firstNumber);
  lcd.setCursor(10, 0);
  lcd.print(operation);
  lcd.setCursor(12, 0);
  lcd.print(secondNumber);
  delay(60);
}

void displayLCD_Rez()
{
  lcd.setCursor(0, 0);
  lcd.print("A");
  lcd.print(operation);
  lcd.print("B:");
  lcd.setCursor(5, 0);
  lcd.print(firstNumber);
  lcd.setCursor(10, 0);
  lcd.print(operation);
  lcd.setCursor(12, 0);
  lcd.print(secondNumber);

  lcd.setCursor(0, 1);
  lcd.print("Result: ");
  lcd.print(result);
  delay(60);
}

void displayLCD_DecimalAndBinary(double value) {
  int intValue = (int)value;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DEC: ");
  lcd.print(intValue);
  lcd.setCursor(0, 1);
  if (abs(intValue) > 128) {
    lcd.print("Per didelis skaicius");
    delay(60);
    return;
  }
  lcd.print("BIN: ");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DEC: ");
  lcd.print(intValue);
  lcd.setCursor(0, 1);
  lcd.print("BIN: ");
  if (intValue < 0) {
    lcd.print("1.");  // Add '1.' to indicate negative value
    intValue = abs(intValue);
  } else {
    lcd.print("0.");  // Add '0.' to indicate positive value
  }
  for (int i = 6; i >= 0; i--) {
    lcd.print((intValue >> i) & 1);
  }
  delay(60);
}

void displayOutroScreen() {
  introScreen = true;  // Set introScreen to true to require reactivation
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Aciu, kad");
  lcd.setCursor(0, 1);
  lcd.print("skaiciavai <3");
  delay(3000); // Display for 2 seconds before clearing
  lcd.clear();
}

void calculate() {
   switch (operation) {
    case '+':
      result = firstNumber + secondNumber;
      break;

    case '-':
      result = firstNumber - secondNumber;
      break;

    case '*':
      result = firstNumber * secondNumber;
      break;

    case '/':
      if (secondNumber != 0) {
          result = (float)firstNumber / secondNumber;
      } else {
        result = 0;  // Prevent division by zero
      }
      break;
   }
}

void blinkBinaryResult(int value) {
  if (value > 128) {
    return;
  }
  if (value < 0) {
    digitalWrite(ledPin, HIGH);  // Blink to indicate negative value
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
    value = abs(value);
  }
  for (int i = 7; i >= 0; i--) {
    int bit = (value >> i) & 1;
    if (bit == 1) {
      digitalWrite(ledPin, HIGH);
      delay(300);  // Blink duration for '1'
    } else {
      digitalWrite(ledPin, LOW);
      delay(300);  // Blink duration for '0'
    }
    digitalWrite(ledPin, LOW);
    delay(100);  // Short delay between bits
  }
  delay(600);  // Delay between displaying full binary sequences
}
