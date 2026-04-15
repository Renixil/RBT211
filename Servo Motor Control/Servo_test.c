Code:
/*
* Servo_test.c
*
* Created: 7/16/2023 7:54:08 PM
* Author : Renixil
*/
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
// Datasheet 102
#define PULSE_MIN 999U
#define PULSE_MAX 4799U
#define PULSE_MID 2899U
#define TOP_VALUE 39999 // 50 Hz 39999
// Button pins
#define BUTTON1_PIN 2
#define BUTTON2_PIN 3
#define BUTTON3_PIN 4
// Potentiometer pin
#define POTENTIOMETER_PIN 0
int main(void)
{
// Timer/Counter1 Configuration
ICR1 = TOP_VALUE;
TCCR1A |= (1 << WGM11);
TCCR1A |= (1 << COM1A1); // Sets OC1A as direct output PB1
TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11); // Prescaler of 8
DDRB |= (1 << PB1);
OCR1A = PULSE_MIN;
// Button Configuration
DDRD &= ~((1 << BUTTON1_PIN) | (1 << BUTTON2_PIN) | (1 << BUTTON3_PIN)); // Set button pins as inputs
PORTD |= (1 << BUTTON1_PIN) | (1 << BUTTON2_PIN) | (1 << BUTTON3_PIN); // Enable internal pull-up resistors for
buttons
// Potentiometer Configuration
ADMUX |= (1 << REFS0); // Set reference voltage to AVCC
ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Enable ADC and set ADC prescaler to 128
uint16_t speed = PULSE_MIN; // Initial speed
while (1)
{
// Automatic Spin
for (uint16_t i = PULSE_MIN; i < PULSE_MAX; i = i + 20)
{
OCR1A = i;
_delay_ms(10);
}
for (uint16_t i = PULSE_MAX; i > PULSE_MIN; i = i - 20)
{
OCR1A = i;
_delay_ms(10);
}
// Read Potentiometer Value
ADCSRA |= (1 << ADSC); // Start ADC conversion
while (ADCSRA & (1 << ADSC)); // Wait for conversion to complete
uint16_t potentiometerValue = ADC;
// Button 1 - Set Duty Cycle to 90%
if (!(PIND & (1 << BUTTON1_PIN)))
{
speed = PULSE_MIN + ((PULSE_MAX - PULSE_MIN) * 9 / 10);
}
// Button 2 - Set Duty Cycle to 60%
if (!(PIND & (1 << BUTTON2_PIN)))
{
speed = PULSE_MIN + ((PULSE_MAX - PULSE_MIN) * 6 / 10);
}
// Button 3 - Set Duty Cycle to 30%
if (!(PIND & (1 << BUTTON3_PIN)))
{
speed = PULSE_MIN + ((PULSE_MAX - PULSE_MIN) * 3 / 10);
}
// Potentiometer - Adjust Speed
speed = PULSE_MIN + ((PULSE_MAX - PULSE_MIN) * potentiometerValue / 1023);
// Set Speed
OCR1A = speed;
_delay_ms(10);
}
}
