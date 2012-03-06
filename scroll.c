// Based on code in Project 21 of Beginning Arduino by Mike McRoberts

#include <avr/pgmspace.h>
#include <util/delay.h>
#include "tlc5940.h"
#include "scroll.h"

static uint8_t font[][8] PROGMEM = {
  // The printable ASCII characters only (32-126)
  {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000}, 
  {0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000000, 0b00000100}, 
  {0b00001010, 0b00001010, 0b00001010, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000}, 
  {0b00000000, 0b00001010, 0b00011111, 0b00001010, 0b00011111, 0b00001010, 0b00011111, 0b00001010}, 
  {0b00000111, 0b00001100, 0b00010100, 0b00001100, 0b00000110, 0b00000101, 0b00000110, 0b00011100}, 
  {0b00011001, 0b00011010, 0b00000010, 0b00000100, 0b00000100, 0b00001000, 0b00001011, 0b00010011}, 
  {0b00000110, 0b00001010, 0b00010010, 0b00010100, 0b00001001, 0b00010110, 0b00010110, 0b00001001}, 
  {0b00000100, 0b00000100, 0b00000100, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000}, 
  {0b00000010, 0b00000100, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00000100, 0b00000010}, 
  {0b00001000, 0b00000100, 0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000100, 0b00001000}, 
  {0b00010101, 0b00001110, 0b00011111, 0b00001110, 0b00010101, 0b00000000, 0b00000000, 0b00000000}, 
  {0b00000000, 0b00000000, 0b00000100, 0b00000100, 0b00011111, 0b00000100, 0b00000100, 0b00000000}, 
  {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000110, 0b00000100, 0b00001000}, 
  {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00001110, 0b00000000, 0b00000000, 0b00000000}, 
  {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000100}, 
  {0b00000001, 0b00000010, 0b00000010, 0b00000100, 0b00000100, 0b00001000, 0b00001000, 0b00010000}, 
  {0b00001110, 0b00010001, 0b00010011, 0b00010001, 0b00010101, 0b00010001, 0b00011001, 0b00001110}, 
  {0b00000100, 0b00001100, 0b00010100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00011111}, 
  {0b00001110, 0b00010001, 0b00010001, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00011111}, 
  {0b00001110, 0b00010001, 0b00000001, 0b00001110, 0b00000001, 0b00000001, 0b00010001, 0b00001110}, 
  {0b00010000, 0b00010000, 0b00010100, 0b00010100, 0b00011111, 0b00000100, 0b00000100, 0b00000100}, 
  {0b00011111, 0b00010000, 0b00010000, 0b00011110, 0b00000001, 0b00000001, 0b00000001, 0b00011110}, 
  {0b00000111, 0b00001000, 0b00010000, 0b00011110, 0b00010001, 0b00010001, 0b00010001, 0b00001110}, 
  {0b00011111, 0b00000001, 0b00000001, 0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00010000}, 
  {0b00001110, 0b00010001, 0b00010001, 0b00001110, 0b00010001, 0b00010001, 0b00010001, 0b00001110}, 
  {0b00001110, 0b00010001, 0b00010001, 0b00001111, 0b00000001, 0b00000001, 0b00000001, 0b00000001}, 
  {0b00000000, 0b00000100, 0b00000100, 0b00000000, 0b00000000, 0b00000100, 0b00000100, 0b00000000}, 
  {0b00000000, 0b00000100, 0b00000100, 0b00000000, 0b00000000, 0b00000100, 0b00000100, 0b00001000}, 
  {0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00001000, 0b00000100, 0b00000010, 0b00000001}, 
  {0b00000000, 0b00000000, 0b00000000, 0b00011110, 0b00000000, 0b00011110, 0b00000000, 0b00000000}, 
  {0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00000010, 0b00000100, 0b00001000, 0b00010000}, 
  {0b00001110, 0b00010001, 0b00010001, 0b00000010, 0b00000100, 0b00000100, 0b00000000, 0b00000100}, 
  {0b00001110, 0b00010001, 0b00010001, 0b00010101, 0b00010101, 0b00010001, 0b00010001, 0b00011110}, 
  {0b00001110, 0b00010001, 0b00010001, 0b00010001, 0b00011111, 0b00010001, 0b00010001, 0b00010001}, 
  {0b00011110, 0b00010001, 0b00010001, 0b00011110, 0b00010001, 0b00010001, 0b00010001, 0b00011110}, 
  {0b00000111, 0b00001000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00001000, 0b00000111}, 
  {0b00011100, 0b00010010, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010010, 0b00011100},
  {0b00011111, 0b00010000, 0b00010000, 0b00011110, 0b00010000, 0b00010000, 0b00010000, 0b00011111}, 
  {0b00011111, 0b00010000, 0b00010000, 0b00011110, 0b00010000, 0b00010000, 0b00010000, 0b00010000}, 
  {0b00001110, 0b00010001, 0b00010000, 0b00010000, 0b00010111, 0b00010001, 0b00010001, 0b00001110}, 
  {0b00010001, 0b00010001, 0b00010001, 0b00011111, 0b00010001, 0b00010001, 0b00010001, 0b00010001}, 
  {0b00011111, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00011111}, 
  {0b00011111, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00010100, 0b00001000}, 
  {0b00010001, 0b00010010, 0b00010100, 0b00011000, 0b00010100, 0b00010010, 0b00010001, 0b00010001}, 
  {0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00011111}, 
  {0b00010001, 0b00011011, 0b00011111, 0b00010101, 0b00010001, 0b00010001, 0b00010001, 0b00010001}, 
  {0b00010001, 0b00011001, 0b00011001, 0b00010101, 0b00010101, 0b00010011, 0b00010011, 0b00010001}, 
  {0b00001110, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00001110}, 
  {0b00011110, 0b00010001, 0b00010001, 0b00011110, 0b00010000, 0b00010000, 0b00010000, 0b00010000}, 
  {0b00001110, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010101, 0b00010011, 0b00001111}, 
  {0b00011110, 0b00010001, 0b00010001, 0b00011110, 0b00010100, 0b00010010, 0b00010001, 0b00010001}, 
  {0b00001110, 0b00010001, 0b00010000, 0b00001000, 0b00000110, 0b00000001, 0b00010001, 0b00001110}, 
  {0b00011111, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100}, 
  {0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00001110}, 
  {0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00001010, 0b00000100}, 
  {0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00010101, 0b00010101, 0b00001010}, 
  {0b00010001, 0b00010001, 0b00001010, 0b00000100, 0b00000100, 0b00001010, 0b00010001, 0b00010001}, 
  {0b00010001, 0b00010001, 0b00001010, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100}, 
  {0b00011111, 0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00010000, 0b00011111}, 
  {0b00001110, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00001110}, 
  {0b00010000, 0b00001000, 0b00001000, 0b00000100, 0b00000100, 0b00000010, 0b00000010, 0b00000001}, 
  {0b00001110, 0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00001110}, 
  {0b00000100, 0b00001010, 0b00010001, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000}, 
  {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011111}, 
  {0b00001000, 0b00000100, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000}, 
  {0b00000000, 0b00000000, 0b00000000, 0b00001110, 0b00010010, 0b00010010, 0b00010010, 0b00001111}, 
  {0b00000000, 0b00010000, 0b00010000, 0b00010000, 0b00011100, 0b00010010, 0b00010010, 0b00011100}, 
  {0b00000000, 0b00000000, 0b00000000, 0b00001110, 0b00010000, 0b00010000, 0b00010000, 0b00001110}, 
  {0b00000000, 0b00000001, 0b00000001, 0b00000001, 0b00000111, 0b00001001, 0b00001001, 0b00000111}, 
  {0b00000000, 0b00000000, 0b00000000, 0b00011100, 0b00010010, 0b00011110, 0b00010000, 0b00001110}, 
  {0b00000000, 0b00000011, 0b00000100, 0b00000100, 0b00000110, 0b00000100, 0b00000100, 0b00000100}, 
  {0b00000000, 0b00001110, 0b00001010, 0b00001010, 0b00001110, 0b00000010, 0b00000010, 0b00001100}, 
  {0b00000000, 0b00010000, 0b00010000, 0b00010000, 0b00011100, 0b00010010, 0b00010010, 0b00010010}, 
  {0b00000000, 0b00000000, 0b00000100, 0b00000000, 0b00000100, 0b00000100, 0b00000100, 0b00000100}, 
  {0b00000000, 0b00000010, 0b00000000, 0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00001100}, 
  {0b00000000, 0b00010000, 0b00010000, 0b00010100, 0b00011000, 0b00011000, 0b00010100, 0b00010000}, 
  {0b00000000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00001100}, 
  {0b00000000, 0b00000000, 0b00000000, 0b00001010, 0b00010101, 0b00010001, 0b00010001, 0b00010001}, 
  {0b00000000, 0b00000000, 0b00000000, 0b00010100, 0b00011010, 0b00010010, 0b00010010, 0b00010010}, 
  {0b00000000, 0b00000000, 0b00000000, 0b00001100, 0b00010010, 0b00010010, 0b00010010, 0b00001100}, 
  {0b00000000, 0b00011100, 0b00010010, 0b00010010, 0b00011100, 0b00010000, 0b00010000, 0b00010000}, 
  {0b00000000, 0b00001110, 0b00010010, 0b00010010, 0b00001110, 0b00000010, 0b00000010, 0b00000001}, 
  {0b00000000, 0b00000000, 0b00000000, 0b00001010, 0b00001100, 0b00001000, 0b00001000, 0b00001000}, 
  {0b00000000, 0b00000000, 0b00001110, 0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00011110}, 
  {0b00000000, 0b00010000, 0b00010000, 0b00011100, 0b00010000, 0b00010000, 0b00010000, 0b00001100}, 
  {0b00000000, 0b00000000, 0b00000000, 0b00010010, 0b00010010, 0b00010010, 0b00010010, 0b00001100}, 
  {0b00000000, 0b00000000, 0b00000000, 0b00010001, 0b00010001, 0b00010001, 0b00001010, 0b00000100}, 
  {0b00000000, 0b00000000, 0b00000000, 0b00010001, 0b00010001, 0b00010001, 0b00010101, 0b00001010}, 
  {0b00000000, 0b00000000, 0b00000000, 0b00010001, 0b00001010, 0b00000100, 0b00001010, 0b00010001}, 
  {0b00000000, 0b00000000, 0b00010001, 0b00001010, 0b00000100, 0b00001000, 0b00001000, 0b00010000}, 
  {0b00000000, 0b00000000, 0b00000000, 0b00011111, 0b00000010, 0b00000100, 0b00001000, 0b00011111}, 
  {0b00000010, 0b00000100, 0b00000100, 0b00000100, 0b00001000, 0b00000100, 0b00000100, 0b00000010}, 
  {0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100}, 
  {0b00001000, 0b00000100, 0b00000100, 0b00000100, 0b00000010, 0b00000100, 0b00000100, 0b00001000}, 
  {0b00000000, 0b00000000, 0b00000000, 0b00001010, 0b00011110, 0b00010100, 0b00000000, 0b00000000}  
};

#define GREEN(x) ((x) * 2)
#define RED(x) (((x) * 2)  + 1)

void scroll(char myString[]) {
  uint8_t firstChrRow, secondChrRow;
  uint8_t ledOutput;
  uint8_t chrPointer = 0; // Initialise the string position pointer
  uint8_t Char1, Char2; // the two characters that will be displayed
  uint8_t scrollBit = 0;
  uint8_t strLength = 0;
  //unsigned long time;
  //unsigned long counter;
  
  // Increment count till we reach the string 
  while (myString[strLength]) {strLength++;}
  
  for (;;) {
    while (chrPointer < (strLength-1)) {
      while(gsUpdateFlag); // wait until we can modify gsData
      Char1 = myString[chrPointer];
      Char2 = myString[chrPointer+1];
      for (uint8_t y= 0; y<8; y++) {
        firstChrRow = pgm_read_byte(&font[Char1 - 32][y]);
        secondChrRow = (pgm_read_byte(&font[Char2 - 32][y])) << 1;
        ledOutput = (firstChrRow << scrollBit) | (secondChrRow >> (8 - scrollBit) );
        for (uint8_t x = 0; x < 8; x++) {
          if (ledOutput & 0x80) {
            TLC5940_SetGS(y, RED(x), 0);
            TLC5940_SetGS(y, GREEN(x), 4095);
          } else {
            TLC5940_SetGS(y, RED(x), 0);
            TLC5940_SetGS(y, GREEN(x), 0);
          }
          ledOutput <<= 1;
        }
      }
      scrollBit++; 
      if (scrollBit > 6) { 
        scrollBit = 0;
        chrPointer++;
      }
      TLC5940_SetGSUpdateFlag();
      _delay_ms(50);
    }
    chrPointer = 0;
  }
}



