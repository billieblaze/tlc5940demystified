/*

  main.c

  Copyright 2010-2011 Matthew T. Pandina. All rights reserved.
  Copyright 2012 J.B. Langston III. All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY MATTHEW T. PANDINA "AS IS" AND ANY EXPRESS OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
  EVENT SHALL MATTHEW T. PANDINA OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
#include <avr/interrupt.h>
#include "tlc5940.h"
#include "plasma.h"
#include "scroll.h"
#include "pixel.c"

#define BLUE(x) (x+32)
#define GREEN(x) (x+16)
#define RED(x) (x)



void drawPixel(int row, int col, int r, int g, int b) {
		TLC5940_SetGS(row, RED(col), r);
		TLC5940_SetGS(row, GREEN(col), g);
		TLC5940_SetGS(row, BLUE(col), b);
	
		TLC5940_SetGSUpdateFlag();
	
}


int main(void) {
  TLC5940_Init();

#if (TLC5940_INCLUDE_DC_FUNCS)
  TLC5940_SetAllDC(63);
  TLC5940_ClockInDC();
#endif

  // Manually clock in last set of values to be multiplexed
  TLC5940_ClockInGS();

  // Enable Global Interrupts
  sei();

  //do_plasma();
  //scroll(" Hello, world! I \x03 TLC5940. ");
	
	//int x = 0;
	int y = 0;
	
	drawPixel(y,0,2048,0,0);
	drawPixel(y,1,0,2048,0);
	drawPixel(y,2,0,0,2048);
	
	drawPixel(2,12,4090,0,0);
	drawPixel(3,12,0,4090,0);
	drawPixel(4,12,0,0,4090);
	
	
	drawPixel(4,15,4090,4090,0);
	drawPixel(5,15,0,4090,4090);
	drawPixel(6,15,4090,0,4090);
	drawPixel(7,15,4090,4090,4090);
	
	for (;;) {
		while(gsUpdateFlag);
			
		
	}
	
  
  return 0;
}

