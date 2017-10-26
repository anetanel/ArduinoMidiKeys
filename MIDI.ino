

// include the library code:
#include <LiquidCrystal.h>


#define KEY_LEFT_CTRL   0x01
#define KEY_LEFT_SHIFT  0x02
#define KEY_LEFT_ALT    0x04

// define needed keys here according to the keyboardpage table: http://www.usb.org/developers/hidpage/Hut1_12v2.pdf
#define KEY_M 0x10
#define KEY_C 0x06
#define KEY_S 0x16
#define KEY_Y 0x1c
#define KEY_COMB KEY_LEFT_CTRL + KEY_LEFT_ALT

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//set number of emulators, names and shourtcut keys
const int numOfEmulators = 4;
String emulators[numOfEmulators] = {"MT-32", "CM-32L", "SC-55", "YAMAHA"};
int shourtcutKey[numOfEmulators] = {KEY_M, KEY_C, KEY_S, KEY_Y};

int currentEmulator = 0;
int activeEmulator = -100;
uint8_t buf[8] = {0};

void setup() {
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("MIDI Emulator:");
  lcd.setCursor(0, 1);
  lcd.print(emulators[currentEmulator]);
  delay(200);
}

void loop() {
  int btnValue = analogRead (0);
  lcd.setCursor(0, 1);

  if (btnValue > 140 && btnValue < 150) { // Up button
    delay(300);
    if (currentEmulator == numOfEmulators - 1) {
      currentEmulator = -1;
    }
    currentEmulator++;
    if (currentEmulator == activeEmulator) {
      lcd.print (emulators[currentEmulator] + String(" SELECTED "));
    } else {
      lcd.print (emulators[currentEmulator] + String("           "));
    }
  }
  else if (btnValue > 340 && btnValue < 350) { // Down button
    delay(300);
    if (currentEmulator == 0) {
      currentEmulator = numOfEmulators;
    }
    currentEmulator--;
    if (currentEmulator == activeEmulator) {
      lcd.print (emulators[currentEmulator] + String(" SELECTED "));
    } else {
      lcd.print (emulators[currentEmulator] + String("           "));
    }
  }
  else if (btnValue > 720 && btnValue < 740) { // Select button
    activeEmulator = currentEmulator;
    for (int i = 0; i < 3; i++) {
      lcd.setCursor(0, 1);
      lcd.print ("               ");
      delay(200);
      lcd.setCursor(0, 1);
      lcd.print (emulators[currentEmulator] + " SELECTED");
      delay(1000);
    }
    buf[0] = KEY_COMB;
    buf[2] = shourtcutKey[currentEmulator];
    Serial.write(buf, 8);
    releaseKey();
  }
}
void releaseKey()
{
  buf[0] = 0;
  buf[2] = 0;
  Serial.write(buf, 8);
  delay(200);
}


