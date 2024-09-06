// Define pins for LEDs
const int redLed = 4;
const int amberLed = 3;
const int greenLed = 2;

// Define pins for Ultrasonic Sensor
const int trigPin = 8;
const int echoPin = 9;

// Define variables for Ultrasonic Sensor
long duration;
int distance;
int pedestrianCount = 0;

// Set the minimum distance to detect a pedestrian (in cm)
int thresholdDistance = 10; // 10 cm constraint

// Pedestrian limit before triggering lights
const int pedestrianLimit = 5;

// Timing variables
unsigned long amberFlashDuration = 5000;  // 3 seconds for Amber flashing
unsigned long redDuration = 15000;          // 5 seconds for Red

void setup() {
  // Initialize LED pins as output
  pinMode(redLed, OUTPUT);
  pinMode(amberLed, OUTPUT);
  pinMode(greenLed, OUTPUT);

  // Initialize ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Begin Serial communication for debugging
  Serial.begin(9600);

  // Turn on Green light initially (Cars go)
  digitalWrite(greenLed, HIGH);

  // Print initial message to Serial Monitor
  Serial.println("System initialized. Green light is ON. Waiting for pedestrians...");
}

void loop() {
  // Get distance from ultrasonic sensor
  distance = getDistance();

  // Show distance on Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Check if a pedestrian passes
  if (distance > 0 && distance <= thresholdDistance) {
    pedestrianCount++;
    Serial.print("Pedestrian detected! Count: ");
    Serial.println(pedestrianCount);

    delay(500); // Small delay to avoid multiple counts for one pedestrian
  }

  // Check if pedestrian count reaches the limit
  if (pedestrianCount >= pedestrianLimit) {
    Serial.println("Pedestrian limit reached. Triggering traffic lights...");

    // Trigger amber light for warning
    amberSignal();
    
    // Trigger red light for pedestrians to cross
    redSignal();
    
    // Reset the pedestrian count after red light
    pedestrianCount = 0;
    
    // Turn green back on for cars after red light
    digitalWrite(greenLed, HIGH);
    Serial.println("Green light is ON. Cars can go.");
  }

  // If no pedestrians detected, keep green light on
  if (pedestrianCount == 0) {
    digitalWrite(greenLed, HIGH);
    digitalWrite(amberLed, LOW);
    digitalWrite(redLed, LOW);
    Serial.println("No pedestrians. Green light is ON. Cars can go.");
  }

  delay(200); // Loop delay for stability
}

// Function to calculate distance using Ultrasonic sensor
int getDistance() {
  // Send pulse to trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo pin
  duration = pulseIn(echoPin, HIGH);

  // Calculate distance in cm
  int distance = duration * 0.034 / 2;

  return distance;
}

// Function to handle amber light with flashing behavior
void amberSignal() {
   digitalWrite(greenLed, LOW);
  unsigned long startTime = millis();
  Serial.println("Amber light is FLASHING. Warning drivers...");
  
  while (millis() - startTime < amberFlashDuration) {
    digitalWrite(amberLed, HIGH);  // Turn on Amber
    delay(500);                    // Wait for half a second
    digitalWrite(amberLed, LOW);   // Turn off Amber
    delay(500);                    // Wait for half a second
  }
}

// Function to handle red light
void redSignal() {
  digitalWrite(greenLed, LOW);   // Turn off Green
  digitalWrite(redLed, HIGH);     // Turn on Red
  Serial.println("Red light is ON. Pedestrians can cross.");
  delay(redDuration);              // Wait for red duration
  digitalWrite(redLed, LOW);      // Turn off Red
}
