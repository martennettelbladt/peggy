//
//    PPPP      EEEEE     GGGG      GGGG     Y   Y
//    P   P     E        G         G          Y Y
//    PPPP      EEEE     G  GG     G  GG       Y
//    P         E        G   G     G   G       Y
//    P         EEEEE     GGGG      GGGG       Y
//
//    peggy.h  //  Mårten Nettelbladt  //  2022-11-13


#ifndef peggy_h
#define peggy_h

class Arpeggiator
{
    // class code
  public:
    Arpeggiator(); // class constructor
    void begin();
    
    // note selection
    void updateNotes(); // calculates the current notes
    
    bool mirror = false; // if TRUE the scale is mirrored. The 1st and 5th note stay the same. So intervals 1-4 are reversed in order and intervals 5-7. New order: 4, 3, 2, 1, 7, 6, 5
    bool invert = false; // same as MIRROR but only affects the chord intervals. Changes between major and minor version of chord.
    byte key = 0; // 0-11, is the root of the scale. 0 = C, 1 = C#, 2= D et cetera
    byte pitch = 60; // The lowest note in the corrent octave
    byte oldPitch = pitch;
    byte scale = 0;  // 0-37 selects current interval set from the list of 38 sets
    byte mode = 0; // 0-6 sets the mode rotation of the scale
    byte chord = 0; // 0-6. In the "normal" C major scale: 0 = C major, 1 = d minor, 2 = e minor, 3 = F major, 4 = G major, 5 = a minor, 6 = b diminished
    byte note[4][7]; // Arrays holding the current notes
    byte noteType = 3; // 0 = Melody, 1 = Baseline, 2 = Chord, 3 = Sorted chord
    byte oldNote[7] = {0, 0, 0, 0, 0, 0, 0};

    // timing
    bool internalSyncCheck(); // checks time elapsed on internal clock
    bool externalSyncCheck(); // checks external sync
    void constrainBeatValue();
    bool beatSyncCheck();
    void syncOut();
    void updateSyncCount();
    void updatePatternCount(); 
    void resetBeat(); 
    
    bool beatReset = false;
    bool sendSync = false; // default is turned off to prevent more than one object to send sync
    byte syncRate = 24; // MIDI sync = 24 syncs per 1/4 beat
    byte beat = 4; // number of beats per bar
    float tempo = 120; // BPM as a float
    

    // play
    void play(); // 
    void continuous();
    void fade(); // chords fade into each other. Maybe not possible on MIDI. Something for Bela! Or motorized sliders on a mixer!
    void pulse();
    void strum();
    void arpeggio();
    void bytes();
    void melody();
    void eeprom();

    bool mute = false; // sound on/off
    bool oldMute = false;
    
    bool sustain = false; // sustain pedal
    bool automatic = true; // if false, plays only when pitch has changed

    byte playMode = 0;
    byte oldPlayMode = 0;

    // notes on/off
    void trackPlayingNotes();
    void noteOn(byte, byte);
    void noteOff(byte);
    
    byte noteRecord[109];
    
    byte offset = 0;
    byte velocity = 60;
    byte duration = 12; // Length of note in number of sync cycles. 1 sync cycle = 1/96 of bar (if syncRate == 24).
    byte variation = 0;
    byte patternNumber = 0;
    byte notePatternNumber = 0;
    byte bytePatternNumber = 0;
    


    // other
    void programChange();
    
    byte midiChannel = 0, midiInstrument = 0, oldMidiInstrument = 0;

    // custom
    bool bool1, bool2, bool3, bool4;
    byte byte1, byte2, byte3, byte4;
    int int1, int2, int3, int4;
    byte patternByte = 0;
    
  private:
    void scaleIntervals();
    void chordIntervals();
    void melodyNotes();
    void baselineNotes();
    void chordNotes();
    void sortedNotes();
    void sortThreeBytes(byte, byte, byte);
    
    byte scaleInterval[7];
    byte chordInterval[7];
    byte patternNote;
    int skipCount = 0;
    byte _note;
    int patternAddress;
    
    unsigned long tempoMicros;
    unsigned long lastInternalSync = 0;
    unsigned long lastExternalSync = 0;
    int syncCount = 0;
    int beatCount = 0;
    int patternCount = 0;
};

// -------------------------------- I N T E R V A L
const byte interval[38][7] =
{
  {2, 2, 1, 2, 2, 2, 1}, //  0  a1

  {1, 2, 1, 4, 1, 2, 1}, //  1 d8 quite surprising!
  {1, 1, 2, 2, 1, 2, 3}, //  2 b3
  {2, 2, 1, 2, 1, 2, 2}, //  3 a2
  {1, 2, 1, 2, 2, 1, 3}, //  4 b7
  {1, 2, 1, 2, 1, 2, 3}, //  5 b6 Trygg Hansa
  {1, 3, 1, 2, 1, 3, 1}, //  6 c8 Bir iki / Flamenco

  {1, 1, 2, 2, 2, 1, 3}, //  7 b4
  {2, 1, 1, 1, 2, 2, 3}, //  8 b8
  {1, 1, 2, 1, 2, 2, 3}, //  9 b2
  {2, 1, 1, 2, 1, 2, 3}, // 10 b9
  {1, 1, 2, 1, 2, 1, 4}, // 11 d5
  {1, 1, 3, 1, 3, 1, 2}, // 12 c5

  {1, 2, 1, 1, 2, 2, 3}, // 13 b5
  {1, 3, 1, 1, 1, 3, 2}, // 14 c6
  {1, 1, 1, 2, 2, 1, 4}, // 15 d3
  {1, 2, 1, 1, 1, 2, 4}, // 16 d6
  {1, 1, 1, 2, 2, 2, 3}, // 17 b1
  {1, 1, 3, 1, 1, 3, 2}, // 18 c4

  {3, 1, 1, 2, 1, 1, 3}, // 19 c9
  {1, 1, 1, 3, 1, 3, 2}, // 20 c2
  {1, 1, 3, 2, 3, 1, 1}, // 21 c7
  {1, 1, 2, 1, 1, 2, 4}, // 22 d4
  {2, 1, 1, 4, 1, 1, 2}, // 23 d9
  {1, 1, 1, 1, 3, 1, 4}, // 24 e2

  {2, 1, 1, 2, 2, 1, 3}, // 25 b10
  {1, 2, 2, 2, 2, 2, 1}, // 26 a3
  {1, 1, 1, 2, 1, 2, 4}, // 27 d2
  {1, 1, 1, 3, 1, 1, 4}, // 28 e3
  {1, 1, 1, 1, 3, 3, 2}, // 29 c1
  {1, 1, 1, 3, 3, 1, 2}, // 30 c3

  {1, 1, 1, 1, 2, 2, 4}, // 31 d1
  {1, 1, 1, 1, 1, 3, 4}, // 32 e1
  {1, 1, 1, 2, 1, 1, 5}, // 33 f3
  {1, 1, 2, 4, 2, 1, 1}, // 34 d7
  {1, 1, 1, 1, 2, 1, 5}, // 35 f2
  {1, 1, 1, 1, 1, 2, 5}, // 36 f1

  {1, 1, 1, 6, 1, 1, 1}  // 37 g1
};

#endif

// -------------------------------- 3 2   D A S H E S
