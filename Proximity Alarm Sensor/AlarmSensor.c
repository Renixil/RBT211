Code
/*
 * AlarmSensor.c
 *
 * Created: 7/12/2023 6:42:21 PM
 * Author : Renixil
 */
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Ultrasonic sensor pins
#define TRIG_PIN    PD2    // (Trig)
#define ECHO_PIN    PD3    //(Echo)

// Buzzer and LED pins
#define BUZZER_PIN  PD7
#define LED_PIN     PD6

// Variables for ultrasonic distance calculation
volatile uint16_t pulse_duration = 0;
volatile uint16_t distance_cm = 0;

// Flag to track the presence of an object
volatile uint8_t object_detected = 0;

// Function to initialize ultrasonic sensor
void ultrasonic_init()
{
    // Configure TRIG_PIN as output
    DDRD |= (1 << TRIG_PIN);
    
    // Configure ECHO_PIN as input
    DDRD &= ~(1 << ECHO_PIN);
    
    // Enable external interrupt INT0 for ECHO_PIN
    EIMSK |= (1 << INT0);
    
    // Configure INT0 to trigger on any logical change
    EICRA |= (1 << ISC00);
    
    // Enable global interrupts
    sei();
}

// Function to generate ultrasonic pulse
void ultrasonic_trigger()
{
    // Generate a 10us pulse on TRIG_PIN
    PORTD |= (1 << TRIG_PIN);
    _delay_us(10);
    PORTD &= ~(1 << TRIG_PIN);
}

// Interrupt service routine for ultrasonic sensor
ISR(INT0_vect)
{
    static uint16_t start_time = 0;
    
    if (PIND & (1 << ECHO_PIN)) {
        // Rising edge, start time
        start_time = TCNT1;
    } else {
        // Falling edge, calculate pulse duration
        pulse_duration = TCNT1 - start_time;
        
        // Convert pulse duration to distance in cm
        distance_cm = (pulse_duration * 10) / 58;
        
        // Set object detected flag
        if (distance_cm <= 10) {
            object_detected = 1;
        } else {
            object_detected = 0;
        }
    }
}

// Function to initialize buzzer and LED
void output_init()
{
    // Configure BUZZER_PIN and LED_PIN as outputs
    DDRD |= (1 << BUZZER_PIN) | (1 << LED_PIN);
    
    // Turn off the buzzer and LED initially
    PORTD &= ~(1 << BUZZER_PIN) & ~(1 << LED_PIN);
}

// Function to start the buzzer and turn on the LED
void start_alarm()
{
    // Enable the buzzer and LED
    PORTD |= (1 << BUZZER_PIN) | (1 << LED_PIN);
    
    // Configure Timer2 for PWM on BUZZER_PIN
    TCCR2A |= (1 << COM2A1) | (1 << WGM20) | (1 << WGM21); // Fast PWM, non-inverted mode
    TCCR2B |= (1 << CS21); // Prescaler 8
}

// Function to stop the buzzer and turn off the LED
void stop_alarm()
{
    // Disable Timer2 PWM on BUZZER_PIN
    TCCR2A &= ~((1 << COM2A1) | (1 << WGM20) | (1 << WGM21));
    TCCR2B &= ~(1 << CS21);
    
    // Disable the buzzer and LED
    PORTD &= ~(1 << BUZZER_PIN) & ~(1 << LED_PIN);
}

int main(void)
{
    // Initialize ultrasonic sensor
    ultrasonic_init();
    
    // Initialize buzzer and LED
    output_init();
    
    // Initialize Timer1 for time measurement
    TCCR1B |= (1 << CS10);  // Prescaler 1
    
    while (1)
    {
        // Trigger ultrasonic sensor to measure distance
        ultrasonic_trigger();
        
        // Control buzzer and LED based on distance
        if (object_detected) {
            // Object detected, trigger alarm
            start_alarm();
        } else {
            // No object detected, stop the alarm
            stop_alarm();
        }
        
        _delay_ms(5);  // Delay between measurements
    }
}
