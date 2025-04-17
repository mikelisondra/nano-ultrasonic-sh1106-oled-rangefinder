#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define i2c_Address 0x3C // I2C address for SH1106 OLED display
#define trigPin 2
#define echoPin 4

int led_1 = 11;
int led_2 = 10;
int led_3 = 9;
int led_4 = 8;

float distanceMeter;
long distanceCm, distanceMm, distanceInch;

unsigned long previousMillis = 0; // Store last time something was updated

// Add these new declarations
unsigned long previousLEDMillis = 0; // Store last time LEDs were updated
const int ledUpdateInterval = 500;   // Interval for LED updates (milliseconds)

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  pinMode(led_4, OUTPUT);

  Serial.begin(9600);  // Start serial communication

  // Initialize the SH1106 OLED display
  if (!display.begin(i2c_Address, true)) {
    Serial.println(F("SH1106 OLED initialization failed"));
    for (;;);
  }

  // Clear the display buffer and show a startup message
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.print("Distance Measurement");
  display.setTextSize(2); 
  display.setCursor(0, 10); 
  display.print("Sensor");
  display.setTextSize(1); 
  display.setCursor(20, 40); 
  display.print("BY:");
  display.setCursor(40, 50); 
  display.print("GROUP 5");
  display.display();           // Show the message on the screen
  delay(1500);                 // Delay for 1.5 seconds
  display.clearDisplay();      // Clear the display
}

void loop() {
  long duration;

  // Trigger the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo and calculate distance in centimeters
  duration = pulseIn(echoPin, HIGH);
  distanceCm = (duration / 2) / 29.1;    // Distance in cm
  distanceMm = distanceCm * 10;          // Distance in millimeters
  distanceInch = distanceCm / 2.54;      // Convert to inches
  distanceMeter = distanceCm / 100.0;    // Convert to meters

  // Display the distance on the OLED
  display.clearDisplay();
  display.setCursor(0, 0);  
  display.print("DISTANCE MEASURED:");
  display.setCursor(0, 20);
  display.print("M: ");
  display.print(distanceMeter, 2);  
  display.setCursor(0, 30);
  display.print("CM: ");
  display.print(distanceCm);
  display.setCursor(0, 40);
  display.print("INCH: ");
  display.print(distanceInch);
  display.setCursor(0, 50);
  display.print("MM: ");
  display.print(distanceMm);
  display.display(); // Update the OLED display with the new data

  // Debug print for distance meter
  Serial.print("Distance in Meters: ");
  Serial.println(distanceMeter);
  Serial.print("Distance in Millimeters: ");
  Serial.println(distanceMm);

  unsigned long currentMillis = millis();

  // Update LEDs
  if (currentMillis - previousLEDMillis >= ledUpdateInterval) {
    previousLEDMillis = currentMillis;

    // LED Pattern
    if ((distanceCm >= 0) && (distanceCm <= 100)) {
      digitalWrite(led_1, HIGH);
      digitalWrite(led_2, LOW);
      digitalWrite(led_3, LOW);
      digitalWrite(led_4, LOW);
    }
    if ((distanceCm >= 101) && (distanceCm <= 200)) {
      digitalWrite(led_1, HIGH);
      digitalWrite(led_2, HIGH);
      digitalWrite(led_3, LOW);
      digitalWrite(led_4, LOW);
    }

    if ((distanceCm >= 201) && (distanceCm <= 300)) {
      digitalWrite(led_1, HIGH);
      digitalWrite(led_2, HIGH);
      digitalWrite(led_3, HIGH);
      digitalWrite(led_4, LOW);
    }

    if ((distanceCm >= 301) && (distanceCm <= 400)) {
      digitalWrite(led_1, HIGH);
      digitalWrite(led_2, HIGH);
      digitalWrite(led_3, HIGH);
      digitalWrite(led_4, HIGH);
    }

    delay(200); // Pause to let things settle
  }
}
