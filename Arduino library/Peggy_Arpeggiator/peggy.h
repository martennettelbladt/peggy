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
    byte pitch = 60; // Start working octave from this note
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

// -------------------------------- N O T E   P A T T E R N
const byte pattern[48][16];

// -------------------------------- N O T E   P A T T E R N
const byte notePattern[8][16] =
{
  {1, 3, 5, 3, 5, 3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 2, 3, 2, 4, 2, 3, 2, 1, 2, 3, 2, 5, 2, 3, 2},
  {1, 2, 3, 4, 2, 3, 4, 5, 3, 4, 5, 6, 2, 3, 4, 5},
  {1, 2, 3, 4, 6, 5, 4, 3, 2, 3, 4, 5, 6, 5, 4, 3},
  {1, 2, 3, 4, 3, 2, 3, 2, 1, 2, 3, 4, 5, 4, 3, 2},
  {1, 2, 3, 2, 1, 2, 3, 4, 3, 2, 1, 2, 3, 4, 3, 2},
  {1, 2, 3, 4, 1, 2, 3, 5, 1, 2, 3, 6, 1, 2, 3, 5},
  {1, 1, 3, 3, 1, 2, 3, 5, 1, 2, 3, 6, 1, 2, 3, 5}
};

// -------------------------------- B Y T E   P A T T E R N
const byte bytePattern[5][16] =
{
  {1, 2, 4, 8, 16, 32, 64, 64}, // Decimal
  {128, 64, 32, 16, 8, 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0}, // Decimal
  {0x70, 0x07, 0x70, 0x07, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Hexadecimal
  {0b1111011, 0b1111011, 0b1111011, 0b1111011, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Binary
  {0b1111011, 0b1111011, 0b1111011, 0b1111011, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}  // Binary
  // Ganska coolt att välja ett högre tal än antal mönster. Glitch!
};

static const byte PROGMEM peggySineTable[256] =
{
	0,   0,   0,   0,   1,   1,   2,   2,   3,   3,   4,   5,   6,   7,   8,   9,
	10,  12,  13,  14,  16,  17,  19,  21,  22,  24,  26,  28,  30,  32,  34,  36,
	38,  41,  43,  45,  48,  50,  53,  55,  58,  61,  63,  66,  69,  72,  74,  77,
	80,  83,  86,  89,  92,  95,  98,  101, 104, 107, 110, 113, 117, 120, 123, 126,
	129, 132, 135, 138, 142, 145, 148, 151, 154, 157, 160, 163, 166, 169, 172, 175,
	178, 181, 183, 186, 189, 192, 194, 197, 200, 202, 205, 207, 210, 212, 214, 217,
	219, 221, 223, 225, 227, 229, 231, 233, 234, 236, 238, 239, 241, 242, 243, 245,
	246, 247, 248, 249, 250, 251, 252, 252, 253, 253, 254, 254, 255, 255, 255, 255, 
	255, 255, 255, 255, 254, 254, 253, 253, 252, 252, 251, 250, 249, 248, 247, 246,
	245, 243, 242, 241, 239, 238, 236, 234, 233, 231, 229, 227, 225, 223, 221, 219,
	217, 214, 212, 210, 207, 205, 202, 200, 197, 194, 192, 189, 186, 183, 181, 178,
	175, 172, 169, 166, 163, 160, 157, 154, 151, 148, 145, 142, 138, 135, 132, 129,
	126, 123, 120, 117, 113, 110, 107, 104, 101, 98,  95,  92,  89,  86,  83,  80,
	77,  74,  72,  69,  66,  63,  61,  58,  55,  53,  50,  48,  45,  43,  41,  38,
	36,  34,  32,  30,  28,  26,  24,  22,  21,  19,  17,  16,  14,  13,  12,  10,
	9,   8,   7,   6,   5,   4,   3,   3,   2,   2,   1,   1,   0,   0,   0,   0
};

/*Octave  C     C#    D     D#    E     F     F#    G     G#    A     A#    B
  -1      0     1     2     3     4     5     6     7     8     9     10    11
  0       12    13    14    15    16    17    18    19    20    21    22    23
  1       24    25    26    27    28    29    30    31    32    33    34    35
  2       36    37    38    39    40    41    42    43    44    45    46    47
  3       48    49    50    51    52    53    54    55    56    57    58    59
  4       60    61    62    63    64    65    66    67    68    69    70    71
  5       72    73    74    75    76    77    78    79    80    81    82    83
  6       84    85    86    87    88    89    90    91    92    93    94    95
  7       96    97    98    99    100   101   102   103   104   105   106   107
  8       108   109   110   111   112   113   114   115   116   117   118   119
  9       120   121   122   123   124   125   126   127

  Octave  C     C#    D     D#    E     F     F#    G     G#    A     A#    B
  -1      0x00  0x01  0x02  0x03  0x04  0x05  0x06  0x07  0x08  0x09  0x0a  0x0b
  0       0x0c  0x0d  0x0e  0x0f  0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17
  1       0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f  0x20  0x21  0x22  0x23
  2       0x24  0x25  0x26  0x27  0x28  0x29  0x2a  0x2b  0x2c  0x2d  0x2e  0x2f
  3       0x30  0x31  0x32  0x33  0x34  0x35  0x36  0x37  0x38  0x39  0x3A  0x3B
  4       0x3C  0x3D  0x3E  0x3F  0x40  0x41  0x42  0x43  0x44  0x45  0x46  0x47
  5       0x48  0x49  0x4A  0x4B  0x4C  0x4D  0x4E  0x4F  0x50  0x51  0x52  0x53
  6       0x54  0x55  0x56  0x57  0x58  0x59  0x5A  0x5B  0x5C  0x5D  0x5E  0x5F
  7       0x60  0x61  0x62  0x63  0x64  0x65  0x66  0x67  0x68  0x69  0x6A  0x6B
  8       0x6C  0x6D  0x6E  0x6F  0x70  0x71  0x72  0x73  0x74  0x75  0x76  0x77
  9       0x78  0x79  0x7A  0x7B  0x7C  0x7D  0x7E  0x7F

  PEGGY logo bytes: 
  0x3e, 0x0a, 0x0e, 0x00, 0x3e, 0x2a, 0x0e, 0x00, 0x3e, 0xaa, 0xee, 0x00, 0x3e, 0xaa, 0xee, 0x00, 0x0e, 0x28, 0x3e
  p                       e                       g                       g                       y
  
  .byte $3E ; |  XXXXX |
  .byte $0A ; |    X X |
  .byte $0E ; |    XXX |
  .byte $00 ; |        |
  .byte $3E ; |  XXXXX |
  .byte $2A ; |  X X X |
  .byte $0E ; |    XXX |
  .byte $00 ; |        |
  .byte $3E ; |  XXXXX |
  .byte $AA ; |X X X X |
  .byte $EE ; |XXX XXX |
  .byte $00 ; |        |
  .byte $0E ; |    XXX |
  .byte $28 ; |  X X   |
  .byte $3E ; |  XXXXX |
  .byte $00 ; |        |
  https://www.masswerk.at/vcs-tools/TinySpriteEditor/

*/

#endif

// -------------------------------- 3 2   D A S H E S
