#include <Arduino.h>
#include <avr/sleep.h>

#define ledpin 0
#define anPin 1
#define ausPin 2
//prototype
void gotoSleep(void);

volatile bool statusLeuchten = false;
bool statusSleep = true;
volatile byte myIntFlags = 0;

//Blinking LED on ATTINY85 (0 = PB0 of ATtiny85)                               


void setup() {
  pinMode(ledpin, OUTPUT);
  pinMode(anPin, OUTPUT);
  pinMode(ausPin, OUTPUT);
  digitalWrite(ledpin,HIGH);
  digitalWrite(anPin,HIGH);
  digitalWrite(ausPin,HIGH);
  SREG &= 0x7F;
  GIMSK |= (1 << PCIE);
  PCMSK |= (1 << PCINT3);
  PCMSK |= (1 << PCINT4);
  SREG |= 0x80;
}

void loop() {
if(myIntFlags==1){
  if (statusLeuchten) {
    digitalWrite(anPin,LOW);
    digitalWrite(ledpin,LOW);
    delay(100);
    digitalWrite(ledpin,HIGH);
    delay(1000);
    digitalWrite(anPin,HIGH);
    statusLeuchten= false;
    statusSleep=true;
    myIntFlags = 0;
  }
}
if(myIntFlags==2){
  
    if (statusLeuchten) {
      digitalWrite(ausPin,LOW);
      digitalWrite(ledpin,LOW);
      delay(100);
      digitalWrite(ledpin,HIGH);
      delay(1000);
      digitalWrite(ausPin,HIGH);
      statusLeuchten= false;
      statusSleep=true;
      myIntFlags = 0;
    }
  }
  if (statusSleep){
    gotoSleep();
    statusSleep=false;
  }
}

//Attiny in den Schlafmodus setzen

void gotoSleep()
{
  byte adcsra;

  adcsra = ADCSRA; //ADC Control and Status Register A sichern
  ADCSRA &= ~_BV(ADEN); //ADC ausschalten

  MCUCR |= (1 << SM1) & ~(1 << SM0); //Sleep Modus = Power Down
  MCUCR |= _BV(SE); //Sleep Enable setzen
  sleep_cpu(); //Schlafe ....
  MCUCR &= ~(1 << SE); //Sleep Disable setzen
 
  ADCSRA = adcsra; //ADCSRA-Register rückspeichern
}


//Interrupt Serviceroutine (Pin-Change-Interrupt)
ISR(PCINT0_vect)
{
  statusLeuchten = true;
  if (digitalRead(3) == HIGH){ 
    myIntFlags=1;}
  if (digitalRead(4) == HIGH){ 
    myIntFlags=2;}
}

