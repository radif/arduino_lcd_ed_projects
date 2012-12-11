#include "Arduino.h"
#include <LiquidCrystal.h>

const String _phrases[]={
"Press right\nbutton to continue",
"Press left and right buttons\nto go backward and forward",
"Press up or down buttons\nto repeat the message",
"Press reset button\nto start over",
"Hello <name>!\nHappy THANKSGIVING!",
"Congradulations!!!\nYou got the present finder!",
//"It's called\na present finder!",
"It will help you to\nfind your present!!!!",
"But you need to\nfollow instructions.",
"Let's start\nthe adventure!",
"Step 1:\nSay your mommy you love her!",
"Step 2:\nGive your mommy thanks!",
"Step 3:\nGive your whole family thanks!",
"Step 4:\nPromise you will eat your dinner!",
"Step 5:\nPromise to study well",
"Step 6:\nClimb on the door!",
"Step 7:\nShow a ballet dance",
"Step 8:\nDo 5 pullups",
"Step 9:\nFind the TV",
"Step 10:\nLook under!",
//"Here is your\nPresent!",
"Happy\nThanksgiving!!!!"
};

#define SAMPLE_WAIT -1
#define NO_KEY 0
#define UP_KEY 3
#define DOWN_KEY 4
#define LEFT_KEY 2
#define RIGHT_KEY 5
#define SELECT_KEY 1

class DFR_Key{
public:
  DFR_Key();
  int getKey();
  void setRate(int);
private:
  int _refreshRate;
  int _keyPin;
  int _threshold;
  int _keyIn;
  int _curInput;
  int _curKey;
  int _prevInput;
  int _prevKey;
  boolean _change;
  unsigned long _oldTime;
};

static int DEFAULT_KEY_PIN = 0; 
static int DEFAULT_THRESHOLD = 5;

static int UPKEY_ARV = 144; //that's read "analogue read value"
static int DOWNKEY_ARV = 329;
static int LEFTKEY_ARV = 505;
static int RIGHTKEY_ARV = 0;
static int SELKEY_ARV = 742;
static int NOKEY_ARV = 1023;

DFR_Key::DFR_Key(){	
  _refreshRate = 10;
  _keyPin = DEFAULT_KEY_PIN;
  _threshold = DEFAULT_THRESHOLD;
  _keyIn = NO_KEY;
  _curInput = NO_KEY;
  _curKey = NO_KEY;
  _prevInput = NO_KEY;
  _prevKey = NO_KEY;
  _oldTime = 0;
}



int DFR_Key::getKey(){
  if (millis() > _oldTime + _refreshRate){
    _prevInput = _curInput;
    _curInput = analogRead(_keyPin);
    if (_curInput == _prevInput){
      _change = false;
      _curKey = _prevKey;
    }else{
      _change = true;
      _prevKey = _curKey;
      if (_curInput > UPKEY_ARV - _threshold && _curInput < UPKEY_ARV + _threshold ) _curKey = UP_KEY;
      else if (_curInput > DOWNKEY_ARV - _threshold && _curInput < DOWNKEY_ARV + _threshold ) _curKey = DOWN_KEY;
      else if (_curInput > RIGHTKEY_ARV - _threshold && _curInput < RIGHTKEY_ARV + _threshold ) _curKey = RIGHT_KEY;
      else if (_curInput > LEFTKEY_ARV - _threshold && _curInput < LEFTKEY_ARV + _threshold ) _curKey = LEFT_KEY;
      else if (_curInput > SELKEY_ARV - _threshold && _curInput < SELKEY_ARV + _threshold ) _curKey = SELECT_KEY;
      else _curKey = NO_KEY;
    }
    if (_change) return _curKey; 
    else return SAMPLE_WAIT;
    _oldTime = millis();
  }

}

void DFR_Key::setRate(int rate){
  _refreshRate = rate;
}


const int lineSize(16);
const int colSize(2);





void spellText(String wordString, int posX=0, int posY=0, int delayTime=150);
void blinkText(String wordString, int times=5, int posX=0, int posY=0, int delayTime=500);
void nextLevel();
void prevLevel();
void renderLevel(int level=0);
static int _currentLevel=0;
const int _maxLevel=sizeof(_phrases)/sizeof(String);


//Pin assignments for SainSmart LCD Keypad Shield
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 
//---------------------------------------------

DFR_Key keypad;

int localKey = 0;
String keyString = "";

void setup(){ 
  _currentLevel=0;
  lcd.begin(lineSize, colSize);
  lcd.clear();
  lcd.setCursor(0, 0);
  //lcd.print("Starting up!");
  //delay(1500);
  blinkText("Welcome");
  delay(2500);
  //lcd.clear();
  renderLevel();
  keypad.setRate(10);

}

void loop() { 
  localKey = keypad.getKey();
  if (localKey != SAMPLE_WAIT && localKey!=NO_KEY){
    switch (localKey){
        case SELECT_KEY:
        break;
        case LEFT_KEY:
          prevLevel();
        break;
        case UP_KEY:
          renderLevel(_currentLevel);
        break;
        case DOWN_KEY:
          renderLevel(_currentLevel);
        break;
        case RIGHT_KEY:
          nextLevel();
        break;
    }
  }
}

void nextLevel(){
  _currentLevel++;
  if (_currentLevel>=_maxLevel) _currentLevel=_maxLevel;
  renderLevel(_currentLevel);
}
void prevLevel(){
  _currentLevel--;
  if (_currentLevel<0) _currentLevel=0;
  renderLevel(_currentLevel);
}
void renderLevel(int level){
    lcd.clear();
  String phrase(_phrases[level]);
  size_t loc(phrase.lastIndexOf("\n"));
  if (loc <= 0)
    spellText(phrase);
  else{
      String line1(phrase.substring(0,loc));
      String line2(phrase.substring(loc+1));
      spellText(line1);
      spellText(line2,0,1);

  }
}

void blinkText(String wordString, int times, int posX, int posY, int delayTime){
        lcd.clear();
    for (int i(0);i<times;++i){
      lcd.setCursor(posX, posY);
      lcd.print(wordString);
      delay(delayTime);
      lcd.clear();
      delay(delayTime);

    }
}

void spellText(String wordString, int posX, int posY, int delayTime){
  int count(wordString.length());
  int spaceLeft(lineSize-posX);
  for (int i(0);i<=count;++i){
    int startingIndex(i>spaceLeft?i-spaceLeft:0);
    String substring(wordString.substring(startingIndex,i));
    lcd.setCursor(posX, posY);
    lcd.print("                ");
    lcd.setCursor(posX, posY);
    lcd.print(substring);
    delay(startingIndex?delayTime*2:delayTime);
  }
}

