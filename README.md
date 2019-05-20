INPUT TO MIDI PROGRAM (ITMP)

Programming the board notes:
Arduino boards have 2 programming chips: one that has the main customizable program (BIG) and the other that dictates how the computer recognizes board (SMALL).
HIDUINO will be used to change the SMALL chip to allow computers to recognize the Arduino board as a MIDI device and not an Arduino board.**
Flashing an Arduino board will require a second Arduino to act as a Flasher.**

What is MIDI?
Musical Instrument Digital Interface is a type of message that allows hardware to play synths and sounds on a computer. Example is:


Note	Channel		Velocity	 Event
C5		   1		  127		Note On

Note: the pitch (may be represented by number; C5 = 72).
Channel: the MIDI pathway the note is going through (one channel per instrument, will likely stick with 1 for now).
Velocity:the dynamic. 0 is softest and 127 is the loudest.
Event: trigger for note on and off. Please take care that Note Off needs to be sent as an event to stop a note.

Board Features:
13 Digital I/O (advertises as 14, but one is only an output)
6 Analog I/O - can be used as digital if needed, or even multi-variable switches
Powered by USB cable, no need for external power supply

Program Features:
Will feature 2 modes:
Mode 0: MIDI messages are triggered by blowing into a microphone and the actual note is determined by the pattern of buttons pushed. Will simulate playing a live flute as much as possible and used for live performances
For example, C5 on the flute is played by blowing into the mic to trigger the Note On whilst pressing the Pin2 and Pin9. Stopping the air triggers the Note Off event.
Changing notes while blowing air will trigger a Note Off event for the first note as well as trigger a Note On event for the new note.
Mode 1: MIDI messages are triggered solely by button presses. Will not be used for live performance but instead used in external program to show which key presses are being pushed (see component 3)
For example, C5 would be triggered on pressing Pin2 and C#5 would be triggered on pressing Pin3 (but this doesn’t affect Pin2). Corresponding Note off event is triggered upon releasing a Pin.



AWESOME NOTES TO REMEMBER ABOUT HIDUINO

1) What you will need:
- Install WinAVR (best to do on c drive)
- Install Arduino IDE
- Download HIDUINO
- Purchase 2 arduino-compatible boards (One Uno, other can be Uno or Mega. Make sure they have Atmega16u2 or Atmega8u2 microchip as the USB controller).
- 

2) Set up Arduino Uno board as programmer:
- Connect Arduino Uno via USB
- Open Arduino IDE
- Open File > Examples > ArduinoISP
- Go to Tools > make sure Board is set to Arduino/Genuino Uno and Port is set as well. May need to check Device Manager to see port. Programmer should be set to Arduino as ISP.
- click on the '=>' button on the main window to upload sketch to programmer board.

3) Connect Programmer board to Target Board and flash the HIDUINO firmware:
- Using M-F jumpers, connect the pins from Programmer to ICSP headers on the Target USB controller. Note that the pins are close to the USB connector. Use the following diagrams on how to make the connections.

///PROGRAMMER////////       TARGET			/////Example ICSP Header/////
   Digital(10)		 		Reset			//////_________________//////
   Digital(11)		 		MOSI			/////|GRD	      RESET|/////
   Digital(12) 		 		MISO			/////|MOSI	        SCK|/////
   Digital(13)		 		SCK				/////|5V		   MISO|/////
   5V				 		5V				/////|_________________|/////
   GRD				 		GRD				////////////////////////o////

- Connect GRD and RESET of the Programmer to each other with a 10uF capacitor. Note the polarity of the capacitor (negative to ground).
- Connect Target board into computer via USB.
- Open up 'cmd' and enter the following code to flash HIDUINO firmware onto Atmega16u2. Best to make sure the 'arduino_midi.hex' file is on the C drive:
avrdude -p m16u2 -F -P "PORT YOU HAVE YOUR PROGRAMMER PLUGGED INTO" -c avrisp -U flash:w:arduino_midi.hex -U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xF4:m -U lock:w:0x0F:m -b 19200
- Once done, unplug and replug the Target back into the computer.

4) Connect programmer and target to allow manual flashing of the main microcontroller for sketches:
- Using M-M jumpers, connect the pins from the Programmer to the following pins of the Target.

///PROGRAMMER////////TARGET////////////
   Digital(10)			Reset - power rail
   Digital(11)			Digital(11)
   Digital(12)			Digital(12)
   Digital(13)			Digital(13)
   5V					5V    - power rail
   GRD			   		GRD   - power rail

- Connect GRD and RESET of the Programmer to each other with a 10uF capacitor. Note the polarity of the capacitor (negative to ground).
- In the Arduino IDE, set the Programmer to 'Arduino as ISP'. When flashing sketches, make sure to hold shift and then click the '=>' to use the programmer. 
- You are now able to upload custom sketches! :D
