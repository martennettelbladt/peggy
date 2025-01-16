//
//    PPPP      EEEEE     GGGG      GGGG     Y   Y
//    P   P     E        G         G          Y Y
//    PPPP      EEEE     G  GG     G  GG       Y
//    P         E        G   G     G   G       Y
//    P         EEEEE     GGGG      GGGG       Y
//
//    peggy.cpp  //  Mårten Nettelbladt  //  2022-11-13 (rev 2025-01-01)


#include <Arduino.h>
#include "peggy.h"
#include "interface.h"

#define noteMin 21 // Lowest note. 21 = A0 =  Lowest note on 88-key piano
#define noteMax 108 // Highest note. 108 = C8 = Highest note on 88-key piano

int i, j, k; // Used in FOR loops
byte x, y, z, temp; // For sorting notes

// -------------------------------- C O N S T R U C T O R
// This code will be executed every time an object is created in the sketch
Arpeggiator::Arpeggiator()
{

}

// -------------------------------- B E G I N
// Code to initiate the object 
void Arpeggiator::begin()
{
  interfaceBegin();
}

// -------------------------------- P L A Y
// Main function called from Arduino sketch.
// In fact it's the only function that needs to be called in a minimal setup
void Arpeggiator::play()
{
  if (internalSyncCheck() == true)
  {
    if (playMode != oldPlayMode)
    {
      if (oldPlayMode == 0)
      {
        for (i = 0; i < 3; i++)
        {
          noteOff(oldNote[i]);
        }
      }
      if (playMode == 0)
      {
        for (i = 0; i < 3; i++)
        {
          if (noteRecord[oldNote[i]] == 0)
          {
            oldNote[i] = 0;
          }
        }
      }
      oldPlayMode = playMode;
    }
    switch (playMode)
    {
      case 0:
        continuous();
        break;
      case 1:
        pulse();
        break;
      case 2:
        arpeggio();
        break;
      case 3:
        bytes();
        break;
      case 4:
        melody();
        break;
    }
  }
}
// -------------------------------- I N T E R N A L   S Y N C   C H E C K
bool Arpeggiator::internalSyncCheck()
{
  tempoMicros = (1000000 * (60 / tempo)) / 24; // hanteras "tempo" som en float här?
  if (micros() - lastExternalSync > 100000 && micros() - lastInternalSync >= tempoMicros)
  {
    updateSyncCount();
    lastInternalSync = lastInternalSync + tempoMicros;
    return true;
  }
  else
  {
    return false;
  }
}


// -------------------------------- E X T E R N A L   S Y N C   C H E C K
bool Arpeggiator::externalSyncCheck()
{
  if (interfaceSyncCheck == true)
  {
    updateSyncCount();
    lastExternalSync = micros();
    return true;
  }
  else
  {
    return false;
  }

}

// -------------------------------- S Y N C   O U T
void Arpeggiator::syncOut()
{
  if (sendSync == true)
  {
    interfaceSyncOut();
  }
}

// -------------------------------- U P D A T E   S Y N C   C O U N T
void Arpeggiator::updateSyncCount()
{
  syncOut();
  syncCount++;
  updateNotes();
  programChange();
  if (beatReset == true) resetBeat();
  if (syncCount > 95) syncCount = 0;
}

// -------------------------------- B E A T   S Y N C   C H E C K
bool Arpeggiator::beatSyncCheck()
{
  programChange();
  trackPlayingNotes();
  constrainBeatValue();
  if ((syncCount % ((syncRate * 4) / beat) == 0 || syncCount == 0 ) && (automatic == true || pitch != oldPitch))
  {
    oldPitch = pitch;
    // updateNotes();
    updatePatternCount();
    return true;
  }
  else
  {
    return false;
  }
}

// -------------------------------- C O N S T R A I N   B E A T   V A L U E
void Arpeggiator::constrainBeatValue()
{
  if (beat > syncRate * 4 || (syncRate * 4) % beat != 0)
  {
    beat = syncRate * 2; //
  }
}

// -------------------------------- R E S E T   B E A T
void Arpeggiator::resetBeat()
{
  syncCount = -1;
  patternCount = -1;
  skipCount = -1;
}

// -------------------------------- U P D A T E   P A T T E R N   C O U N T
void Arpeggiator::updatePatternCount()
{
  patternCount ++;
  if (patternCount > 15)
  {
    patternCount = 0;
  }
}

// -------------------------------- N O T E   O N
void Arpeggiator::noteOn(byte noteNumber, byte velocityValue)
{
  if (mute == false)
  {
    _note = constrain (noteNumber, noteMin, noteMax); // keeps note within valid range
    interfaceNoteOn(midiChannel, _note, velocityValue);
    noteRecord[_note] = duration; // stores note length in record
  }
}

// -------------------------------- N O T E   O F F
void Arpeggiator::noteOff(byte noteNumber)
{
  interfaceNoteOff(midiChannel, noteNumber);
}

// -------------------------------- T R A C K   P L A Y I N G   N O T E S
void Arpeggiator::trackPlayingNotes()
{
  for (i = noteMin; i < noteMax + 1; i++)
  {
    if (noteRecord[i] > 1)
    {
      noteRecord[i] --;
    }
    if (noteRecord[i] == 1 && sustain == false)
    {
      noteOff(i);
      noteRecord[i] = 0;
    }
  }
}

// -------------------------------- P R O G R A M   C H A N G E
void Arpeggiator::programChange()
{
  if (midiInstrument != oldMidiInstrument)
  {
    interfaceProgramChange(midiInstrument);
    oldMidiInstrument = midiInstrument;
  }
}

// -------------------------------- C O N T I N U O U S
void Arpeggiator::continuous()
{
  // updateNotes();
  // turn outdated notes off
  for (i = 0; i <= 2; i++)
  {
    if ((oldNote[i] != note[noteType][0]) & (oldNote[i] != note[noteType][2]) & (oldNote[i] != note[noteType][4]))
    {
      noteOff(oldNote[i]);
    }
  }
  // turn new notes on
  for (i = 0; i < 3; i++)
  {
    if ((note[noteType][i * 2] != oldNote[0]) & (note[noteType][i * 2] != oldNote[1]) & (note[noteType][i * 2] != oldNote[2]))
    {
      noteOn(note[noteType][i * 2], 60);
    }
  }

  for (i = 0; i < 3; i++)
  {
    oldNote[i] = note[noteType][i * 2];
  }

  if (mute == true && oldMute == false)
  {
    for (i = 0; i < 3; i++)
    {
      noteOff(oldNote[i]);
      oldMute = true;
    }
  }
  if (mute == false && oldMute == true)
  {
    for (i = 0; i < 3; i++)
    {
      noteOn(oldNote[i], 60);
      oldMute = false;
    }
  }
}

// -------------------------------- P U L S E
void Arpeggiator::pulse()
{
  if (beatSyncCheck() == true)
  {
    for (i = 0; i < 3; i++)
    {
      // reversed order for Mono Synths, so first note is sent last:
      noteOn(note[noteType][4 - i * 2], velocity);
    }
  }
}

// -------------------------------- F A D E
void Arpeggiator::fade()
{

}

// -------------------------------- S T R U M
void Arpeggiator::strum()
{

}

// -------------------------------- A R P E G G I O
void Arpeggiator::arpeggio()
{
  if (beatSyncCheck() == true)
  {
    patternNote = note[noteType][(notePattern[notePatternNumber][patternCount]) - 1 ];
    noteOn(patternNote, velocity);


  }
}

// -------------------------------- B Y T E S
void Arpeggiator::bytes()
{
  if (beatSyncCheck() == true)
  {
    //patternByte = bytePattern[bytePatternNumber][patternCount];
    patternAddress = 256 + (bytePatternNumber * 16) + patternCount;
    patternByte = pgm_read_byte(patternAddress) % 8;
    byte c;
    c = patternByte & B00000001; if (c != 0) noteOn(note[noteType][0], velocity);
    c = patternByte & B00000010; if (c != 0) noteOn(note[noteType][2], velocity);
    c = patternByte & B00000100; if (c != 0) noteOn(note[noteType][4], velocity);
    c = patternByte & B00001000; if (c != 0) noteOn(note[noteType][6], velocity);
    c = patternByte & B00010000; if (c != 0) noteOn(note[noteType][1], velocity);
    c = patternByte & B00100000; if (c != 0) noteOn(note[noteType][3], velocity);
    c = patternByte & B01000000; if (c != 0) noteOn(note[noteType][5], velocity);
    c = patternByte & B10000000; if (c != 0) sustain = true;
    else
    {
      sustain = false;
    }
    patternCount++;
    if (patternCount > 15)
    {
      patternCount = 0;
    }
  }
}


/* -------------------------------- E E P R O M
  // Write data to a given address
  #include <EEPROM.h>
  EEPROM.write(address, value);
  EEPROM.read(address);
*/

// -------------------------------- M E L O D Y
void Arpeggiator::melody()
{
  if (beatSyncCheck() == true)
  {

  }
}

// -------------------------------- U P D A T E   N O T E S
void Arpeggiator::updateNotes()
{
  scaleIntervals();
  chordIntervals();
  melodyNotes();
  baselineNotes();
  chordNotes();
  sortedNotes();
}

// -------------------------------- S C A L E   I N T E R V A L S
void Arpeggiator::scaleIntervals()
{
  // Scale intervals
  for (i = 0; i < 7; i ++)
  {
    scaleInterval[i] = interval[scale][(mode + i) % 7];
  }
  if (mirror == true)
  {
    // swaps interval 1 and 4
    temp = scaleInterval[0];
    scaleInterval[0] = scaleInterval[3];
    scaleInterval[3] = temp;

    // swaps interval 2 and 3
    temp = scaleInterval[1];
    scaleInterval[1] = scaleInterval[2];
    scaleInterval[2] = temp;

    // swaps interval 5 and 7
    temp = scaleInterval[4];
    scaleInterval[4] = scaleInterval[6];
    scaleInterval[6] = temp;
  }
}

// -------------------------------- C H O R D   I N T E R V A L S
void Arpeggiator::chordIntervals() {
  for (i = 0; i < 7; i ++) {
    chordInterval[i] = scaleInterval[(chord + i) % 7];
  }

  if (invert == true)
  {
    // swaps interval 1 and 4
    temp = chordInterval[0];
    chordInterval[0] = chordInterval[3];
    chordInterval[3] = temp;

    // swaps interval 2 and 3
    temp = chordInterval[1];
    chordInterval[1] = chordInterval[2];
    chordInterval[2] = temp;

    // swaps interval 5 and 7
    temp = chordInterval[4];
    chordInterval[4] = chordInterval[6];
    chordInterval[6] = temp;
  }
}

// -------------------------------- M E L O D Y   N O T E S
void Arpeggiator::melodyNotes()
{
  // note 0
  note[0][0] = (key + (12 - pitch % 12) % 12) % 12 + pitch;

  // notes 2-7
  for (i = 1; i < 7; i ++)
  {
    note[0][i] = note[0][i - 1] + scaleInterval[i - 1];
  }
}

// -------------------------------- B A S E L I N E   N O T E S
void Arpeggiator::baselineNotes()
{
  // note 1, chosen from one of the key notes
  note[1][0] = note[0][chord];

  // makes sure the first note is not above the current octave range.
  if (note[1][0] - pitch > 11)
  {
    note[1][0] -= 12;
  }

  // notes 2-7
  for (i = 1; i < 7; i ++)
  {
    note[1][i] = note[1][i - 1] + chordInterval[i - 1];
  }
}

// -------------------------------- C H O R D   N O T E S
void Arpeggiator::chordNotes()
{
  // Type 2: wrapped notes = all notes within current octave

  note[2][0] = note[1][0];
  for (i = 1; i < 7; i ++)
  {
    if (note[1][i] - pitch < 12)
    {
      note[2][i] = note[1][i];
    }
    else {
      note[2][i] = note[1][i] - 12;
    }
  }
}


// -------------------------------- S O R T E D   N O T E S
void Arpeggiator::sortedNotes()
{
  // Type 3: sorted notes. Notes 1, 3, 5 of the main triad of the chord, sorted.
  // notes 2, 4, 6 are the alternate triad, also sorted. Note 7 unchanged

  // sort notes 1, 3, 5
  sortThreeBytes(note[2][0], note[2][2], note[2][4]);
  note[3][0] = x;
  note[3][2] = y;
  note[3][4] = z;

  // sort notes 2, 4, 6
  sortThreeBytes(note[2][1], note[2][3], note[2][5]);
  note[3][1] = x;
  note[3][3] = y;
  note[3][5] = z;

  // note 7
  note[3][6] = note[2][6];
}

// -------------------------------- S O R T   T H R E E   B Y T E S
void Arpeggiator::sortThreeBytes(byte a, byte b, byte c)
{
  if (a > b)
  {
    temp = a; a = b; b = temp;
  }
  if (a > c)
  {
    temp = a; a = c; c = temp;
  }
  if (b > c)
  {
    temp = b; b = c; c = temp;
  }
  x = a; y = b; z = c;
}


// --------------------------------
