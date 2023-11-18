#include "rgbControls.h"

// rgb
int g_rgbAutomatic = true;

void setRgbLight()
{
    if(g_rgbAutomatic)
    {
        // no threshold exceeded
        if(g_lastBrightnessReading < g_brightnessThresholdValue && g_lastProximityReading > g_proximityThresholdValue)
        {
            analogWrite(RED_LED_PIN, LED_OFF); 
            analogWrite(GREEN_LED_PIN, LED_ON); 
            analogWrite(BLUE_LED_PIN, LED_OFF);
        } 
        else if(g_lastBrightnessReading > g_brightnessThresholdValue && g_lastProximityReading < g_proximityThresholdValue)
        {
            analogWrite(RED_LED_PIN, LED_ON); 
            analogWrite(GREEN_LED_PIN, LED_OFF); 
            analogWrite(BLUE_LED_PIN, LED_ON);
        }
        else if(g_lastBrightnessReading > g_brightnessThresholdValue)
        {
            analogWrite(RED_LED_PIN, LED_ON); 
            analogWrite(GREEN_LED_PIN, LED_OFF); 
            analogWrite(BLUE_LED_PIN, LED_OFF);
        } 
        else if(g_lastProximityReading < g_proximityThresholdValue)
        {
            analogWrite(RED_LED_PIN, LED_OFF); 
            analogWrite(GREEN_LED_PIN, LED_OFF); 
            analogWrite(BLUE_LED_PIN, LED_ON);
        } 
    }
}
