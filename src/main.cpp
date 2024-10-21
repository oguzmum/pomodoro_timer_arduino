//nichts von arduino abstraktion verwenden :) 
#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned count = 0; 
volatile boolean pd2pressed = false; 

ISR(INT0_vect){
  //button wurde eben nicht gedrückt; jetzt erst neu
  if(!pd2pressed)
  { 
    pd2pressed = true; 
    char buffer[50];
    sprintf(buffer, "Button gedrueckt %i", count);
    count++;
    Serial.println(buffer);
  }
  
}

ISR(INT1_vect){
  //hier pausieren
}




int main(){
  Serial.begin(115200);

  //erstmal nur die Hardware Interrupts zum testen
  
  //PD2 und PD3 als Eingang konfigurieren
  //0 Eingang, 1 Ausgang
  DDRD |= 0b00000000; //oder: 0x00 oder 0 :)

  //bit 7-4 keine fkt
  //bit 3 & 2 für INT1 (PD3) -> will hiermit Pausieren
  //bit 1 & 0 für INT0 (PD2) -> damit will ich Reset
  EICRA = 0b00001010; //beides 10 - also fallende Flanke
  
  //interrupts allgemein enablen für int0 und int1 /bit 1 udn 0 auf 1
  EIMSK = 0x03; 


  //bit 7 vom status register um interrupts global zu aktivieren
  SREG |= 0x80; 

  Serial.println("Hello World\n");

  
  while(1){

    //Lösung um das > 2 mal ausführen der HW ISR bei Buttonklick zu umgehen
    //ja ich weiss, ich hätte auch direkt die ISR für den HW Interrupt weglassen können wenn ich das so löse
    //aber mir ging es darum, Interrupts anzuwenden, deswegen jz die Lösung erstmal so :)
    if (PIND & (1 << PD2)) { // button pd2 ist im moment nicht gedrückt
      pd2pressed = false; 
    }

  }

  return 0; 
}