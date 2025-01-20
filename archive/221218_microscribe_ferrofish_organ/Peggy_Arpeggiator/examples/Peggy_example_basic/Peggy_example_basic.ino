
#define SerialMidi Serial
#define serialSyncIn Serial
#define serialSyncOut Serial
#define serialNoteOut Serial 
#define serialMonitor Serial
// Use 'Serial1' for Arduino Micro and other possible numbers for Arduino Mega

#include <peggy.h>
#include <pattern.h>
#include <interface.h>

Arpeggiator peggy;

// -------------------------------- S E T U P
void setup() {
  SerialMidi.begin(31250);
  pinMode(0, INPUT);

  peggy.begin();
  peggy.playMode = 0;
  peggy.pitch = 59;
  peggy.midiChannel = 0;
}

void loop() {
  // Musical Key determined by value from pot A0
  peggy.key = floor(analogRead(A0) / 86);
  peggy.play();

}

// -------------------------------- M A P F
float mapf(float value, float istart, float istop, float ostart, float ostop) {
  return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

// -------------------------------- M I D I   C C 
void midiCC(int controller, int value) {
  SerialMidi.write(0xb0);
  SerialMidi.write(controller);
  SerialMidi.write(constrain(value, 0, 127));
}

// -------------------------------- I N T E R F A C E   N O T E   O N
void interfaceNoteOn(byte channel, byte note, byte velocity)
{
  if (peggy[0].mute == false)
  {
    serialNoteOut.write(0x90 + channel);
    serialNoteOut.write(note);
    serialNoteOut.write(velocity);

    /*
    serialNoteOut.write(0x90 + channel + 1);
    serialNoteOut.write(note);
    serialNoteOut.write(velocity);
    */
  }
}

// -------------------------------- I N T E R F A C E   N O T E   O F F
void interfaceNoteOff(byte channel, byte note)
{
  serialNoteOut.write(0x90 + channel);
  serialNoteOut.write(note);
  serialNoteOut.write(0);

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

// -------------------------------- I N T E R F A C E   S Y N C
void interfaceSyncOut()
{

}

// -------------------------------- I N T E R F A C E   P R O G R A M   C H A N G E
void interfaceProgramChange(byte instrument)
{
  serialNoteOut.write(0xC0);
  serialNoteOut.write(instrument); //Elpiano
}
