#include "PinChangeInt.h"

#define PIR 2
#define BUTTON 11
#define ROTARY 3

volatile int interruptNumber = 0;

// LED Pin
const int LED_PIN = PB5;

// Timer1 settings
const uint16_t TIMER1_LOAD = 0;
const uint16_t TIMER1_COMPARE = 15625; // 1 second with 1024 prescaler

void setup() {
    Serial.begin(9600);
    
    pinMode(PIR, INPUT);
    pinMode(BUTTON, INPUT);
    pinMode(ROTARY, INPUT);
    
    DDRB |= (1 << LED_PIN); // Set LED pin as output
    
    attachInterrupt(digitalPinToInterrupt(PIR), externalInterrupt1, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ROTARY), externalInterrupt2, RISING);
    attachPinChangeInterrupt(BUTTON, externalInterrupt3, RISING);

    // Configure Timer1
    TCCR1A = 0; // Reset Timer1 control register A
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // CTC mode, prescaler 1024
    
    TCNT1 = TIMER1_LOAD;
    OCR1A = TIMER1_COMPARE;
    TIMSK1 = (1 << OCIE1A); // Enable Timer1 compare interrupt

    sei(); // Enable global interrupts
    
    Serial.println("Program Running...");
}

void loop() {
    delay(100);
}

ISR(TIMER1_COMPA_vect) {
    PORTB ^= (1 << LED_PIN);
    Serial.println("A Timer interrupt has occurred");
}

void externalInterrupt1() {
    interruptNumber = 1;
    Serial.println("External interrupt 1 has occurred (PIR)");
}

void externalInterrupt2() {
    delay(1); // Debounce
    interruptNumber = 2;
    Serial.println("External interrupt 2 has occurred (Rotary)");
}

void externalInterrupt3() {
    delay(1); // Debounce
    interruptNumber = 3;
    Serial.println("External interrupt 3 has occurred (Button)");
}
