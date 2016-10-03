// FASTLED stuff
#include "FastLED.h"
#define NUM_LEDS 258
#define DATA_PIN 5
CRGB leds[NUM_LEDS];

//MSGEQ7 stuff
#include "MSGEQ7.h"
#define pinAnalogLeft A0
#define pinReset 4
#define pinStrobe 2
#define MSGEQ7_INTERVAL ReadsPerSecond(50)
#define MSGEQ7_SMOOTH 220
CMSGEQ7<MSGEQ7_SMOOTH, pinReset, pinStrobe, pinAnalogLeft> MSGEQ7;

//WIFI Stuff
#include <ESP8266WiFi.h>
const char* ssid = "BTHub5-NQ38";
const char* password = "schoolroadmassive";
WiFiServer server(80);

int bass, mid, treble, inputBass, lastBass, deltaBass, lastMid, deltaMid;
bool onOff = true;
bool brightnessChange = false;
int brightness = 200;
int current = 3;
bool wifiOn = true;

int simpleColour = 0;

void setup() {
  Serial.begin(115200);
  // This will set the IC ready for reading
  MSGEQ7.begin();
  
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  delay(10);
  if(wifiOn){
  // Connect to WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("connecting... ");
  }
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  server.begin();
  }
  
  
  pinMode(15,INPUT);
}

void loop() {
   if(wifiOn){
    WiFiClient client = server.available();
    if(client){
      // Read the first line of the request
      String request = client.readStringUntil('\r');
      Serial.println(request);
      client.flush();
      String s = "HTTP/1.1 200 OK\r\n";
      s += "Content-Type: text/html\r\n\r\n";
      s += "<!DOCTYPE HTML>\r\n<html></html>\r\n";
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
        if(current > 3  ){
          current = 1;
        }
       
      }
      
      client.stop();
    }
   }

  
  if(digitalRead(15) == HIGH){
    onOff = !onOff;
    delay(500);
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
        nicksIdea();
        delay(2);
        break;
      case 2:
        
        bassMidBars();
        delay(3);
        break;
      case 3:
        setupValues();
        simpleLightBeat();
        break;
    }
    FastLED.setBrightness(brightness);
    FastLED.show();
  }else{
    fill_solid( leds, NUM_LEDS, CRGB(0, 0, 0)); 
    FastLED.show();
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
  fill_solid( leds, NUM_LEDS, CHSV(47, 120, 255)); 
  
}


void setupValues(){
  bass = MSGEQ7.get(MSGEQ7_BASS);
  mid = MSGEQ7.get(MSGEQ7_MID);
  treble = MSGEQ7.get(MSGEQ7_HIGH);
  bass = mapNoise(bass);
  mid = mapNoise(mid);
  treble = mapNoise(treble);
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
  int baseline = 50;
  
  // Visualize the average bass of both channels
  uint8_t inputBass = MSGEQ7.get(MSGEQ7_BASS);
  inputBass = mapNoise(inputBass);
  static uint8_t lastInputBass = 0;
  int deltaBass = inputBass - lastInputBass;
  lastInputBass = inputBass;

//  if (deltaBass >= 20){
//    fill_solid( leds, NUM_LEDS, CRGB(inputBass,000,000)); 
//    FastLED.show();
//    Serial.println("bass");
//    
//  }
  
  if(inputBass < baseline-20){
    inputBass =0;
  }else{
    inputBass = inputBass -30;
  }
    uint8_t inputMid = MSGEQ7.get(MSGEQ7_MID);
  inputMid = mapNoise(inputMid);
  if(inputMid < (baseline )){
    inputMid =0;
  }else{
    inputMid = inputMid -50;
  }
    uint8_t inputHigh = MSGEQ7.get(MSGEQ7_HIGH);
  inputHigh = mapNoise(inputHigh);
  if(inputHigh < (baseline + 20)){
    inputHigh =0;
  }else{
    inputHigh = inputHigh -70;
  }
   int bassLevel = map(inputBass, 0, 255, 0, 117);
   for (int i=0; i <= bassLevel; i++){
      leds[i] = CRGB(255,0,0);
   }
   for (int i=bassLevel; i <= 117; i++){
      leds[i] = CRGB(100,100,100);
   }
   int midLevel = map(inputMid, 0, 255, 117, 183);
   for (int i=117; i <= midLevel; i++){
      leds[i] = CRGB(0,255,0);
   }
   for (int i=midLevel; i <= 183; i++){
      leds[i] = CRGB(100,100,100);
   }
   
    for (int i=183; i <= 258; i++){
      leds[i] = CRGB(0,0,inputHigh);
      
   } 

}

