/* 
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2 
---->	http://www.adafruit.com/products/1438
*/


#include <Wire.h>
#include <Adafruit_MotorShield.h>
int irPin1 = 2 ;
volatile byte state = FORWARD ;
volatile int stepMot = 20 ;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Stepper test!");

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  myMotor->setSpeed(20);  // 10 rpm 
  pinMode(irPin1, INPUT) ;
  //attachInterrupt(digitalPinToInterrupt(irPin), mot_stop, HIGH);
  //int i = 0 ;  
}

void loop() {
  Serial.println("Double coil steps");
  if ( digitalRead(irPin1) == HIGH )
  { if ( state == FORWARD )
      state = BACKWARD ;
    else if ( state == BACKWARD )
      state = FORWARD ;
    myMotor->step(stepMot, state, DOUBLE);
    stepMot = 0 ;
  }
  myMotor->step(stepMot, state, DOUBLE); 
  //myMotor->step(0, BACKWARD, DOUBLE);
//  
//  Serial.println("Interleave coil steps");
//  myMotor->step(500, FORWARD, INTERLEAVE); 
//  myMotor->step(0, BACKWARD, INTERLEAVE); 
  
  
// myMotor->step(250, FORWARD, MICROSTEP); 
//  myMotor->step(0, BACKWARD, MICROSTEP);
 // delay(1000);
//  if ( i > 2 )
//    while ( 1 ) {Serial.println("Delay");delay(10);}
}
void mot_stop(){
  if ( state == FORWARD )
    state = BACKWARD ;
  else if ( state == BACKWARD )
    state = FORWARD ;
}

