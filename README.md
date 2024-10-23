- [Motivation für dieses Projekt](#motivation-für-dieses-projekt)
- [Skizze zum Vorhaben](#skizze-zum-vorhaben)
- [Technische Umsetzung](#technische-umsetzung)
  - [Zeit](#zeit)
    - [Berechnungen für die Interrupts](#berechnungen-für-die-interrupts)
    - [7 Segment Anzeige](#7-segment-anzeige)
    - [Umsetzung der Zeitberechnung im Code](#umsetzung-der-zeitberechnung-im-code)
  - [Knopfdruck (Hardware Interrupt)](#knopfdruck-hardware-interrupt)
  - [Rundenzahl und 4 Zyklen Runden](#rundenzahl-und-4-zyklen-runden)
  - [Buzzer](#buzzer)
- [Probleme (während der Entwicklung)](#probleme-während-der-entwicklung)
  - [Hardware Interrupt löst zweimal oder öfter aus](#hardware-interrupt-löst-zweimal-oder-öfter-aus)
    - [Lösung](#lösung)
      - [alternative](#alternative)
  - [ATMega328P hat zu wenig Pins für meinen Anwendungsfall](#atmega328p-hat-zu-wenig-pins-für-meinen-anwendungsfall)



# Motivation für dieses Projekt 
Bisher habe ich ausschließlich die Arduino IDE genutzt, um Mikrocontroller zu programmieren. Im Rahmen meines Studiums, im Modul Mikrocontroller, habe ich jedoch die direkte Programmierung von Mikrocontrollern in C kennengelernt. Dabei ist mir klar geworden, wie stark die Arduino IDE vieles abstrahiert und vereinfacht.

Um dieser Abstraktion zu entgehen und das "richtige" Programmieren von Mikrocontrollern zu lernen, habe ich mich entschieden, dieses Projekt durchzuführen. Außerdem möchte ich Timer- und Hardware-Interrupts verwenden, und ich denke, dass sich ein Pomodoro-Timer dafür hervorragend eignet.

# Skizze zum Vorhaben
- keine feste Fokuszeit (`fokusZeit`) sondern läuft quasi unendlich lange
- basierend auf `fokusZeit` die Dauer der Pause (`pausenZeit`) berechnen
    - mit Knopfdruck die FokusZeit abbrechen und in die Pause gehen
        - die ersten drei male wird 20% der `fokusZeit` als `pausenZeit` genommen
            -  $\small pausenZeit = fokusZeit \cdot 0.2$
        - beim vierten mal 40% 
            - $\small  pausenZeit = fokusZeit \cdot 0.4$
        - wenn's nach dem vierten mal weitergeht, dann wieder erst drei mal 20%, dann 40% etc. 
- 7 Segment Display für : 
    - Zeit 
    - Rundenzahl {0, 1, 2, 3}
    - die 4 Zyklen Rundenzahl
- Buzzer Sound o.ä der ein Audiosignal gibt, wenn die Pause zu Ende ist
- Knopf der alles zurücksetzt


# Technische Umsetzung

Ich nutze ein Uno R3 mit einem ATMega328P Mikrocontroller. 
Die Register beziehen sich also auf den ATMega328P $\mu$C 

- Register die man allg. braucht: 
    - `SREG` - Interrupts allgemein aktivieren


## Zeit
- Benötigte Register (was ich zmd aktuell denke :) ):
    - `TCNT1` - aktueller Wert des Zählers (16 Bit)
    - `OCR1A/B` - Wert festlegen, wann der Interrupt ausgelöst werden soll
    - `TCCR1A/B/C` - timer an sich konfigurieren
    - `TIMSK0/1` - das `OCR1A/B` aktivieren für den Vergleich
    - `TIFR1` - Interrupt Flags abfragen

<br/>

- Fokuszeit zb mit `TCCR1A` und damit einhergehende Register
    - wo drin speichern, wie lange man fokussiert war?
        - zB eine Variable die aber nur 1 mal pro (halbe) Minute aktualisiert wird
- Pausenzeit zb mit `TCCR1B` ""
    - hier auch eine Variable die vergleicht, wann die Pause zu Ende ist
<br/>

- so umsetzten das jede Sekunde der Timer Interrupt ausgelöst wird? 
    - und in der ISR dann die 7 Segment Anzeige aktualieren

- ==$\rightarrow$ oder ich nutze nur einen Timer statt zwei und mache eine Fallunterscheidung==


### Berechnungen für die Interrupts
- ATMega328P hat 20 MHz 
$$
\begin{aligned}
    f &= \frac{1}{T} \\[0.3cm]
    20 \cdot 10^6 \frac{1}{s} &= \frac{1}{T} \\[0.3cm]
    T &= \frac{1}{20 \cdot 10^6 \frac{1}{s}} \\[0.3cm]
    T &= 50 \cdot 10^{-9}s = 50\ ns
\end{aligned}
$$ 

---

- Wunschzeit für Interrupt Auslösung: 1 sekunde
    - Berechnung: $t_{wunschzeit} = \frac{1}{f_{atmega328P}} \cdot prescaler \cdot comparewert$
        - prescaler - lässt sich einstellen in dem Register `TCCR1B`
            - Wirkung: Frequenz teilen um bestimmten Wert; 1, 8, 64, 256 oder 1024
        - comparewert - der Wert im `OCR1A/B` Register
        - --> beide frei wählbar um bestmöglich auf 1 sekunde zu kommen :)
    - Meine Konfig: $1s = \frac{1}{20 \cdot 10^6 \frac{1}{s}} \cdot 1024 \cdot 19531$
        - damit komme ich auf $0,9999872\ s$

### 7 Segment Anzeige
- ich habe die 5161AS 7 Segment Anzeigen
  - die basieren auf gleicher Kathode. Sind also High Aktiv

| Ziffer | A | B | C | D | E | F | G |
|--------|---|---|---|---|---|---|---|
| 0      | 1 | 1 | 1 | 1 | 1 | 1 | 0 |
| 1      | 0 | 1 | 1 | 0 | 0 | 0 | 0 |
| 2      | 1 | 1 | 0 | 1 | 1 | 0 | 1 |
| 3      | 1 | 1 | 1 | 1 | 0 | 0 | 1 |
| 4      | 0 | 1 | 1 | 0 | 0 | 1 | 1 |
| 5      | 1 | 0 | 1 | 1 | 0 | 1 | 1 |
| 6      | 1 | 0 | 1 | 1 | 1 | 1 | 1 |
| 7      | 1 | 1 | 1 | 0 | 0 | 0 | 0 |
| 8      | 1 | 1 | 1 | 1 | 1 | 1 | 1 |
| 9      | 1 | 1 | 1 | 1 | 0 | 1 | 1 |


### Umsetzung der Zeitberechnung im Code
- grundlegend würde ich nur Minuten und Sekunden anzeigen. also zB 10:25 (10 Minuten, 25 Sekunden), 90:12 (90 Minuten, 12 Sekunden)

- zwei Ansätze die mir einfallen
1. für jede Stelle eine eigene Variable `einserSekunde`, `zehnerSekunde`, `einserMinute`, `zehnerMinute`
  - in der ISR die dann entsprechend hochzählen ... etc
  - --> aber das ist ziemlich ineffektiv und unlesbar
2. bessere Alternative: die Sekunde nur hochzählen (eine Variable)
   - mit mathematischen Rechnungen aus der Sekunde die zu anzeigenden Zahlen herausarbeiten

- in Beiden fällen natürlich in der ISR nur das hochzählen, dass eigentlichen ansteuern der 7 Segment Displays in der hauptschleife

## Knopfdruck (Hardware Interrupt)
- Pins PD3 (INT1) und PD2 (INT0) eignen sich dafür
- Register: 
    - `EICRA` - logikzustand für INT1 und INT0 einstellen (wann interrupt ausgelöst werden soll)
    - `EIMSK` - pins allg. aktivieren für Interrupt

- Pausieren: mit INT0
- Reset: mit INT1

## Rundenzahl und 4 Zyklen Runden
- in einer einfachen Variable speichern 
    - die Variable Dekodiert auf 7 Segment anzeige ausgeben :)

## Buzzer
- mit Ende der Pausenzeit den Buzzer Sound aktivieren
    - in der ISR mit einbauen, dass das am Ende passieren muss


# Probleme (während der Entwicklung)

## Hardware Interrupt löst zweimal oder öfter aus

- zB der Hardware Interrupt mit INT0
- eingestellt auf Low Flanke die ISR auszulösen
- hat aber bei einem mal Knopf betätigen 2 oder sogar $\gt$ 2 ausgelöst
Code womit ich das festgestellt habe (`count` ist eine globale Variable)
```c
ISR(INT0_vect){
  char buffer[50];
  sprintf(buffer, "Button gedrueckt %i", count);
  count++;
  Serial.println(buffer);
}
```
### Lösung
- zustand in einer Variable zwischenspeichern und in der while loop prüfen 
- ja ich weiss, ich hätte auch direkt die ISR für den HW Interrupt weglassen können wenn ich das so löse aber mir ging es darum, Interrupts anzuwenden, deswegen jz die Lösung erstmal so :)
#### alternative
- `EIFR |= 0x01;` 
    - External Interrupt Flag Register zurücksetzen am Anfang der ISR
    - weiss aber nicht ob sowas gemacht werden soll/darf, deswegen verschmissen
- man kanns wohl auch mit einem RC filter lösen - zu viel Aufwand für mich jz aber :)


## ATMega328P hat zu wenig Pins für meinen Anwendungsfall

- PortD gibt es 8 Pins, da benutze ich aber 2 von für Hardware Interrupts
- PortC hat zwar 7, aber einer davon für Reset reserviert
- Port B nur 6 
- --> kann also leider nicht nur einen Port nutzen für die 7 Segment anzeigen, muss das iwie anders lösen
  - ich werde wsh Port C Pin 0 - 5 nehmen und Port D Pin 6 - 7