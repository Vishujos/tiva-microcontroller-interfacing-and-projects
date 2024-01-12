#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int setButton = PF_4;
const int modeButton = PF_0;
const int buzzerPin = PE_3;

int Hour = 0;
int Minute = 0;
int seconds = 0;
bool alarmOn = false;
int alarmHour = 0;
bool alarmSettingMode = false;
int alarmMinute = 0;

bool updateTime = true; // Flag to control whether to update the running time

void setup() {
  lcd.begin(16, 2);
  pinMode(setButton, INPUT_PULLUP);
  pinMode(modeButton, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  // Initialize the clock with a specific time if needed
  // Example: setClockTime(10, 30);
}

void updateCurrentTime() {
  if (updateTime) { // Only update time when the flag is true
    seconds++;
    if (seconds > 59) {
      seconds = 0;
      Minute++;
    }
    if (Minute > 59) {
      Minute = 0;
      Hour++;
    }
    if (Hour > 23) {
      Hour = 0;
    }
  }
}



void setAlarmTime() {
  int buttonHour = digitalRead(setButton);
  int buttonMinute = digitalRead(modeButton);

  if (buttonHour == LOW) {
    alarmHour = (alarmHour + 1) % 24;
    delay(500); // Debounce delay
  }

  if (buttonMinute == LOW) {
    alarmMinute = (alarmMinute + 1) % 60;
    delay(500); // Debounce delay
  }

  lcd.clear();

  if (alarmSettingMode) {
    lcd.print("ALARM SET");
    lcd.setCursor(0, 1);
    lcd.print("Set Alarm: ");
    lcd.print(alarmHour);
    lcd.print(":");
    lcd.print(alarmMinute);
    updateTime = false; // Disable running time update
  } else {
    updateTime = false; // Enable running time update
    lcd.setCursor(0, 0);
    lcd.print("Running : ");
    lcd.print(Hour);
    lcd.print(":");
    lcd.print(Minute);
    lcd.print(":");
    lcd.print(seconds);
    updateTime = true;
    lcd.setCursor(0, 1);
    lcd.print("Alarm Set: ");
    lcd.print(alarmHour);
    lcd.print(":");
    lcd.print(alarmMinute);
  }

  // Check if the alarm should be activated
  if (alarmOn && Hour == alarmHour && Minute == alarmMinute) {
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
  }
}

void setTime() {
  int buttonHour = digitalRead(setButton);
  int buttonMinute = digitalRead(modeButton);

  if (!alarmSettingMode) { // Only update time when not in alarm setting mode
    if (buttonHour == LOW) {
      Hour = (Hour + 1) % 24;
      delay(500); // Debounce delay
    }

    if (buttonMinute == LOW) {
      Minute = (Minute + 1) % 60;
      if (Minute == 0) {
        Hour = (Hour + 1) % 24;
      }
      delay(500); // Debounce delay
    }
  }
}

void loop() {
  setTime();
  delay(1000);
  setAlarmTime();
  delay(1000);
  updateCurrentTime();
  
}
