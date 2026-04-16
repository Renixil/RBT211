Code:
/*
* External Interrupts.c
*
* Created: 6/21/2023 8:32:31 PM
* Author : Renixil
*/
#ifndef F_CPU
#define F_CPU 16000000UL // Utilizes the 16MHz clock onboard the Arduino
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
int main(void)
{
// Set inputs and outputs
DDRB |= (1 << DDB5); // Set Pin 13 (PB5) as output for the onboard LED
DDRD &= ~(1 << PD2); // Clear pin PD2 for input
PORTD |= (1 << PD2); // Set the internal pull-up resistor
DDRD |= (1 << PD7); // Set pin PD7 as output
// SET UP INTERRUPT
EIMSK |= (1 << INT0);
EICRA |= (1 << ISC00);
sei();
/* Replace with your application code */
while (1)
{
// CREATE A BLINKING LED
// Toggle the LED connected to Pin 7 on and off with a delay
PORTD ^= (1 << PD7); // Toggle the LED state
_delay_ms(1000); // Delay for 1 second
}
return 0;
}
ISR(INT0_vect)
{
if (!(PIND & (1 << PD2)))
{
PORTB |= (1 << PB5); // Set PB5 high to turn on the onboard LED
}
else if (PIND & (1 << PD2))
{
PORTB &= ~(1 << PB5); // Set PB5 low to turn off the onboard LED
}
else
{
// Do nothing
}
}
