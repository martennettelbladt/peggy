

#include <peggy.h>
#include <pattern.h>
#include <interface.h>

Arpeggiator peggy;

// -------------------------------- S E T U P
void setup() {
  
  pinMode(0, INPUT);
  pinMode(1, INPUT);

  peggy.begin();
  peggy.playMode = 0;
  peggy.pitch = 59;
  peggy.midiChannel = 0;
}

// -------------------------------- L O O P
void loop() {

  peggy.chord = (floor ((1023 - analogRead(A0)) / 93));
  peggy.pitch = (1023 - analogRead(A1)) / 15; 
  
  peggy.play();
  
}
