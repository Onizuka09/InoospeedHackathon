
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include <secrets.h>
#include<HardwareSerial.h> 





 // Define the RX and TX pins for Serial 2
 #define RXD2 16
 #define TXD2 17

 #define Arduino_BAUD 9600
 
 // Create an instance of the HardwareSerial class for Serial 2
 HardwareSerial arduinoSerial(2);
// -- Pinouts and instances

#define RED 4
#define BLUE 18
#define GREEN 19



// PWM setup ESP32 

#define PWM_CHANNEL 0   // Choose a free channel (0–15)
#define PWM_FREQ 1600000   // PWM frequency in Hz
#define PWM_RES 8  

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

int val1 = 0;
bool prevVal1 = 0; // Variable to store the previous state of the IR sensor
int counter = 0;

int freq = 5000; 
int resolution = 8  ; 
void setup()
{

  Serial.begin(115200);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  arduinoSerial.begin(Arduino_BAUD, SERIAL_8N1, RXD2, TXD2);



  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Firbase stuff
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  /* Assign the api key (required) */
  config.api_key = API_KEY;
  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;
  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  Firebase.begin(&config, &auth);
  // Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);
  Firebase.setDoubleDigits(5);
  // -- end of firebase stuff

  delay(500);

  // Serial.printf("Get int ref... %s\n", Firebase.RTDB.setInt(&fbdo, F("/counter"), 0) ? "ok" : fbdo.errorReason().c_str());
  digitalWrite(RED, 0);
  digitalWrite(GREEN, 0);
  digitalWrite(BLUE, 0);


}
String buff = " ";

bool status = false;
void loop()
{

  arduinoSerial.println("happy");


  // get dooor status
  if (Firebase.ready())
  {
    Serial.printf("Set string... %s\n", Firebase.RTDB.getString(&fbdo, F("/emotions")) ? "ok" : fbdo.errorReason().c_str());
    buff = fbdo.to<const char *>();
    if (buff == "happy")
    {
      Serial.println("happy");
      arduinoSerial.println("happy");
      digitalWrite(RED, 0);
      digitalWrite(GREEN, 1);
      digitalWrite(BLUE, 0);
      // fan on
      // smile face MATRIX
    }
    else if (buff == "sad")
    {
      Serial.println("sad");
      arduinoSerial.println("sad");

      digitalWrite(RED, 0);
      digitalWrite(GREEN, 0);
      digitalWrite(BLUE, 1);
      // fan off

      // sad emoji
    }
    else if (buff == "angry")
    {
      Serial.println("angry");
      arduinoSerial.println("angry");

      digitalWrite(RED, 1);
      digitalWrite(GREEN, 0);
      digitalWrite(BLUE, 0);

      // angry emoji
      // fan off
    }
    else
    {
      // fan off
      Serial.println("neutral");
      
      digitalWrite(RED, 0);
      digitalWrite(GREEN, 0);
      digitalWrite(BLUE, 0);
    }
  }
}