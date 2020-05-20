//change this with your own device ID, keyA and keyB
#define EMRINE_DEVICE_ID "1kJHWKybKeSqz4vZa94s"
#define EMRINE_KEYA 2722
#define EMRINE_KEYB 13553

#define EMRINE_HOST "https://emrine.com"
#define EMRINE_MAX_QUESTION 25000
#define SCREEN_OFFSET_X 6
#define SCREEN_OFFSET_Y 6
#define QRCODE_BLOCK 4
#define TEXT_OFFSET_X 18
#define CODE_TITLE "code"
#define HINT_VALIDATE "Press \"#\" to validate"
#define HINT_CANCEL "Press \"*\" to cancel"
#define UNLOCKED_TEXT "Access granted !"
#define LOCKED_TEXT1 "Press \"#\""
#define LOCKED_TEXT2 "to request access"
#define QRCODE_INSTRUCTION1 "Scan the following"
#define QRCODE_INSTRUCTION2 "QR code with your"
#define QRCODE_INSTRUCTION3 "smartphone"

#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735

#define TFT_CS   A1
#define TFT_RST  A0
#define TFT_DC   10

#include "qrcode.h"
#include <Keypad.h>

const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //four columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

byte pin_rows[ROW_NUM] = {9,8,7,6}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
QRCode qrcode;

enum appState{STATE_LOCKED,STATE_QRCODE,STATE_TYPING_CODE,STATE_CHECKING_CODE,STATE_UNLOCKED};

int state = STATE_LOCKED;
String codeStr = "";
int codeHeight = 0; //height position in pixel for the code (in the code view)
int question = 0;

void setup(void) {
  Serial.begin(9600);
  //init screen
  tft.initR(INITR_144GREENTAB);
  tft.setRotation(2);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  
  randomSeed(analogRead(A5));

  state = STATE_LOCKED;
  processState();
}

void processState(){
  switch(state){
    case STATE_LOCKED: {
      showLockedView();
      break;
    }
    case STATE_QRCODE: {
      showQRCodeInstructions();
      //get a random number between 0 and EMRINE_MAX_QUESTION
      question = random(EMRINE_MAX_QUESTION);
      Serial.println(question);
      generateQRCode();
      break;
    }
    case STATE_TYPING_CODE: {
      showCode();
      break;
    }
    case STATE_CHECKING_CODE: {
      state = checkingCode() ? STATE_UNLOCKED : STATE_LOCKED;
      processState();
      break;
    }
    case STATE_UNLOCKED: {
      showUnlockedView();
      delay(5000);
      state = STATE_LOCKED;
      processState();
      break;
    }
  }
}

bool checkingCode(){
  long step1 = question + EMRINE_KEYA;
  long step2 = step1 * step1;
  String step3 = String(step2);
  if (step3.length() >= 2){
    step3.remove(step3.length()-2, 2);
  }
  String step4 = step3.length() >= 4 ? step3.substring(step3.length() - 4) : step3;
  long step5 = step4.toInt() + EMRINE_KEYB;
  long step6 = step5 * step5;
  String step7 = String(step6);
  if (step7.length() >= 2){
    step7.remove(step7.length()-2, 2);
  }
  String step8 = step7.length() >= 4 ? step7.substring(step7.length() - 4) : step7;
  return step8.toInt() == codeStr.toInt();
}

void showLockedView(){
  tft.fillScreen(ST77XX_BLACK);
  //draw lock 
  uint16_t color = 0xE945;
  uint8_t lockSize = 70;
  uint8_t lockX = (tft.width()-lockSize)/2;
  uint8_t lockY = 38;
  tft.fillRoundRect(lockX + lockSize/5, lockY - 60/2, 3*lockSize/5, 20 + 60/2, 20, color);
  tft.fillRoundRect(lockX + lockSize/5 + 8, lockY - 60/2 + 8, 3*lockSize/5 - 8*2, 40, 20, ST77XX_BLACK);
  tft.fillRoundRect(lockX, lockY, lockSize, 60, 10, color);
  tft.fillCircle(lockX + lockSize/2, lockY+ 60/2, 8, ST77XX_BLACK);

  //draw text
  int pos = getTextCenterPos(strlen(LOCKED_TEXT1), 1);
  int initHeight = 105;
  tft.setCursor(pos, initHeight);
  tft.setTextSize(1);
  tft.println(LOCKED_TEXT1);
  pos = getTextCenterPos(strlen(LOCKED_TEXT2), 1);
  initHeight+=7*1 + 3;
  tft.setCursor(pos, initHeight);
  tft.println(LOCKED_TEXT2);
}

void showUnlockedView(){
  tft.fillScreen(ST77XX_BLACK);
  //draw lock 
  uint16_t color = 0x5667;
  uint8_t lockSize = 70;
  uint8_t lockX = (tft.width()-lockSize)/2;
  uint8_t lockY = 38;
  tft.fillRoundRect(lockX + lockSize/5, lockY - 60/2, 3*lockSize/5, 20 + 60/2, 20, color);
  tft.fillRoundRect(lockX + lockSize/5 + 8, lockY - 60/2 + 8, 3*lockSize/5 - 8*2, 40, 20, ST77XX_BLACK);
  tft.fillRoundRect(lockX, lockY, lockSize, 60, 10, color);
  tft.fillRect(lockX, lockY-10, lockSize/2, 10, ST77XX_BLACK);
  uint8_t x0 = lockX+15; uint8_t y0 = lockY+29;
  uint8_t x1 = lockX+20; uint8_t y1 = lockY+24;
  uint8_t x2 = lockX+31; uint8_t y2 = lockY+34;
  uint8_t x3 = lockX+31; uint8_t y3 = lockY+45;
  uint8_t x4 = lockX+56; uint8_t y4 = lockY+20;
  uint8_t x5 = lockX+51; uint8_t y5 = lockY+15;
  tft.fillTriangle(x0, y0, x1, y1, x2, y2, ST77XX_BLACK);
  tft.fillTriangle(x0, y0, x2, y2, x3, y3, ST77XX_BLACK);
  tft.fillTriangle(x2, y2, x3, y3, x4, y4, ST77XX_BLACK);
  tft.fillTriangle(x2, y2, x4, y4, x5, y5, ST77XX_BLACK);

  //draw text
  int pos = getTextCenterPos(strlen(UNLOCKED_TEXT), 1);
  tft.setCursor(pos, 105);
  tft.setTextSize(1);
  tft.println(UNLOCKED_TEXT);
}
void showQRCodeInstructions(){
    tft.fillScreen(ST77XX_BLACK);
    int initHeight = tft.height()/2 - (7 + 3 + 7 + 3 + 7 + 3 + 7)/2;
    int pos = getTextCenterPos(strlen(QRCODE_INSTRUCTION1), 1);
    tft.setCursor(pos, initHeight);
    tft.setTextSize(1);
    tft.println(QRCODE_INSTRUCTION1);
    initHeight+=7*1 + 3;
    pos = getTextCenterPos(strlen(QRCODE_INSTRUCTION2), 1);
    tft.setCursor(pos, initHeight);
    tft.println(QRCODE_INSTRUCTION2);
    initHeight+=7*1 + 3;
    pos = getTextCenterPos(strlen(QRCODE_INSTRUCTION3), 1);
    tft.setCursor(pos, initHeight);
    tft.println(QRCODE_INSTRUCTION3);
    delay(1500);
}

void generateQRCode(){
  uint8_t qrcodeData[qrcode_getBufferSize(3)];

  String url = String(EMRINE_HOST) + "/dev_" + String(EMRINE_DEVICE_ID) + "/qst_" + question;
  qrcode_initText(&qrcode, qrcodeData, 3, 0, url.c_str());
  
  tft.fillScreen(ST77XX_BLACK);
  //display QR code
  int offsetX = 0;
  int offsetY = 0;
  for (uint8_t y = 0; y < qrcode.size; y++) {
      offsetX = 0;
      for (uint8_t x = 0; x < qrcode.size; x++) {
        if (qrcode_getModule(&qrcode, x, y)){
          tft.drawRect(SCREEN_OFFSET_X + x + offsetX, SCREEN_OFFSET_Y + y + offsetY, QRCODE_BLOCK, QRCODE_BLOCK, ST77XX_WHITE);
          tft.fillRect(SCREEN_OFFSET_X + x + offsetX, SCREEN_OFFSET_Y + y + offsetY, QRCODE_BLOCK, QRCODE_BLOCK, ST77XX_WHITE);
        }
        offsetX+=QRCODE_BLOCK - 1;
      }
      offsetY+=QRCODE_BLOCK - 1;
  }
}

void showCode(){
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextWrap(true);

  uint8_t initHeight = tft.height()/2 - ((7*2) + 3 + (7*3) + 3 + 7 + 3 + 7)/2;
  uint8_t pos = getTextCenterPos(strlen(CODE_TITLE), 2);
  tft.setCursor(pos, initHeight);
  tft.setTextSize(2);
  tft.println(CODE_TITLE);

  initHeight+=7*2 + 3;
  codeHeight = initHeight;
  updateCode();

  initHeight+=7*3 + 3;
  pos = getTextCenterPos(strlen(HINT_VALIDATE), 1);
  tft.setCursor(pos, initHeight);
  tft.setTextSize(1);
  tft.print(HINT_VALIDATE);

  tft.setCursor(pos, initHeight + 10);
  tft.print(HINT_CANCEL);
}

void updateCode(){
  uint8_t pos = getTextCenterPos(codeStr.length(), 3);
  tft.drawRect(0, codeHeight, 128, 7*3, ST77XX_BLACK);
  tft.fillRect(0, codeHeight, 128, 7*3, ST77XX_BLACK);
  tft.setCursor(pos, codeHeight);
  tft.setTextSize(3);
  tft.println(codeStr);
}

int getTextCenterPos(int length, int block){
  return (tft.width() - (5*length*block + (length -1)*block))/2;
}

void loop() {
  char key = keypad.getKey();
  if (key){
    switch(key){
      case '#': {
        if (state == STATE_LOCKED) {
          state = STATE_QRCODE;
          processState();
        } else if (state == STATE_TYPING_CODE){
          state = STATE_CHECKING_CODE;
          processState();
        }
        break;
      }
      case '*': {
        if (state == STATE_QRCODE) {
          state = STATE_LOCKED;
          processState();
        } else if (state == STATE_TYPING_CODE && codeStr.length()>0){
          codeStr.remove(codeStr.length()-1);
          updateCode();
        } else if (state == STATE_TYPING_CODE && codeStr.length()==0){
          state = STATE_LOCKED;
          processState();
        }
        break;
      }
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case '0': {
        if (state == STATE_QRCODE) {
          state = STATE_TYPING_CODE;
          codeStr = key;
          processState();
        } else if (state == STATE_TYPING_CODE){
          if (codeStr.length()<5){
            codeStr+= key;
            updateCode();
          }
        }
        break;
      }
    }
  }
}