# Motivation für dieses Projekt 
Bisher habe ich ausschließlich die Arduino IDE genutzt, um Mikrocontroller zu programmieren. Im Rahmen meines Studiums, im Modul Mikrocontroller, habe ich jedoch die direkte Programmierung von Mikrocontrollern in C kennengelernt. Dabei ist mir klar geworden, wie stark die Arduino IDE vieles abstrahiert und vereinfacht.

Um dieser Abstraktion zu entgehen und das "richtige" Programmieren von Mikrocontrollern zu lernen, habe ich mich entschieden, dieses Projekt durchzuführen. Außerdem möchte ich Timer- und Hardware-Interrupts verwenden, und ich denke, dass sich ein Pomodoro-Timer dafür hervorragend eignet.

# Skizze zum Vorhaben
- keine feste Lern-/Fokuszeit (`lernZeit`) sondern läuft quasi unendlich lange
- basierend auf `lernZeit` die Dauer der Pause (`pausenZeit`) berechnen
    - mit Knopfdruck die Lernzeit abbrechen und in die Pause gehen
        - die ersten drei male wird 20% der `lernZeit` als `pausenZeit` genommen
            -  $\small pausenZeit = lernZeit \cdot 0.2$
        - beim vierten mal 40% 
            - $\small  pausenZeit = lernZeit \cdot 0.4$
        - wenn's nach dem vierten mal weitergeht, dann wieder erst drei mal 20%, dann 40% etc. 
- 7 Segment Display für : 
    - Zeit 
    - Rundenzahl {0, 1, 2, 3}
    - die 4 Zyklen Rundenzahl
- Buzzer Sound o.ä der ein Audiosignal gibt, wenn die Pause zu Ende ist
- Knopf der alles zurücksetzt