#include <Wire.h>
#include <Adafruit_MotorShield.h>

int STEPMOT = 20 ;
int irPin1 = 2 ;
int irPin2 = 3 ;
int irPin3 = 4 ;
volatile byte state = FORWARD ;
volatile unsigned int stepMot = 0 ;
volatile int motStop = 1 ;
unsigned long dispCount = 0L ; //init to 0
volatile byte limitsw1 = 1 ;
volatile byte limitsw2 = 1 ;
byte sRPM = 1 ;
int hitOnce = 1 ;
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
  
  myMotor->setSpeed(sRPM);  // 10 rpm 
  pinMode(irPin1, INPUT) ; //setting pin irPin1 for limit switch 1
  pinMode(irPin2, INPUT) ;
 
}

void loop() 
{
  check_irq3();
  check_irq1();
  check_irq2();
  myMotor->step(stepMot, state, DOUBLE);
  dispCount = dispCount + ( ( state == BACKWARD ) ? -stepMot : stepMot ) ; 
  if ( stepMot > 0 )
  dispCount.print();
  check_serial();
}

void check_irq1() //limitswitch 1
{
    if ( digitalRead(irPin1) == HIGH && limitsw1 ) //if the pin is set AND it has been hit the first time (to get rid of the multiple change direction problem)
  { 
    Serial.println("Hit limit switch 1 on irPin 1"); //let us know
    limitsw1 = 0 ; //so that doesn't enter the function even if the switch is high for quite a few loops
    chdir() ; //reverse, but in the real case should just change direction of motion
//    myMotor->setSpeed(100); //reverse at 200 rpm
//    myMotor->step(4000, FORWARD, DOUBLE); //step back 3000 steps
//    if ( stepMot > 0 )
//     dispCount = dispCount - 4000 ; //displaced by one step backward the first time
    stepMot = 0 ; //stop motor
    motStop = 1 ; //indicate that motor has stopped
//    myMotor->setSpeed(sRPM);
  }
}
void check_irq2() //limitswitch 1
{
    if ( digitalRead(irPin2) == HIGH && limitsw2 ) //if the pin is set AND it has been hit the first time (to get rid of the multiple change direction problem)
  { 
    Serial.println("Hit limit switch 2 on irPin 2"); //let us know
    limitsw2 = 0 ; //so that doesn't enter the function even if the switch is high for quite a few loops
    chdir() ; //reverse, but in the real case should just change direction of motion
//    myMotor->setSpeed(100);
//    myMotor->step(4000, BACKWARD, DOUBLE); //step back a bit
//    if ( stepMot > 0 )
//     dispCount = dispCount + 4000 ; //displaced by one step backward the first time
    stepMot = 0 ; //stop motor
    motStop = 1 ; //indicate that motor has stopped
//    myMotor->setSpeed(sRPM);
  }
}

void check_serial() //check serial for input
{
  if ( motStop ) //only when motor is stopped
  {
    if ( hitOnce ){
      Serial.println("Options: (h)ome, (d)irection, (s)tart, (p)reset");
          if ( state == FORWARD )
        Serial.println("Towards LS 2");
    if (state == BACKWARD )
        Serial.println("Towards LS 1");
      hitOnce = 0 ;
    }
      Serial.flush();
    if (Serial.available() > 0 ) //if only serial input is available
    {
      char inCh = Serial.read() ; //read char from serial
      SWITCH_HEAD: Serial.flush() ;
      switch ( inCh ) {
      default: goto SWITCH_HEAD ;
      break ;
      case 'h' : //go home //needs to be modified severely
      {
        stepMot = STEPMOT ;
        Serial.println(F("Going home...")) ;
        dispCount.print();
        while (!(dispCount==0)){
          myMotor -> step(stepMot, state, DOUBLE) ;
          --dispCount ;
        }
        dispCount = 0 ;
        stepMot = 0 ;
        motStop = 1 ;
        Serial.flush();
        hitOnce = 1 ;
      }
      break ;
      case 'd': //change direction 
      {
        //Serial.println("Changing state...");
        Serial.print(F("Initial state: ")) ;
        Serial.println(state);
        chdir() ;
        Serial.print(F("Final state: ")) ;
        Serial.println(state);
        Serial.flush();
         hitOnce = 1 ;
      }
      break ;
      case 's': //restart motor
      {
        Serial.println(F("Restarting motor."));
        stepMot = STEPMOT ;
        motStop = 0 ;
        limitsw1 = 1 ;
        limitsw2 = 1 ;
        Serial.flush();
         hitOnce = 1 ;
      }
      break ;
      case 'p': //restart motor
      {
        Serial.println(F("Set step (2 digits): "));
        Serial.flush();
        while (Serial.available() < 2 );
        STEPMOT = Serial.parseInt() ;
        Serial.flush();
        Serial.println(F("Set RPM (3 digits): "));
        while (Serial.available() < 3 );
        sRPM = Serial.parseFloat() ;
        if ( sRPM > 200 )
          sRPM = 1 ;
        Serial.print(F("Step: "));
        Serial.println(STEPMOT);
         Serial.print(F("RPM: "));
        Serial.println(sRPM);
        myMotor->setSpeed(sRPM);
        stepMot = STEPMOT ;
        motStop = 0 ;
        limitsw1 = 1 ;
        limitsw2 = 1 ;
        Serial.flush();
         hitOnce = 1 ;
      }
      break ;
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

