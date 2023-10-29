#define FLOOR_0_LED 10
#define FLOOR_1_LED 11
#define FLOOR_2_LED 12

#define FLOOR_0_INPUT 5
#define FLOOR_1_INPUT 6
#define FLOOR_2_INPUT 7

#define DEBOUNCE_TIME 50

unsigned long g_inputTime0 = 0, g_previousInputTime0 = 0;
bool g_inputState0 = LOW, g_previousInputState0 = LOW;
bool g_previousCountedState0 = HIGH;
unsigned long g_presses0 = 0;

unsigned long g_inputTime1 = 0, g_previousInputTime1 = 0;
bool g_inputState1 = LOW, g_previousInputState1 = LOW;
bool g_previousCountedState1 = HIGH;
unsigned long g_presses1 = 0;

unsigned long g_inputTime2 = 0, g_previousInputTime2 = 0;
bool g_inputState2 = LOW, g_previousInputState2 = LOW;
bool g_previousCountedState2 = HIGH;
unsigned long g_presses2 = 0;

void testInputOutput();

void setup()
{
	Serial.begin(115200);

	pinMode(FLOOR_0_LED, OUTPUT);
	pinMode(FLOOR_1_LED, OUTPUT);
	pinMode(FLOOR_2_LED, OUTPUT);

	pinMode(FLOOR_0_INPUT, INPUT_PULLUP);
	pinMode(FLOOR_1_INPUT, INPUT_PULLUP);
	pinMode(FLOOR_2_INPUT, INPUT_PULLUP);
}

void debounce(const int p_input)
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

			presses = &g_presses0;

			break;

		case(FLOOR_1_INPUT):
			auxLed = FLOOR_1_LED;

			time = &g_inputTime1;
			previousTime = &g_previousInputTime1;

			state = &g_inputState1;
			previousState = &g_previousInputState1;
			previousCountedState = &g_previousCountedState1;

			presses = &g_presses1;

			break;

		case(FLOOR_2_INPUT):
			auxLed = FLOOR_2_LED;

			time = &g_inputTime2;
			previousTime = &g_previousInputTime2;

			state = &g_inputState2;
			previousState = &g_previousInputState2;
			previousCountedState = &g_previousCountedState2;

			presses = &g_presses2;

			break;

		default:
			return;
	}
	const int led = auxLed; 

	*time = millis();
	*state = digitalRead(p_input);

	// if the button has a constant state
	if(*state == *previousState)
	{
		// if the constant state has been kept for a while
		if(*time - *previousTime > DEBOUNCE_TIME && *previousCountedState != *state)
		{
			digitalWrite(led, *state);
			*previousCountedState = *state;

			if(*state == HIGH)
			{
				*presses = *presses + 1;
				Serial.print("Times pressed input ");
				Serial.print(p_input - FLOOR_0_INPUT);
				Serial.print(": ");
				Serial.println(*presses);
			}
		}
	}
	else
	{
		*previousTime = *time;
	}

	*previousState = *state;
}

void loop()
{
	debounce(FLOOR_0_INPUT);
	debounce(FLOOR_1_INPUT);
	debounce(FLOOR_2_INPUT);
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
