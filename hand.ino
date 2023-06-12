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

//delay constant between each select signal
const int  wait = 10;

//threshold voltage above the reference that counts as a 1
//it has 1023 steps between 0 to 5V so 205 is about 1V
const int thresh = 205;

//varaibles
bool Go = false;

int selectIndex = 0;

int avg;

int refv;

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
  int referenceValue = analogRead(referencePin) + threshold;
  if (inputValue > referenceValue) {
    return 1;
  } else {
    return 0;
  }
}



void setup() {
  //est connection between arduino and computer
  Serial.begin(9600);
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
}

void loop() {

  //Type g in serial monitor to start and stop
  if (Serial.available()) {
    char gatherData = Serial.read();
    if (gatherData == 'g') {
      Go = !Go;
    }
  }
  //if collecting the LED is on else its off
  if (Go == true) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else if (Go == false) {
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
    Serial.print(compareAnalogRead(input0, ref, thresh));
    Serial.print(compareAnalogRead(input1, ref, thresh));
    Serial.print(compareAnalogRead(input2, ref, thresh));
    Serial.print(compareAnalogRead(input3, ref, thresh));
    Serial.print(compareAnalogRead(input4, ref, thresh));
    Serial.print(compareAnalogRead(input5, ref, thresh));
    Serial.print(compareAnalogRead(input6, ref, thresh));
    Serial.print(compareAnalogRead(input7, ref, thresh));
    Serial.print("\n");
    


    avg = ((analogRead(input0) + analogRead(input1) + analogRead(input2) + analogRead(input3) + analogRead(input4) + analogRead(input5) +analogRead(input6) + analogRead(input7))/8);
    Serial.print(avg);
    Serial.print("\n");
    
    Serial.print(analogRead(ref));
    Serial.print("\n");
    

    selectIndex = selectIndex + 1;
    if (selectIndex == 32){
      selectIndex = 0;
    }
    delay(wait);
  }
  else {
    Serial.println("111111111111111");
    digitalWrite(CE0, HIGH);
    digitalWrite(CE1, HIGH);
    digitalWrite(CE2, HIGH);
    digitalWrite(CE3, HIGH);
    digitalWrite(sel0, LOW);
    digitalWrite(sel1, LOW);
    digitalWrite(sel2, LOW);
    delay(wait);
  }

}

























