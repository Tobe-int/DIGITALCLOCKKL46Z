#include "mbed.h"
#include "ShiftOut.h"


#define SET_LATCH() (LATCH = 0)
#define RESET_LATCH() (LATCH = 1)

#define ENABLE_RESET() (RESET = 0)
#define DISABLE_RESET() (RESET = 1)

static char stateArr[8*REGISTER_CNT] = {0};
static bool hasChanged = false;

ShiftOut::ShiftOut(PinName ser, PinName srclk, PinName rclk, 
                   PinName oe, PinName reset) : DSERIAL(ser), SRCLK(srclk), RCLK(rclk), LATCH(oe), RESET(reset)
{
    writeByte(0x00); // Reset the values of the registers to 0  
    if(RESET != NC){  
    DISABLE_RESET();
    }
}

//Pulses the register
void ShiftOut::updateRegister(){
    SRCLK = 1;
    wait_us(2);
    SRCLK = 0;    
}
//Updates the output register
void ShiftOut::updateOutput(){
    RCLK = 1;
    wait_us(2);
    RCLK = 0;    
}
//Writes a byte to the shift register
void ShiftOut::writeByte(unsigned char byte){
    hasChanged = true;
    for(int i = 0; i<8*REGISTER_CNT; i++){
        DSERIAL = (byte & 0x01<<i)>>i;
        updateRegister();    
    } 
    updateOutput();   
}
//Writes a bit to the shift register
void ShiftOut::writeBit(unsigned char bit){
    DSERIAL = bit & 0x01;
    updateRegister();
    updateOutput();
    } 
//Writes multiple bits from an array to create an animation
void ShiftOut::animate(int arr[][8], int lines, int delay_ms){
    hasChanged = true;
    for(int i = 0; i < lines; i++){
        for(int j = 0; j < 8; j++){
           writeBit(arr[i][j]); 
        } 
        wait_ms(delay_ms);   
    }
}

void ShiftOut::animationExample(){
    hasChanged = true;
    int strobe[][8]= {{1,0,0,0,0,0,0,0},
                     {0,1,0,0,0,0,0,0},
                     {0,0,1,0,0,0,0,0},
                     {0,0,0,1,0,0,0,0},
                     {0,0,0,0,1,0,0,0},
                     {0,0,0,0,0,1,0,0},
                     {0,0,0,0,0,0,1,0},
                     {0,0,0,0,0,0,0,1}};
                     
    int nightrider[18][8]= {{1,0,0,0,0,0,0,0},
                           {1,1,0,0,0,0,0,0},
                           {1,1,1,0,0,0,0,0},
                           {0,1,1,1,0,0,0,0},
                           {0,0,1,1,1,0,0,0},
                           {0,0,0,1,1,1,0,0},
                           {0,0,0,0,1,1,1,0},
                           {0,0,0,0,0,1,1,1},
                           {0,0,0,0,0,0,1,1},
                           {0,0,0,0,0,0,0,1},
                           {0,0,0,0,0,0,1,1},
                           {0,0,0,0,0,1,1,1},
                           {0,0,0,0,1,1,1,0},
                           {0,0,0,1,1,1,0,0},
                           {0,0,1,1,1,0,0,0},
                           {0,1,1,1,0,0,0,0},
                           {1,1,1,0,0,0,0,0},
                           {1,1,0,0,0,0,0,0}};
                        
        animate(nightrider, 18, 50);
        wait(1);
        animate(strobe, 8, 200);
    }
    
void ShiftOut::writeBitAtPos(unsigned char pin, bool state){
    if(hasChanged){
        clearStateArray();
        hasChanged = false;
    } 
    if(pin < 8*REGISTER_CNT){
        stateArr[pin] = state;        
    }
    writeArray(stateArr);
}

void ShiftOut::writeArray(char arr[8*REGISTER_CNT]){
    for(int i = (8*REGISTER_CNT)-1; i >= 0; i--) {
        writeBit(arr[i]);
    }       
    
}

void ShiftOut::clearStateArray(){
    for(int i = 0; i < 8*REGISTER_CNT; i++){
        stateArr[i] = 0;
        }
    }