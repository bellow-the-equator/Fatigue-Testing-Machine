#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "A4988.h"
#include <menu.h>
#include <menuIO/u8g2Out.h>
#include <menuIO/chainStream.h>
#include <menuIO/serialOut.h>
#include <menuIO/serialIn.h>
#include <EasyBuzzer.h>

using namespace Menu;
#define LEDPIN LED_BUILTIN
#define USE_ST7920
//rotary encoder pins
#define encA    24
#define encB    25
#define encBtn  26
#define beeper  27




#include <SPI.h>
#define USE_HWSPI
#define U8_DC 9
#define U8_CS 8
#define U8_RST 7
#define fontName u8g2_font_5x7_tf
#define fontX 5
#define fontY 9
#define offsetX 0
#define offsetY 0
#define U8_Width 128
#define U8_Height 64
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 17, /* data=*/ 29, /* CS=*/ 30, /* reset=*/ U8X8_PIN_NONE);

const colorDef<uint8_t> colors[6] MEMMODE={
  {{0,0},{0,1,1}},//bgColor
  {{1,1},{1,0,0}},//fgColor
  {{1,1},{1,0,0}},//valColor
  {{1,1},{1,0,0}},//unitColor
  {{0,1},{0,0,1}},//cursorColor
  {{1,1},{1,0,0}},//titleColor
};result doAlert(eventMask e, prompt &item);

int test=55;

int ledCtrl=HIGH;

result myLedOn() {
  ledCtrl=HIGH;
  return proceed;
}
result myLedOff() {
  ledCtrl=LOW;
  return proceed;
}

TOGGLE(ledCtrl,setLed,"Led: ",doNothing,noEvent,noStyle//,doExit,enterEvent,noStyle
  ,VALUE("On",HIGH,doNothing,noEvent)
  ,VALUE("Off",LOW,doNothing,noEvent)
);

int selTest=0;
SELECT(selTest,selMenu,"Select",doNothing,noEvent,noStyle
  ,VALUE("Zero",0,doNothing,noEvent)
  ,VALUE("One",1,doNothing,noEvent)
  ,VALUE("Two",2,doNothing,noEvent)
);

int chooseTest=-1;
CHOOSE(chooseTest,chooseMenu,"Choose",doNothing,noEvent,noStyle
  ,VALUE("First",1,doNothing,noEvent)
  ,VALUE("Second",2,doNothing,noEvent)
  ,VALUE("Third",3,doNothing,noEvent)
  ,VALUE("Last",-1,doNothing,noEvent)
);

// //customizing a prompt look!
// //by extending the prompt class
// class altPrompt:public prompt {
// public:
//   altPrompt(constMEM promptShadow& p):prompt(p) {}
//   Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t panelNr) override {
//     return out.printRaw(F("special prompt!"),len);;
//   }
// };

MENU(subMenu,"Sub-Menu",doNothing,noEvent,noStyle
  ,OP("Sub1",doNothing,noEvent)
  // ,altOP(altPrompt,"",doNothing,noEvent)
  ,EXIT("<Back")
);

uint16_t hrs=0;
uint16_t mins=0;

//define a pad style menu (single line menu)
//here with a set of fields to enter a date in YYYY/MM/DD format
altMENU(menu,timeMenu,"Time",doNothing,noEvent,noStyle,(systemStyles)(_asPad|Menu::_menuData|Menu::_canNav|_parentDraw)
  ,FIELD(hrs,"",":",0,11,1,0,doNothing,noEvent,noStyle)
  ,FIELD(mins,"","",0,59,10,1,doNothing,noEvent,wrapStyle)
);

char * constMEM hexDigit MEMMODE="0123456789ABCDEF";
char * constMEM hexNr[]  MEMMODE={"0","x",hexDigit,hexDigit};
char buf1[]="0x11";

MENU(mainMenu,"Main menu",doNothing,noEvent,wrapStyle
  ,OP("Op1",doNothing,noEvent)
  ,OP("Op2",doNothing,noEvent)
  //,FIELD(test,"Test","%",0,100,10,1,doNothing,noEvent,wrapStyle)
  ,SUBMENU(timeMenu)
  ,SUBMENU(subMenu)
  ,SUBMENU(setLed)
  ,OP("LED On",myLedOn,enterEvent)
  ,OP("LED Off",myLedOff,enterEvent)
  ,SUBMENU(selMenu)
  ,SUBMENU(chooseMenu)
  ,OP("Alert test",doAlert,enterEvent)
  ,EDIT("Hex",buf1,hexNr,doNothing,noEvent,noStyle)
  ,EXIT("<Exit")
);

#define MAX_DEPTH 2

//encoderIn<encA,encB> encoder;//simple quad encoder driver
 //encoderInStream<encA,encB> encStream(encoder,4);// simple quad encoder fake Stream

//a keyboard with only one key as the encoder button
// keyMap encBtn_map[]={{-encBtn,defaultNavCodes[enterCmd].ch}};//negative pin numbers use internal pull-up, this is on when low
// keyIn<1> encButton(encBtn_map);//1 is the number of keys

// menuIn* inputsList[]={&encBuitton,&Serial};
// chainStream<2> in(inputsList);//1 is the number of inputs

serialIn serial(Serial);
MENU_INPUTS(in,&serial);
// MENU_INPUTS(in,&encStream,&encButton);//,&serial);

MENU_OUTPUTS(out,MAX_DEPTH
  ,U8G2_OUT(u8g2,colors,fontX,fontY,offsetX,offsetY,{0,0,U8_Width/fontX,U8_Height/fontY})
  ,SERIAL_OUT(Serial)
);

NAVROOT(nav,mainMenu,MAX_DEPTH,in,out);

result alert(menuOut& o,idleEvent e) {
  if (e==idling) {
    o.setCursor(0,0);
    o.print("alert test");
    o.setCursor(0,1);
    o.print("press [select]");
    o.setCursor(0,2);
    o.print("to continue...");
  }
  return proceed;
}

result doAlert(eventMask e, prompt &item) {
  nav.idleOn(alert);
  return proceed;
}

//when menu is suspended
result idle(menuOut& o,idleEvent e) {
  o.clear();
  switch(e) {
    case idleStart:o.println("suspending menu!");break;
    case idling:o.println("suspended...");break;
    case idleEnd:o.println("resuming menu.");break;
  }
  return proceed;
}

void setup() {

  Serial.begin(9600);
  while(!Serial);
  Serial.println("menu 4.x test");Serial.flush();
  //encButton.begin();
  //encoder.begin();
  // pinMode(LEDPIN,OUTPUT);//cant use pin 13 when using hw spi
  // and on esp12 i2c can be on pin 2, and that is also led pin
  // so check first if this is adequate for your board
  pinMode(beeper, OUTPUT);
  EasyBuzzer.setPin(beeper);
  //EasyBuzzer.beep(1000);
  Serial.println("1");
  #if defined(USE_HWSPI)
    SPI.begin();
    u8g2.begin();
  #elif defined(USE_HWI2C)
    Wire.begin();
    Serial.println("2");
    u8g2.begin();
  #else
    #error "please choose your interface (I2c,SPI)"
  #endif
   Serial.println("2");
  u8g2.setFont(fontName);
   Serial.println("3");
  // u8g2.setBitmapMode(0);

  // disable second option
  mainMenu[1].enabled=disabledStatus;
   Serial.println("4");
  nav.idleTask=idle;//point a function to be used when menu is suspended
  Serial.println("setup done.");Serial.flush();
}

void loop() {
  //EasyBuzzer.beep(1000);
  //EasyBuzzer.update();
  //EasyBuzzer.beep(1000);
  nav.doInput();
  // digitalWrite(LEDPIN, ledCtrl);
  if (nav.changed(0)) {//only draw if menu changed for gfx device
    //change checking leaves more time for other tasks
    u8g2.firstPage();
    do nav.doOutput(); while(u8g2.nextPage());
  }
  delay(100);//simulate other tasks delay
}