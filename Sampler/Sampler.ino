#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

int velocity = 64;
int channel = 1;
int inputMemory[] = {0, 0, 0, 0};
int inputMemoryPitch[10] = {83, 84, 85, 86};

void setup() {
  pinMode (2, INPUT_PULLUP);
  pinMode (3, INPUT_PULLUP);
  pinMode (4, INPUT_PULLUP);
  pinMode (5, INPUT_PULLUP);
}

void loop() {
  for (int j = 2; j < 6; j++){
    if (!digitalRead(j) == 1){
      if (inputMemory[j] == 0){
        MIDI.sendNoteOn(inputMemoryPitch[j], velocity, channel);
        inputMemory[j] = 1;
      }
    }
    else{
      if (inputMemory[j] == 1){
        MIDI.sendNoteOff(inputMemoryPitch[j], velocity, channel);
        inputMemory[j] = 0;
      }
    }
  }
}
