//References:
//https://create.arduino.cc/projecthub/biharilifehacker/arduino-with-pir-motion-sensor-fd540a
//https://create.arduino.cc/projecthub/rowan07/make-a-simple-led-circuit-ce8308
//https://create.arduino.cc/projecthub/Raushancpr/arduino-with-ir-sensor-1579b6
//https://www.youtube.com/watch?v=J61_PKyWjxU (Youtube video)
//https://dronebotworkshop.com/interrupts
//https://www.youtube.com/watch?v=wIcC8-g9Lnw

//Pins
const uint8_t PIR_SENSOR = 2;//2
const uint8_t LED_PIR = A0;//A0 Red

const uint8_t TILT_SENSOR = 3;//3
const uint8_t LED_TILT = 13;//13 Blue

const uint8_t BUTTON = 2;
const uint8_t LED_BUTTON = 12;

const uint8_t LED = 13;//Timer interrupt
int timer1_compare_match;

volatile bool PIR_state = false;
volatile bool TILT_state = false;
volatile bool BUTTON_state = false;

ISR(TIMER1_COMPA_vect)
// Interrupt Service Routine for compare mode
{
  // Preload timer with compare match value
  TCNT1 = timer1_compare_match;
  
  // Write opposite value to LED
  digitalWrite(LED, digitalRead(LED) ^ 1);
}

void setup() {
  // Set LED as output
  pinMode(LED, OUTPUT); 
  
  // Initialize Timer1
  TCCR1A = 0;
  TCCR1B = 0;
 
  // Set timer1_compare_match to the correct compare match register value
  // 256 prescaler & 31246 compare match = 2Hz
  timer1_compare_match = 31249;
 
  // Preload timer with compare match value
  TCNT1 = timer1_compare_match; 
  // Set prescaler to 256
  TCCR1B |= (1 << CS12); 
  // Enable timer interrupt for compare mode
  TIMSK1 |= (1 << OCIE1A); 
  // Enable all interrupts
  interrupts();
    
  //PIR motion sensor setting
  pinMode(LED_PIR, OUTPUT);
  pinMode(PIR_SENSOR, INPUT_PULLUP);

  //IR sensor setting
  pinMode (LED_TILT, OUTPUT);
  pinMode (TILT_SENSOR, INPUT_PULLUP);
  
  //BUTTON setting:
  pinMode(LED_BUTTON, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  
  //Enable Port D
  PCICR |= B00000100;
  //Select PCINT2 D2
  PCMSK2 |= B00000100;//D2
  
  Serial.begin(9600);
}

void loop(){
}

ISR (PCINT2_vect)//PIR, TILT, BUTTON
{
  if (digitalRead(PIR_SENSOR) == LOW) {
    PIR_state = !PIR_state;
    digitalWrite(PIR_SENSOR, LED_PIR);
    Serial.println("LED is ON");
  }
 
  // Check if this was D7
  if (digitalRead(TILT_SENSOR) == LOW) {
    TILT_state = TILT_state;
    digitalWrite(TILT_SENSOR, TILT_state);
    Serial.println("LED is ON");
  }
  
   if (digitalRead(BUTTON) == LOW) {
    BUTTON_state = BUTTON_state;
    digitalWrite(LED_BUTTON, BUTTON_state);
    Serial.println("LED is ON");
  }
}  