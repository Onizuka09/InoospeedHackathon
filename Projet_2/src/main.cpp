
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Firebase_ESP_Client.h>
#include <ESP32_Servo.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include <secrets.h>
// -- Pinouts and instances
#define IR 4
#define servoPin 18

Servo myservo;                      // create servo object to control a servo
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

int val1 = 0;
bool prevVal1 = 0; // Variable to store the previous state of the IR sensor
int counter = 0;

void setup()
{

  Serial.begin(115200);
  myservo.attach(servoPin); // attaches the servo on pin 18 to the servo object

  pinMode(IR, INPUT);
  lcd.init(); // initialize the lcd
  lcd.backlight();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  lcd.setCursor(0, 0); // Move cursor to (0, 0)
  lcd.print("Connecting .");

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

  lcd.setCursor(0, 0); // Move cursor to (0, 0)
  lcd.print("Connected");
  delay(500);
  lcd.clear();
  myservo.write(90); // Servo closed at position 90

  Serial.printf("Get int ref... %s\n", Firebase.RTDB.setInt(&fbdo, F("/counter"), 0) ? "ok" : fbdo.errorReason().c_str());
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Counter: ");
  lcd.print(counter);
}
char buff[255]; 

bool status = false;
void loop()
{
  // get dooor status
  if (Firebase.ready())
  {
    Serial.printf("Set string... %s\n", Firebase.RTDB.getBool(&fbdo, F("/doorStatus")) ? "ok" : fbdo.errorReason().c_str());
    status = fbdo.to<bool>();
    if (status)
    {
      myservo.write(0); // door opend
      Serial.println("door opened ");
      if (val1 == HIGH && prevVal1 == LOW )
      {
        counter++;
    
        sprintf(buff, "%d Person(s) Entered  ", counter);
    
        Serial.printf("Set string... %s\n", Firebase.RTDB.setString(&fbdo, F("/notification"), buff) ? "ok" : fbdo.errorReason().c_str());
    
    
        Serial.printf("Get int ref... %s\n", Firebase.RTDB.setInt(&fbdo, F("/counter"), counter) ? "ok" : fbdo.errorReason().c_str());
        memset(buff,0,sizeof(buff)); 
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Front Montant");
        lcd.setCursor(0, 1);
        lcd.print("Counter: ");
        lcd.print(counter);
      }
    
      prevVal1 = val1; // Update the previous state of the IR sensor²
    }
    else
    {
      myservo.write(90); // door closed
      Serial.println("door closed ");
    }
    
  }
  // detect person intrance
  val1 = digitalRead(IR); // Read the current state of the IR sensor

  // Detect Rising edge
 

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Counter: ");
  lcd.print(counter);
}