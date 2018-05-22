#include <Wire.h>
#include <Adafruit_MotorShield.h>

#define STEPMOT 1
int irPin1 = 2 ;
int irPin2 = 3 ;
volatile byte state = FORWARD ;
volatile int stepMot = STEPMOT ;
volatile int motStop = 0 ;
int dispCount = 0 ;
volatile byte limitsw1 = 1 ;
volatile byte limitsw2 = 1 ;
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
  check_irq1();
  check_irq2();
  myMotor->step(stepMot, state, DOUBLE);
  if ( stepMot > 0 ) {
    dispCount = dispCount + 1 ;
  Serial.println(dispCount); }
  check_serial();
}

void check_irq1() //limitswitch 1
{
    if ( digitalRead(irPin1) == HIGH && limitsw1 ) //if the pin is set AND it has been hit the first time (to get rid of the multiple change direction problem)
  { 
    Serial.println("Hit limit switch"); //let us know
    limitsw1 = 0 ; //so that doesn't enter the function even if the switch is high for quite a few loops
    chdir() ; //reverse, but in the real case should just change direction of motion
    myMotor->step(stepMot, state, DOUBLE); //step back a bit
    if ( stepMot > 0 )
     dispCount = dispCount - 1 ; //displaced by one step backward the first time
    stepMot = 0 ; //stop motor
    motStop = 1 ; //indicate that motor has stopped
  }
}
void check_irq2() //limitswitch 1
{
    if ( digitalRead(irPin2) == HIGH && limitsw2 ) //if the pin is set AND it has been hit the first time (to get rid of the multiple change direction problem)
  { 
    Serial.println("Hit limit switch"); //let us know
    limitsw1 = 0 ; //so that doesn't enter the function even if the switch is high for quite a few loops
    chdir() ; //reverse, but in the real case should just change direction of motion
    myMotor->step(stepMot, state, DOUBLE); //step back a bit
    if ( stepMot > 0 )
     dispCount = dispCount - 1 ; //displaced by one step backward the first time
    stepMot = 0 ; //stop motor
    motStop = 1 ; //indicate that motor has stopped
  }
}

void check_serial() //check serial for input
{
  if ( motStop ) //only when motor is stopped
  {
    if (Serial.available() > 0) //if only serial input is available
    {
      char inCh = Serial.read() ; //read char from serial
      if ( inCh == 'h' ) //go home
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
      if ( inCh == 'd' ) //change direction 
      {
        //Serial.println("Changing state...");
        Serial.print("Initial state: ") ;
        Serial.println(state);
        chdir() ;
        Serial.print("Final state: ") ;
        Serial.println(state);
      }
      if ( inCh == 's' ) //restart motor
      {
        Serial.println("Restarting motor.");
        stepMot = STEPMOT ;
        motStop = 0 ;
        limitsw1 = 1 ;
      }
    }
  }
}

void chdir()
{
  Serial.println("Changing dir...");
  if ( state == FORWARD )
    state = BACKWARD ;
  else if ( state == BACKWARD )
    state = FORWARD ;
}

