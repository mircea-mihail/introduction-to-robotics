#define RED_LED_CONTROL A2
#define GREEN_LED_CONTROL A1
#define BLUE_LED_CONTROL A0

#define RED_LED 9
#define GREEN_LED 10
#define BLUE_LED 11

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
  
  nRedControlValue = map(nRedControlValue, 0, 1023, 0, 255);
  nGreenControlValue = map(nGreenControlValue, 0, 1023, 0, 255);
  nBlueControlValue = map(nBlueControlValue, 0, 1023, 0, 255);

  analogWrite(RED_LED, nRedControlValue);
  analogWrite(GREEN_LED, nGreenControlValue);
  analogWrite(BLUE_LED, nBlueControlValue);
}
