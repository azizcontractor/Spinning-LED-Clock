# Spinning-LED-Clock
## Goal
<p>The goal of this project is to produce a static two dimensional image by spinning a one dimensional row of LEDs by utilizing a concept in the psychophysics of vision called flicker fusion threshold. This concept is defined as the frequency at which an intermittent light stimulus appears to be completely steady to the average human. As long as the modulation frequency is kept above this threshold, the human eye does not notice the flicker in light. The spinning motor provides a frequency above the flicker fusion threshold of the average human and a PIC microcontroller can be used to precisely time the on and off sequence of the LEDs to produce the static image. The final goal of this project is to use the built-in real-time clock and the timers in the PIC to show the current time using the spinning LED display. </p>

## The Setup
-A set of 7 vertically alligned LEDs spinning clockwise on a CPU fan motor that has a max speed of 3600 RPM<br/>
-A PIC24FV32KA302 microcontroller to control the timing of the LEDs turning on and off<br/>
-A TRCT5000 optical sensor that has a range of 2.5 mm<br/>
-Two PCBs to solder the chip, LEDs and the wiring<br/>
-A 3.7V 800mAh battery to power the PIC<br/>
-Another PIC24FV32KA302 microcontroller to manage the speed of the motor via PWM (Pulse-Width Modulation)<br/>
-A PICKit 3 to program the microcontrollers<br/>

## The Theory (Simpler)
<p>If a light is blinking at a frequency higher than the flicker fusion threshold of the human eye it appears static and we cannot see the flickering. If we can spin the motor and use our microcontroller to turn on the LED in the exact spot for every rotation it will appear as a constant dot. We can use these dots to display various characters my lighting different LEDs at different times during the rotation cycle. Use this to show the current time (currently running from LPRC will be switch to 50Hz PWM in the future)</p>

## The Process
-Solder PIC to PCB and wire its power supply<br/>
-Wire the PGED,PGEC,MCLR,VDD,and VSS pins to a header that will be used to connect to the PICKit 3<br/>
-Wire LEDs to a smaller PCB so that they share the ground but are powered seperately and screw it in to the PCB <br/>
-Connect LEDs to pins 15 (bottom LED) to 9 (top LED) in order (use 220 Ohm resistors)<br/>
-Place PCB on cardboard or plastic (better because can be drilled and screwed in) material<br/>
-Use double-sided tape to attach bottom to motor and batteries on each end (because they balance each other so no vibrations)<br/>
-Use double-sided tape to attach motor to base<br/>
-Go and read lots of datasheets, reference manuals and examples on Microchip's website<br/>
-Scratch head because you still dont get it<br/>
-Use code example provided here and modify text
-VOILA!! It works!!
