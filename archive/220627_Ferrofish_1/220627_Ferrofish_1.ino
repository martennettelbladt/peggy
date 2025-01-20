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

*/
#define SerialMidi Serial3

# include "hci.h"
# include "arm.h"
# include "drive.h"


arm_rec arm;

long baud = 9600L;
int done = 0;
int port = 1;
int note = 0;

int tipX, tipY, tipZ;
int dirX, dirY, dirZ;
int joint0, joint1, joint2, joint3, joint4, joint5;



void setup() {
  Serial.begin(9600);
  SerialMidi.begin(31250);

  arm_init(&arm);
  arm_install_simple(&arm);
  arm_result result;

  result = arm_connect(&arm, port, baud);
  Serial.println(result);

  if (result == SUCCESS) {
    Serial.print("Connection established.\n");
  }
  
}

void loop() {
   arm_result result;
   result = arm_stylus_6DOF_update(&arm);
   //result = arm_stylus_6DOF_update(&arm);

   tipX = map(arm.stylus_tip.x, 0, 500, 0, 127);
   tipY = map(arm.stylus_tip.y, -500, 500, 50, 76);
   tipZ = map(arm.stylus_tip.z, 0, 200, 127, 0);

   tipX = constrain(tipX, 0, 127);
   tipY = constrain(tipY, 0, 127);
   tipZ = constrain(tipZ, 0, 127);

   //joint0 = constrain(arm.joint_deg[0] / 2, 0, 127);
   //joint1 = constrain(arm.joint_deg[1], 0, 127);
   //joint2 = constrain(arm.joint_deg[2], 0, 127);
   
   joint0 = arm.joint_deg[0];
   if (joint2 < 180) joint2 += 360;
   joint0 = map(joint0, 270, 450, 0, 127);
   
   joint1 = arm.joint_deg[1];
   joint1 = map(joint1, 0, 140, 0, 127);

   joint2 = arm.joint_deg[2];
   if (joint2 < 180) joint2 += 360;
   //joint2 = map(joint2, 270, 450, 127, 0);
   joint2 = map(joint2, 290, 400, 0, 127);
   
   joint3 = arm.joint_deg[3];
   if (joint3 < 180) joint3 += 360; // Lätt att råka gå över 180. 225 bättre
   joint3 = map(joint3, 510, 210, 0, 127);
   
   joint4 = arm.joint_deg[4];
   if (joint4 < 180) joint4 += 360;
   //joint4 = map(joint4, 419, 196, 127, 0);
   joint4 = map(joint4, 196, 300, 0, 127);

   joint4 = constrain(joint4, 0, 127);
   joint2 = constrain(joint2, 0, 127);
   
   // joint5 = constrain(arm.joint_deg[5] / 4, 0, 127); // DENNA FINNS INTE


    //Serial.print(joint2); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    //Serial.println(" "); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
/*
   // Serial.print(" dir X "); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    Serial.print(arm.stylus_tip.x); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    Serial.print("   "); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    Serial.print(arm.stylus_tip.y); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    Serial.print("   "); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    Serial.print(arm.stylus_tip.z); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    Serial.println(" "); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 
    // Serial.write(ch); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/
   midiCC(5, joint0);
   midiCC(6, joint1);
   midiCC(38, joint4);
   midiCC(37, joint3);

   if (joint4 > 110) {
    midiCC(40, 0); // stänger av Percussion om nivån går under ett visst värde
   }
   else midiCC(40, 127);

   
   midiCC(11, joint2);
   //midiCC(52, tipX);
   midiCC(108, tipZ);
   midiCC(43, tipY);
   //noteOn(note, 90);
   
   /*
   
   midiCC(5, tipX);
   midiCC(6, tipY);
   midiCC(3, joint0);
   midiCC(33, joint1);
   */
   delay(50);
   //   delay(50 * pow(2, joint2));
   //noteOn(note, 0);


}

void noteOn(int pitch, int velocity) {
  SerialMidi.write(0x90);
  SerialMidi.write(40);
  SerialMidi.write(constrain(velocity, 0, 127));
}

void midiCC(int controller, int value) {
  SerialMidi.write(0xb0);
  SerialMidi.write(controller);
  SerialMidi.write(constrain(value, 0, 127));
}
