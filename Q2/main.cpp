/* Selman Tabet (@selmantabet - https://selman.io/) - UIN 724009859
Assignment 4 - Question 2

This function uses the board's built-in timer to measure the amount of time
a user pressed a button. The timer starts upon a button press, and is stopped
upon a button release. The measured time is then printed into through the
board's Serial port.

Developed using the Mbed IDE. Tested on an EA LPC4088 QuickStart Board. */

#include "mbed.h"

Serial pc(USBTX, USBRX); //Serial channel over HDK USB interface
Timer swatch; //Define timer object
InterruptIn button(p23); //Define the pushbutton as an interrupt

void isr1(void){
    swatch.start(); swatch.reset(); //Start and reset timer to zero
    }
    
void isr2(void){ //Stop and measure time elapsed then print via the Serial port
    swatch.stop();
    pc.printf("Pushbutton pressed for %f seconds!\n", swatch.read());
    }

int main(){
    button.mode(PullUp); //Enable pullup resistor
    button.fall(&isr1); //Register an ISR on the falling edge
    button.rise(&isr2); //Register an ISR on the rising edge
    pc.printf("Selman Tabet’s Stopwatch:\n");
    while(1); //Run forever.
}