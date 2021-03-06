#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

///////////////////////////////////// LIST OF FLUTE FINGERING PATTERNS ////////////////////////////////////

// Each pattern is based off flute fingerings. The position of each list element corresponds with Pin number.
// **Reminder** Pin(1) (TX pin) cannot be used since it cannot act as an input. Therefore, the Pin order starts-
// with Pin(0), then Pin(2), then Pin(3) and so on until Pin(9). Half steps are represented in flat (or 'b').
// '0' means button is open. '1' means button is closed.

String SfluteFiveC  = "010000001";    // Pitch = 72
String SfluteFiveDb = "000000001";    // Pitch = 73
String SfluteFiveD  = "101101110";    // Pitch = 74
String SfluteFiveEb = "101101111";    // Pitch = 75
String SfluteFiveE  = "111101101";    // Pitch = 76
String SfluteFiveF  = "111101001";    // Pitch = 77
String SfluteFiveGb = "111100011";    // Pitch = 78
String SfluteFiveG  = "111100001";    // Pitch = 79
String SfluteFiveAb = "111110001";    // Pitch = 80
String SfluteFiveA  = "111000001";    // Pitch = 81
String SfluteFiveBb = "110001001";    // Pitch = 82
String SfluteFiveB  = "110000001";    // Pitch = 83
String SfluteArray[] = {SfluteFiveC, SfluteFiveDb, SfluteFiveD, SfluteFiveEb, 
                        SfluteFiveE, SfluteFiveF, SfluteFiveGb, SfluteFiveG,
                        SfluteFiveAb, SfluteFiveA, SfluteFiveBb, SfluteFiveB};

int fluteFiveCPitch    = 72;
int fluteFiveDbPitch   = 73;
int fluteFiveDPitch    = 74;
int fluteFiveEbPitch   = 75;
int fluteFiveEPitch    = 76;
int fluteFiveFPitch    = 77;
int fluteFiveGbPitch   = 78;
int fluteFiveGPitch    = 79;
int fluteFiveAbPitch   = 80;
int fluteFiveAPitch    = 81;
int fluteFiveBbPitch   = 82;
int fluteFiveBPitch    = 83;
int fluteArrayPitches[] = {fluteFiveCPitch, fluteFiveDbPitch, fluteFiveEbPitch, fluteFiveEbPitch, 
                           fluteFiveEPitch, fluteFiveFPitch, fluteFiveGbPitch, fluteFiveGPitch,
                           fluteFiveAbPitch, fluteFiveAPitch, fluteFiveBbPitch, fluteFiveBPitch};

///////////////////////////////////// VARIABLES AND LISTS /////////////////////////////////////

int mode;                      // determines if mode 0 or 1 is activated
int modeMem;                   // memory history of the mode, allows to determine if a new mode is being selected
const int delayTimer = 10;     // timer setting for the delays
int velocity = 64;             // Dynamic for the note (Volume or Gain). Between 0 (silent) and 127 (loudest).
const int channel = 1;         // MIDI channel to send message ('1' by default).

//Synth Mode variables

bool activeNote = false;          // 'false' means note is off. 'true' means note is on.
int pitch = 0;                    // MIDI value for given note (ie, '72' is note C5).
int pitchMem = 0;                 // memory pitch MIDI value.
int snapshotLarge = 0;            // highest number within the snapshot
const int noteOnThreshold = 100;  // threshold to trigger the activeNote to true.
const int noteOffThreshold = 50;  // threshold to trigger the activeNote to false.
int snapshot[30];                 // array that scans for the thresholds.
String senseFingerString;

//Learn Mode variables

int button;
int inputMemory[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int inputMemoryPitch[10] = {83, 84, 85, 86, 87, 88, 89, 90, 91};

///////////////////////////////////// MODE SYNTH (Live performance) /////////////////////////////////////

//Takes multple samples to read analog inputs detected by microphone. Largest sample is used to determine thresholds.
void GetTriggerSnapShot(){ 
  snapshotLarge = 0; 
  for (int i = 0; i < 30; i++){
    snapshot[i] = analogRead(0);
    if (snapshot[i] > snapshotLarge){
      snapshotLarge = snapshot[i];
    }
  }
}

//Largest sample is used to determine thresholds. If note is playing, check to see if pattern matches and if pattern is different than before.
//If note is not playing, check to see if note was playing before and then stop, or do nothing
void CheckTriggerSnapshot(){
  if (activeNote){
    if (snapshotLarge <= noteOffThreshold){
      activeNote = false;
      MIDI.sendNoteOff(pitch, velocity, channel);
    }
    else if (snapshotLarge > noteOffThreshold){
      CheckFingering();
      if (pitch != pitchMem){
        MIDI.sendNoteOff(pitchMem, velocity, channel);
        MIDI.sendNoteOn(pitch, velocity, channel);
        pitchMem = pitch;
      }
    }
  }
  else{
    if (snapshotLarge >= noteOnThreshold){
      CheckFingering();
      if (pitch != 0){
        MIDI.sendNoteOn(pitch, velocity, channel);
        pitchMem = pitch;
        activeNote = true;
      }
    }
  }
}

//Reads the digital inputs and compares them against the pattern arrays above for a potential match.
void CheckFingering(){
  int senseFinger[] = {!digitalRead(2), !digitalRead(3), !digitalRead(4), 
                       !digitalRead(5), !digitalRead(6), !digitalRead(7),
                       !digitalRead(8), !digitalRead(9), !digitalRead(10)};

  for(int i = 0; i < 9; i++){
    senseFingerString = String(senseFingerString + senseFinger[i]);
  }
  for(int i = 0; i < 12; i++){
    if(senseFingerString == SfluteArray[i]){
      pitch = fluteArrayPitches[i];
    }
  }
}
  
///////////////////////////////////// MODE LEARN (Learning Aid) ///////////////////////////////////////////////////////////

//Detects each button one at a time and determines if it has changed by comparing it against inputMemory
void ModeLearn(){
  for (int j = 0; j < 9; j++){
    if (!digitalRead(j+2) == 1){
      if (inputMemory[j] == 0){
        MIDI.sendNoteOn(inputMemoryPitch[j], velocity, channel);
        inputMemory[j] = 1;}}
    else{
      if (inputMemory[j] == 1){
        MIDI.sendNoteOff(inputMemoryPitch[j], velocity, channel);
        inputMemory[j] = 0;}}}
}

///////////////////////////////////// NON-MODE FUNCTIONS /////////////////////////////////////////////////////////

//If there is a change in the mode, this is called to make sure nothing affects the new mode or going back to the old mode.
void ResetVars(){
  activeNote = false;
  pitch = 0;
  pitchMem = 0;
  button = 0;
  inputMemory[0, 1, 2, 3, 4, 5, 6, 7, 8, 9] = 0;
}

//Enables MIDI abilities and sets up inputs as pullup. This way, buttons need only be connected from Pin to Ground.
void setup(){
  MIDI.begin(MIDI_CHANNEL_OMNI);
  pinMode (0, INPUT_PULLUP);      //checks mode
  pinMode (2, INPUT_PULLUP);      //thumb key
  pinMode (3, INPUT_PULLUP);      //index LH
  pinMode (4, INPUT_PULLUP);      //middle LH
  pinMode (5, INPUT_PULLUP);      //ring LH
  pinMode (6, INPUT_PULLUP);      //pinky LH
  pinMode (7, INPUT_PULLUP);      //index RH
  pinMode (8, INPUT_PULLUP);      //middle RH
  pinMode (9, INPUT_PULLUP);      //ring RH
  pinMode (10, INPUT_PULLUP);     //pinky RH
  
}

//Reads the mode input and determines if mode 0 or mode 1. If there is a change in made (detected by modeMem), then ResetVars is triggered.
//Repeats over and over until program is terminated by unplugging the Arduino board.
void loop(){
  mode = digitalRead(0);
  if (mode == 0){
    if (mode != modeMem){
      ResetVars();
      modeMem = 0;
    }
    GetTriggerSnapShot();
    CheckTriggerSnapshot();
  }
  else{
    if (mode != modeMem){
      ResetVars();
      modeMem = 1;
    }
    ModeLearn();
    delay(delayTimer);
  }
}

/*
int fluteFiveC[]  = { 0, 1, 0, 0, 0, 0, 0, 0, 1 };    // Pitch = 72
int fluteFiveDb[] = { 0, 0, 0, 0, 0, 0, 0, 0, 1 };    // Pitch = 73
int fluteFiveD[]  = { 1, 0, 1, 1, 0, 1, 1, 1, 0 };    // Pitch = 74
int fluteFiveEb[] = { 1, 0, 1, 1, 0, 1, 1, 1, 1 };    // Pitch = 75
int fluteFiveE[]  = { 1, 1, 1, 1, 0, 1, 1, 0, 1 };    // Pitch = 76
int fluteFiveF[]  = { 1, 1, 1, 1, 0, 1, 0, 0, 1 };    // Pitch = 77
int fluteFiveGb[] = { 1, 1, 1, 1, 0, 0, 0, 1, 1 };    // Pitch = 78
int fluteFiveG[]  = { 1, 1, 1, 1, 0, 0, 0, 0, 1 };    // Pitch = 79
int fluteFiveAb[] = { 1, 1, 1, 1, 1, 0, 0, 0, 1 };    // Pitch = 80
int fluteFiveA[]  = { 1, 1, 1, 0, 0, 0, 0, 0, 1 };    // Pitch = 81
int fluteFiveBb[] = { 1, 1, 0, 0, 0, 1, 0, 0, 1 };    // Pitch = 82
int fluteFiveB[]  = { 1, 1, 0, 0, 0, 0, 0, 0, 1 };    // Pitch = 83

// C5 //
  if (senseFinger[0] == fluteFiveC[0] && senseFinger[1] == fluteFiveC[1] && senseFinger[2] == fluteFiveC[2] &&
      senseFinger[3] == fluteFiveC[3] && senseFinger[4] == fluteFiveC[4] && senseFinger[5] == fluteFiveC[5] &&
      senseFinger[6] == fluteFiveC[6] && senseFinger[7] == fluteFiveC[7] && senseFinger[8] == fluteFiveC[8]) {pitch = fluteFiveCPitch;}
// Db5 //
  else if (senseFinger[0] == fluteFiveDb[0] && senseFinger[1] == fluteFiveDb[1] && senseFinger[2] == fluteFiveDb[2] &&
           senseFinger[3] == fluteFiveDb[3] && senseFinger[4] == fluteFiveDb[4] && senseFinger[5] == fluteFiveDb[5] &&
           senseFinger[6] == fluteFiveDb[6] && senseFinger[7] == fluteFiveDb[7] && senseFinger[8] == fluteFiveDb[8]) {pitch = fluteFiveDbPitch;}
// D5 //
  else if (senseFinger[0] == fluteFiveD[0] && senseFinger[1] == fluteFiveD[1] && senseFinger[2] == fluteFiveD[2] &&
           senseFinger[3] == fluteFiveD[3] && senseFinger[4] == fluteFiveD[4] && senseFinger[5] == fluteFiveD[5] &&
           senseFinger[6] == fluteFiveD[6] && senseFinger[7] == fluteFiveD[7] && senseFinger[8] == fluteFiveD[8]) {pitch = fluteFiveDPitch;}
// Eb5 //
  else if (senseFinger[0] == fluteFiveEb[0] && senseFinger[1] == fluteFiveEb[1] && senseFinger[2] == fluteFiveEb[2] &&
           senseFinger[3] == fluteFiveEb[3] && senseFinger[4] == fluteFiveEb[4] && senseFinger[5] == fluteFiveEb[5] &&
           senseFinger[6] == fluteFiveEb[6] && senseFinger[7] == fluteFiveEb[7] && senseFinger[8] == fluteFiveEb[8]) {pitch = fluteFiveEbPitch;}
// E5 //
  else if (senseFinger[0] == fluteFiveE[0] && senseFinger[1] == fluteFiveE[1] && senseFinger[2] == fluteFiveE[2] &&
           senseFinger[3] == fluteFiveE[3] && senseFinger[4] == fluteFiveE[4] && senseFinger[5] == fluteFiveE[5] &&
           senseFinger[6] == fluteFiveE[6] && senseFinger[7] == fluteFiveE[7] && senseFinger[8] == fluteFiveE[8]) {pitch = fluteFiveEPitch;}
// F5 //
  else if (senseFinger[0] == fluteFiveF[0] && senseFinger[1] == fluteFiveF[1] && senseFinger[2] == fluteFiveF[2] &&
           senseFinger[3] == fluteFiveF[3] && senseFinger[4] == fluteFiveF[4] && senseFinger[5] == fluteFiveF[5] &&
           senseFinger[6] == fluteFiveF[6] && senseFinger[7] == fluteFiveF[7] && senseFinger[8] == fluteFiveF[8]) {pitch = fluteFiveFPitch;}
// Gb5 //
  else if (senseFinger[0] == fluteFiveGb[0] && senseFinger[1] == fluteFiveGb[1] && senseFinger[2] == fluteFiveGb[2] &&
           senseFinger[3] == fluteFiveGb[3] && senseFinger[4] == fluteFiveGb[4] && senseFinger[5] == fluteFiveGb[5] &&
           senseFinger[6] == fluteFiveGb[6] && senseFinger[7] == fluteFiveGb[7] && senseFinger[8] == fluteFiveGb[8]) {pitch = fluteFiveGbPitch;}
// G5 //
  else if (senseFinger[0] == fluteFiveG[0] && senseFinger[1] == fluteFiveG[1] && senseFinger[2] == fluteFiveG[2] &&
           senseFinger[3] == fluteFiveG[3] && senseFinger[4] == fluteFiveG[4] && senseFinger[5] == fluteFiveG[5] &&
           senseFinger[6] == fluteFiveG[6] && senseFinger[7] == fluteFiveG[7] && senseFinger[8] == fluteFiveG[8]) {pitch = fluteFiveGPitch;}
// Ab5 //
  else if (senseFinger[0] == fluteFiveAb[0] && senseFinger[1] == fluteFiveAb[1] && senseFinger[2] == fluteFiveAb[2] &&
           senseFinger[3] == fluteFiveAb[3] && senseFinger[4] == fluteFiveAb[4] && senseFinger[5] == fluteFiveAb[5] &&
           senseFinger[6] == fluteFiveAb[6] && senseFinger[7] == fluteFiveAb[7] && senseFinger[8] == fluteFiveAb[8]) {pitch = fluteFiveAbPitch;}
// A5 //
  else if (senseFinger[0] == fluteFiveA[0] && senseFinger[1] == fluteFiveA[1] && senseFinger[2] == fluteFiveA[2] &&
           senseFinger[3] == fluteFiveA[3] && senseFinger[4] == fluteFiveA[4] && senseFinger[5] == fluteFiveA[5] &&
           senseFinger[6] == fluteFiveA[6] && senseFinger[7] == fluteFiveA[7] && senseFinger[8] == fluteFiveA[8]) {pitch = fluteFiveAPitch;}   
// Bb5 //
  else if (senseFinger[0] == fluteFiveBb[0] && senseFinger[1] == fluteFiveBb[1] && senseFinger[2] == fluteFiveBb[2] &&
           senseFinger[3] == fluteFiveBb[3] && senseFinger[4] == fluteFiveBb[4] && senseFinger[5] == fluteFiveBb[5] &&
           senseFinger[6] == fluteFiveBb[6] && senseFinger[7] == fluteFiveBb[7] && senseFinger[8] == fluteFiveBb[8]) {pitch = fluteFiveBbPitch;}
// B5 //
  else if (senseFinger[0] == fluteFiveB[0] && senseFinger[1] == fluteFiveB[1] && senseFinger[2] == fluteFiveB[2] &&
           senseFinger[3] == fluteFiveB[3] && senseFinger[4] == fluteFiveB[4] && senseFinger[5] == fluteFiveB[5] &&
           senseFinger[6] == fluteFiveB[6] && senseFinger[7] == fluteFiveB[7] && senseFinger[8] == fluteFiveB[8]) {pitch = fluteFiveBPitch;}   
  else {pitch = 0;}    
*/
