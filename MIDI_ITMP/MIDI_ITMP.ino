#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

///////////////////////////////////// LIST OF FLUTE FINGERING PATTERNS ////////////////////////////////////

// Each pattern is based off flute fingerings. The position of each list element corresponds with Pin number.
// **Reminder** Pin(1) (TX pin) cannot be used since it cannot act as an input. Therefore, the Pin order starts-
// with Pin(0), then Pin(2), then Pin(3) and so on until Pin(9). Half steps are represented in flat (or 'b').
// '0' means button is open. '1' means button is closed.

int flute_c_five[]    = { 0, 1, 0, 0, 0, 0, 0, 0, 1 };    // Pitch = 72
int flute_db_five[]   = { 0, 0, 0, 0, 0, 0, 0, 0, 1 };    // Pitch = 73
int flute_d_five[]    = { 1, 0, 1, 1, 0, 1, 1, 1, 0 };    // Pitch = 74
int flute_eb_five[]   = { 1, 0, 1, 1, 0, 1, 1, 1, 1 };    // Pitch = 75
int flute_e_five[]    = { 1, 1, 1, 1, 0, 1, 1, 0, 1 };    // Pitch = 76
int flute_f_five[]    = { 1, 1, 1, 1, 0, 1, 0, 0, 1 };    // Pitch = 77
int flute_gb_five[]   = { 1, 1, 1, 1, 0, 0, 0, 1, 1 };    // Pitch = 78
int flute_g_five[]    = { 1, 1, 1, 1, 0, 0, 0, 0, 1 };    // Pitch = 79
int flute_ab_five[]   = { 1, 1, 1, 1, 1, 0, 0, 0, 1 };    // Pitch = 80
int flute_a_five[]    = { 1, 1, 1, 0, 0, 0, 0, 0, 1 };    // Pitch = 81
int flute_bb_five[]   = { 1, 1, 0, 0, 0, 1, 0, 0, 1 };    // Pitch = 82
int flute_b_five[]    = { 1, 1, 0, 0, 0, 0, 0, 0, 1 };    // Pitch = 83

int flute_c_five_pitch    = 72;
int flute_db_five_pitch   = 73;
int flute_d_five_pitch    = 74;
int flute_eb_five_pitch   = 75;
int flute_e_five_pitch    = 76;
int flute_f_five_pitch    = 77;
int flute_gb_five_pitch   = 78;
int flute_g_five_pitch    = 79;
int flute_ab_five_pitch   = 80;
int flute_a_five_pitch    = 81;
int flute_bb_five_pitch   = 82;
int flute_b_five_pitch    = 83;

///////////////////////////////////// CHANGING VARIABLES AND LISTS /////////////////////////////////////


int mode;                       // determines if mode 0 or 1 is activated
int mode_mem;                   // memory history of the mode, allows to determine if a new mode is being selected
int delaytimer = 10;            // timer setting for the delays

//mode_0 variables

int active_note = 0;            // '0' means note is off. '1' means note is on.
int pitch = 0;                  // MIDI value for given note (ie, '72' is note C5).
int pitch_mem = 0;              // memory pitch MIDI value.
int snapshot_large = 0;         // highest number within the snapshot

int velocity = 64;              // Dynamic for the note (Volume or Gain). Between 0 (silent) and 127 (loudest).
int channel = 1;                // MIDI channel to send message ('1' by default).
int noteon_threshold = 100;     // threshold to trigger the active_note to true.
int noteoff_threshold = 50;     // threshold to trigger the active_note to false.
int snapshot[800];              // list that scans for the thresholds.


//mode_1 variables

int input_button = 0;
int button;
int input_mem[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int input_mempitch[10] = {83, 0, 84, 85, 86, 87, 88, 89, 90, 91};


///////////////////////////////////// MODE ZERO (Live performance) /////////////////////////////////////


void mode_zero() //Takes multple samples to read analog inputs detected by microphone. Largest sample is used to determine thresholds.
{
  snapshot_large = 0; 
  for (int i = 0; i < 799; i++)
  {
    snapshot[i] = analogRead(0);
    if (snapshot[i] > snapshot_large)
    {
      snapshot_large = snapshot[i];
    }
  }
  check_snapshot();
}

void check_snapshot() //Largest sample is used to determine thresholds. If note is playing, check to see if pattern matches and if pattern is different than before.
                      // If note is not playing, check to see if note was playing before and then stop, or do nothing.
{
  if (active_note == 1) 
  {
    if (snapshot_large <= noteoff_threshold) 
    {
      active_note = 0;
      MIDI.sendNoteOff(pitch, velocity, channel);
      delay(delaytimer);
    }
    else if (snapshot_large > noteoff_threshold)
    {
      check_fingering();
      if (pitch != pitch_mem)
      {
        MIDI.sendNoteOff(pitch_mem, velocity, channel);
        MIDI.sendNoteOn(pitch, velocity, channel);
        pitch_mem = pitch;
        delay(delaytimer);
      }
    }
  }
  else if (active_note == 0)
  {
    if (snapshot_large >= noteon_threshold) 
    {
      check_fingering();
      if (pitch != 0) 
      {
        MIDI.sendNoteOn(pitch, velocity, channel);
        pitch_mem = pitch;
        active_note = 1;
        delay(delaytimer);
      }
    }
    else if (snapshot_large < noteon_threshold)
    {
      ;
    }
  }
}

void check_fingering() //Reads the digital inputs and compares them against the pattern arrays above for a potential match.
{
  int sensefinger[] = {!digitalRead(0), !digitalRead(2), !digitalRead(3), 
                       !digitalRead(4), !digitalRead(5), !digitalRead(6),
                       !digitalRead(7), !digitalRead(8), !digitalRead(9)};

// C5 //  
  if (sensefinger[0] == flute_c_five[0] && sensefinger[1] == flute_c_five[1] && sensefinger[2] == flute_c_five[2] &&
      sensefinger[3] == flute_c_five[3] && sensefinger[4] == flute_c_five[4] && sensefinger[5] == flute_c_five[5] &&
      sensefinger[6] == flute_c_five[6] && sensefinger[7] == flute_c_five[7] && sensefinger[8] == flute_c_five[8]) {pitch = flute_c_five_pitch;}
// Db5 //
  else if (sensefinger[0] == flute_db_five[0] && sensefinger[1] == flute_db_five[1] && sensefinger[2] == flute_db_five[2] &&
           sensefinger[3] == flute_db_five[3] && sensefinger[4] == flute_db_five[4] && sensefinger[5] == flute_db_five[5] &&
           sensefinger[6] == flute_db_five[6] && sensefinger[7] == flute_db_five[7] && sensefinger[8] == flute_db_five[8]) {pitch = flute_db_five_pitch;}
// D5 //
  else if (sensefinger[0] == flute_d_five[0] && sensefinger[1] == flute_d_five[1] && sensefinger[2] == flute_d_five[2] &&
           sensefinger[3] == flute_d_five[3] && sensefinger[4] == flute_d_five[4] && sensefinger[5] == flute_d_five[5] &&
           sensefinger[6] == flute_d_five[6] && sensefinger[7] == flute_d_five[7] && sensefinger[8] == flute_d_five[8]) {pitch = flute_d_five_pitch;}
// Eb5 //
  else if (sensefinger[0] == flute_eb_five[0] && sensefinger[1] == flute_eb_five[1] && sensefinger[2] == flute_eb_five[2] &&
           sensefinger[3] == flute_eb_five[3] && sensefinger[4] == flute_eb_five[4] && sensefinger[5] == flute_eb_five[5] &&
           sensefinger[6] == flute_eb_five[6] && sensefinger[7] == flute_eb_five[7] && sensefinger[8] == flute_eb_five[8]) {pitch = flute_eb_five_pitch;}
// E5 //
  else if (sensefinger[0] == flute_e_five[0] && sensefinger[1] == flute_e_five[1] && sensefinger[2] == flute_e_five[2] &&
           sensefinger[3] == flute_e_five[3] && sensefinger[4] == flute_e_five[4] && sensefinger[5] == flute_e_five[5] &&
           sensefinger[6] == flute_e_five[6] && sensefinger[7] == flute_e_five[7] && sensefinger[8] == flute_e_five[8]) {pitch = flute_e_five_pitch;}
// F5 //
  else if (sensefinger[0] == flute_f_five[0] && sensefinger[1] == flute_f_five[1] && sensefinger[2] == flute_f_five[2] &&
           sensefinger[3] == flute_f_five[3] && sensefinger[4] == flute_f_five[4] && sensefinger[5] == flute_f_five[5] &&
           sensefinger[6] == flute_f_five[6] && sensefinger[7] == flute_f_five[7] && sensefinger[8] == flute_f_five[8]) {pitch = flute_f_five_pitch;}
// Gb5 //
  else if (sensefinger[0] == flute_gb_five[0] && sensefinger[1] == flute_gb_five[1] && sensefinger[2] == flute_gb_five[2] &&
           sensefinger[3] == flute_gb_five[3] && sensefinger[4] == flute_gb_five[4] && sensefinger[5] == flute_gb_five[5] &&
           sensefinger[6] == flute_gb_five[6] && sensefinger[7] == flute_gb_five[7] && sensefinger[8] == flute_gb_five[8]) {pitch = flute_gb_five_pitch;}
// G5 //
  else if (sensefinger[0] == flute_g_five[0] && sensefinger[1] == flute_g_five[1] && sensefinger[2] == flute_g_five[2] &&
           sensefinger[3] == flute_g_five[3] && sensefinger[4] == flute_g_five[4] && sensefinger[5] == flute_g_five[5] &&
           sensefinger[6] == flute_g_five[6] && sensefinger[7] == flute_g_five[7] && sensefinger[8] == flute_g_five[8]) {pitch = flute_g_five_pitch;}
// Ab5 //
  else if (sensefinger[0] == flute_ab_five[0] && sensefinger[1] == flute_ab_five[1] && sensefinger[2] == flute_ab_five[2] &&
           sensefinger[3] == flute_ab_five[3] && sensefinger[4] == flute_ab_five[4] && sensefinger[5] == flute_ab_five[5] &&
           sensefinger[6] == flute_ab_five[6] && sensefinger[7] == flute_ab_five[7] && sensefinger[8] == flute_ab_five[8]) {pitch = flute_ab_five_pitch;}
// A5 //
  else if (sensefinger[0] == flute_a_five[0] && sensefinger[1] == flute_a_five[1] && sensefinger[2] == flute_a_five[2] &&
           sensefinger[3] == flute_a_five[3] && sensefinger[4] == flute_a_five[4] && sensefinger[5] == flute_a_five[5] &&
           sensefinger[6] == flute_a_five[6] && sensefinger[7] == flute_a_five[7] && sensefinger[8] == flute_a_five[8]) {pitch = flute_a_five_pitch;}   
// Bb5 //
  else if (sensefinger[0] == flute_bb_five[0] && sensefinger[1] == flute_bb_five[1] && sensefinger[2] == flute_bb_five[2] &&
           sensefinger[3] == flute_bb_five[3] && sensefinger[4] == flute_bb_five[4] && sensefinger[5] == flute_bb_five[5] &&
           sensefinger[6] == flute_bb_five[6] && sensefinger[7] == flute_bb_five[7] && sensefinger[8] == flute_bb_five[8]) {pitch = flute_bb_five_pitch;}
// B5 //
  else if (sensefinger[0] == flute_b_five[0] && sensefinger[1] == flute_b_five[1] && sensefinger[2] == flute_b_five[2] &&
           sensefinger[3] == flute_b_five[3] && sensefinger[4] == flute_b_five[4] && sensefinger[5] == flute_b_five[5] &&
           sensefinger[6] == flute_b_five[6] && sensefinger[7] == flute_b_five[7] && sensefinger[8] == flute_b_five[8]) {pitch = flute_b_five_pitch;}   
  else {pitch = 0;}    
}


///////////////////////////////////// MODE ONE (Learning Aid) ///////////////////////////////////////////////////////////


void mode_one() //Detects each button one at a time and determines if it has changed by comparing it against input_mem.
{
  for (int j = 0; j < 10; j++)
  {
    switch(j) 
    {
      case 1:
        break;
      default:
        if (!digitalRead(j) == 1) 
        {
          if (input_mem[j] == 0) 
          {
            MIDI.sendNoteOn(input_mempitch[j], velocity, channel);
            input_mem[j] = 1;
            ;
          }
          else if (input_mem[j] == 1)
          {
            ;
          }
        }
        else 
        {
          if (input_mem[j] == 1)
          {
            MIDI.sendNoteOff(input_mempitch[j], velocity, channel);
            input_mem[j] = 0;
            ;
          }
          else if (input_mem[j] == 0)
          {
            ;
          }
        }
    }
  }
}

///////////////////////////////////// NON-MODE FUNCTIONS /////////////////////////////////////////////////////////


void reset_vars() //If there is a change in the mode, this is called to make sure nothing affects the new mode or going back to the old mode.
{
  active_note = 0;
  pitch = 0;
  pitch_mem = 0;
  input_button = 0;
  button = 0;
  input_mem[0, 1, 2, 3, 4, 5, 6, 7, 8, 9] = 0;
}

void setup() //Enables midi abilities and sets up inputs as pullup. This way, buttons need only be connected from Pin to Ground.
{
  MIDI.begin(MIDI_CHANNEL_OMNI);
  pinMode (0, INPUT_PULLUP);      //thumb key
  pinMode (2, INPUT_PULLUP);      //index LH
  pinMode (3, INPUT_PULLUP);      //middle LH
  pinMode (4, INPUT_PULLUP);      //ring LH
  pinMode (5, INPUT_PULLUP);      //pinky LH
  pinMode (6, INPUT_PULLUP);      //index RH
  pinMode (7, INPUT_PULLUP);      //middle RH
  pinMode (8, INPUT_PULLUP);      //ring RH
  pinMode (9, INPUT_PULLUP);      //pinky RH
  pinMode (13, INPUT_PULLUP);     //checks the mode
  
}

void loop()   //Reads the mode input and determines if mode 0 or mode 1. If there is a change in made (detected by mode_mem), then reset_vars is triggered.
              //Repeats over and over until program is terminated by unplugging the Arduino board.
{
  mode = !digitalRead(13);
  if (mode == 0) 
  {
    if (mode != mode_mem) 
    {
      reset_vars();
      mode_mem = 0;
    }
    mode_zero();
  }
  else 
  {
    if (mode != mode_mem) 
    {
      reset_vars();
      mode_mem = 1;
    }
    mode_one();
    delay(delaytimer);
  }
}
