/* 2022-05-30 efter strul med hårdvara så funkar nu denna på min stora
  Arduino Mega + Sparkfun Max3232. Ansluter till Microscribe.

  Kopplingar på Max3232:
  RS-232:
  T1 OUT till Microscribe RX = vänstra på mellanraden i SERIAL
  T2 OUT -
  R1 IN till Micriscribe TX = högra på mellanraden i SERIAL
  R2 IN -

  3V-5.5V till 5v på Arduino Mega
  GND till ground på Arduino Mega

  LOGIC (TTL):
  T1 IN till TX1 på Arduino Mega, pin 18
  T2 IN -
  R1 OUT till RX1 på Arduino Mega, pin 19
  R2 OUT -

  I "drive.cpp" anges SerialArm till Serial1

  MIDI:
  SerialMidi = Serial3

  TX3 Pin 14 på Arduino Mega till DIN pin 5 via 220 ohm motstånd
  GND på Arduino Mega till DIN pin 2
  5v på Arduino Mega till DIN pin 4 via 220 ohm motstånd

  5 pin DIN
  Female front:     I

              / / I \ \
              3 5 2 4 1

  MIDI CC
  0xB0 (Channel 0)
  44 = Cutoff på Volca Keys, 6 = cutoff på typhon
  0-127

  2022-11-11.
  Denna version funkar med 2 nya bibliotek: Microscribe och Peggy
  plus använder pedalen för första gången!
  Fick också fason på sinusfunktionen idag.
  Problemet var att map() gör om float till heltal!
  Min namnsdag idag. Vernissage på ArkDes igår.

  2022-12-05
  Ny hårdvara, men samma funktion.
  Mycket nöjd med FADE-funktionen till Ferrofish som fejdar mellan Upper och Lower!

*/
#define SerialMidi Serial3

# include <hci.h>
# include <arm.h>
# include <drive.h>

#include <peggy.h>
#include <pattern.h>
#include <interface.h>

#define serialSyncIn Serial3
#define serialSyncOut Serial3
#define serialNoteOut Serial3 // 'Serial1' for Arduino Micro | 'Serial' for Arduino Uno 
#define serialMonitor Serial3

#define PI 3.1415926535898

arm_rec arm;
arm_result result;

// Två stycken peggy. En för Upper och en för Lower
Arpeggiator peggy[2];

long baud = 9600L;
int done = 0;
int port = 1;
int note = 60;

int tipX, tipY, tipZ;
int dirX, dirY, dirZ;
float joint0, joint1, joint2, joint3, joint4;
int subTip;

float sineCentre, sineWidth, swell;
float radiansValue;
byte drawBarValue[9];
byte chordValue;
int byteSineValue;

byte midiData;
byte midiClock = 0xf8;
int distance;

float fadeSine[128];

// -------------------------------- S E T U P
void setup() {
  Serial.begin(9600);
  SerialMidi.begin(31250);
  pinMode(0, INPUT);

  arm_init(&arm);
  arm_install_simple(&arm);

  result = arm_connect(&arm, port, baud);
  Serial.println(result);

  if (result == SUCCESS) {
    Serial.print("Connection established.\n");
  }
  // gör inställningar för bägge peggy. de kommunicerar på varsin MIDI-kanal
  for (int i = 0; i < 2; i++)
  {
    peggy[i].begin();
    peggy[i].playMode = 0;
    peggy[i].pitch = 59;
    peggy[i].midiChannel = i;
  }

  // Skapar en halv cosinustabell som börjar på 1 och slutar på 0
  // Denna används för fade mellan Upper och Lower
  // Sparas i en array av float med 128 värden
  for (int i = 0; i < 128; i++)
  {
    fadeSine[i] = cos(i * PI / 128) / 2 + 0.5;
  }
  
}

void loop() {

  result = arm_stylus_3DOF_update(&arm);
/*
  tipX = constrain(arm.stylus_tip.x, 150, 500);
  tipY = constrain(arm.stylus_tip.y, -300, 300);
  tipZ = constrain(arm.stylus_tip.z, 0, 500);

  subTip = constrain(arm.endpoint[3].z, 0, 500);
  subTip = map(subTip, 0, 500, 127, 0);

  tipX = map(tipX, 150, 500, 0, 127);
  tipY = map(tipY, -300, 300, 0, 7);
  tipZ = map(tipZ, 0, 500, 50, 70);
*/

  joint0 = arm.joint_deg[0]; // Swell
  if (joint0 < 180) joint0 += 360;
  joint0 = constrain (joint0, 315, 405);
  joint0 = mapf(joint0, 315, 405, 0.1, 1);

  joint1 = arm.joint_deg[1]; // sineCentre
  joint1 = mapf(joint1, 0, 140, 0, 3.2);

  joint2 = arm.joint_deg[2]; // sineWidth
  if (joint2 < 180) joint2 += 360;
  joint2 = mapf(joint2, 270, 450, 0, 7);

  joint3 = arm.joint_deg[3]; // chord
  if (joint3 < 180) joint3 += 360; // 180 byter högst upp
  chordValue = map(joint3, 202.5, 517.5, 0, 7); // ÄNDRA INTE!!! map(joint3, 202.5, 517.5, 0, 7);
  chordValue = constrain(chordValue, 0, 6);

  joint4 = arm.joint_deg[4]; // fade
  if (joint4 < 180) joint4 += 360;

  // Tonart styrs av pot A0
  peggy[1].key = peggy[0].key = floor(analogRead(A0) / 86);

  if (joint4 > 275)
  {
    peggy[0].chord = (chordValue * 2 + 1) % 7;
    peggy[0].invert = arm.hci.button[0];
    peggy[0].flat = arm.hci.button[1];
  }

  if (joint4 < 265)
  {
    peggy[1].chord = (chordValue * 2 + 1) % 7;
    peggy[1].invert = arm.hci.button[0];
    peggy[1].flat = arm.hci.button[1];
  }

  // Ställer in fade mellan upper och lower
  int sineIndexUpper = mapf(joint4, 254, 295, 0, 127);
  int sineIndexLower = mapf(joint4, 245, 286, 127, 0);

  sineIndexUpper = constrain(sineIndexUpper, 0, 127);
  sineIndexLower = constrain(sineIndexLower, 0, 127);
  float panUpper = fadeSine[sineIndexUpper];
  float panLower = fadeSine[sineIndexLower];

  
  float swell = joint0;

  
  sineCentre = pow(2, joint1) - 3;
  sineWidth = 0.4 * pow(2, joint2);

  for (int i = 0; i < 9; i ++)
  {
    byteSineValue = mapf(i, sineCentre - sineWidth, sineCentre + sineWidth, 0, 255);
    if (byteSineValue < 0 || byteSineValue > 255) byteSineValue = 0;
    drawBarValue[i] = pgm_read_byte(&peggySineTable[byteSineValue]) * 0.5 * swell; //
    drawBarValue[i] = constrain (drawBarValue[i], 0, 127);
  }

  drawBarValue[0] = constrain (drawBarValue[0], 12, 127); // Ger grundton

  //upper
  midiCC(27, drawBarValue[0] * panUpper);
  midiCC(28, drawBarValue[2] * panUpper);
  midiCC(29, drawBarValue[1] * panUpper);
  midiCC(30, drawBarValue[3] * panUpper);
  midiCC(31, drawBarValue[4] * panUpper);
  midiCC(33, drawBarValue[5] * panUpper); // 32 = bank number

  // De tre sista är så gälla och konstiga, hörs direkt när de sätts på fast det tonas...
  midiCC(34, 0); //drawBarValue[6]);
  midiCC(35, 0); //drawBarValue[7]);
  midiCC(36, 0); //drawBarValue[8]);

  //lower
  midiCC(18, drawBarValue[0] * panLower);
  midiCC(19, drawBarValue[2] * panLower);
  midiCC(20, drawBarValue[1] * panLower);
  midiCC(21, drawBarValue[3] * panLower);
  midiCC(22, drawBarValue[4] * panLower);
  midiCC(23, drawBarValue[5] * panLower); // 32 = bank number

  // De tre sista är så gälla och konstiga, hörs direkt när de sätts på fast det tonas...
  midiCC(24, 0); //drawBarValue[6]);
  midiCC(25, 0); //drawBarValue[7]);
  midiCC(26, 0); //drawBarValue[8]);


  peggy[0].play();
  peggy[1].play();



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

  /*
  serialNoteOut.write(0x90 + channel + 1);
  serialNoteOut.write(note);
  serialNoteOut.write(0);
  */
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
