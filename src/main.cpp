//nichts von arduino abstraktion verwenden :) 
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>


ISR(INT0_vect){
//hier ein reset
}

ISR(INT1_vect){
  //hier pausieren
}




int main(){
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


  return 0; 
}