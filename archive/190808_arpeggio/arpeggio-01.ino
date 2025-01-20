/*
  MIDI note player

  This sketch shows how to use the serial transmit pin (pin 1) to send MIDI note data.
  If this circuit is connected to a MIDI synth, it will play the notes
  F#-0 (0x1E) to F#-5 (0x5A) in sequence.

  The circuit:
  - digital in 1 connected to MIDI jack pin 5
  - MIDI jack pin 2 connected to ground
  - MIDI jack pin 4 connected to +5V through 220 ohm resistor
  - Attach a MIDI cable to the jack, then to a MIDI synth, and play music.

  created 13 Jun 2006
  modified 13 Aug 2012
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Midi
*/

void setup() {
  // Set MIDI baud rate:
  Serial1.begin(31250);
  // Set Analog pins 0-5 as input:
  pinMode (A0, INPUT);
  pinMode (A1, INPUT);
  pinMode (A2, INPUT);
  pinMode (A3, INPUT);
  pinMode (A4, INPUT);
  pinMode (A5, INPUT);
}

void loop() {
  //get values from Analog Read
  //
  int grundTon = 0x3c;
  // Första tonen
  int tonArt = analogRead(A0)/42;
  int multipelA = floor((tonArt+2)/3)*3+0;
  int First = grundTon + (int) (multipelA * 3.5) % 12;
  noteOn(0x90, First, (analogRead(A4)/10)-random(15));
  delay(analogRead(A5));
  noteOn(0x90, First, 0x00);
  delay(10);
  //
  // Andra tonen
  tonArt = analogRead(A0)/42;
  int multipelB = floor((tonArt+1)/3)*3+1;
  int Second = grundTon + (int) (multipelB * 3.5) % 12 ;
  noteOn(0x90, Second, (analogRead(A4)/10)-random(15));
  delay(analogRead(A5));
  noteOn(0x90, Second, 0x00);
  delay(10);
  //
  // Tredje tonen
  tonArt = analogRead(A0)/42;
  int multipelC = floor((tonArt+0)/3)*3+2;
  int Third = grundTon + (int) (multipelC * 3.5) % 12;
  noteOn(0x90, Third, (analogRead(A4)/10)-random(15));
  delay(analogRead(A5));
  noteOn(0x90, Third, 0x00);
  delay(10);
  //
}

// Defines the function 'noteOn' that plays a MIDI note
void noteOn(int cmd, int pitch, int velocity) {
  Serial1.write(cmd);
  Serial1.write(pitch);
  Serial1.write(velocity);
}
