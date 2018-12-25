#include <Arduino.h>
#include <FastLED.h>
uint8_t lvl = 0;
#define LED_PIN     5
#define NUM_LEDS    50
#define BRIGHTNESS  56
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
uint8_t brightness = 0;
#define UPDATES_PER_SECOND 100
int i = 0;

// This example shows several ways to set up and use 'palettes' of colors
// with FastLED.
//
// These compact palettes provide an easy way to re-colorize your
// animation on the fly, quickly, easily, and with low overhead.
//
// USING palettes is MUCH simpler in practice than in theory, so first just
// run this sketch, and watch the pretty lights as you then read through
// the code.  Although this sketch has eight (or more) different color schemes,
// the entire sketch compiles down to about 6.5K on AVR.
//
// FastLED provides a few pre-configured color palettes, and makes it
// extremely easy to make up your own color schemes with palettes.
//
// Some notes on the more abstract 'theory and practice' of
// FastLED compact palettes are at the bottom of this file.



CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;
uint8_t brights[54] = {0};


void setup() {
    Serial.begin(9600);
    delay( 3000 ); // power-up safety delay
    //FastLED.setDither(0);
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    currentPalette = RainbowColors_p; //RainbowColors_p
    currentBlending = LINEARBLEND;
    
}

void GeneratePulse(uint8_t brightness[], uint8_t events){
  for( int i = 0; i < NUM_LEDS+2; i++){
    if (brightness[i]!=0){
      brightness[i] = brightness[i]-1;
    }
    }
  for( int i = 0; i < events; i++){
    int indx = rand() % 53 + 2;
    brightness[indx] = brightness[indx] + 60;
    brightness[indx+1] = brightness[indx+1] + 15;
    brightness[indx-1] = brightness[indx-1] + 15;
    brightness[indx-2] = brightness[indx-2] + 5;
    brightness[indx+2] = brightness[indx+2] + 5;
  }
  for( int i = 0; i < NUM_LEDS+2; i++){
    if (brightness[i]>255){
      brightness[i] = 255;
    }
    }
}

void FillLEDsFromPaletteColors( uint8_t colorIndex, uint8_t *Thebrights, uint8_t events)
{
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brights[i+2], currentBlending);
        colorIndex += 3;
    }
}


  


void loop()
{
    static uint8_t startIndex = 0;
    i+=1;
    if (i==20){
    startIndex = startIndex + 1;
    i=0;
      }
 /* motion speed */
        if (Serial.peek() > 0) {
                // read the incoming byte:
               lvl = (Serial.parseInt());
               brightness = lvl;
        }
        
        GeneratePulse(brights, lvl);
        FillLEDsFromPaletteColors( startIndex, brights, lvl);
        FastLED.show();
        lvl =0;
    
    //FastLED.delay(1000 / UPDATES_PER_SECOND);
}
