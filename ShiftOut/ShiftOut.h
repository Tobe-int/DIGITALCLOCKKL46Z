/*
 * Copyright 2015 Benjamin R. Moeklegaard
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#ifndef SHIFT_H
#define SHIFT_H

//Constant for managing n-numbers of registers, Function supported (writeByte, writeBit, writeBitAtPos)
#define REGISTER_CNT 1 
/**
 *  This is a library for easy interfacing with the SN74HC595N 8-bit Shift Register.
 *  The library includes functions for writing bits, bytes and animation array to the register.
 *  The Functions are mainly based for writting 8-bits or one byte and can be moddified to work
 *  with multiple shift registers.
 *@code
 * #include "mbed.h"
 * #include "ShiftOut.h"
 * ShiftOut reg(PA_8, PA_9, PC_7, PB_6, D1);
 *
 * int main(){              
 *    while(1){
 *     reg.writeBitAtPos(3, 1);
 *     wait(2);
 *     reg.writeByte(0x30);
 *     wait(2);
 *    }
 * }@endcode
 */

class ShiftOut
{
public:

    /** Create a ShiftOut interface
      *
      * @param ser      Serial data line
      * @param srclk    Data register clock
      * @param rclk     Output register clock
      * @param oe       Output enable (Active Low)
      * @param reset    Reset line for data register (Active Low)
      * Writing Byte Example:
      * @code
      * #include "mbed.h"
      * #include "ShiftOut.h"
      *
      * ShiftOut reg(PA_8, PA_9, PC_7, PB_6, D1);
      * int main()
      * {
      *     reg.writeByte(0x00); //Writes each bit to the SN74HC595N
      *     while(1) {
      *         wait_ms(300);
      *     }
      * }
      * @endcode
      */


ShiftOut(PinName ser, PinName srclk, PinName rclk,
         PinName oe, PinName reset);

/**
 * Writes a byte to the shift register
 * @param byte   0xXX or numbers from 0-255
 */
void writeByte(unsigned char);




/**
 * Writes a bit to the first output on the shift register
 * @param bit   0 or 1
 */

void writeBit(unsigned char);

/**
 * Writes bits from an 2D array, with configurable delay
 * @param int array, int lines, int delay_ms 
 * writes a 2D array with n-lines with a configurable delay in ms 
 * @code
 * #include "mbed.h"
 * #include "ShiftOut.h"
 * ShiftOut reg(PA_8, PA_9, PC_7, PB_6, D1);
 *
 * int main(){
 *   int strobe[][8]= {{1,0,0,0,0,0,0,0},
 *                    {0,1,0,0,0,0,0,0},
 *                    {0,0,1,0,0,0,0,0},
 *                    {0,0,0,1,0,0,0,0},
 *                    {0,0,0,0,1,0,0,0},
 *                    {0,0,0,0,0,1,0,0},
 *                    {0,0,0,0,0,0,1,0},
 *                    {0,0,0,0,0,0,0,1}};
 *      while(1){           
 *          reg.animate(strobe, 8, 200);
 *      }
 *  }
 * @endcode
 */

void animate(int[][8], int, int);

/**
 *  Demonstrates two animation examples by using the animate function
 */

void animationExample(void);

/**
 *   Writes the desired state to the output on the shift register
 *   @param  char output, state
 */

void writeBitAtPos(unsigned char, bool);

/**
 *   Writes the corresponding array item to the output on the shift register
 *   @param  char array   writes to the output from a state array
 */

void writeArray(char[8]);

protected:
void updateRegister(void);
void updateOutput(void);
void clearStateArray(void);
DigitalOut DSERIAL, LATCH, RCLK, SRCLK, RESET; 

};

#endif