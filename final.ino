// FASTLED stuff
#include <FastLED.h>
#define NUM_LEDS 256

CRGB leds[NUM_LEDS];

//MSGEQ7 stuff
#include "MSGEQ7.h"
#define pinAnalogLeft A0

//PCB
//#define DATA_PIN 4
//#define pinReset 14
//#define pinStrobe 16


// My Room
#define DATA_PIN 5
#define pinReset 4
#define pinStrobe 2
#define MSGEQ7_INTERVAL ReadsPerSecond(50)
#define MSGEQ7_SMOOTH 210
CMSGEQ7<MSGEQ7_SMOOTH, pinReset, pinStrobe, pinAnalogLeft> MSGEQ7;

//WIFI Stuff
#include <ESP8266WiFi.h>

WiFiServer server(80);

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 


int bass, mid, treble, inputBass, lastBass, deltaBass, lastMid, deltaMid,volume,deltaVolume, lastVolume;
bool onOff = true;
bool brightnessChange = false;
int brightness = 200;
int current = 9;
bool wifiOn = true;
int simpleColour = 0;
int orange = 0;
bool meggyness = false;
int colour = 160;
int variable = 10;
int sensitivity = 20;
bool strobeNow = false;
int strobeLast = 0;


void setup() {
  Serial.begin(115200);
  // This will set the IC ready for reading
    WiFi.hostname("robslights");
    //WiFi.hostname("ed");
     WiFiManager wifiManager;
     
      //wifiManager.resetSettings();
      //wifiManager.setSTAStaticIPConfig(IPAddress(192,168,123,123), IPAddress(192,168,0,1), IPAddress(255,255,255,0));
      wifiManager.autoConnect("LEDSetup");

 
  MSGEQ7.begin();
  
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

 // delay(10);
//  if(wifiOn){
//    // Connect to WiFi network
//    WiFi.begin(ssid, password);
//    while (WiFi.status() != WL_CONNECTED) {
//      delay(500);
//      Serial.println("connecting... ");
//    }
//    Serial.println("WiFi connected");
//    Serial.println(WiFi.localIP());
//    
server.begin();
//  }
  
  
  pinMode(15,INPUT);
}

void loop() {
   if(wifiOn){
    WiFiClient client = server.available();
    if(client){
      if(client.available()){
        // Read the first line of the request
        String request = client.readStringUntil('\r');
        Serial.println(request);
        client.flush();
        String s = "HTTP/1.1 200 OK\r\n";
        s += "Content-Type: text/html\r\n\r\n";
        s += "<!DOCTYPE HTML>\r\n<html>";
        s += "<style>button{font-size: 100px;}</style>";
        s += "<h1>Party Selecter</h1>";
        s += "<br><a href=\"strobe\"><button>STROBE</button></a></br>";
        s += "<br><a href=\"onOff\"><button>On Off</button></a></br>";
        s += "<br><a href=\"upSensitivity\"><button>+ sensitivity</button></a></br>";
        s += "<br><a href=\"downSensitivity\"><button>- sensitivity</button></a></br>";
        s += "<br><a href=\"white\"><button>White</button></a></br>";
   
        s += "<br><a href=\"fillColour\"><button>Rainbow to the beat</button></a></br>";
        s += "<br><a href=\"beatSploge\"><button>Beat Sploges</button></a></br>";
        s += "<br><a href=\"strobeSetTime\"><button>Nice and Strobey</button></a></br>";
        s += "<br><a href=\"changeProgram\"><button>Cycle party lighting</button></a></br>";
        s += "<br><a href=\"bassMidBars\"><button>RGB to the beat</button></a></br>";
        s += "<br><a href=\"simpleLightBeat\"><button>Strip cycle rainbow to beat</button></a></br>";
        s += "<br><a href=\"bassBullets\"><button>Bass bullets</button></a></br>";
        s += "<br><a href=\"strobeBass\"><button>Maybe too strobey?</button></a></br>";
        s += "<br><a href=\"colourMove\"><button>Party Rainbow to the beat</button></a></br>";
        
        s += "</html>\r\n";
        client.println(s);
      
        if(request.indexOf("white") != -1){
          current = 0;
         
        }
        if(request.indexOf("onOff") != -1){
          onOff = !onOff;
         
        }
        if(request.indexOf("brightnessUp") != -1){
          brightness = brightness +20;
          if(brightness >255){
            brightness = 255;
          }
         
        }
        if(request.indexOf("brightnessDown") != -1){
          if(brightness <19){
            brightness = 15;
          }else{
            brightness = brightness -20;
          }
         
        }
        if(request.indexOf("changeProgram") != -1){
          current++;
          if(current > 8 ){
            current = 1;
          }
          sensitivity = 20;
         
        }
        if(request.indexOf("orange") != -1){
          if(orange < 5){
          orange++;
          }else{
            orange = 0;
          }
         
        }
        if(request.indexOf("meg") != -1){
          meggyness=!meggyness;
         
        }
        if(request.indexOf("fillColour") != -1){
          current = 1;  
          sensitivity = 20;  
        }
        if(request.indexOf("bassMidBars") != -1){
          current = 2;
          sensitivity = 20;
        }
        if(request.indexOf("simpleLightBeat") != -1){
          current = 3;
          sensitivity = 20;
        }
        if(request.indexOf("bassBullets") != -1){
          current = 4;
          sensitivity = 20;
        }
        if(request.indexOf("strobeBass") != -1){
          current = 5;
          sensitivity = 20;
        }
        if(request.indexOf("colourMove") != -1){
          current = 6;
          sensitivity = 20;
        }
        if(request.indexOf("strobeSetTime") != -1){
          current = 7;
          sensitivity = 20;
        }
        if(request.indexOf("beatSploge") != -1){
          current = 8;
          sensitivity = 20;
        }
  
        if(request.indexOf("upSensitivity") != -1){
          sensitivity += 3;
        }
        if(request.indexOf("downSensitivity") != -1){
          sensitivity -= 3;
        }
        if(request.indexOf("strobe") != -1){
          strobeNow = true;
        }
      }
      client.stop();
    }
   }

  
  if(digitalRead(15) == HIGH){
    current++;
    if(current > 6  ){
      current = 1;
    }
    sensitivity = 20;
    delay(300);
  }
  if(onOff){
    bool newReading = MSGEQ7.read(MSGEQ7_INTERVAL);
    switch (current) {
      case 0:
        white();
        break;
      case 1:
        // Analyze without delay every interval
        
        setupValues();
        fillColour();
        delay(15);
        break;
      case 2:
        setupValues();
        bassMidBars();
        delay(6);
        break;
      case 3:
        setupValues();
        simpleLightBeat();
        break;
      case 4:
        setupValues();
        bassBullets();
        break;
      case 5:
        setupValues();
        strobeBass();
        break;
      case 6:
        setupValues();
        colourMove();
        break;
      case 7:
        setupValues();
        strobeSetTime();
        break;
      case 8:
        setupValues();
        setBeats();
        break;
      case 9:
        setupValues();
        reggaeMiddle();
        break;
    }
    
    FastLED.setBrightness(brightness);
    if(strobeNow){
      if((millis() - strobeLast) >10){
        fill_solid( leds, NUM_LEDS, CRGB(255, 255, 255)); 
        strobeLast = millis();
      }
      strobeNow = false;
    }
    FastLED.show();
  }else{
    fill_solid( leds, NUM_LEDS, CRGB(0, 0, 0)); 
    FastLED.show();
  }

  if (Serial.available() > 0) {
    variable = Serial.parseInt(); 
  }
}
int strobeCount = 0;
int lastStrobe = 0;
int movementStrobe = 0;

void reggaeMiddle(){
  int middle = NUM_LEDS/2;

  int bassMap = map(bass,100,255,middle,NUM_LEDS);
  fill_solid(leds, NUM_LEDS, CRGB(0,0,0));
  for(int i =middle; i<= bassMap; i++){
    leds[i] = CHSV(colour,255,255);
    
  }
  bassMap = map(bass,100,255,middle,0);
  for(int i =middle; i>= bassMap; i--){
    leds[i] = CHSV(colour,255,255);
    
  }
  colour++;
  
}





int movement = 0;
void setBeats(){
  deltaBass = bass - lastBass;
  lastBass = bass;
  deltaMid = mid - lastMid;
  lastMid = mid;
  
  deltaVolume = volume - lastVolume;
  lastVolume = volume;
  
//  if(deltaBass >10  ){
//    movement += 40;
//  }
//  if(millis()%10){
//  for(int i = 0; i<= NUM_LEDS;i++){
//    volume = map(volume, 0,255,10,40);
//    if(!leds[i]){
//    leds[i] += ColorFromPalette( PartyColors_p , (i+movement)/2,volume);
//    }
//  }
//  }
  
  if(deltaBass > sensitivity-7){
    int randomMid = random(30,NUM_LEDS -30);
    int counter = 0;
    for(int i = randomMid -30; i<= randomMid +30; i++){
      if(counter <=30){
        int dither = map(counter, 0,30,0,255);
        leds[i] = CHSV(colour-15,255,dither);
      }else{
        int dither = map(counter, 30,60,255,0);
        leds[i] = CHSV(colour-15,255,dither);
      }
      counter++;
    }
    colour += 2;
  }



  if(deltaMid > sensitivity-8){
    int randomMid = random(30,NUM_LEDS -30);
    int counter = 0;
    for(int i = randomMid -30; i<= randomMid +30; i++){
      if(counter <=30){
        int dither = map(counter, 0,30,0,255);
        leds[i] = CHSV(colour,255,dither);
      }else{
        int dither = map(counter, 30,60,255,0);
        leds[i] = CHSV(colour,255,dither);
      }
      counter++;
    }
    colour += 2;
  }
  if(deltaVolume > sensitivity-7){
    if(millis()%6){
      int randomMid = random(30,NUM_LEDS -30);
      int counter = 0;
      for(int i = randomMid -30; i<= randomMid +30; i++){
        if(counter <=30){
          int dither = map(counter, 0,30,0,255);
          leds[i] = CHSV(colour+15,255,dither);
        }else{
          int dither = map(counter, 30,60,255,0);
          leds[i] = CHSV(colour+15,255,dither);
        }
        counter++;
      }
      colour += 2;
    }
  }
  

  if(millis()%2 ){
    for(int i; i<= NUM_LEDS;i++){
      leds[i] %= 230;
    }
  }


}


void strobeSetTime(){
//  if(millis() %3 ==0){
//     movementStrobe += 2;
//  }
//  if(millis() %5 ==0){
//     deltaMid = mid - lastMid;
//    lastMid = mid;
//    mid = mid- (deltaMid/3);
//    for(int i = 0; i<= NUM_LEDS;i++){
//      
//      leds[i] = ColorFromPalette( PartyColors_p, (i+movementStrobe)/3);
//    }
//    for(int i = 0; i<= NUM_LEDS;i++){
//      
//      leds[i] -= CRGB(210 - mid,140 - mid,210 - mid);
//    }
//  }
   int mapBass = map(bass, 0,255, 0,10);
    movementStrobe += mapBass + (sensitivity -20);
    for(int i = 0; i<= NUM_LEDS;i++){
      
      leds[i] = ColorFromPalette( PartyColors_p, (i+movementStrobe)/3);
      leds[i] -= CRGB(100,40,100);
    }
  
  deltaBass = bass - lastBass;
  lastBass = bass;
  if(deltaBass > (sensitivity +15) ){
    if((millis() - lastStrobe) > 300){
      lastStrobe = millis();
      strobeCount = 0;
    }
  }
  if(strobeCount <= 6){
    if(strobeCount %2 == 0){
      delay(20);
      fill_solid(leds, NUM_LEDS, CRGB(0,0,0));
    }else{
      delay(20);
      fill_solid(leds, NUM_LEDS, CRGB(255,255,255));
    }
    strobeCount++;
  }else{
    
  }
}





void fillColour(){
  if(millis() %3 ==0){
     movement += 4;
  }
 
  for(int i = 0; i<= NUM_LEDS;i++){
    
    leds[i] = ColorFromPalette( PartyColors_p, (i+movement)/3);
  }
  for(int i = 0; i<= NUM_LEDS;i++){
    
    leds[i] -= CRGB(210 - bass,210 - bass,210 - bass);
  }
}



void colourMove(){
   deltaBass = bass - lastBass;
  lastBass = bass;
  int mapBass = map(bass, 0,255, 0,10);
  movement += mapBass + (sensitivity -20);
  for(int i = 0; i<= NUM_LEDS;i++){
    
    leds[i] = ColorFromPalette( PartyColors_p, (i+movement)/3);
  }
  
}
int strobeCounter =0;
bool bassStrobeOn = false;
void strobeBass() {

  deltaBass = bass - lastBass;
  lastBass = bass;
  
  int midMap = map(mid,0,255,0,200);
  int midBackground = 255-midMap;
  fill_solid(leds, NUM_LEDS, CHSV(colour,midBackground,255));
  if(millis() %100 ==0){
    colour+= 4;
  }
  if(deltaBass > (sensitivity +5) ){
    
    colour += 3;
    fill_solid(leds, NUM_LEDS, CHSV(bass,variable,255));
     
     FastLED.show();
     delay(15);
     fill_solid( leds, NUM_LEDS, CRGB(0,0,0)); 
     FastLED.show();
     delay(15);
    strobeCounter= strobeCounter +5;
  }
   
   


  
  
}

int bassBulletArray[50];
int bassBulletCounter = 0;
int midBulletArray[50];
int midBulletCounter = 0;
bool allBlack = true;
//CRGBPalette16 currentPalette = (
//                                   0x9700FF,  0x310CE8,  0x003FFF,  0x0C9CE8,
//                                     0x05FFED );
void bassBullets(){
  fadeToBlackBy(leds,NUM_LEDS,100);
  if(deltaBass >10  ){
    movement += 40;
  }
  for(int i = 0; i<= NUM_LEDS;i++){
    volume = map(volume, 0,255,10,40);
    leds[i] = ColorFromPalette( PartyColors_p , (i+movement)/2,volume);
  }
  
  //For the bass
  for(int i = 0; i <49; i++){
      if(bassBulletArray[i] <(NUM_LEDS-1)){
        bassBulletArray[i] = bassBulletArray[i] +2;
      }
  }
  deltaBass = bass - lastBass;
  lastBass = bass;
  if(deltaBass >30){
    bassBulletArray[bassBulletCounter] = 0;
    bassBulletCounter++;
    colour++;
  }
  for(int i = 0; i <49; i++){
    leds[bassBulletArray[i]] += CHSV( 213, 0, 255);
    leds[bassBulletArray[i]+1] += CHSV( 213, 0, 255);  
  }
  if(bassBulletCounter == 49){
    bassBulletCounter = 0;
  }



   //For the mids
  for(int i = 0; i <49; i++){
      if(midBulletArray[i] <NUM_LEDS){
        midBulletArray[i] = midBulletArray[i] + 1;
      }
  }
  deltaMid = mid - lastMid;
  lastMid = mid;
  if(deltaMid >28){
    midBulletArray[midBulletCounter] = 0;
    midBulletCounter++;
  }
  for(int i = 0; i <49; i++){
    leds[midBulletArray[i]] += CHSV( colour, 200, 255);  
    leds[midBulletArray[i]+1] += CHSV( colour, 200, 255);  
    leds[midBulletArray[i]+2] += CHSV( colour, 200, 255);  
  }
  if(midBulletCounter == 49){
    midBulletCounter = 0;
  }

  
  
  if(deltaBass > (sensitivity + 13)){
    fill_solid(leds,NUM_LEDS,CHSV(255,0,255));
    delay(2);
  }

  for(int i = 0; i <= NUM_LEDS; i++){
    if(leds[i] != CHSV(0,0,0)){
      allBlack = false;
    }
  }
//  if(allBlack){
//    addGlitter(10);
//    Serial.println("black");
//  }
  
}

void addGlitter( fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    
    if((millis() %2 )== 0){
      leds[ random16(NUM_LEDS) ] += CHSV(255,0,mid);
      
      }
  }
      
}




void simpleLightBeat(){
  deltaBass = bass - lastBass;
  lastBass = bass;
  if(deltaBass >10){
    int colourChange = map(bass, 20, 255, 0, 14);
    simpleColour = simpleColour + colourChange;
    fill_solid( leds, NUM_LEDS, CHSV(simpleColour, 225, 255));
  }
}


void animateBrightnessUp(){
  if(brightnessChange){
    
  }
  
}

void white(){
  int yellowness = 120 + (orange *15);
  fill_solid( leds, NUM_LEDS, CHSV(35, yellowness, 255)); 
  if(meggyness){
    fill_solid( leds, NUM_LEDS, CHSV(35, 180, 255));
    if(brightness > 95){ 
      brightness = 95;
    }
  
    
  }
}


void setupValues(){
  bass = MSGEQ7.get(MSGEQ7_BASS);
  mid = MSGEQ7.get(MSGEQ7_MID);
  treble = MSGEQ7.get(MSGEQ7_HIGH);
  bass = mapNoise(bass);
  mid = mapNoise(mid);
  treble = mapNoise(treble);
  volume = MSGEQ7.getVolume();
  volume = mapNoise(volume);
}




void nicksIdea(){
  bassBeat();
  midBeat();
}
void bassBeat(){
  
  deltaBass = bass - lastBass;
  lastBass = bass;

  int bassMap = map(bass, 0, 255, 170, 220);
  int bassMapValue =map(bass, 0,255, 150, 255);
  fill_solid( leds, NUM_LEDS, CHSV(bassMap, 255, bassMapValue)); 

  
  
   if (deltaBass >= 60){
      fill_solid( leds, NUM_LEDS, CRGB(255,255,255)); 
      
   }
}

void midBeat(){
   
  deltaMid = mid - lastMid;
  lastMid = mid;
  mid = mid - (deltaMid/4);
   int midMap = map(mid, 0, 255, 0, 40);
   midMap = midMap - 4;

   for (int i=58; i <= (58 +midMap); i++){
      leds[i] = CHSV(255,0,mid);
   }
   for (int i=58; i >= (58 -midMap); i--){
       leds[i] = CHSV(255,0,mid);
   }
}




//where bass and mid are seperate and each varys with bars of different length on beat
void bassMidBars() {
  fill_solid( leds, NUM_LEDS, CRGB(0, 0, 0));
 for(int i = 0; i <= (NUM_LEDS/3); i++){
   leds[i] = CRGB(bass,0,0);
 }
 for(int i = (NUM_LEDS/3); i <= ((NUM_LEDS/3)*2); i++){
   leds[i] = CRGB(0,mid,0);
 }
 for(int i = ((NUM_LEDS/3)*2); i <= NUM_LEDS; i++){
   leds[i] = CRGB(0,0,treble);
 }
 

}

