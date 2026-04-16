Code:
/*
* Interrupts, Timers, and more!.c
*
* Created: 6/25/2023 1:09:24 PM
* Author : Renixil
*/
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <avr/interrupt.h>
// Define your pins, ports, and DDR here
#define LED0 PB5 // Onboard LED
#define LED1 PB0 // External LED 1
#define LED2 PB1 // External LED 2
#define BUTTON PD2 // Push button
int main(void)
{
// Set up pins as inputs or outputs
DDRB |= (1 << LED0) | (1 << LED1) | (1 << LED2); // LED pins as outputs
DDRD &= ~(1 << BUTTON); // Button pin as input
PORTD |= (1 << BUTTON); // Enable internal pull-up resistor for button pin
// Turn off the onboard LED
PORTB &= ~(1 << LED0);
// Timer 0 setup - 8-bit timer
TCCR0A = (1 << WGM01); // CTC mode
TCCR0B = (1 << CS02) | (1 << CS00); // Prescaler = 1024
TIMSK0 = (1 << OCIE0A); // Enable Timer 0 compare match A interrupt
OCR0A = 156; // Count to 156 (1 second delay with prescaler 1024)
// Timer 1 setup - 16-bit timer
TCCR1A = 0; // Normal mode
TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // CTC mode, Prescaler = 1024
TIMSK1 = (1 << OCIE1A); // Enable Timer 1 compare match A interrupt
OCR1A = 15625; // Count to 15625 (1 second delay with prescaler 1024)
// External Interrupt setup
EICRA |= (1 << ISC01) | (1 << ISC00); // INT0 generates interrupt request
EIMSK |= (1 << INT0); // Enable external interrupt INT0
sei(); // Enable global interrupts
while (1)
{
// No code should exist here
}
return (0);
}
// Timer 0 compare match A interrupt
ISR(TIMER0_COMPA_vect)
{
// No code here, to maintain onboard LED state
}
// Timer 1 compare match A interrupt
ISR(TIMER1_COMPA_vect)
{
// Toggle LED1 and LED2
PORTB ^= (1 << LED1) | (1 << LED2);
}
// External Interrupt 0 ISR
ISR(INT0_vect)
{
if (!(PIND & (1 << BUTTON))) // Check if button is pressed
{
PORTB |= (1 << LED0); // Turn LED0 ON
}
else
{
PORTB &= ~(1 << LED0); // Turn LED0 OFF
}
}
