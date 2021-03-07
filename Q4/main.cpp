/* Selman Tabet (@selmantabet - https://selman.io/) - UIN 724009859
Assignment 4 - Question 4

This function uses the PWM interface to control the duty cycle and period of
LED1. The duty cycle defines how much, in percentage, of a period does a 
does the board send a logical HIGH signal to the LED. Given that LED1 is
Active Low, that would translate into how much of a time period would the LED
be off.

A short button press would result in a slight increase to the duty cycle,
while a press that is longer than 3 seconds would result in an increase in
the period. When the maximum duty cycle value is reached (1.0f or 100%), the
next short press would result in its reset to 0.0f. This same approach is taken
for the time period, as the limit of 5 seconds is reached, it would be reset to
one second on a long press.

Developed using the Mbed IDE. Tested on an EA LPC4088 QuickStart Board. */

#include "mbed.h"

Serial pc(USBTX, USBRX); //Serial channel over HDK USB interface
Timer swatch; //Define timer object
InterruptIn button(p23);

PwmOut my_led1(LED1); //Define LED1 as PWM output (Active Low)
DigitalOut my_led2(LED2); //Active Low
DigitalOut my_led3(LED3); //Active High
DigitalOut my_led4(LED4); //Active High

float period = 2.0f; //Initial period
float duty_cycle = 0.50f; //Initial duty cycle


void isr1(void){ //Start counting
    swatch.start(); swatch.reset(); //Start and reset timer to zero
    }

void isr2(void){
    swatch.stop(); //Stop counting and measure to see which action to take
    if (swatch.read() < 3.0f){
        if (duty_cycle >= 1.0f) duty_cycle = 0.0f; //Duty cycle reset
        else duty_cycle += 0.1f; //Increment duty cycle by 10%
        my_led1.period(period); //Enforce new PWM parameters
        my_led1.write(duty_cycle);
        pc.printf("LED1 PWM Duty Cycle: %f\n", duty_cycle);
    }
    else {
        if (period >= 5.0f) period = 1.0f; // >= because of an IEEE 754 quirk
        else period += 1.0f; //Increment period by 100ms
        pc.printf("LED1 Period: %f seconds\n", period);
        my_led1.period(period); //Enforce new PWM parameters
        my_led1.write(duty_cycle);
    }
}

int main(){
    button.mode(PullUp); //Initialize button resistor.
    pc.printf("Selman Tabet’s PWM Controller:\n");
    my_led2 = 1; my_led3 = 0; my_led4 = 0; //All LEDs (except LED1) OFF
    my_led1.period(period); //Period set to two seconds at the start
    my_led1.write(duty_cycle); //Initially set to 50%
    
    button.fall(&isr1); //Register an ISR on the falling edge
    button.rise(&isr2); //Register an ISR on the rising edge
    
    while(1); //Run forever.
}