// constants won't change. They're used here to set pin numbers and motor speed values:


// variables will change:
byte x = 0;   //variable for serial input

void getDataFromPC();
void analyze_byte();

void setup() {        // put your setup code here, to run once:
  // initialize the input/output pins:
  pinMode(LED_BUILTIN, OUTPUT); 
  // set up Serial library at 9600 bps
  Serial.begin(9600);
}
void loop() {  
  // put your main code here, to run repeatedly:
  getDataFromPC();
  analyze_byte(); 
}

void getDataFromPC() {
    // receive data from PC and save it into inputBuffer
  if(Serial.available() > 0) {
    x = Serial.read();
    //digitalWrite(LED_BUILTIN, HIGH);
  }
  
 // if(x==2)   {    //bit 1 is set
    //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on
 // }
}

void analyze_byte() {

  if(x==2)  {    //bit 2 is set
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on
  }
  else  {
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off
  }
}
