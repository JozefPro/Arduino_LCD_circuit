#include <LiquidCrystal.h>

const int button1_Pin = 6;   // Toggle addition/subtraction
const int button2_Pin = 7;   // Multiplication challenge
const int button3_Pin = 8;   // Confirm operation and calculate
const int button4_Pin = 10;  // Clear/reset calculation
const int button5_Pin = 9;   // Show result when pressed
const int analog2_Pin = A0;  // Analog input for adjustable answer
const int ledPin = 13;       // LED output

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int firstNumber = 0;
int secondNumber = 0;
int userAnswer = 0;
int correctAnswer = 0;
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
  lcd.print("Portable math");
  lcd.setCursor(0, 1);
  lcd.print("teacher");
}

void loop() {
  static int previousAnalogValue = -101;  // Store the previous analog value
  // Check if button2 and button4 are pressed to show outro screen
  if (digitalRead(button2_Pin) == HIGH) {
    displayOutroScreen();
    return;
  }

  // Show intro screen until button1  pressed
  if (introScreen) {
    if (digitalRead(button1_Pin) == HIGH) {
      introScreen = false;
      lcd.clear();
            generateChallenge();
    }
    return;
  }

  // Handle challenge mode
  {
    int currentAnalogValue = analogRead(analog2_Pin);
    userAnswer = map(currentAnalogValue, 0, 1023, 100, 0);
    if (userAnswer != previousAnalogValue) {
      lcd.clear();
      previousAnalogValue = userAnswer;
      lcd.setCursor(0, 0);
      lcd.print(firstNumber);
      lcd.print(" * ");
      lcd.print(secondNumber);
      lcd.print(" = ?");
    }
    lcd.setCursor(0, 1);
    lcd.print("Your answer: ");
    lcd.print(userAnswer);

    // Confirm answer with button3
    if (digitalRead(button3_Pin) == HIGH) {
      if (userAnswer == correctAnswer) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Correct!");
        digitalWrite(ledPin, HIGH);
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Wrong!  ");
        lcd.setCursor(0, 1);
        lcd.print("Correct: ");
        lcd.print(correctAnswer);
      }
      delay(2000);
      lcd.clear();
      generateChallenge();
    }
  }
}

void generateChallenge() {
  digitalWrite(ledPin, LOW);  // Turn off LED when generating a new challenge
  firstNumber = random(1, 11);  // Generate a number between 1 and 10
  secondNumber = random(1, 11); // Generate a number between 1 and 10
  correctAnswer = firstNumber * secondNumber;

  lcd.setCursor(0, 0);
  lcd.print(firstNumber);
  lcd.print(" * ");
  lcd.print(secondNumber);
  lcd.print(" = ?");
}

void displayOutroScreen() {
  introScreen = true;  // Set introScreen to true to require reactivation
    lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Thanks for");
  lcd.setCursor(0, 1);
  lcd.print("calculating <3");
  delay(2000); // Display for 2 seconds before clearing
  lcd.clear();
}
