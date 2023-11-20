#include "rgbControls.h"

// rgb
extern bool g_rgbAutomatic;

extern int g_defaultRedColor;
extern int g_defaultBlueColor;
extern int g_defaultGreenColor;

bool g_hasVisitedManualSensorVals = false;

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
    else
    {
        analogWrite(RED_LED_PIN, g_defaultRedColor); 
        analogWrite(GREEN_LED_PIN, g_defaultGreenColor); 
        analogWrite(BLUE_LED_PIN, g_defaultBlueColor);
    }
}

void printRgbMenu()
{
    Serial.print(F("\nRBG MENU: \n"));
    Serial.print(F("1 - Manual RGB control\n2 - Toggle automatic RGB (now "));
    Serial.print(g_rgbAutomatic == true ? "on)\n" : "off)\n");
    Serial.print(F("3 - Go back to main menu\n"));
}

void toggleRgbControl()
{
    Serial.print(F("Automatic RGB control has been turned "));
    if(g_rgbAutomatic == true)
    {
        g_rgbAutomatic = false;
        Serial.print(F("off\n"));
    }
    else
    {
        g_rgbAutomatic = true;
        Serial.print(F("on\n"));
    }
    EEPROM.update(AUTO_LED_ADDRESS, g_rgbAutomatic);
    
    g_rgbMenu = SELECTED;
    printRgbMenu();
}   

void setRgbValues()
{
    if(!g_hasVisitedManualSensorVals)
    {
        Serial.print(F("Input values for the red green and blue between 0 and 255"));
        Serial.print(F("\nFor example: 255 14 0 (red 255, blue 14, green 0)\n"));
        g_hasVisitedManualSensorVals = true;
    }

    if(Serial.available())
    {
        String serialColorInput = Serial.readString();
        // char * auxString = new char[serialColorInput.length() + 1];
        // strcpy(auxString, serialColorInput.c_str());

        sscanf(serialColorInput.c_str(), "%d %d %d", &g_defaultRedColor, &g_defaultGreenColor, &g_defaultBlueColor);

        EEPROM.update(DEFAULT_RED_COLOR_ADDRESS, g_defaultRedColor);
        EEPROM.update(DEFAULT_GREEN_COLOR_ADDRESS, g_defaultGreenColor);
        EEPROM.update(DEFAULT_BLUE_COLOR_ADDRESS, g_defaultBlueColor);

        g_hasVisitedManualSensorVals = false;
        g_rgbMenu = SELECTED;
        printRgbMenu();
    }
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