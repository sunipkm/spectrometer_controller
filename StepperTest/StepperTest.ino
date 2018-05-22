#include <Wire.h>
#include <Adafruit_MotorShield.h>

#define STEPMOT 1
int irPin1 = 2 ;
volatile byte state = FORWARD ;
volatile int stepMot = STEPMOT ;
volatile int motStop = 0 ;
int dispCount = 0 ;
volatile byte limitsw = 1 ;
// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("System up!");

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  myMotor->setSpeed(0.1);  // 10 rpm 
  pinMode(irPin1, INPUT) ; //setting pin irPin1 for limit switch 1
 
}

void loop() {
  check_irq();
  myMotor->step(stepMot, state, DOUBLE);
  if ( stepMot > 0 ) {
    dispCount = dispCount + 1 ;
  Serial.println(dispCount); }
  check_serial();
}

void check_irq()
{
    if ( digitalRead(irPin1) == HIGH && limitsw )
  { 
    Serial.println("Hit limit switch");
    limitsw = 0 ;
    chdir() ;
    myMotor->step(stepMot, state, DOUBLE);
    if ( stepMot > 0 )
     dispCount = dispCount - 1 ;
    stepMot = 0 ;
    motStop = 1 ;
  }
}

void check_serial()
{
  if ( motStop )
  {
    if (Serial.available() > 0)
    {
      char inCh = Serial.read() ;
      if ( inCh == 'h' )
      {
        stepMot = STEPMOT ;
        Serial.println("Going home...") ;
        Serial.println(dispCount);
        while (dispCount--)
          myMotor -> step(stepMot, state, DOUBLE) ;
        dispCount = 0 ;
        stepMot = 0 ;
        motStop = 1 ;
      }
      if ( inCh == 'd' ) {
      Serial.println("Changing state...");
      Serial.print("Initial state: ") ;
      Serial.println(state);
        chdir() ;
      Serial.print("Final state: ") ;
      Serial.println(state);
      }
      if ( inCh == 's' )
      {
        Serial.println("Restarting motor.");
        stepMot = STEPMOT ;
        motStop = 0 ;
        limitsw = 1 ;
      }
    }
  }
}

void chdir(){
  Serial.println("Changing dir...");
  if ( state == FORWARD )
    state = BACKWARD ;
  else if ( state == BACKWARD )
    state = FORWARD ;
}

