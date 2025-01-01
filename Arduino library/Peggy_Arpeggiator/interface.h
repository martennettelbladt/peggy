//
//    PPPP      EEEEE     GGGG      GGGG     Y   Y
//    P   P     E        G         G          Y Y
//    PPPP      EEEE     G  GG     G  GG       Y
//    P         E        G   G     G   G       Y
//    P         EEEEE     GGGG      GGGG       Y
//
//    interface.h  //  peggyInstruments  //  2022-10-23

#ifndef interface_h
#define interface_h

bool interfaceSyncCheck(); // check if sync is coming in
void interfaceSyncOut(); // send a sync out
void interfaceNoteOn(byte, byte, byte); // channel, note, velocity
void interfaceNoteOff(byte, byte); // channel, note
void interfaceMidiCC(byte, byte); // number, value
void interfaceProgramChange(byte);

#endif
