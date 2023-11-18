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

void printRgbMenu()
{
    Serial.print("\nRBG MENU: \n");
    Serial.print("1 - Manual RGB control\n2 - Toggle automatic RGB (now");
    Serial.print(g_rgbAutomatic == true ? "on)\n" : "off)\n");
    Serial.print("3 - Check system status\n4 - Control the RGB LED\n");
}

void toggleRgbControl()
{

}

void setRgbValues()
{
    
}

void goToRgbMenu()
{ 
    if(g_rgbMenu == NOT_SELECTED)
    {
        printRgbMenu();
        g_rgbMenu = SELECTED;
    }

    if(Serial.available() > NO_SERIAL_DATA || g_rgbMenu != SELECTED)
    {
        if(g_rgbMenu == SELECTED)
        {
            g_rgbMenu = Serial.parseInt();
        }

        switch (g_rgbMenu)
        {
            case MANUAL_RGB_CONTROL:
                setRgbValues();
                break;
            
            case TOGGLE_AUTOMATIC_RGB:
                toggleRgbControl();
                break;
            
            case GO_BACK:
                g_mainMenu = COMING_BACK_TO_MAIN;
                g_rgbMenu = NOT_SELECTED;
                break;

            default:
                g_rgbMenu = SELECTED;
                printRgbMenu();
                break;
        }
    }
}