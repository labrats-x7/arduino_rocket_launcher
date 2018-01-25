#include <AFMotor.h>


// constants won't change. They're used here to set pin numbers and motor speed values:
const int azi_left_stop = A2;     // the number of the azimuth left stop switch pin
const int azi_right_stop = A1;    // the number of the azimuth right stop switch pin
const int ele_down_stop = A4;     // the number of the elevation down stop switch pin
const int ele_up_stop = A3;       // the number of the elevation up stop switch pin
//const int rocket1 = 2;      // the number of the "rocktet1 inserted" pin
//const int rocket2 = 13;     // the number of the "rocktet2 inserted" pin
//const int rocket3 = A5;     // the number of the "rocktet3 inserted" pin
const int fire_state_pin = A0;  // the number of the fire_state pin for the 3 round fire mechanism
const int spd_low = 150;    // low speed value
const int spd_high = 255;   // high speed value


// variables will change:
int azi_left_stop_state = 0;         // variable for reading the azimuth left stop switch status
int azi_right_stop_state = 0;         // variable for reading the azimuth right stop switch status
int ele_down_stop_state = 0;         // variable for reading the elevation down stop switch status
int ele_up_stop_state = 0;         // variable for reading the elevation up stop switch status
int spd = 0;    // variable for speed: 0 -> low, 1 -> high, for values see constants
int spd_var = 100;    // variable for speed value
int go_azi_left = 0;   // variable for azimuth turn left command
int go_azi_right = 0;   // variable for azimuth turn right command
int go_ele_down = 0;   // variable for elevation turn down command
int go_ele_up = 0;   // variable for elevation turn up command
int fire_shot = 0;   // variable for rocket fire signal status
int fire_state = 0; // variable for fire mechanics state
int fire_state_old = 1; // variable for signal detection
char x = 0;   //variable for serial input
int fail = 0;   // variable for validating connection status
int fire_initial_pos = 0; //variable for positioning fire mechanics at first startup


void getDataFromPC();
void analyze_byte();
void setspeed();
void azimuth();
void elevation();
void fire();
//void sec();


// unsigned long currentMillis = 0;
// unsigned long previousMillis = 0;        // will store last time data was recieved


AF_DCMotor azi_motor(1, MOTOR12_8KHZ);  // azimuth motor on port 1
AF_DCMotor ele_motor(2, MOTOR12_8KHZ);  // elevation motor on port 2
AF_DCMotor fire_motor(3);   // fire motor on port 3


void setup() {    // put your setup code here, to run once:

  // initialize the input/output pins:
  pinMode(azi_left_stop, INPUT);
  pinMode(azi_right_stop, INPUT);
  pinMode(ele_down_stop, INPUT);
  pinMode(ele_up_stop, INPUT);
  pinMode(fire_state_pin, INPUT);
  // pinMode(rocket1, INPUT);
  // pinMode(rocket2, INPUT);
  // pinMode(rocket3, INPUT);
  
  // set up Serial library at 9600 bps
  Serial.begin(9600);
  //Serial.println("Arduino is ready");
}


void loop() {
  
  getDataFromPC();
  analyze_byte();
  setspeed();
  azimuth();
  elevation();
  fire();
//  sec();
}


void getDataFromPC() {

    // receive data from PC and save it into inputBuffer
  if(Serial.available() > 0) {
    x = Serial.read();
  }
}


void analyze_byte() {

  if(x&(1<<0))   {    //bit 0 is set
    spd = 1;
  }
  else  {
    spd = 0;
  }
  
  if(x&(1<<1))   {    //bit 1 is set
    go_azi_left = 1;
  }  
  else {
    go_azi_left = 0;
  }
  
  if(x&(1<<2))  {    //bit 2 is set
    go_azi_right = 1;
  }
  else  {
    go_azi_right = 0;
  }

  if(x&(1<<3))  {    //bit 3 is set
    go_ele_down = 1;
  }
  else  {
    go_ele_down = 0;
  }  

  if(x&(1<<4))  {    //bit 4 is set
    go_ele_up = 1;
  }
  else  {
    go_ele_up = 0;
  }

  if(x&(1<<5))   {    //bit 5 is set
    fire_shot = 1;
  }
  else  {
    fire_shot = 0;
  }
}


void setspeed() {
  if(spd == 1)  {
    spd_var = spd_high;
  }
  if(spd == 0)  {
    spd_var = spd_low;
  }
}


void azimuth()  {

    azi_left_stop_state = digitalRead(azi_left_stop);
    azi_right_stop_state = digitalRead(azi_right_stop);
  
    // turn left
    if (go_azi_left == 1 && azi_left_stop_state == 0) {
      azi_motor.run(RELEASE);  // turn on azimuth motor
      azi_motor.setSpeed(spd_var);
      azi_motor.run(FORWARD);
      //Serial.println("azimuth left");
    }

    // turn right
    if (go_azi_right == 1 && azi_right_stop_state == 0) {
      azi_motor.run(RELEASE);  // turn on azimuth motor
      azi_motor.setSpeed(spd_var);
      azi_motor.run(BACKWARD);
      //Serial.println("azimuth right");
    }
    
    // left end stop
    if (go_azi_left == 1 && azi_left_stop_state == 1) {
      go_azi_left = 0;
    }

    // right end stop
    if (go_azi_right == 1 && azi_right_stop_state == 1) {
      go_azi_right = 0;
    }
    
    // power down azimuth motor
    if (go_azi_left == 0 && go_azi_right == 0)  {
      azi_motor.run(RELEASE);
    }
}


void elevation()  {

    ele_down_stop_state = digitalRead(ele_down_stop);
    ele_up_stop_state = digitalRead(ele_up_stop);
  
    // turn down
    if (go_ele_down == 1 && ele_down_stop_state == 0) {
      ele_motor.run(RELEASE);    // turn on elevation motor
      ele_motor.setSpeed(spd_var);
      ele_motor.run(FORWARD);
      //Serial.println("elevation down");
    }

    // turn up
    if (go_ele_up == 1 && ele_up_stop_state == 0) {
      ele_motor.run(RELEASE);    // turn on elevation motor
      ele_motor.setSpeed(spd_var);
      ele_motor.run(BACKWARD);
      //Serial.println("elevation up");
    }

    // lower end stop
    if (go_ele_down == 1 && ele_down_stop_state == 1) {
      go_ele_down = 0;
    }

    // upper end stop
    if (go_ele_up == 1 && ele_up_stop_state == 1) {
      go_ele_up = 0;
    }
    
    // power down elevation motor
    if (go_ele_down == 0 & go_ele_up == 0)  {
      ele_motor.run(RELEASE);
    }
}


void fire()  {
  
fire_state = digitalRead(fire_state_pin);

  // move fire mechanism in correct position if out of initial position (powerdown while fireing)
  if (fire_initial_pos == 0 && fire_state == 0)  {
    fire_motor.run(RELEASE);    // turn on fire motor
    fire_motor.setSpeed(255);
    fire_motor.run(FORWARD);
    //Serial.println("fire_Motor initial run");
    fire_initial_pos = 1;
  }

  if (fire_shot == 1 && fire_state == 1) {
    fire_motor.run(RELEASE);    // turn on fire motor
    fire_motor.setSpeed(255);
    fire_motor.run(FORWARD);
    //Serial.println("fire_Motor running");
  }
  
  if (fire_state == 0) {
    fire_state_old = fire_state;
  }
  
  // power down fire motor
  if (fire_state == 1 && fire_state_old == 0)  {
    delay(100); // move fire mechaics a bit further to prevent the switch not to be pressed enough
    fire_motor.run(RELEASE);
    fire_state_old = fire_state;
  }
}

/*
void sec()  {

  if(millis() >= previousMillis + 200) {
        // shut down the motors
      azi_motor.run(RELEASE);
      ele_motor.run(RELEASE);
      fail = 1;
  }
    if(millis() < previousMillis + 200) {
      fail = 0;
  }
}
*/
