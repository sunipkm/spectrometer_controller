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
  Serial.read() ;
  eepmem_setup() ;

  Serial.flush() ;
  while ( Serial.available() > 0 )
    Serial.read() ;
  Serial.println("Setup complete.");
}

void loop()
{
  check_irq1() ; check_irq2() ; check_irq3() ; check_irq4() ;
  //Serial.println(dest);
  //Serial.println(dispCount);
  if ( dest != dispCount && stepMot != 0 )
  {
    myMotor -> step (stepMot , state , DOUBLE ) ; //double coil
    if ( state == BACKWARD )
      dispCount -- ;
    else if ( state == FORWARD )
      dispCount ++ ; 
  }
  else if ( dest == dispCount && stepMot != 0 )
  {
    stepMot = 0 ; motStop = 1 ; Serial.print("Arrived at ") ; Serial.println(dispCount) ;
  }
  check_serial() ;
}

