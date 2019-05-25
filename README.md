****INPUT TO MIDI PROGRAM (ITMP)****

Programming the board notes:

 Arduino boards have 2 programming chips: one that has the main customizable program (BIG) and the other that dictates how the computer recognizes board (SMALL).

 HIDUINO will be used to change the SMALL chip to allow computers to recognize the Arduino board as a MIDI device and not an Arduino board.

 Flashing an Arduino board will require a second Arduino to act as a Flasher.

What is MIDI?

Musical Instrument Digital Interface is a type of message that allows hardware to play synths and sounds on a computer. Example is:


*Note*	*Channel*	*Velocity*	 *Event*

C5		   1		  127		Note On

Note: the pitch (may be represented by number; C5 = 72).

Channel: the MIDI pathway the note is going through (one channel per instrument, will likely stick with 1 for now).

Velocity:the dynamic. 0 is softest and 127 is the loudest.

Event: trigger for note on and off. Please take care that Note Off needs to be sent as an event to stop a note.

Board Features:

- 13 Digital I/O (advertises as 14, but one is only an output)
- 6 Analog I/O - can be used as digital if needed, or even multi-variable switches
- Powered by USB cable, no need for external power supply

Will feature 2 modes:

Mode 0: MIDI messages are triggered by blowing into a microphone and the actual note is determined by the pattern of buttons pushed. Will simulate playing a live flute as much as possible and used for live performances
For example, C5 on the flute is played by blowing into the mic to trigger the Note On whilst pressing the Pin2 and Pin9. Stopping the air triggers the Note Off event.
Changing notes while blowing air will trigger a Note Off event for the first note as well as trigger a Note On event for the new note.

Mode 1: MIDI messages are triggered solely by button presses. Will not be used for live performance but instead used in external program to show which key presses are being pushed (see component 3)
For example, C5 would be triggered on pressing Pin2 and C#5 would be triggered on pressing Pin3 (but this doesn’t affect Pin2). Corresponding Note off event is triggered upon releasing a Pin.