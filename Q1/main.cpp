/* Selman Tabet (@selmantabet - https://selman.io/) - UIN 724009859
Assignment 4 - Question 1

This function leverages the board's SysTick timer to turn LEDs between two
states, one where LEDs 2, 3 and 4 turn on for two seconds, the other state 
turns LED1 and would switch on and off every half a second, for a period of two
seconds. Which means that LED1 would be blinking twice through that period.
The board's clock rate is 120MHz, and since the reload register is 24 bits wide
the maximum value that can be used is around 16.7 million. Thus, the value of
15 million cycles was used for the SysTick implementation here. Being set at 
8th of a second, this reload value would result in eight ticks per second.

Developed using the Mbed IDE. Tested on an EA LPC4088 QuickStart Board. */

#include "mbed.h"

#define STCTRL             (*((volatile unsigned long *) 0xE000E010))
#define STRELOAD           (*((volatile unsigned long *) 0xE000E014))
#define STCURR             (*((volatile unsigned long *) 0xE000E018))

DigitalOut my_led1(LED1); //Active Low
DigitalOut my_led2(LED2); //Active Low
DigitalOut my_led3(LED3); //Active High
DigitalOut my_led4(LED4); //Active High

int ticks = 0; //To keep track of SysTicks done so far.
int state_id = 0; //Zero = LEDs 2-3-4 ON, One = LED1 ON and blinking. 

extern "C" void SysTick_Handler(void){ //SysTick interrupt handler
    ticks++; //tick!
    if (ticks > 15){ //Two seconds (AKA 16 ticks) elapsed, switch states.
        ticks = 0; //Ticks reset.
        if (state_id == 0){ //Switch to LED1 ON.
            my_led2 = 1; my_led3 = 0; my_led4 = 0;
            my_led1 = 0;
        }
        else { //Switch to LED2,3,4 ON.
            my_led2 = 0; my_led3 = 1; my_led4 = 1;
            my_led1 = 1;
        }
        state_id = !state_id; //State alternated.
    }
    else { //Turn ON/OFF every 4 ticks, i.e. every half a second.
        if ((state_id == 1) && (ticks%4 == 0)) my_led1 = !my_led1;
    }
}

int main(void){
    my_led1 = 1; my_led2 = 0; my_led3 = 1; my_led4 = 1; //Default state.
    SystemInit(); //Initialize the oscillator (PLL) on the MCU.
    
    STCTRL |= 0x1 << 0; //Enable system tick counter (bit 0)
    STCTRL |= 0x1 << 1; //Enable system interrupt (bit 1)
    STCTRL |= 0x1 << 2; //Select CPU clock as source (bit 2)
    
    /*Core_clk is 120MHz (which was unspecified in the handout btw -.-)
    Since the max value to reload is almost 17m due to bit-width limitation,
    15m is a choice that would make subsequent tick-related calculations 
    far more convenient.
    
    Half a second = 4 ticks. One second = 8 ticks. Two seconds = 16 ticks.*/
    int RELOAD_VAL = SystemCoreClock/8; //15 million cycles == 8th of a second

    STRELOAD = RELOAD_VAL;
    while(1); //Run forever.
}