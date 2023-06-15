//8 analog inputs from the array
const int input0 = A0;
const int input1 = A1;
const int input2 = A2;
const int input3 = A3;
const int input4 = A4;
const int input5 = A5;
const int input6 = A9;
const int input7 = A10;

//1 analog reference input
const int ref = A11;

//7 digital outputs to select input from multiplexers
//with 4 chip enables and 3 select
const int CE0 = 16;
const int CE1 = 17;
const int CE2 = 1;
const int CE3 = 0;
const int sel0 = 6;
const int sel1 = 7;
const int sel2 = 8;

//speaker output port
const int pwmPin = 5;  // D5 pin for PWM output

//delay constant between each select signal
const int  wait = 1;

//threshold voltage above the reference that counts as a 1
//it has 1023 steps between 0 to 5V so 205 is about 1V
const int thresh = 205;

//variables
bool Go = false;

long freq = 0;
int selectIndex = 0;

//select signal 2D array
byte selectSig[32][7] = {
  {LOW, HIGH, HIGH, HIGH, LOW, LOW, LOW},     // Chip Enable: 0111, Select: 000
  {LOW, HIGH, HIGH, HIGH, LOW, LOW, HIGH},    // Chip Enable: 0111, Select: 001
  {LOW, HIGH, HIGH, HIGH, LOW, HIGH, LOW},    // Chip Enable: 0111, Select: 010
  {LOW, HIGH, HIGH, HIGH, LOW, HIGH, HIGH},   // Chip Enable: 0111, Select: 011
  {LOW, HIGH, HIGH, HIGH, HIGH, LOW, LOW},    // Chip Enable: 0111, Select: 100
  {LOW, HIGH, HIGH, HIGH, HIGH, LOW, HIGH},   // Chip Enable: 0111, Select: 101
  {LOW, HIGH, HIGH, HIGH, HIGH, HIGH, LOW},   // Chip Enable: 0111, Select: 110
  {LOW, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH},  // Chip Enable: 0111, Select: 111

  {HIGH, LOW, HIGH, HIGH, LOW, LOW, LOW},     // Chip Enable: 1011, Select: 000
  {HIGH, LOW, HIGH, HIGH, LOW, LOW, HIGH},    // Chip Enable: 1011, Select: 001
  {HIGH, LOW, HIGH, HIGH, LOW, HIGH, LOW},    // Chip Enable: 1011, Select: 010
  {HIGH, LOW, HIGH, HIGH, LOW, HIGH, HIGH},   // Chip Enable: 1011, Select: 011
  {HIGH, LOW, HIGH, HIGH, HIGH, LOW, LOW},    // Chip Enable: 1011, Select: 100
  {HIGH, LOW, HIGH, HIGH, HIGH, LOW, HIGH},   // Chip Enable: 1011, Select: 101
  {HIGH, LOW, HIGH, HIGH, HIGH, HIGH, LOW},   // Chip Enable: 1011, Select: 110
  {HIGH, LOW, HIGH, HIGH, HIGH, HIGH, HIGH},  // Chip Enable: 1011, Select: 111

  {HIGH, HIGH, LOW, HIGH, LOW, LOW, LOW},     // Chip Enable: 1101, Select: 000
  {HIGH, HIGH, LOW, HIGH, LOW, LOW, HIGH},    // Chip Enable: 1101, Select: 001
  {HIGH, HIGH, LOW, HIGH, LOW, HIGH, LOW},    // Chip Enable: 1101, Select: 010
  {HIGH, HIGH, LOW, HIGH, LOW, HIGH, HIGH},   // Chip Enable: 1101, Select: 011
  {HIGH, HIGH, LOW, HIGH, HIGH, LOW, LOW},    // Chip Enable: 1101, Select: 100
  {HIGH, HIGH, LOW, HIGH, HIGH, LOW, HIGH},   // Chip Enable: 1101, Select: 101
  {HIGH, HIGH, LOW, HIGH, HIGH, HIGH, LOW},   // Chip Enable: 1101, Select: 110
  {HIGH, HIGH, LOW, HIGH, HIGH, HIGH, HIGH},  // Chip Enable: 1101, Select: 111

  {HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW},     // Chip Enable: 1110, Select: 000
  {HIGH, HIGH, HIGH, LOW, LOW, LOW, HIGH},    // Chip Enable: 1110, Select: 001
  {HIGH, HIGH, HIGH, LOW, LOW, HIGH, LOW},    // Chip Enable: 1110, Select: 010
  {HIGH, HIGH, HIGH, LOW, LOW, HIGH, HIGH},   // Chip Enable: 1110, Select: 011
  {HIGH, HIGH, HIGH, LOW, HIGH, LOW, LOW},    // Chip Enable: 1110, Select: 100
  {HIGH, HIGH, HIGH, LOW, HIGH, LOW, HIGH},   // Chip Enable: 1110, Select: 101
  {HIGH, HIGH, HIGH, LOW, HIGH, HIGH, LOW},   // Chip Enable: 1110, Select: 110
  {HIGH, HIGH, HIGH, LOW, HIGH, HIGH, HIGH},  // Chip Enable: 1110, Select: 111

};

//function for comparing inputs to reference
int compareAnalogRead(int inputPin, int referencePin, int threshold) {
  int inputValue = analogRead(inputPin);
  int referenceValue = analogRead(referencePin);
  return (inputValue - referenceValue);
  // if (inputValue > referenceValue + threshold) {
  //   return 1;
  // } else {
  //   return 0;
  // }
}

void setFreq(long f) {
  if (f == 0) {
    ICR3 = 0;
    OCR3A = 0;
  } else {
    ICR3 = 2*pow(10,6)/f - 1;                   // Set the PWM frequency to f = (16MHz / (8 * (ICR3 + 1)))
    OCR3A = ICR3/2;                             // Set duty-cycle to 50% on pwmPin
  }
}


void setup() {
  //est connection between arduino and computer
  Serial.begin(9600);
  //init all the analog inputs as inputs
  pinMode(input0, INPUT);
  pinMode(input1, INPUT);
  pinMode(input2, INPUT);
  pinMode(input3, INPUT);
  pinMode(input4, INPUT);
  pinMode(input5, INPUT);
  pinMode(input6, INPUT);
  pinMode(input7, INPUT);
  
  //init all the digital select outputs as outputs
  pinMode(CE0, OUTPUT);
  pinMode(CE1, OUTPUT);
  pinMode(CE2, OUTPUT);
  pinMode(CE3, OUTPUT);
  pinMode(sel0, OUTPUT);
  pinMode(sel1, OUTPUT);
  pinMode(sel2, OUTPUT);

  //to see if its collecting 
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(pwmPin, OUTPUT);                          // Set pwm sound output pin to an output
  TCCR3A = _BV(COM3A1) | _BV(COM3B1) | _BV(WGM31);  // Enable PWM outputs for OC1A and OC1B on digital pins 9, 10
  TCCR3B = _BV(WGM33) | _BV(WGM32) | _BV(CS31);     // Set fast PWM and prescaler of 8 on timer 1
  setFreq(freq);
}

void loop() {

  //Type g in serial monitor to start and stop
  if (Serial.available() > 0) {
    char gatherData = Serial.read();
    if (gatherData == 'g') {
      Go = !Go;
    } else if (gatherData == 'f') {
      String fs = "";
      while (Serial.available() > 0) {
        char rc = Serial.read();
        fs += rc;
      }
      int f = fs.toInt();
      setFreq(f);
    }
  }

  //if collecting the LED is on else its off
  if (Go) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
  }

  //if Go, so collecting data, send the select signals to Serial monitor
  if (Go) {
    Serial.print(static_cast<int>(selectSig[selectIndex][0]));
    Serial.print(static_cast<int>(selectSig[selectIndex][1]));
    Serial.print(static_cast<int>(selectSig[selectIndex][2]));
    Serial.print(static_cast<int>(selectSig[selectIndex][3]));
    Serial.print(static_cast<int>(selectSig[selectIndex][4]));
    Serial.print(static_cast<int>(selectSig[selectIndex][5]));
    Serial.print(static_cast<int>(selectSig[selectIndex][6]));

    //then send the select signals to the select pins
    digitalWrite(CE0, selectSig[selectIndex][0]);
    digitalWrite(CE1, selectSig[selectIndex][1]);
    digitalWrite(CE2, selectSig[selectIndex][2]);
    digitalWrite(CE3, selectSig[selectIndex][3]);
    digitalWrite(sel0, selectSig[selectIndex][4]);
    digitalWrite(sel1, selectSig[selectIndex][5]);
    digitalWrite(sel2, selectSig[selectIndex][6]);

    //Send the inputs to the serial monitor digitized to the reference
    Serial.print(" ");
    Serial.print(compareAnalogRead(input0, ref, thresh));
    Serial.print(" ");
    Serial.print(compareAnalogRead(input1, ref, thresh));
    Serial.print(" ");
    Serial.print(compareAnalogRead(input2, ref, thresh));
    Serial.print(" ");
    Serial.print(compareAnalogRead(input3, ref, thresh));
    Serial.print(" ");
    Serial.print(compareAnalogRead(input4, ref, thresh));
    Serial.print(" ");
    Serial.print(compareAnalogRead(input5, ref, thresh));
    Serial.print(" ");
    Serial.print(compareAnalogRead(input6, ref, thresh));
    Serial.print(" ");
    Serial.print(compareAnalogRead(input7, ref, thresh));
    Serial.print("\n");

    selectIndex = (selectIndex + 1) % 32;
  }
  else {
    digitalWrite(CE0, HIGH);
    digitalWrite(CE1, HIGH);
    digitalWrite(CE2, HIGH);
    digitalWrite(CE3, HIGH);
    digitalWrite(sel0, LOW);
    digitalWrite(sel1, LOW);
    digitalWrite(sel2, LOW);
  }
}
