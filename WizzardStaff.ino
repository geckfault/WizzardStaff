#include "FastLED.h"

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    5
#define DATA_PIN2   6

#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS_PER_STRIP 60
#define NUM_STRIPS 2
#define NUM_LEDS NUM_LEDS_PER_STRIP * NUM_STRIPS

int mappedLEDs [] { 
  59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119
};
CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];


#define FRAMES_PER_SECOND  120

//fire2012 55
#define COOLING  18
#define SPARKING 120
bool gReverseDirection = false;
//buttonstuff

int pushButton1 = 2;
int pushButton2 = 3;
int buttonState1 = 0;
int lastButtonState1=0;
int buttonState2 = 0;
int lastButtonState2 = 0;




//exploder
uint32_t looptime = 5000L;

void setup() {
    Serial.begin(57600);
  Serial.println("resetting");
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
    FastLED.addLeds<LED_TYPE,DATA_PIN2,COLOR_ORDER>(leds,0, NUM_LEDS).setCorrection(TypicalLEDStrip);

  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds,60, NUM_LEDS).setCorrection(TypicalLEDStrip);
  pinMode(pushButton1, INPUT);
  pinMode(pushButton2, INPUT);

  // set master brightness control
  FastLED.setBrightness(20);
        leds[mappedLEDs[0]] = CRGB::Red;
          
        leds[mappedLEDs[58]] = CRGB::Green;
        leds[mappedLEDs[59]] = CRGB::Green;

        leds[mappedLEDs[60]] = CRGB::Blue;
        leds[mappedLEDs[61]] = CRGB::Blue;
        leds[mappedLEDs[62]] = CRGB::Blue;

        leds[mappedLEDs[116]] = CRGB::Cyan;
        leds[mappedLEDs[117]] = CRGB::Cyan;
        leds[mappedLEDs[118]] = CRGB::Cyan;
        leds[mappedLEDs[119]] = CRGB::Cyan;

      FastLED.show(); 
       delay(1000);
  
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {confetti,sinelon, rainbow, fire2012 };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns


void loop()
{
  //potentiometer brightness
    int sensorValue = analogRead(A0);
int mappedValue = map(sensorValue, 0, 1023, 0, 255);
FastLED.setBrightness(mappedValue);
  

  
  
  
  
  int buttonState1 = digitalRead(pushButton1);
  int buttonState2 = digitalRead(pushButton2);
if(buttonState1 != lastButtonState1){
  if(buttonState1 == HIGH)
  {
    nextPattern();
    Serial.print("Nexz");
  }
}
if(buttonState2 != lastButtonState2){
  if(buttonState2 == HIGH)
  {
    exploder();
    Serial.print("Badabumm");
  }
}


  //Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 5 ) { gHue++; } // slowly cycle the "base color" through the rainbow

  
  
  //EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
  //delay(3000);
  //Serial.print("Button1 ");
  //Serial.println( buttonState1);
  //Serial.print("Button2 ");
  //Serial.println(buttonState2);
  //Serial.print("Pot ");
  //Serial.print(sensorValue);
  //Serial.print(" Mapped:");
//Serial.println(mappedValue);

   lastButtonState1 = buttonState1;
   lastButtonState2 = buttonState2;


}



#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}



void twinkle()
{
  fill_solid( leds, NUM_LEDS, CRGB::Black);
  addGlitter(80);
 }

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[mappedLEDs[pos]] += CHSV( gHue, 255, 192);
}



void exploder()
{
  static uint8_t hue=0;
  fill_solid( leds, NUM_LEDS, CRGB::Black);
  delay(1000);
  
  //for ( uint32_t tStart = millis();  (millis()-tStart) < 4000;  )
  //{
    for(int i = 0;i < NUM_LEDS; i++)
    {
      leds[mappedLEDs[mappedLEDs[i]]] = CRGB::Green;
      FastLED.show();
      FastLED.delay(500); 
      fadeToBlackBy( leds, NUM_LEDS, 50);
    }
  //}
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
  addGlitter(80);
}


void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void fire2012()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[mappedLEDs[pixelnumber]] = color;
    }
}

