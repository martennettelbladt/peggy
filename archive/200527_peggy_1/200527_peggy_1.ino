/*   TX   Usb  RX opto
   _|__|__||___|_|_
  | ()  ()  ()  () |= 13  LED
  | A8  A9  A10 A11|
  |                |= 2             P  E  G  G  Y 
  |                |= 3
  |   __      __   |= 5             Arduino-based MIDI Arpeggiator
  | / A4 \  / A5 \ |                3 May 2020
  | \    /  \    / |= 7             Mårten Nettelbladt
  |   ¯¯  ()  ¯¯   |= 11
  |       A6       |
   ¯||¯¯||¯¯||¯¯||¯        A7 not used
    A0  A1  A2  A3    
    
    2$€µ~€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€3
    
    */
//bork  

// -------------------------------- V A R I A B L E S

bool sustain = false;
bool continuous = false;

byte i;
byte j;
byte swap;
byte mode;
byte sharp;
byte referenceNote = 0x1a; // (26) = D3
byte keyIndex;
byte minorInvert;
byte minorMajor;
byte ledValue;

byte voice;             //note 1-3
byte voiceRemainder;
byte voiceInvert;
int voiceShift;
byte voice2;            //note 4-6
byte voice2remainder;
byte voice2invert;
byte potVelocity;
byte skipVelocity;
byte beatFactor;
byte noteSelect;
byte noteCount;

int beatRead;

int clockCount = 0;
byte chordType;

byte notePatternNumber;
byte skipPatternNumber;
byte noteStepsCount;
byte skipStepsCount;
byte noteStep = 1;
byte skipStep = 1;
byte noteLength;
byte patternNote;

int noteLengthPot;
byte beatValue;
byte beatLength;
int tempo;

byte noteLengthRecord[109];
byte basicNote[8];
byte selectedNote[7];
byte transposedNote[7];
byte sortedNote[7];
byte currentNote[7];
byte playNote[109];

byte midiStart = 0xfa;
byte midiStop = 0xfc;
byte midiClock = 0xf8;
byte midiData;
//KORV ÄR GOTT nnr
byte note1;
byte note2;
byte note3;
byte note4;
byte note5;
byte note6;

unsigned long lastMidiClock;
unsigned long lastTempo;

// -------------------------------- P A T T E R N S

byte selectPatternCount = 5;
byte notePatternCount = 6;
byte skipPatternCount = 5;

byte selectPattern[5][7] = {
  {0, 1, 3, 5, 1, 3, 5},
  {0, 1, 3, 5, 7, 2, 4},
  {0, 1, 3, 5, 6, 7, 2},
  {0, 3, 4, 5, 1, 2, 7},
  {0, 1, 2, 3, 4, 5, 6}
};
byte notePattern[7][17] = {
  { 6, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {16, 1, 2, 3, 2, 4, 2, 3, 2, 1, 2, 3, 2, 5, 2, 3, 2},
  {16, 1, 2, 3, 4, 2, 3, 4, 5, 3, 4, 5, 6, 2, 3, 4, 5},
  {16, 1, 2, 3, 4, 6, 5, 4, 3, 2, 3, 4, 5, 6, 5, 4, 3},
  {16, 1, 2, 3, 4, 3, 2, 3, 2, 1, 2, 3, 4, 5, 4, 3, 2},
  {16, 1, 2, 3, 2, 1, 2, 3, 4, 3, 2, 1, 2, 3, 4, 3, 2},
  {16, 1, 2, 3, 4, 1, 2, 3, 5, 1, 2, 3, 6, 1, 2, 3, 5}
};
byte skipPattern[5][16] = {
  {1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0},
  {1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0},
  {1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0},
  {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

// -------------------------------- S E T U P

//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(14, 1); // RX, TX

void setup() {

  // allNotesOffFunc();

  Serial1.begin(31250); // Sets MIDI baud rate
//  mySerial.begin(31250);

  pinMode (A0, INPUT);    // note select pattern
  pinMode (A1, INPUT);    // note pattern
  pinMode (A2, INPUT);    // skip pattern length
  pinMode (A3, INPUT);    // skip pattern

  pinMode (A4, INPUT);    // key
  pinMode (A5, INPUT);    // voice
  pinMode (A6, INPUT);    // voice 2 offset ?? Chord?
  pinMode (A7, INPUT);    // DISCONNECTED

  pinMode (A8, INPUT);    // velocity
  pinMode (A9, INPUT);    // duration
  pinMode (A10, INPUT);   // tempo
  pinMode (A11, INPUT);   // beatLength

  pinMode (2, INPUT);     // minorInvert
  pinMode (3, INPUT);     // sharp #
  pinMode (5, INPUT);     // beatReset
  pinMode (7, INPUT);     // chords on/off
  pinMode (11, INPUT);    // on/off
  pinMode (13, OUTPUT);    // LED
}


// -------------------------------- L O O P

void loop() {
  checkMidiFunc();
  checkTimeFunc();
}
// -------------------------------- C H E C K   M I D I

void checkMidiFunc() {

  if (Serial1.available() > 0) {
    midiData = Serial1.read();
    if (midiData == midiClock) {
      clockLoopFunc();
      lastMidiClock = millis();
    }
    if (midiData == midiStart) {
      beatResetFunc();
    }
  }
}
// -------------------------------- C H E C K   T I M E

void checkTimeFunc() {

  if (millis() - lastMidiClock > 100) {
    tempo = map(analogRead(A10), 0, 1023, 2, 42);
    //10 = 240 bpm, 42 = 60 bpm

    if (digitalRead(7) == HIGH) {
      tempo = 21;
    }

    if (millis() - lastTempo >= tempo) {
      clockLoopFunc();
      lastTempo = millis();
    }
  }
}

// -------------------------------- C L O C K   L O O P

void clockLoopFunc() {

  sendMidiClockFunc();
  trackPlayingNotesFunc();
  beatLengthFunc();
  if (clockCount % beatLength == 0 || clockCount == 0) {
    modeSelectFunc();
  }
  if (clockCount == 0) {
    ledFirstFunc();
  }
  if (clockCount % 24 == 0 && clockCount != 0) {
    ledOnFunc();
  }
  if (clockCount % 24 == 12) {
    ledOffFunc();
  }

  clockCountFunc();
}
// -------------------------------- S E N D   M I D I   C L O C K

void sendMidiClockFunc() {

  Serial1.write(midiClock);
  // sends MIDI clock signal out
  //(24 times per quarter note)
}

// -------------------------------- T R A C K   P L A Y I N G   N O T E S
void trackPlayingNotesFunc() {

  for (i = 21; i < 109; i++) {
    if (noteLengthRecord[i] > 1) {
      noteLengthRecord[i] --;
    }
    if (noteLengthRecord[i] == 1 && sustain == false) {
      noteOffFunc(i);
      noteLengthRecord[i] = 0;
    }
    if (noteLengthRecord[i] == -1 /*&& continuous == false*/) {
      noteOffFunc(i);
      noteLengthRecord[i] = 0;
    }
  }
}

// -------------------------------- C L O C K   C O U N T

void clockCountFunc() {

  clockCount ++;
  if (clockCount > 95) {
    clockCount = 0;
  }

  if (digitalRead(5) == HIGH) {
    beatResetFunc();
    ledOffFunc();
  }

}

// -------------------------------- L E D   F I R S T

void ledFirstFunc(){
  analogWrite(13, 80);
}

// -------------------------------- L E D   O N

void ledOnFunc(){
  analogWrite(13, 15);
}

// -------------------------------- L E D   O F F

void ledOffFunc(){
  analogWrite(13, 0);
}

// -------------------------------- B E A T   R E S E T

void beatResetFunc() {

  clockCount = -1;
  noteStep = 1;
  skipStep = 1;
}

// -------------------------------- B E A T   L E N G T H

void beatLengthFunc() {

  beatValue = map(analogRead(A11), 0, 1023, 0, 6);

  if (digitalRead(7) == LOW) {
    beatValue = beatValue + 1;
  }

  switch (beatValue) {
    case  0:
      beatLength = 3;
      break;
    case  1:
      beatLength = 6;
      break;
    case  2:
      beatLength = 12;
      break;
    case  3:
      beatLength = 24;
      break;
    case  4:
      beatLength = 48;
      break;
    case  5:
      beatLength = 96;
      break;

  }
}

// -------------------------------- M O D E   S E L E C T

void modeSelectFunc() {

//  if (digitalRead(7) == HIGH) {
//    arpeggioFunc();
//  }
//  else {
    chordsFunc();
//  }
}

// -------------------------------- A R P E G G I O

void arpeggioFunc() {

  currentNotesFunc();
  noteLengthFunc();
  velocityFunc();
  notePatternFunc();
  skipPatternFunc();

  noteOnFunc(patternNote, skipVelocity);

}

// -------------------------------- C H O R D S

void chordsFunc() {
  currentNotesFunc();
  noteCountFunc();
  noteLengthFunc();
  velocityFunc();
  skipPatternFunc();


  for (i = 1; i < noteCount + 1; i++) {
    noteOnFunc(currentNote[i], skipVelocity);
  }
}

// -------------------------------- C O N T I N U O U S

void continuousFunc() {
  currentNotesFunc();
  noteCountFunc();
  velocityFunc();

  for (i = noteCount; i > 0; i--) {
    if (noteLengthRecord[currentNote[i]] == 0) {
      noteOnFunc(currentNote[i], potVelocity);
    }
    noteLengthRecord[currentNote[i]] = -2;
  }

  for (i = 21; i < 109; i++) {
    if (noteLengthRecord[i] == -1) {
      //      noteOffFunc(i);
    }
  }

  for (i = noteCount; i > 0; i--) {
    noteLengthRecord[currentNote[i]] = -1;
  }
}

// -------------------------------- C U R R E N T   N O T E S

void currentNotesFunc() {
  
  if (noteStep == 1 || noteStep == 2) {
    basicNotesFunc();
  }
  selectNotesFunc();
  transposeNotesFunc();
  sortNotesFunc();

  for (i = 1; i < 7; i++) {
    currentNote[i] = sortedNote[i];
  }
}

// -------------------------------- N O T E   C O U N T

void noteCountFunc() {

  noteCount = (1023 - analogRead(A6)) / 171 + 1;
}

// -------------------------------- B A S I C   N O T E S

void basicNotesFunc() {
  keyIndex = (floor ((1023 - analogRead(A4)) / 93));
  // gives 45 deg between major keys
  minorInvertFunc();
  sharpFunc();

  basicNote[1] = floor((keyIndex) * 3.5) + sharp;
  basicNote[2] = basicNote[1] + 2;
  basicNote[3] = basicNote[1] + 3 + minorMajor;
  basicNote[4] = basicNote[1] + 5;
  basicNote[5] = basicNote[1] + 7;
  basicNote[6] = basicNote[1] + 8 + minorMajor;
  basicNote[7] = basicNote[1] + 10 + minorMajor;
}

// -------------------------------- M I N O R   I N V E R T

void minorInvertFunc() {

  if (digitalRead(2) == HIGH) {
    minorInvert = 1;
  }
  else {
    minorInvert = 0;
  }
  minorMajor = (keyIndex + minorInvert)  % 2;
}

// -------------------------------- S H A R P #

void sharpFunc() {

  if (digitalRead(3) == HIGH) {
    sharp = 1;
  }
  else {
    sharp = 0;
  }
}

// -------------------------------- S E L E C T   N O T E S

void selectNotesFunc() {

  noteSelect = (1023 - analogRead(A0)) / 171;

  if (noteSelect > notePatternCount - 1) {
    noteSelect = notePatternCount - 1;
  }

  for (i = 1; i < 7; i++) {

    selectedNote[i] = basicNote[selectPattern[noteSelect][i]];
  }
}

// -------------------------------- T R A N S P O S E   N O T E S

void transposeNotesFunc() {

  voice = (1023 - analogRead(A5)) / 15;         // notes 1-3
  voiceRemainder = voice % 12;
  voiceInvert = 12 - voiceRemainder;

  voiceShift = (1023 - analogRead(A6)) / 64;

  voice2 = voice + 12; //voiceShift;                 // notes 4-6
  voice2remainder = voice2 % 12;
  voice2invert = 12 - voice2remainder;

  for (i = 1; i < 4; i++) { //don't remember exacly how this works...
    transposedNote[i] = referenceNote + ((selectedNote[i] + voiceInvert) % 12) + voice;
  }
  for (i = 4; i < 7; i++) {
    transposedNote[i] = referenceNote + ((selectedNote[i] + voiceInvert) % 12) + voice + 12;
  }
}

// -------------------------------- S O R T   N O T E S

void sortNotesFunc() {

  for (i = 1; i < 7; i++) {
    sortedNote[i] = transposedNote[i];
  }

  // N O T E S   1 - 3

  for (int j = 0; j < 2; j++) {
    for (int i = 1; i < 3; i++) {
      if (sortedNote[i] > sortedNote[i + 1]) {
        swap = sortedNote[i];
        sortedNote[i] = sortedNote[i + 1];
        sortedNote[i + 1] = swap;
      }
    }
  }

  // N O T E S   4 - 6

  for (int j = 0; j < 2; j++) {
    for (int i = 4; i < 6; i++) {
      if (sortedNote[i] > sortedNote[i + 1]) {
        swap = sortedNote[i];
        sortedNote[i] = sortedNote[i + 1];
        sortedNote[i + 1] = swap;
      }
    }
  }
}

// -------------------------------- N O T E   L E N G T H

void noteLengthFunc() {

  noteLength = (1023 - analogRead(A9)) / 10 + 1;
  if (analogRead(A9) == 0) {
    sustain = true;
  }
  else {
    sustain = false;
  }

}

// -------------------------------- V E L O C I T Y

void velocityFunc() {

  potVelocity = (1023 - analogRead(A8)) / 10 + random(10);
}

// -------------------------------- N O T E   P A T T E R N

void notePatternFunc() {

  notePatternNumber = (1023 - analogRead(A1)) / 171; // 0 - 5
  noteStepsCount = notePattern[notePatternNumber][0] + 1;

  if (notePatternNumber > notePatternCount - 1) {
    notePatternNumber = notePatternCount - 1;
  }

  patternNote = currentNote[notePattern[notePatternNumber][noteStep]];
  noteStep++;

  if (noteStep >= noteStepsCount) {
    noteStep = 1;
  }
}

// -------------------------------- S K I P   P A T T E R N

void skipPatternFunc() {

  skipStepsCount = (1023 - analogRead(A3)) / 60.5; // 0 - 15
  skipPatternNumber = (1023 - analogRead(A2)) / 171; // 0 - 5

  if (skipPatternNumber > skipPatternCount - 1) {
    skipPatternNumber = skipPatternCount - 1;
  }

  if (skipStep > skipStepsCount) {
    skipStep = 0;
  }

  skipVelocity = potVelocity * skipPattern[skipPatternNumber][skipStep];
  skipStep++;
}

// -------------------------------- N O T E   O N

void noteOnFunc(byte note, byte velocity) {

  if (note < 21) {            // Midi note 21 = A0 = lowest key on 88 key piano
    note = 21;
  }
  if (note > 108) {           // Midi note 108 = C8 = highest key on 88 key piano
    note = 108;
  }

  if (digitalRead(11) == HIGH) {
    Serial1.write(0x90);
    Serial1.write(note);
    Serial1.write(velocity);

    noteLengthRecord[note] = noteLength; // stores note length in record
  }
}

// -------------------------------- N O T E   O F F

void noteOffFunc(byte note) {

  Serial1.write(0x90);
  Serial1.write(note);
  Serial1.write(0x00);
}

// -------------------------------- A L L   N O T E S   O F F

void allNotesOffFunc() {

  for (i = 21; i < 109; i++) {
    noteOffFunc(i);
    noteLengthRecord[i] = 0;
  }
}

// -------------------------------- T H E   E N D

/*
  Octave  C     C#    D     D#    E     F     F#    G     G#    A     A#    B
  1       0x00  0x01  0x02  0x03  0x04  0x05  0x06  0x07  0x08  0x09  0x0a  0x0b
  2       0x0c  0x0d  0x0e  0x0f  0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17
  3       0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f  0x20  0x21  0x22  0x23
  4       0x24  0x25  0x26  0x27  0x28  0x29  0x2a  0x2b  0x2c  0x2d  0x2e  0x2f
  5       0x30  0x31  0x32  0x33  0x34  0x35  0x36  0x37  0x38  0x39  0x3A  0x3B
  6       0x3C  0x3D  0x3E  0x3F  0x40  0x41  0x42  0x43  0x44  0x45  0x46  0x47
  7       0x48  0x49  0x4A  0x4B  0x4C  0x4D  0x4E  0x4F  0x50  0x51  0x52  0x53
  8       0x54  0x55  0x56  0x57  0x58  0x59  0x5A  0x5B  0x5C  0x5D  0x5E  0x5F
  9       0x60  0x61  0x62  0x63  0x64  0x65  0x66  0x67  0x68  0x69  0x6A  0x6B
  10      0x6C  0x6D  0x6E  0x6F  0x70  0x71  0x72  0x73  0x74  0x75  0x76  0x77
  11      0x78  0x79  0x7A  0x7B  0x7C  0x7D  0x7E  0x7F */
