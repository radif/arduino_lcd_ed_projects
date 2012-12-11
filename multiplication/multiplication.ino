#include "Arduino.h"
#include <LiquidCrystal.h>


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

const int _maxRandomNumber=10;
const bool _useNegativeAnswers=false;


int _inputNumber=0;
int _score=0;
bool _isGameOver=false;
String _currentTestString;
int _currentTestAnswer;

//Pin assignments for SainSmart LCD Keypad Shield
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 
//---------------------------------------------

DFR_Key keypad;

int localKey = 0;
String keyString = "";

void setup(){ 
  _inputNumber=0;
  randomSeed(analogRead(0));   
  keypad.setRate(500);

  lcd.begin(lineSize, colSize);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Welcome to the");
  lcd.setCursor(0,1);
  lcd.print("Math game!!!!");
  delay(2000);
    printScore(); 
  printLevel();
}
void printLevel(){
  generateTest();
  printTest();
}
void generateTest(){
  int x=random(_maxRandomNumber);
  int y=random(_maxRandomNumber);
 
    String test(String(x)+" * " + String(y)+" = ?");
    _currentTestString=test;
    _currentTestAnswer=x*y;
  
}
void printTest(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("How much is:");
  lcd.setCursor(0,1);
  lcd.print(_currentTestString);
  _inputNumber=0;
}
void printScore(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Your score is:");
  lcd.setCursor(0,1);
  lcd.print(_score);
  delay(2000);
}
void printResult(bool correct){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(correct?"Correct!":"Wrong!!!");
  delay(2000);
}
bool checkScore(){
if(_score<0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Game Over");
    lcd.setCursor(0,1);
    lcd.print("You lose!");
    _isGameOver=true;
     return false;
}else if(_score>=10){
   lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Congrarulations!");
    lcd.setCursor(0,1);
    lcd.print("You Win!!!!!!!!");
    _isGameOver=true;
    return false;
}
 return true;
}
void checkAnswer(){
  if(_inputNumber==_currentTestAnswer){
    _score++;
    printResult(true);
    printScore();
    if(checkScore())
      printLevel();
  }else{
    _score--;
    printResult(false);
    printScore();
    if(checkScore())
      printTest();
  }
}
void loop() { 
  if (_isGameOver) return;
  localKey = keypad.getKey();
  if (localKey != SAMPLE_WAIT && localKey!=NO_KEY){
    bool touched(false);
    switch (localKey){
        case SELECT_KEY:
        break;
        case LEFT_KEY:
        break;
        case UP_KEY:
          _inputNumber++;
          touched=true;
        break;
        case DOWN_KEY:
           _inputNumber--;
           touched=true;
        break;
        case RIGHT_KEY:
          checkAnswer();
        break;
    }
    if(touched){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(_inputNumber);
      lcd.setCursor(0,1);
      lcd.print(_currentTestString);
      delay(200);
    }

    
  }
  
}

