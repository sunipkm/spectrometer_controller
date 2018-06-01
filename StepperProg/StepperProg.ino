#include "function_headers.h"

void setup()
{
  Serial.begin(9600) ;
  Serial.println("System up!") ;

  pinMode(irPin1, INPUT) ; //setting pin irPin1 for limit switch 1
  pinMode(irPin2, INPUT) ;
  pinMode(irPin3, INPUT) ;

  AFMS.begin() ;

  Serial.println("Press any key to continue...") ;
  Serial.flush() ;
  while ( Serial.available() < 1 ) ;
  
  eepmem_setup() ;

  Serial.flush() ;
  
  Serial.println("Setup complete.");
}

void loop()
{
  check_irq1() ; check_irq2() ; check_irq3() ;
  //Serial.println(dest);
  //Serial.println(dispCount);
  if ( dest != dispCount )
  {
    myMotor -> step (stepMot , state , DOUBLE ) ; //double coil
    dispCount = dispCount + ( ( state == BACKWARD ) ? -stepMot : stepMot ) ; 
  }
  else if ( dest == dispCount )
  {
    stepMot = 0 ; motStop = 1 ; Serial.print("Arrived at ") ; Serial.println(dispCount) ;
  }
  check_serial() ;
}

