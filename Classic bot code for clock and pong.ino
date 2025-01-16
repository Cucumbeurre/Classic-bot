// Library for RTC module
#include <TimeLib.h>
#include <Wire.h>
#include <DS1307RTC.h> 
// Library for display
#include <U8g2lib.h>

#include <USBAPI.h>

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R2, /* reset=*/ U8X8_PIN_NONE);

//Variables Button
const int PIN_UP = 3 ,PIN_DOWN = 2;
int currentTimeMode = 1, currentRobotMode = 1, currentMode = 0;
// Variables position of the eyes
  int wEyes = 10 ,hEyes = 0 ,wMouth = 42 ,hMouth = 4 ,EyeSpace = 84 , xSpace = (128-EyeSpace-(wEyes*2))/2, ySpace = (64-hEyes)-xSpace-12 ,EyesClosed = 0 ;
// Strings
int W,W1,W2 ;
char S[10],S1[10],S2[10] ;
int HFont = 24 ;
int suffix = "er";
//Calendar
char *monthName[13] = {"","Janvier", "Fevrier", "Mars", "Avril", "Mai", "Juin", "Juillet" , "Aout" , "Septembre" , "Octobre" , "Novembre" , "Decembre"};
char *weekDayName[8] = {"","Dimanche","Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi","Samedi"};
//Time
unsigned long previousMillis = 0;

// Ball&Lign size & position
int Balllenght = 4,BallxPos = 62 ,BallyPos = 30 ,BallxSpeed = 4,BallySpeed = 2;
int LignW = 2;

// Bar size & position Player
int Barxlenght = 4,Barylenght = 12,BarxPos = 4 ,BaryPos = 32-(Barylenght/2) ,BarySpeed = 3;

// Bar size & position Bot
int Botxlenght = 4, Botylenght = 12 ,BotxPos = 128-8 ,BotyPos = 32-(Barylenght/2) ,BotySpeed = 3;
int FailRate = 0;
//Score
int PlayerScore = 0 ,WPlayerScore = 0, BotScore = 0,WBotScore = 0;
char SPlayerScore[10],SBotScore[10];
// Victory
char SVictory[10];
int WVictory = 0;
int Victory = "PLAYER";
//String Menu
int StringH ,DisplayH ,DisplayW ;
int MenuW ,SRobotW ,SClockW , SPongW ,SpaceW ;
//Selection
int selection = 1 ,ModeSelection = 1 ,RobotSelection = U8G2_BTN_BW1 ,ClockSelection = U8G2_BTN_BW0 ,PongSelection = U8G2_BTN_BW0 ;

void setup() {
   u8g2.begin();
  StartupGreeting();
  // RTC module setup
    Serial.begin(9600);
  while (!Serial) ; // wait until Arduino Serial Monitor opens
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if(timeStatus()!= timeSet) 
     Serial.println("Unable to sync with the RTC");
  else
     Serial.println("RTC has set the system time"); 
  // Mode switch setup
   pinMode(PIN_UP, INPUT_PULLUP);
   pinMode(PIN_DOWN, INPUT_PULLUP);
   DisplayH = u8g2.getDisplayHeight();
   DisplayW = u8g2.getDisplayWidth();
  
}
void StartupGreeting() {
   // Startup greating
   u8g2.setFont(u8g2_font_logisoso16_tr);
   W = u8g2.getStrWidth("hello.");
u8g2.firstPage();

  do {
      u8g2.drawStr(64-(W/2),32+(HFont/2),"hello.");

  } while (u8g2.nextPage());
 delay (1500);

}

void ModeTime() {
  if (digitalRead(PIN_DOWN) == LOW) {
    currentTimeMode++ ;
    delay (200) ;
   }
   if (currentTimeMode > 2) {
    currentTimeMode = 1 ;
   }
   if (currentTimeMode==1) {
 HFont = 32 ;
  u8g2.setFont(u8g2_font_logisoso32_tn);
    sprintf(S, "%02d:%02d" , hour(), minute() );
    W = u8g2.getStrWidth(S);
    
    u8g2.firstPage();

  do {
      u8g2.drawStr(64-(W/2),32+(HFont/2),S);

  } while (u8g2.nextPage());
   }
  
  if (currentTimeMode==2){
       //font settings
   HFont = 18 ;
  u8g2.setFont(u8g2_font_logisoso16_tr);
       // Date 
    if (day()==1)
    {
      suffix = "er";
    } else { suffix = "";}
    
    { 
      sprintf(S2, "%d" ,year());
    W2 = u8g2.getStrWidth(S2);
      sprintf(S, "%s" ,weekDayName[weekday()] );
    W = u8g2.getStrWidth(S);  
     sprintf(S1, "%d%s %s" ,day(),suffix,monthName[month()]);
    W1 = u8g2.getStrWidth(S1);

    }
    u8g2.firstPage();

  do {
      u8g2.drawStr(64-(W/2),64/3,S);
      u8g2.drawStr(64-(W1/2),(64/3*2),S1);
      u8g2.drawStr(W2-56,64,S2);

  } while (u8g2.nextPage());
  }
}

void ModeRobot() {

u8g2.firstPage();
  do  {
    u8g2.drawBox(xSpace,ySpace,wEyes,hEyes);
    u8g2.drawBox(EyeSpace+xSpace+wEyes,ySpace,wEyes,hEyes);
    u8g2.drawBox(64-(wMouth/2),64-hMouth*2,wMouth,hMouth);
  } while (u8g2.nextPage() );

u8g2.sendBuffer();
unsigned long currentMillis = millis();
   if (hEyes < 24 && EyesClosed == 0) {
    hEyes += 4;
    ySpace -=2 ;
   }
   if (hEyes == 24 && currentMillis - previousMillis >= random(6000,12000)) {
    previousMillis = currentMillis;
    EyesClosed = 1 ;
   }
   if (hEyes > 0 && EyesClosed == 1 ){
    hEyes -= 4;
    ySpace +=2 ;
   }
   if (hEyes == 0) {
    EyesClosed = 0 ;
   }
   }

void ModePong() {
  //String Victory
  u8g2.setFont(u8g2_font_bauhaus2015_tn);
  StringH = u8g2.getAscent();
  sprintf(SBotScore, "%0d" ,BotScore);
  sprintf(SPlayerScore, "%0d" ,PlayerScore);
  WBotScore = u8g2.getStrWidth(SBotScore);
  WPlayerScore = u8g2.getStrWidth(SPlayerScore);

  //Draw screen
 u8g2.firstPage();
  do {
    
      u8g2.drawBox(BallxPos,BallyPos,Balllenght,Balllenght);
      u8g2.drawBox(BarxPos,BaryPos,Barxlenght,Barylenght);
      u8g2.drawBox(BotxPos,BotyPos,Botxlenght,Botylenght);
      u8g2.drawBox(DisplayW/2-LignW/2, 0,LignW,DisplayH);
      u8g2.drawStr(DisplayW/2+8,StringH,SBotScore);
      u8g2.drawStr(DisplayW/2-(WPlayerScore+8),StringH,SPlayerScore);
  } while (u8g2.nextPage());
  // Ball movement
  BallxPos += BallxSpeed;
  BallyPos += BallySpeed;
  if (BallyPos > DisplayH-Balllenght or BallyPos < 0) {
    BallySpeed *= -1;   
  }
  if ((BallyPos + Balllenght == BaryPos  || BallyPos == BaryPos + Barylenght) && BallxPos > BarxPos && BallxPos < BarxPos + Barxlenght) {
    BallySpeed *= -1;
    BallyPos += BarySpeed;
  }
    if ((BallyPos + Balllenght == BotyPos  || BallyPos == BotyPos + Botylenght) && BallxPos > BotxPos && BallxPos < BotxPos + Botxlenght) {
    BallySpeed *= -1;
    BallyPos += BotySpeed;
  }
  
  if (BallxPos == (BarxPos + Barxlenght) && BallyPos + Balllenght > BaryPos && BallyPos < BaryPos + Barylenght 
  or (BallxPos + Balllenght )== BotxPos && BallyPos + Balllenght > BotyPos && BallyPos < BotyPos + Botylenght ){
    BallxSpeed *= -1;
    FailRate = random(-10,10);
  }
  //BotScore
  if (BallxPos < 0) {
    BallxPos = DisplayW/2-Balllenght;
    BallxSpeed *= -1;
    BotScore ++;
  }
  //PlayerScore
    if ( BallxPos > DisplayW-Balllenght) {
    BallxPos = DisplayW/2-Balllenght;
    BallxSpeed *= -1;
    PlayerScore ++;
  }
  //BotScript
  if ((BallyPos+Balllenght/2+FailRate)< (BotyPos+Botylenght/2) && BotyPos>0) {
  BotyPos -= 2;
  }
  if ( (BallyPos+Balllenght/2+FailRate) > (BotyPos+Botylenght/2) && (Botylenght+BotyPos)<64) {
  BotyPos += 2 ;
  }

  //Bar movement
  if (digitalRead(PIN_UP) == LOW && BaryPos<64-Barylenght) {
    BaryPos += BarySpeed ;
     }
  if (digitalRead(PIN_DOWN) == LOW && BaryPos>0){
    BaryPos -= BarySpeed ;
     }
  //VICTORY
  if (BotScore == 10) {
    Victory = "ROBOT";
  }
    if (PlayerScore == 10) {
    Victory = "PLAYER";
  }

  if (BotScore >=10 or PlayerScore >=10) {
    u8g2.setFont(u8g2_font_logisoso16_tr);
    StringH = u8g2.getAscent();
    sprintf(SVictory, "%s WINS",Victory);
    WVictory = u8g2.getStrWidth(SVictory);
    u8g2.clearDisplay();
     u8g2.firstPage();
  do {
      u8g2.drawStr(DisplayW/2-(WVictory/2),DisplayH/2+StringH/2,SVictory);
  } while (u8g2.nextPage());
  BotScore = 0;
  PlayerScore = 0;
  delay(5000);
  BaryPos = 32-(Barylenght/2);
  currentMode=1;
  }

}

void loop(){
   
 if (currentMode == 0) {
   if (ModeSelection==1) {
    ModeRobot();
   }
   if (ModeSelection==2){
    ModeTime();
   }
   if (ModeSelection==3) {
   ModePong();
   }
    }
   
    if (digitalRead(PIN_UP) == LOW && digitalRead(PIN_DOWN) == LOW) {
  currentMode ++ ;
  if (currentMode > 1) { currentMode = 0;}
    delay (200) ;
 }
 if (currentMode == 1) {
  if (digitalRead(PIN_UP) == LOW) {
    delay (200) ;
    currentMode=0;
    ModeSelection = selection;
      BotScore = 0;
  PlayerScore = 0;
  BallyPos = 32-(Barylenght/2);
  BallxPos = 60;
    }
  if (digitalRead(PIN_DOWN) == LOW) {
    selection ++ ;
    delay (200) ;
  }
  if (selection == 1) {
    RobotSelection = U8G2_BTN_INV;
    } 
  else {RobotSelection = 0;
  }  
  if (selection == 2) {
    ClockSelection = U8G2_BTN_INV;
    }
  else {ClockSelection = 0;
  }
  if (selection == 3) {
    PongSelection = U8G2_BTN_INV; 
    }
    else {PongSelection = 0;
  }
if (selection > 3) {
    selection = 1;
    }
  u8g2.firstPage();
  do  {
  u8g2.setFont(u8g2_font_lastapprenticebold_tr);
  StringH = u8g2.getAscent();
  SRobotW = u8g2.getStrWidth("Robot");
  SClockW = u8g2.getStrWidth("Clock");
  SPongW = u8g2.getStrWidth("Pong");
  MenuW = SRobotW + SClockW + SPongW; 
  SpaceW = (DisplayW-MenuW)/4;
  u8g2.drawButtonUTF8(SpaceW,DisplayH/2+(StringH/2),  RobotSelection, 0,  2,  2, "Robot" );
  u8g2.drawButtonUTF8(SRobotW+SpaceW*2, DisplayH/2+(StringH/2), ClockSelection, 0,  2,  2, "Clock" );
  u8g2.drawButtonUTF8(SRobotW+SClockW+SpaceW*3, DisplayH/2+(StringH/2), PongSelection, 0,  2,  2, "Pong" );
   } while (u8g2.nextPage() );

u8g2.sendBuffer();
 }

}




  
