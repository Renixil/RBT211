/*
 * LightMeter.c
 *
 * Created: 7/9/2023 6:17:34 PM
 * Author : Renixil
 */ 

#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "util/delay.h"

int main(void) {
    DDRD = 0xFF; // Set all Port D pins for output
    PORTD = 0xFF; // Set all Port D pins HIGH (turn on LEDs)
    PORTB = 0xFF; // Set all Port B pins HIGH (turn on LEDs)

    ADMUX = 0b01100000; // Configure ADC multiplexer

    ADCSRA = 0b10100100; // Configure ADC control and status register A

    ADCSRB = 0b00000000; // Configure ADC control and status register B

    // Timer 2 8-bit timer Fast PWM initialization
    TCCR2A |= (1 << WGM20) | (1 << WGM21);    // Set WGM20 and WGM21 bits for Fast PWM mode
    TCCR2A |= (1 << COM2A1) | (1 << COM2B1);  // Set COM2A1 and COM2B1 bits for non-inverted PWM outputs
    TCCR2B |= (1 << CS21) | (1 << CS20);      // Set prescaler to 32

    OCR2A = 255;    // Set TOP value to 255 (8-bit resolution)
    OCR2B = 0;      // Initialize PWM output to 0

    ADCSRA |= (1 << ADSC);      // Start ADC conversion

    ADCSRA |= (1 << ADATE);     // Enable ADC auto trigger (free running mode)

    ADCSRA |= (1 << ADIE);      // Enable ADC interrupt
    sei();                      // Enable global interrupts

    while (1) {                 // Begin infinite loop
        // Your code here
        // ...
        _delay_ms(25);
    }

    return (0);
}

// ADC complete interrupt service routine
ISR(ADC_vect) {
    PORTD = ADCH; // Update all PD pins with analog input value
    PORTB = ADCH; // Update all PB pins with analog input value
    OCR2B = ADCH; // Update PWM output with analog input value
}
