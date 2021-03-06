AWESOME NOTES TO REMEMBER ABOUT HIDUINO

**Diagrams were written in Notepad++ so formatting may appear odd on website**

1) What you will need:
- Install WinAVR (best to do on c drive)
- Install Arduino IDE
- Download HIDUINO
- Have 2 arduino-compatible boards (One Uno, other can be Uno or Mega. Make sure they have Atmega16u2 or Atmega8u2 microchip as the USB controller).

2) Set up Arduino Uno board as programmer:
- Connect Arduino Uno via USB
- Open Arduino IDE
- Open File > Examples > ArduinoISP
- Go to Tools > make sure Board is set to Arduino/Genuino Uno and Port is set as well. May need to check Device Manager to see port. Programmer should be set to Arduino as ISP.
- click on the '=>' button on the main window to upload sketch to programmer board.

3) Connect Programmer board to Target Board and flash the HIDUINO firmware:
- Using M-F jumpers, connect the pins from Programmer to ICSP headers on the Target USB controller. Note that the pins are close to the USB connector. Use the following diagrams on how to make the connections.

   **PROGRAMMER**			**TARGET**		/////Example ICSP Header/////

   Digital(10)				Reset			//////_________________//////
   
   Digital(11)				MOSI			/////|GRD	      RESET|/////
   
   Digital(12)				MISO			/////|MOSI	        SCK|/////
   
   Digital(13)				SCK_			/////|5V		   MISO|/////
   
   5Volts					5V__			/////|_________________|/////
   
   Ground					GRD_			////////////////////////o////

- Connect GRD and RESET of the Programmer to each other with a 10uF capacitor. Note the polarity of the capacitor (negative to ground).
- Connect Target board into computer via USB.
- Open up 'cmd' and enter the following code to flash HIDUINO firmware onto Atmega16u2. Best to make sure the 'arduino_midi.hex' file is on the C drive:
avrdude -p m16u2 -F -P "PORT YOU HAVE YOUR PROGRAMMER PLUGGED INTO" -c avrisp -U flash:w:arduino_midi.hex -U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xF4:m -U lock:w:0x0F:m -b 19200
- Once done, unplug and replug the Target back into the computer.

4) Connect programmer and target to allow manual flashing of the main microcontroller for sketches:
- Using M-M jumpers, connect the pins from the Programmer to the following pins of the Target.

///PROGRAMMER////////////////TARGET////////////
   Digital(10)			Reset - power rail
   Digital(11)			Digital(11)
   Digital(12)			Digital(12)
   Digital(13)			Digital(13)
   5Volts				5V    - power rail
   Ground		   		GRD   - power rail

- Connect GRD and RESET of the Programmer to each other with a 10uF capacitor. Note the polarity of the capacitor (negative to ground).
- In the Arduino IDE, set the Programmer to 'Arduino as ISP'. When flashing sketches, make sure to hold shift and then click the '=>' to use the programmer. 

5) You are now able to upload custom sketches! :D