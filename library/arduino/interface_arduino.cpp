//
//    PPPP      EEEEE     GGGG      GGGG     Y   Y
//    P   P     E        G         G          Y Y
//    PPPP      EEEE     G  GG     G  GG       Y
//    P         E        G   G     G   G       Y
//    P         EEEEE     GGGG      GGGG       Y
//
//    interface_arduino.cpp  //  Mårten Nettelbladt  //  2025-01-01


#define SerialMidi Serial
#define serialSyncIn Serial
#define serialSyncOut Serial
#define serialNoteOut Serial 
//#define serialMonitor Serial
// Arduino Micro must use "Serial1" (pin 0, 1) Confusing!
// Arduino Mega can use "Serial" (pin 0, 1), "Serial1" (pin 19, 18), "Serial2" (pin 17, 16) or "Serial3" (pin 15, 14)

#include "interface.h"

byte midiData;
byte midiClock = 0xf8;

// -------------------------------- I N T E R F A C E   B E G I N
void interfaceBegin()
{
  SerialMidi.begin(31250);
}
// -------------------------------- I N T E R F A C E   S Y N C   C H E C K
bool interfaceSyncCheck()
{
  if (serialSyncIn.available() > 0)
  {
    midiData = serialSyncIn.read();
    if (midiData == midiClock)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}

// -------------------------------- I N T E R F A C E   S Y N C   O U T
void interfaceSyncOut()
{
  serialSyncOut.write(midiClock);
}

// -------------------------------- I N T E R F A C E   N O T E   O N
void interfaceNoteOn(byte channel, byte note, byte velocity)
{
  serialNoteOut.write(0x90 + channel);
  serialNoteOut.write(note);
  serialNoteOut.write(velocity);
}

// -------------------------------- I N T E R F A C E   N O T E   O F F
void interfaceNoteOff(byte channel, byte note)
{
  serialNoteOut.write(0x90 + channel);
  serialNoteOut.write(note);
  serialNoteOut.write(0);
}

// -------------------------------- I N T E R F A C E   M I D I   C C 
void midiCC(int controller, int value) {
  SerialMidi.write(0xb0);
  SerialMidi.write(controller);
  SerialMidi.write(constrain(value, 0, 127));
}

// -------------------------------- I N T E R F A C E   P R O G R A M   C H A N G E
void interfaceProgramChange(byte instrument)
{
  serialNoteOut.write(0xC0);
  serialNoteOut.write(instrument); //Elpiano
}
