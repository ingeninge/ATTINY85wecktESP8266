// This code is based on the example from https://arduino-projekte.webnode.at/meine-projekte/attiny-im-schlafmodus/wecken-mit-pci/


#include <Arduino.h>
#include <avr/sleep.h>

#define ledpin 0  //this is the pin that goes to the Reset Pin of the ESP8266
#define anPin 1   //this ist the Pin that latches the input Pin Change Interrupt source
#define ausPin 2  //this ist the second Pin that latches the input Pin Change Interrupt source
//prototype
void gotoSleep(void);

volatile bool statusLeuchten = false; //since this is copy paste from example code please ignore the name. It is only to determine if the command is completed
bool statusSleep = true;      // if true the ATTiny85 is allowed to fall asleep again ;)
volatile byte myIntFlags = 0; // flag to latch the interrupt source pin

//Blinking LED on ATTINY85 (0 = PB0 of ATtiny85)                               


void setup() {
  pinMode(ledpin, OUTPUT);
  pinMode(anPin, OUTPUT);
  pinMode(ausPin, OUTPUT);
  digitalWrite(ledpin,HIGH);
  digitalWrite(anPin,HIGH);
  digitalWrite(ausPin,HIGH);
  //prepare the Pin Change Registers
  SREG &= 0x7F;
  GIMSK |= (1 << PCIE);
  PCMSK |= (1 << PCINT3);
  PCMSK |= (1 << PCINT4);
  SREG |= 0x80;
}

void loop() {
if(myIntFlags==1){
  if (statusLeuchten) {
    digitalWrite(anPin,LOW);  // an Pin is the Pin that signals my ESP8266 to send a MQTT Message to turn on the Watertap in the garden
    digitalWrite(ledpin,LOW); // as mentioned the ledpin is connected to the Reset line of the ESP8266 witch will then ware up from DeepSleep when ledpin goes high again
    delay(100);
    digitalWrite(ledpin,HIGH);
    delay(1000);              //wait for the ESP8266 to get ready to read the input pins ... 
    digitalWrite(anPin,HIGH); // set the output to high again
    statusLeuchten= false;
    statusSleep=true;
    myIntFlags = 0;           // reset the latch and go to sleep
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

//Attiny in den Schlafmodus setzen / here the ATTiny85 goes to sleep

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
  if (digitalRead(3) == HIGH){  // read the pin status and determine witch pin was the interrupt source
    myIntFlags=1;}              // store the source
  if (digitalRead(4) == HIGH){ 
    myIntFlags=2;}
}

