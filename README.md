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

<img src="media/Hw2-project.jpg" alt="Hw2-project" height="300"/>

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

<img src="media/Hw3-project.jpg" alt="Hw3-project" height="300"/>
</details>

## Homework 4

<details>
<summary> <B> Draw On 7 Segment </B></summary>

### Task 

Use the joystick to control the position of the segment cursor and ”draw” on the display
by pressing on the joystick button. The movement between segments should be natural. 
The cursor is only allowed to jump from the current position to neighboring ones, 
without passing through walls or jumping over neighbors.

In order to make it more obvious what LED the cursor is on, what I did is color the LED instantly with
the oposite color it already has (if it's black i make the blink start from red and vice versa)

#### Behaviour

The initial position should be on the DP (decimal point). The current cursor
position always blinks (irrespective of the fact that the segment is on or
off). Use the joystick to move from one position to neighboring ones. (see table for
corresponding movement). Short pressing the button toggles the segment
state from ON to OFF or from OFF to ON. Long pressing the button
resets the entire display by turning all the segments OFF and moving the
current position to the decimal point. Interrupts are required.

### Hardware Implementation

I used:
* 8 330Ω Resistors for the segments
* one 7 segment display
* wires
* an Arduino Uno
* a Joystick

Below is a youtube link that shows the homework in action and there is also a picture of the project.

https://www.youtube.com/watch?v=VlsCClYC-cU

<img src="media/Hw4-project.jpg" alt="Hw4-project" height="300"/>

</details>

## Homework 5

<details>
<summary> <B> Basic Timer With Laps </B></summary>

### Task 

Using the 4-digit 7-segment display and 3 buttons, you should implement a
stopwatch timer that counts in 10ths of a second and has a save lap functionality
(similar to most basic stopwatch functions on most phones).

#### Behaviour

1. The beginning state of the stopwatch is ”000.0”. 
When pressing the Start button, the timer should start.

2. While the timer is running, laps should be stored in memory, 
(up to 4 laps). pressing the lap button for the 5th time 
should override the 1st saved one. If you press the reset
button while timer is running, nothing happens. If you press the pause
button, the timer stops.

3. In Pause Mode, the lap button doesn’t work anymore. Pressing
the reset button resets you to 000.0.

4. After reset you can press the lap button to cycle through the
lap times. Each time you press the button, it takes you to the
next saved lap. Pressing it continuously should cycle you through it
continuously. Pressing the reset button while in this state resets all
your flags and takes the timer back to ”000.0”.

5. My extra input: I only display the saved laps. If the user only saves 
three laps, I don't display the 4th "empty" one as 000.0. Instead i cycle to 
the first lap. I also mate it impossible to reset the laps without viewing them first
to prevent accidental data loss.
### Hardware Implementation

I used:
* 8 330Ω Resistors for the segments
* one 4 digit 7 segment display
* wires
* an Arduino Uno
* 3 buttons
* a shift register

Below is a youtube link that shows the homework in action and there is also a picture of the project.

https://www.youtube.com/watch?v=ulKlsE1wnI8

<img src="media/Hw5-project.jpg" alt="Hw5-project" height="300"/>

</details>

## Homework 6

<details>
<summary> <B> Environment monitor </B></summary>

### Writer's notes

This homework is partially at fault for some of my early male pattern baldness. A lot of code to write, 
borderline useless final product, no inspiration in sight to programme it. 800 lines of the most boaring repetitive code. I'm glad this one's done. Probably the worst homework this cours had to offer.

### Task 

Develop a ”Smart Environment Monitor and Logger” using Arduino. This system will utilize various sensors to gather environmental data, log this data into EEPROM, and provide both visual 
feedback via an RGB LED and user interaction through a Serial Menu. The project focuses on integrating sensor readings,
memory management, Serial Communication and the general objective of building a menu.

#### Menu Structure

1. Sensor Settings 
  * Sensors Sampling Interval. Here you should be prompted for
a value between 1 and 10 seconds. Use this value as a sampling rate
for the sensors. You can read a separate value for each or have the
same for both.
  * Ultrasonic Alert Threshold. Here you should be prompted
for a threshold value for the ultrasonic sensor. You can decide if that
is the min or max value (you can signal that something is too close).
When sensor value exceeds the threshold value, an alert should be
given. This can be in the form of a message. If the LED is set to
Automatic Mode (see section 4.2), it should also turn red if any of
the sensors are outside the value.
  * LDR Alert Threshold. Here you should be prompted for a
threshold value for the LDR sensor. You can decide if that is the
min or max value (for example, it could signal that night is coming).
When sensor value exceeds the threshold value, an alert should be
given. This can be in the form of a message. If the LED is set to
Automatic Mode (see section 4.2), it should also turn red if any of
the sensors are outside the value.
  * Back // Return to main menu

2. Reset Logger Data. Should print a message, promting if you to
confirm to delete all data. Something like ”are you sure?”, followed by
the submenu with YES or NO. You can reset both sensor data at the same
time, or you can do it individually. Your choice. I did it individually.
  * Yes.
  * No.

3. System Status // Check current status and health
  * Current Sensor Readings. Continuously print sensor readings
at the set sampling rate, from all sensors. Make sure you have a way
to exit this (such as pressing a specific key) and inform the user of
this method through a message.
  * Current Sensor Settings. Displays the sampling rate and
threshold value for all sensors.
  * Back. Return to Main menu.

4. RGB LED Control // Go to submenu
  * Manual Color Control. Set the RGB colors manually. You
decide how to input them, either by making an option for each chan-
nel, or by putting a string etc. If you expect a specific format, make
sure to inform the user.
  * LED: Toggle Automatic ON/OFF. If automatic mode is
ON, then the led color should be GREEN when all sensors value do
not exceed threshold values (aka no alert) and RED when there is an
alert (aka ANY sensor value exceeds the threshold). When automatic
mode is OFF, then the LED should use the last saved RGB values.
  * Back // Return to main men

### Hardware Implementation

I used:
* 3 330Ω Resistors for the segments
* one ultrasonic sensor
* one photoresistor
* an Arduino Uno
* an RGB LED

Below is a youtube link that shows the homework in action and there is also a picture of the project.

https://www.youtube.com/watch?v=DZubaUXpZoM

<img src="media/Hw6-project.jpg" alt="Hw6-project" height="300"/>

</details>

## Homework 7

<details>
<summary> <B> Cowboy Rocket - an 8x8 matrix game </B></summary>

### About the game

This is a top down terminator-tanks style game in which you have to distroy all walls using your trusty gun and... ROCKETS (if you find the power up)

### Task 

Develop a small game on an 8x8 matrix. The game must have at least 3 types
of elements: player (blinks slowly), bombs/bullets (blink fast), wall(doesn’t
blink). The purpose of this to get you started with your matrix project. The
basic idea is that you generate walls on the map (50% - 75% of the map) and then
you move around with the player and destroy them. You can do it Bomberman
style or terminator-tanks style (or another way, be creative).

### My take

While implementing the required task I also went ahead and implemented most bonusses 
mentioned in the task PDF.
  * Animations: there is a start game and a winning annimation
  * Bigger map: implemented a 16x16 logical matrix you walk around with a panning camera above the player
  * Advanced game: added a rocket power up that enables exploding bullets or rockets you are immune to ( but the walls are not )
  * Extra button: using the joystick to shoot is not fun so I had to add an extra button for that
  * Extra sensors: added a brightness sensor that adjusts the brightness of the LED matrix based on outside light
  * Menu structure: everything is organised on game states that switch between in-animation, in-game, won
  * Code structure: most of the code is organised in classes and separate files to make everything modular, easy to read and use
  * Score: the player that destroys all of the walls in the least amount of thime gets a place in the top 3 players in EEPROM

### Hardware Implementation

I used:
* a MAX7219 serial input/parallel output IC
* resistors and capacitors as needed
* a button
* a light sensor
* one 8x8 LED matrix
* an Arduino Uno
* a Joystick
* wires
* a breadboard

Below is a youtube link that shows the homework in action and there are also a couple of pictures of the project.

https://www.youtube.com/watch?v=_4DbtM731EU&t=145s

<img src="media/Hw7-project.jpg" alt="Hw7-project" height="300"/>
<img src="media/Hw7-project-1.jpg" alt="Hw7-project-1" height="300"/>

</details>


## Homework 7.5

<details>
<summary> <B> Cowboy Rocket - LCD Checkpoint </B></summary>

### About the game

As I said in the previous description, this is a top down terminator-tanks style game in which you have to distroy all 
walls using your trusty gun and... ROCKETS (if you find the power up). When it comes to the game I didn't do anything 
extra, but for this homework I did implement an LCD Screen to display a menu before and after each game and to inform the 
player during gameplay how many walls are left to destroy on the map.

Another notable thing is that I ended up fitting the whole project on a single breadboard which is pretty cool. Also, there
are no Digital pins left free, other than pin 0 and pin 1 (used for serial communication with the computer) and three analog pins.

### Task 

#### Add the following functionality for this checkpoint:
  1. Intro Message - When powering up the project, a greeting message should be shown for a few moments.
  2. Menu
    * Start game, starts the initial level of the game.
    * Settings
      * LCD brightness control. Save value to EEPROM and load it upon system start.
      * Matrix brightness control (see function setIntesnity from the led-Control library). Make sure to display 
      something on the matrix when selecting it. Save value to EEPROM and load it upon system start.
      * LCD contrast control. This wasn't here initially but I ended up implementing it anyway as it felt important
    * About: should include details about the creator(s) of the game. At least game name, author and github link or
    user (used the joystick to scroll left and right)
  3. End Message - When the game ends, add a message. Wait for a promt from the user (a button push, for example), 
  before returning to main menu again.
  4. During gameplay:
    * Show relevant details, that are dynamic (change with gameplay): time, level, lives etc. Doesn’t matter, what can be used. 
    You can implement the number of LEDs that are still switched on.
    * Implement an end game / level functionality. You current game should end. For example, if you implemented the game where you
    ”destroy” all the LEDs, it should end when you finished them all. Upon ending, it should display the aforementioned message, wait for
    user prompt and then return to the main menu again.

#### Notable mentitons

Other than implementing everything mentioned in the task, I also went ahead and did some extra things that felt necessary for my project:
* connected the shift register to the matrix fitting wires under the matrix in order to have enough room to keep everything on one breadboard
* added a matrix icon for every main menu option 
* made all animations skippable
* made boot/end-game messages skippable 
* fixed animation bug and refactored the animation related methods to not rely on the map (display directly on the LED matrix)
* drew and displayed two custom characters for brightness and contrast

### Hardware Implementation

Previously used hardware:
* a MAX7219 serial input/parallel output IC
* resistors and capacitors as needed
* a button
* a light sensor
* one 8x8 LED matrix
* an Arduino Uno
* a Joystick
* wires
* a breadboard

Hardware added for this checkpoint:
* 16x2 LCD display

Below is a youtube link that shows the homework in action and there is also some pictures of the project.

https://www.youtube.com/watch?v=iEq4sabx84g

<img src="media/Hw7-5-project.jpg" alt="Hw7-project" height="300"/>
<img src="media/Hw7-5-1-project.jpg" alt="Hw7-5-1-project" height="300"/>
<img src="media/Hw7-5-2-project.jpg" alt="Hw7-5-2-project" height="300"/>
<img src="media/Hw7-5-3-project.jpg" alt="Hw7-5-3-project" width="675"/>

</details>
