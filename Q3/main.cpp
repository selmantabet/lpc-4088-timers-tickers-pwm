/* Selman Tabet (@selmantabet - https://selman.io/) - UIN 724009859
Assignment 4 - Question 3

This function uses the board's ticker interface to switch LEDs every second.
Only one LED would be on at any given time. The sequence is defined as:
LED1 - LED2 - LED3 - LED4
This sequence is done three times in a cyclic manner, then the sequence is 
reversed for another three times, and then back to normal for another three,
and so on.

Developed using the Mbed IDE. Tested on an EA LPC4088 QuickStart Board. */

#include "mbed.h"

Ticker led_flipper; //Define ticker object

DigitalOut my_led1(LED1); //Active Low
DigitalOut my_led2(LED2); //Active Low
DigitalOut my_led3(LED3); //Active High
DigitalOut my_led4(LED4); //Active High

int state_id = 1; //LED Sequence 1-2-4-3 (State 1, 2, 3 and 4, respectively).
int iterator = 0; //To determine the number of turns done so far
int invert_flag = 0; //Clockwise (CW) Vs. Anticlockwise (ACW) ** CW = 0 **

void ledToggle () {
    switch(state_id){
        case 1:
            my_led1 = 0; //LED1 ON
            my_led2 = 1; my_led3 = 0; my_led4 = 0; //Rest of the LEDs OFF.
            if (invert_flag == 1){ //Edge case, a dead-end on ACW turn.
            /*Check if the third turn was completed to invert the direction
            and reset the iterator, otherwise, count as a new turn (iterator
            incremented) and force state 4.*/
                if (iterator == 2) {iterator = 0; invert_flag = 0;}
                else {iterator++; state_id = 4;} //Lap complete, force state 4.
            }
            else state_id++; //Move to state 2.
            break;
        case 2:
            my_led2 = 0; //LED2 ON
            my_led1 = 1; my_led3 = 0; my_led4 = 0; //Rest of the LEDs OFF.
            //Check direction then move accordingly.
            if (invert_flag == 0) state_id++; 
            else state_id--;
            break;
        case 3:
            my_led4 = 1; //LED4 ON
            my_led1 = 1; my_led2 = 1; my_led3 = 0; //Rest of the LEDs OFF.
            //Check direction then move accordingly.
            if (invert_flag == 0) state_id++;
            else state_id--;
            break;
        case 4:
            my_led3 = 1; //LED3 ON
            my_led1 = 1; my_led2 = 1; my_led4 = 0; //Rest of the LEDs OFF.
            if (invert_flag == 0){ //Edge case, a dead-end on CW turn.
            /*Check if the third turn was completed to invert the direction
            and reset the iterator, otherwise, count as a new turn (iterator
            incremented) and force state 1.*/
                if (iterator == 2) {iterator = 0; invert_flag = 1;}
                else {iterator++; state_id = 1;}
            }
            else state_id--;
            break;
        default:
            state_id = 1; //meh, irrelevant to this FSM. 
            //Effective for error handling though :)
    }
}

int main(){
    my_led1 = 1; my_led2 = 1; my_led3 = 0; my_led4 = 0; //All OFF.
    
    led_flipper.attach(&ledToggle, 1.0); //Call ledToggle every second.
    
    while(1); //Run forever.
}