// HEADS UP! g_something is global p_something is parameter

#define FLOOR_0_LED 10
#define FLOOR_1_LED 11
#define FLOOR_2_LED 12
#define FLOOR_CHANGE_LED 13

#define FLOOR_0_INPUT 5
#define FLOOR_1_INPUT 6
#define FLOOR_2_INPUT 7

#define A4 440
#define Cs5 554
#define E5 659
#define ELEVATOR_IS_MOVING 329

#define BUZZER 3

#define DEBOUNCE_TIME 50 // ms
#define TIME_BETWEEN_LED_BLINKS 200

#define TIME_SPENT_ON_FLOOR 1000
#define TIME_SPENT_BETWEEN_FLOORS 2000
 
unsigned long g_inputTime0 = 0, g_previousInputTime0 = 0;
bool g_inputState0 = LOW, g_previousInputState0 = LOW;
bool g_previousCountedState0 = HIGH;

unsigned long g_inputTime1 = 0, g_previousInputTime1 = 0;
bool g_inputState1 = LOW, g_previousInputState1 = LOW;
bool g_previousCountedState1 = HIGH;

unsigned long g_inputTime2 = 0, g_previousInputTime2 = 0;
bool g_inputState2 = LOW, g_previousInputState2 = LOW;
bool g_previousCountedState2 = HIGH;

unsigned short g_floor = 0;
unsigned short g_nextFloor = -1;

unsigned long g_momentOfBlink = 0;
bool g_blinkState = HIGH;

void testInputOutput();
bool debouncedPressDetected(const int p_input);

void setup()
{
	Serial.begin(115200);

	pinMode(FLOOR_0_LED, OUTPUT);
	pinMode(FLOOR_1_LED, OUTPUT);
	pinMode(FLOOR_2_LED, OUTPUT);
	pinMode(FLOOR_CHANGE_LED, OUTPUT);
	pinMode(BUZZER, OUTPUT);


	pinMode(FLOOR_0_INPUT, INPUT_PULLUP);
	pinMode(FLOOR_1_INPUT, INPUT_PULLUP);
	pinMode(FLOOR_2_INPUT, INPUT_PULLUP);

	// the lift starts at floor 0
	digitalWrite(FLOOR_0_LED, HIGH);
	digitalWrite(FLOOR_CHANGE_LED, HIGH);

}

void playMelody()
{
	unsigned long currentTime;

	tone(BUZZER, A4, TIME_SPENT_ON_FLOOR/3);
	currentTime = millis();
	while(millis() - currentTime < TIME_SPENT_ON_FLOOR/3)
	{
		checkForNextFloor();
	}

	tone(BUZZER, Cs5, TIME_SPENT_ON_FLOOR/3);
	currentTime = millis();
	while(millis() - currentTime < TIME_SPENT_ON_FLOOR/3)
	{
		checkForNextFloor();
	}

	tone(BUZZER, E5, TIME_SPENT_ON_FLOOR/3);
	currentTime = millis();
	while(millis() - currentTime < TIME_SPENT_ON_FLOOR/3)
	{
		checkForNextFloor();
	}}

void playMelodyReverse()
{
	unsigned long currentTime;

	tone(BUZZER, E5, TIME_SPENT_ON_FLOOR/3);
	currentTime = millis();
	while(millis() - currentTime < TIME_SPENT_ON_FLOOR/3)
	{
		checkForNextFloor();
	}

	tone(BUZZER, Cs5, TIME_SPENT_ON_FLOOR/3);
	currentTime = millis();
	while(millis() - currentTime < TIME_SPENT_ON_FLOOR/3)
	{
		checkForNextFloor();
	}
	
	tone(BUZZER, A4, TIME_SPENT_ON_FLOOR/3);
	currentTime = millis();
	while(millis() - currentTime < TIME_SPENT_ON_FLOOR/3)
	{
		checkForNextFloor();
	}
}


void blinkFloorChangeLed()
{
	if(millis() - g_momentOfBlink > TIME_BETWEEN_LED_BLINKS)
	{
		g_momentOfBlink = millis();
		g_blinkState = !g_blinkState;
		digitalWrite(FLOOR_CHANGE_LED, g_blinkState);
	}	
}	

void checkForNextFloor()
{
	if(debouncedPressDetected(FLOOR_0_INPUT))
	{
		g_nextFloor = FLOOR_0_INPUT - FLOOR_0_INPUT;
	}

	if(debouncedPressDetected(FLOOR_1_INPUT))
	{
		g_nextFloor = FLOOR_1_INPUT - FLOOR_0_INPUT;
	}

	if(debouncedPressDetected(FLOOR_2_INPUT))
	{
		g_nextFloor = FLOOR_2_INPUT - FLOOR_0_INPUT;
	}
}

void goToFloor(unsigned short p_newFloor)
{
	if(p_newFloor == g_floor)
	{
		return;
	}
	// if the elevator is descendin, the direction is -1
	int elevatorDirection;
	elevatorDirection = (g_floor > p_newFloor) ? -1 : 1;

	digitalWrite(FLOOR_CHANGE_LED, HIGH);

	// generates every floor from the current floor to the new floor: 0 1 2 (from 0 to 2)
	for(int i = g_floor; i != p_newFloor; i += elevatorDirection)
	{
		Serial.println(i);

		digitalWrite(FLOOR_0_LED + i, HIGH);
		unsigned long currentTimeOnFloor = millis();
		
		if(i == g_floor)
		{
			digitalWrite(FLOOR_CHANGE_LED, HIGH);
			playMelodyReverse();
			Serial.print("staying on floor\n");
		}
		else
		{
			while(millis() - currentTimeOnFloor < TIME_SPENT_ON_FLOOR)
			{	
				blinkFloorChangeLed();
				tone(BUZZER, ELEVATOR_IS_MOVING);
				checkForNextFloor();
			}
		}
		
		if(i != p_newFloor)
		{
			digitalWrite(FLOOR_0_LED + i, LOW);
		}

		unsigned long currentTimeBetweenFloors = millis();
		while(millis() - currentTimeOnFloor < TIME_SPENT_BETWEEN_FLOORS)
		{
			blinkFloorChangeLed();
			tone(BUZZER, ELEVATOR_IS_MOVING);
			checkForNextFloor();
		}
	}

	digitalWrite(FLOOR_0_LED + p_newFloor, HIGH);
	g_floor = p_newFloor;
	
	g_blinkState = HIGH;
	digitalWrite(FLOOR_CHANGE_LED, g_blinkState);
	
	playMelody();

	Serial.print("arrived at ");
	Serial.println(g_floor);

	if(g_nextFloor != -1)
	{
		goToFloor(g_nextFloor);
		g_nextFloor = -1;
	}
}


void loop()
{
	if(debouncedPressDetected(FLOOR_0_INPUT))
	{
		goToFloor(FLOOR_0_INPUT - FLOOR_0_INPUT);
	}

	if(debouncedPressDetected(FLOOR_1_INPUT))
	{
		goToFloor(FLOOR_1_INPUT - FLOOR_0_INPUT);
	}

	if(debouncedPressDetected(FLOOR_2_INPUT))
	{
		goToFloor(FLOOR_2_INPUT - FLOOR_0_INPUT);
	}
}

bool debouncedPressDetected(const int p_input)
{
	int auxLed;
	unsigned long *time, *previousTime;
	bool *state, *previousState, *previousCountedState;
	unsigned long *presses;

	// what input do you want to debounce for?
	switch(p_input)
	{
		case(FLOOR_0_INPUT):
			auxLed = FLOOR_0_LED;

			time = &g_inputTime0;
			previousTime = &g_previousInputTime0;

			state = &g_inputState0;
			previousState = &g_previousInputState0;
			previousCountedState = &g_previousCountedState0;

			break;

		case(FLOOR_1_INPUT):
			auxLed = FLOOR_1_LED;

			time = &g_inputTime1;
			previousTime = &g_previousInputTime1;

			state = &g_inputState1;
			previousState = &g_previousInputState1;
			previousCountedState = &g_previousCountedState1;

			break;

		case(FLOOR_2_INPUT):
			auxLed = FLOOR_2_LED;

			time = &g_inputTime2;
			previousTime = &g_previousInputTime2;

			state = &g_inputState2;
			previousState = &g_previousInputState2;
			previousCountedState = &g_previousCountedState2;

			break;

		default:
			return false;
	}
	// LED pin needs to be const int
	const int led = auxLed; 

	*time = millis();
	// more intuitive to be 0 when not pressed and 1 when pressed
	*state = !digitalRead(p_input);

	// if the button has a constant state
	if(*state == *previousState)
	{
		// if the constant state has been kept for a while
		if(*time - *previousTime > DEBOUNCE_TIME && *previousCountedState != *state)
		{
			*previousCountedState = *state;

			//only count presses
			if(*state == HIGH)
			{
				*previousState = *state;
				return true;
			}
		}
	}
	else
	{
		*previousTime = *time;
	}

	*previousState = *state;
	
	return false;
}

void testInputOutput()
{
	bool floor0Val = digitalRead(FLOOR_0_INPUT);
	bool floor1Val = digitalRead(FLOOR_1_INPUT);
	bool floor2Val = digitalRead(FLOOR_2_INPUT);

	digitalWrite(FLOOR_0_LED, floor0Val);
	digitalWrite(FLOOR_1_LED, floor1Val);
	digitalWrite(FLOOR_2_LED, floor2Val);

	Serial.print("floor 0: ");
	Serial.println(floor0Val);
	Serial.print("floor 1: ");
	Serial.println(floor1Val);
	Serial.print("floor 2: ");
	Serial.println(floor2Val);
	Serial.print("\n\n");
}


// debug
	// Serial.print("pin: "); Serial.print(led);
	// Serial.print(", button: "); Serial.println(p_input);
	// Serial.print("time: "); Serial.print(*time);
	// Serial.print(", previous time: "); Serial.println(*previousTime);
	// Serial.print("state: "); Serial.print(*state);
	// Serial.print(", previous state: "); Serial.println(*previousState);
	// Serial.println();
