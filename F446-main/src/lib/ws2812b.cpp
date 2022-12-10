#include "ws2812b.h"

extern Adafruit_NeoPixel stripL;
extern Adafruit_NeoPixel stripR;
extern Adafruit_NeoPixel stripUI;
extern Adafruit_NeoPixel stripTop;

WS2812B::WS2812B(int brightness) {
    stripR.begin();
    stripR.setBrightness(brightness);
    stripR.show();

    stripL.begin();
    stripL.setBrightness(brightness);
    stripL.show();

    stripUI.begin();
    stripUI.setBrightness(brightness);
    stripUI.show();

    stripTop.begin();
    stripTop.setBrightness(brightness);
    stripTop.show();
}

void WS2812B::battery(int percent) {
    for(int percent = 0; percent<=100; percent++){
    if(percent <=10){
    stripUI.setPixelColor(0,255,0,0);
    stripUI.setPixelColor(1,255,0,0);

  }else if(percent <=20){
    stripUI.setPixelColor(2,200,55,0);
    stripUI.setPixelColor(3,200,55,0);

  }else if(percent <=30){
    stripUI.setPixelColor(4,190,65,0);
    stripUI.setPixelColor(5,180,75,0);
    stripUI.setPixelColor(6,180,75,0);

  }else if(percent <=40){
    stripUI.setPixelColor(7,170,85,0);
    stripUI.setPixelColor(8,160,95,0);
    stripUI.setPixelColor(9,160,95,0);

  }else if(percent <=50){
    stripUI.setPixelColor(10,140,115,0);
    stripUI.setPixelColor(11,120,125,0);
    stripUI.setPixelColor(12,120,125,0);
 
  }else if(percent <=60){
    stripUI.setPixelColor(13,100,120,35);
    stripUI.setPixelColor(14,90,110,55);

  }else if(percent <=70){
    stripUI.setPixelColor(15,80,100,75);
    stripUI.setPixelColor(16,70,90,95);

  }else if(percent <=80){
    stripUI.setPixelColor(17,60,80,115);
    stripUI.setPixelColor(18,50,70,135);
 
  }else if(percent <=90){
    stripUI.setPixelColor(19,40,60,155);
    stripUI.setPixelColor(20,30,50,175);

  }else if(percent <=99){
    stripUI.setPixelColor(21,20,40,195);
    stripUI.setPixelColor(22,10,30,215);
   
  }else if(percent <=100){
    stripUI.setPixelColor(23,0,0,255);
  }
    }

    //寝れぬなら　書くまで待とう　プログラム by 家康
}

void WS2812B::show(void) {
    stripR.show();
    stripL.show();
    stripUI.show();
    stripTop.show();
}