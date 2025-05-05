#include <Arduino.h>
#include <HardwareSerial.h>
// Callback for Ethernet events


 // Define the RX and TX pins for Serial 2
#define RXD2 1<6
#define TXD2 17
#define BTN 4
#define Arduino_BAUD 9600

// Create an instance of the HardwareSerial class for Serial 2
HardwareSerial arduinoSerial(2);
static unsigned long pressStart;
static bool longPressed_5s;
static bool longPressed_10s;
static bool longPressed_15s;

void setup(){
  // Serial Monitor
  Serial.begin(115200);
  pinMode(BTN,INPUT); 
  // Start Serial 2 with the defined RX and TX pins and a baud rate of 9600
  arduinoSerial.begin(Arduino_BAUD, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial 2 started at 9600 baud rate");
}
static bool btn_pressed = false;
void HandleButton_5s()
{
    
    // static bool long_pressed_d11etected = false;
    static unsigned long t0 = 0;

    bool btn_state = digitalRead(BTN); //! // Assuming active LOW button
    /* Serial.println(btn_state); */
    if (btn_state)
    { // Button is pressed
            
    //   Serial.println("Serial 2 started at 9600 baud rate");

        if (!btn_pressed)
        { // First press detected
            btn_pressed = true;
            t0 = millis(); // Record the time of the press
        }
        if (millis() - t0 >= 5000 && !longPressed_5s)
        { // Long press detected
            arduinoSerial.println("5");
            Serial.println("btn pressed for more than 5s");

            longPressed_5s = true;
            //btn_pressed = false; // Reset for new detection
        }
        
        else if (millis() - t0 >= 10000 && !longPressed_10s)
        { // Long press detected
            arduinoSerial.println("10");
            Serial.println("btn pressed for more than 10s");

            longPressed_10s = true;
            //btn_pressed = false; // Reset for new detection
        }
        else if (millis() - t0 >= 15000 && !longPressed_15s)
        { // Long press detected
            arduinoSerial.println("15");
            Serial.println("btn pressed for more than 15s");

            longPressed_15s = true;
            btn_pressed = false; // Reset for new detection
        }
    }
    else
    {
        btn_pressed = false;    // Reset press state
        longPressed_5s = false; // Allow new long press detection
        longPressed_10s = false; // Allow new long press detection
        longPressed_15s = false; // Allow new long press detection
        t0 = 0;             // Reset the timer
        // program_status = INIT;
    }

    return; // No long press detected
}

int btn_state = 0 ; 
void loop(){
 HandleButton_5s(); 
}