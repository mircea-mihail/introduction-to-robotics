# Introduction to Robotics (2023 - 2024)
This repo is designed to track my progress through the homework of the Introdution to Robotics Uni course. 

This is a mandatory course taken in the 3rd year of my scholarship at the Faculty of Mathematics and Computer Science, University of Bucharest.  

Throughout this repo you can find homework-specific requirements, implementation details, code, image files and more.


## Homework 1

Create this repo, doesn't get much easier than that.

## Homework 2

<details>
<summary> <B>Control RGB LED</B> </summary>

### Task 

Use a separate potentiometer for controlling each color of an RGB LED: Red,
Green, and Blue.

The purpose of this homework is to leverage digital electronics.
Although this could be done by using the potentiometer's output as an input for each
LED color, it is important to understand the use of an analog input and PWM output
via the Arduino to be able to control a variety of electronics using the MCU.

Specifically, for this task you need to read the potentiometer’s value 
using an Arduino and then write a mapped value of that to the LED's input pins.

### Hardware Implementation

#### Components:
* an Arduino UNO
* the common Cathode RGB LED
* three 50KΩ potentiometer
* four 100Ω resistors. 
The Blue and Green LEDs needed 90Ω resistors to funciton so I used one 100Ω resistor for each
and for the Red LED I used two 100Ω resistors in series to get a 200Ω resistence, as the required resistence for the Red LED was 150Ω.
* multi-coloured wires, properly colour-coded  

Below is a youtube link that shows the homework in action and there is also a picture of the project.

https://www.youtube.com/shorts/vDVLX95Q7Ww 

<img src="media/Hw2-project.jpg" alt="drawing" height="300"/>

</details>

## Homework 3

<details>
<summary> <B> Elevator Simulator </B></summary>

### Task 

Design a control system that simulates a 3-floor elevator using the Arduino
platform. Here are the specific requirements:

* LED Indicators: Each of the 3 LEDs should represent one of the 3 floors.
The LED corresponding to the current floor should light up. Additionally,
another LED should represent the elevator’s operational state. It should
blink when the elevator is moving and remain static when stationary.

* Buzzer: The buzzer should sound briefly during the following scenarios:
  * Elevator arriving at the desired floor (something resembling a ”cling”). I ended
    up playing a melody when opening and closing the door.
  * Elevator movement.

* State Change & Timers: If the elevator is already at the desired floor,
pressing the button for that floor should have no effect. Otherwise, after
a button press, the elevator should ”wait for the doors to close” and then
”move” to the corresponding floor. If the elevator is in movement, it
should either do nothing or it should stack its decision (get to the first
programmed floor, open the doors, wait, close them and then go to the
next desired floor).

* Debounce: Remember to implement debounce for the buttons to avoid
unintentional repeated button presses.

### Hardware Implementation

I ended up using:
* 3 Red LEDs
* 1 Green LED
* 4 330Ω Resistors for the LEDs
* 1 100Ω Resistor for the buzzer
* a buzzer
* three push buttons
* wires
* an Arduino Uno

Below is a youtube link that shows the homework in action and there is also a picture of the project.

https://www.youtube.com/watch?v=6XHYGthXycI

<img src="media/Hw3-project.jpg" alt="drawing" height="300"/>
<details>
