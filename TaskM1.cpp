

// --- Pin Assignments ---
const int buttonPin = 8;      // PCI input
const int trigPin = 10;       // HC-SR04 Trig
const int echoPin = 9;       // HC-SR04 Echo
const int led1Pin = 6;        // LED1: ONLY blinks every second via Timer
const int led2Pin = 4;        // LED2: Blinks when object detected or button pressed

// --- Global Flags ---
volatile bool buttonPressed = false;  // Set by PCI ISR
volatile bool timerFlag = false;      // Set by Timer1 ISR

// --- Sensor Variables ---
long duration;
int distance;
bool objectNear = false;

// --- LED2 Blinking Control ---
bool led2State = LOW;
unsigned long lastBlinkTime = 0;
int blinkInterval = 250;

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);

  // --- Enable PCI on D8 ---
  PCICR |= (1 << PCIE0);
  PCMSK0 |= (1 << PCINT0);

  // --- Setup Timer1 for 1 second blinking (LED1 only) ---
  cli(); // Disable interrupts during setup
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 15624; // 1Hz (16MHz / 1024 / 1Hz - 1)
  TCCR1B |= (1 << WGM12); // CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler 1024
  TIMSK1 |= (1 << OCIE1A); // Enable Timer1 compare interrupt
  sei(); // Enable global interrupts
}

void loop() {
  // --- Read Sensor Distance ---
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  objectNear = (distance < 20);

  // --- LED1: Timer-based Blinking ---
  if (timerFlag) {
    timerFlag = false;
    digitalWrite(led1Pin, !digitalRead(led1Pin));
    Serial.println("LED1: Timer Blinked (every 1s)");
  }

  // --- LED2: Blinks ONLY if object is near OR button is pressed ---
  if (objectNear || buttonPressed) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastBlinkTime >= blinkInterval) {
      lastBlinkTime = currentMillis;
      led2State = !led2State;
      digitalWrite(led2Pin, led2State);

      Serial.print("LED2 Blink: ");
      if (objectNear) Serial.print("Object Near ");
      if (buttonPressed) Serial.print("Button Pressed ");
      Serial.print("| Distance: ");
      Serial.print(distance);
      Serial.println(" cm");
    }
  } else {
    digitalWrite(led2Pin, LOW);
    led2State = LOW;
  }
}

// --- PCI ISR for Button on D8 ---
ISR(PCINT0_vect) {
  buttonPressed = !digitalRead(buttonPin); // Set flag based on button
}

// --- Timer1 ISR for LED1 ---
ISR(TIMER1_COMPA_vect) {
  timerFlag = true;
}