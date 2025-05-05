
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Firebase_ESP_Client.h>
#include <ESP32_Servo.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include <secrets.h>
// -- Pinouts and instances
// Motor A
#define EnA 19
#define In1 4
#define In2 16
// led
#define LED 17
// servo
#define servoPin 18

Servo doorServo;                    // create servo object to control a servo
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

bool boolgate = false;
int stategate = 0;
int gatet0 = 0;

bool booldoor = false;
int statedoor = 0;

int brightnessled = 0;
int firebaseled = 0;
bool boolled = false;

void setup()
{

  Serial.begin(115200);
  doorServo.attach(servoPin); // attaches the servo on pin 18 to the servo object

  // setup  LED
  // pinMode(LED, OUTPUT);
  pinMode(EnA, OUTPUT);

  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);

  pinMode(LED, OUTPUT);

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
  // set servo to init position
  doorServo.write(90); // Servo closed at position 90
  // initialize firbase instances
  Serial.printf("Get int ref... %s\n", Firebase.RTDB.setBool(&fbdo, F("/door"), 0) ? "ok" : fbdo.errorReason().c_str());
  Serial.printf("Get int ref... %s\n", Firebase.RTDB.setBool(&fbdo, F("/gate"), 0) ? "ok" : fbdo.errorReason().c_str());
  Serial.printf("Get int ref... %s\n", Firebase.RTDB.setBool(&fbdo, F("/led"), 0) ? "ok" : fbdo.errorReason().c_str());

  lcd.clear();
}
void motorControlgate(bool clockwise)
{
  // Motor control logic using L298N driver
  digitalWrite(In1, clockwise ? HIGH : LOW);
  digitalWrite(In2, clockwise ? LOW : HIGH);
  // Adjust the motor speed as needed by setting the PWM values on EnA and EnB
  analogWrite(EnA, 255); // Adjust the value (0 to 255) for speed control
}

void motorStop()
{
  // Adjust the motor speed as needed by setting the PWM values on EnA and EnB
  // Adjust the value (0 to 255) for speed control
  analogWrite(EnA, 0);
}
int menu = 0;
String fstatus = "off";
int distance = -1;
void get_firebase_data()
{
  distance = -1;
  fstatus = "off";
  menu = 0;
  Serial.printf("Get int ref... %s\n", Firebase.RTDB.getInt(&fbdo, F("/fdistance"), &distance) ? "ok" : fbdo.errorReason().c_str());
  Serial.printf("Get int ref... %s\n", Firebase.RTDB.getString(&fbdo, F("/fstatus"), &fstatus) ? "ok" : fbdo.errorReason().c_str());
  Serial.printf("Get int ref... %s\n", Firebase.RTDB.getInt(&fbdo, F("/menu"), &menu) ? "ok" : fbdo.errorReason().c_str());
  if (menu == 1)
  {
    boolgate = true;
    booldoor = false;
    boolled = false;
  }
  else if (menu == 2)
  {
    // booldoor =  true ;
    boolgate = false;
    booldoor = false;
    boolled = true;
  }
  else if (menu == 3)
  {
    boolled = false;
    boolgate = false;
    booldoor = true;
  }
  else
  {
    distance = -1;
    fstatus = "off";
    menu = 0;
    boolled = false;
    boolgate = false;
    booldoor = false;
  }
}
void loop()
{
  // open gate
  get_firebase_data();
  if (boolgate == true)
  {
    gatet0 = millis();
    if (fstatus == "on")
    {
      if (millis() - gatet0 < 500)
      {
        // digitalWrite(LED, HIGH );
        motorControlgate(true);
        Serial.printf("Get int ref... %s\n", Firebase.RTDB.setBool(&fbdo, F("/door"), 0) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Get int ref... %s\n", Firebase.RTDB.setBool(&fbdo, F("/gate"), 1) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Get int ref... %s\n", Firebase.RTDB.setBool(&fbdo, F("/led"), 0) ? "ok" : fbdo.errorReason().c_str());
      }
      else if (millis() - gatet0 < 1000)
      {
        motorControlgate(false);
      }
      else
      {
        motorStop();
        boolgate = false;
      }
    }
    // }else if(fstatus == "off"){
    //   if(millis() - gatet0 < 1000){
    //    // digitalWrite(LED, HIGH );
    //     motorControlgate(false);
    //   }
    else
    {
      motorStop();

      Serial.printf("Get int ref... %s\n", Firebase.RTDB.setBool(&fbdo, F("/door"), 0) ? "ok" : fbdo.errorReason().c_str());
      Serial.printf("Get int ref... %s\n", Firebase.RTDB.setBool(&fbdo, F("/gate"), 0) ? "ok" : fbdo.errorReason().c_str());
      Serial.printf("Get int ref... %s\n", Firebase.RTDB.setBool(&fbdo, F("/led"), 0) ? "ok" : fbdo.errorReason().c_str());
    }
  }
  else
  {
    stategate = 0;
    motorStop();
  }

  // dooor
  if (booldoor == true)
  {

    if (fstatus == "on")
    {

      doorServo.write(90); // Adjust the angle for opening
      Serial.printf("Get int ref... %s\n", Firebase.RTDB.setBool(&fbdo, F("/door"), 1) ? "ok" : fbdo.errorReason().c_str());
      Serial.printf("Get int ref... %s\n", Firebase.RTDB.setBool(&fbdo, F("/gate"), 0) ? "ok" : fbdo.errorReason().c_str());
      Serial.printf("Get int ref... %s\n", Firebase.RTDB.setBool(&fbdo, F("/led"), 0) ? "ok" : fbdo.errorReason().c_str());
    }
    else if (fstatus == "off")
    {

      // Close the servo motor
      doorServo.write(0); // Adjust the angle for closing
      Serial.printf("Get int ref... %s\n", Firebase.RTDB.setBool(&fbdo, F("/door"), 0) ? "ok" : fbdo.errorReason().c_str());
      Serial.printf("Get int ref... %s\n", Firebase.RTDB.setBool(&fbdo, F("/gate"), 0) ? "ok" : fbdo.errorReason().c_str());
      Serial.printf("Get int ref... %s\n", Firebase.RTDB.setBool(&fbdo, F("/led"), 0) ? "ok" : fbdo.errorReason().c_str());
    }
  }
  else
  {
    statedoor = 0;
    doorServo.write(0); // Stop the servo
  }

  /// KED
  if (boolled == true)
  {

    // if ( )
    if (fstatus == "off")
    {
      Serial.printf("Get int ref... %s\n", Firebase.RTDB.setBool(&fbdo, F("/door"), 0) ? "ok" : fbdo.errorReason().c_str());
      Serial.printf("Get int ref... %s\n", Firebase.RTDB.setBool(&fbdo, F("/gate"), 0) ? "ok" : fbdo.errorReason().c_str());
      Serial.printf("Get int ref... %s\n", Firebase.RTDB.setBool(&fbdo, F("/led"), 1) ? "ok" : fbdo.errorReason().c_str());
      brightnessled = map(firebaseled, -1, 150, 0, 1023);
    analogWrite(LED, brightnessled);


    }
    else
    {
      Serial.printf("Get int ref... %s\n", Firebase.RTDB.setBool(&fbdo, F("/door"), 0) ? "ok" : fbdo.errorReason().c_str());
      Serial.printf("Get int ref... %s\n", Firebase.RTDB.setBool(&fbdo, F("/gate"), 0) ? "ok" : fbdo.errorReason().c_str());
      Serial.printf("Get int ref... %s\n", Firebase.RTDB.setBool(&fbdo, F("/led"), 0) ? "ok" : fbdo.errorReason().c_str());
     brightnessled = 0 ; 
     analogWrite(LED, brightnessled);

    }
    // brightnessled = map(firebaseled, -1, 150, 0, 255);

   
    

  }
}