/*********************************************************************
 *  Pump Control Module
 *
 *  Controls two pumps for spraying treatment when disease is detected
 *  Pumps can be triggered individually with optional auto-shutoff timers
 *
 *  Commands added to commands.h:
 *    PUMP1_ON        'P' - Turn pump 1 ON
 *    PUMP1_OFF       'Q' - Turn pump 1 OFF
 *    PUMP2_ON        'R' - Turn pump 2 ON
 *    PUMP2_OFF       'S' - Turn pump 2 OFF
 *    PUMP_SET_DURATION 'T' - Set pump duration (format: T <pump_num> <duration_ms>)
 *    PUMP_BOTH_OFF   'U' - Turn both pumps OFF
 *    PUMP_STATUS     'V' - Get pump status (optional)
 *********************************************************************/

#ifndef PUMP_CONTROL_H
#define PUMP_CONTROL_H

/* Pump configuration - Adjust these pins based on your wiring */
#define PUMP1_PIN 22           // Digital pin for pump 1 (pin 22 on Arduino Mega)
#define PUMP2_PIN 24           // Digital pin for pump 2 (pin 24 on Arduino Mega)
#define PUMP_ACTIVE_HIGH false // Set to false if pumps are active LOW

/* Pump control variables - using static to limit scope to this file */
static unsigned long pump1StartTime = 0;
static unsigned long pump2StartTime = 0;
static unsigned long pump1Duration = 3000; // Duration in ms, 0 = indefinite
static unsigned long pump2Duration = 3000;
static bool pump1Active = false;
static bool pump2Active = false;

/* Function prototypes */
void initPumps();
void setPump1(bool on);
void setPump2(bool on);
void setBothPumpsOff();
void setPumpDuration(int pumpNum, unsigned long duration);
void checkPumpDurations();
void processPumpCommand(char cmd, long arg1, long arg2);
void printPumpStatus();

/* Initialize pump pins */
void initPumps()
{
    pinMode(PUMP1_PIN, OUTPUT);
    pinMode(PUMP2_PIN, OUTPUT);

    // Set pumps to inactive state based on active high/low configuration
    if (PUMP_ACTIVE_HIGH)
    {
        digitalWrite(PUMP1_PIN, LOW);
        digitalWrite(PUMP2_PIN, LOW);
    }
    else
    {
        digitalWrite(PUMP1_PIN, HIGH);
        digitalWrite(PUMP2_PIN, HIGH);
    }

    pump1Active = false;
    pump2Active = false;
    pump1Duration = 3000;
    pump2Duration = 3000;

    Serial.println("Pumps initialized");
}

/* Turn pump 1 on/off */
void setPump1(bool on)
{
    if (on)
    {
        if (PUMP_ACTIVE_HIGH)
        {
            digitalWrite(PUMP1_PIN, HIGH);
        }
        else
        {
            digitalWrite(PUMP1_PIN, LOW);
        }
        pump1Active = true;
        pump1StartTime = millis();
        Serial.println("PUMP1_ON");
    }
    else
    {
        if (PUMP_ACTIVE_HIGH)
        {
            digitalWrite(PUMP1_PIN, LOW);
        }
        else
        {
            digitalWrite(PUMP1_PIN, HIGH);
        }
        pump1Active = false;
        Serial.println("PUMP1_OFF");
    }
}

/* Turn pump 2 on/off */
void setPump2(bool on)
{
    if (on)
    {
        if (PUMP_ACTIVE_HIGH)
        {
            digitalWrite(PUMP2_PIN, HIGH);
        }
        else
        {
            digitalWrite(PUMP2_PIN, LOW);
        }
        pump2Active = true;
        pump2StartTime = millis();
        Serial.println("PUMP2_ON");
    }
    else
    {
        if (PUMP_ACTIVE_HIGH)
        {
            digitalWrite(PUMP2_PIN, LOW);
        }
        else
        {
            digitalWrite(PUMP2_PIN, HIGH);
        }
        pump2Active = false;
        Serial.println("PUMP2_OFF");
    }
}

/* Turn both pumps off immediately */
void setBothPumpsOff()
{
    setPump1(false);
    setPump2(false);
    pump1Duration = 0;
    pump2Duration = 0;
    Serial.println("BOTH_PUMPS_OFF");
}

/* Set auto-shutoff duration for a pump (0 = indefinite) */
void setPumpDuration(int pumpNum, unsigned long duration)
{
    if (pumpNum == 1)
    {
        pump1Duration = duration;
        Serial.print("Pump 1 duration set to ");
        Serial.print(duration);
        Serial.println(" ms");
    }
    else if (pumpNum == 2)
    {
        pump2Duration = duration;
        Serial.print("Pump 2 duration set to ");
        Serial.print(duration);
        Serial.println(" ms");
    }
    else
    {
        Serial.println("ERROR: Invalid pump number (use 1 or 2)");
    }
}

/* Check if pumps have exceeded their duration and turn them off */
void checkPumpDurations()
{
    // Check pump 1
    if (pump1Active && pump1Duration > 0)
    {
        if ((millis() - pump1StartTime) >= pump1Duration)
        {
            setPump1(false);
            Serial.println("Pump 1 auto-shutoff");
        }
    }

    // Check pump 2
    if (pump2Active && pump2Duration > 0)
    {
        if ((millis() - pump2StartTime) >= pump2Duration)
        {
            setPump2(false);
            Serial.println("Pump 2 auto-shutoff");
        }
    }
}

/* Process pump-related serial commands */
void processPumpCommand(char cmd, long arg1, long arg2)
{
    switch (cmd)
    {
    case 'P': // Pump 1 ON
        setPump1(true);
        Serial.println("OK");
        break;

    case 'Q': // Pump 1 OFF
        setPump1(false);
        Serial.println("OK");
        break;

    case 'R': // Pump 2 ON
        setPump2(true);
        Serial.println("OK");
        break;

    case 'S': // Pump 2 OFF
        setPump2(false);
        Serial.println("OK");
        break;

    case 'T': // Set duration: T <pump_num> <duration_ms>
        if (arg1 >= 1 && arg1 <= 2)
        {
            setPumpDuration((int)arg1, (unsigned long)arg2);
            Serial.println("OK");
        }
        else
        {
            Serial.println("ERROR: Invalid pump number (use 1 or 2)");
        }
        break;

    case 'U': // Both pumps OFF
        setBothPumpsOff();
        Serial.println("OK");
        break;

    case 'V': // Pump status (for debugging)
        printPumpStatus();
        Serial.println("OK");
        break;

    default:
        // Not a pump command
        break;
    }
}

/* Get pump status (useful for debugging) */
void printPumpStatus()
{
    Serial.print("Pump 1: ");
    Serial.print(pump1Active ? "ON" : "OFF");
    if (pump1Duration > 0)
    {
        Serial.print(" (auto-off in ");
        if (pump1Active)
        {
            unsigned long remaining = pump1Duration - (millis() - pump1StartTime);
            Serial.print(remaining);
        }
        else
        {
            Serial.print(pump1Duration);
        }
        Serial.print(" ms)");
    }
    Serial.println();

    Serial.print("Pump 2: ");
    Serial.print(pump2Active ? "ON" : "OFF");
    if (pump2Duration > 0)
    {
        Serial.print(" (auto-off in ");
        if (pump2Active)
        {
            unsigned long remaining = pump2Duration - (millis() - pump2StartTime);
            Serial.print(remaining);
        }
        else
        {
            Serial.print(pump2Duration);
        }
        Serial.print(" ms)");
    }
    Serial.println();
}

#endif
