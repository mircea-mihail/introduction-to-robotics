#define RED_LED_CONTROL A2
#define GREEN_LED_CONTROL A1
#define BLUE_LED_CONTROL A0

#define RED_LED 9
#define GREEN_LED 10
#define BLUE_LED 11

#define MIN_ANALOG_INPUT_VAL 0
#define MIN_PWM_OUTPUT_VAL 0

#define MAX_ANALOG_INPUT_VAL 1023
#define MAX_PWM_OUTPUT_VAL 255

void setup()
{
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  pinMode(RED_LED_CONTROL, INPUT);
  pinMode(BLUE_LED_CONTROL, INPUT);
  pinMode(GREEN_LED_CONTROL, INPUT);
}

void loop()
{
  int nRedControlValue = analogRead(RED_LED_CONTROL);
  int nGreenControlValue = analogRead(GREEN_LED_CONTROL);
  int nBlueControlValue = analogRead(BLUE_LED_CONTROL);
  
  nRedControlValue = map(nRedControlValue, MIN_ANALOG_INPUT_VAL, MAX_ANALOG_INPUT_VAL, MIN_PWM_OUTPUT_VAL, MAX_PWM_OUTPUT_VAL);
  nGreenControlValue = map(nGreenControlValue, MIN_ANALOG_INPUT_VAL, MAX_ANALOG_INPUT_VAL, MIN_PWM_OUTPUT_VAL, MAX_PWM_OUTPUT_VAL);
  nBlueControlValue = map(nBlueControlValue, MIN_ANALOG_INPUT_VAL, MAX_ANALOG_INPUT_VAL, MIN_PWM_OUTPUT_VAL, MAX_PWM_OUTPUT_VAL);

  analogWrite(RED_LED, nRedControlValue);
  analogWrite(GREEN_LED, nGreenControlValue);
  analogWrite(BLUE_LED, nBlueControlValue);
}
