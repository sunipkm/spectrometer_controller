#include "function_headers.h"

void setup()
{
  Serial.begin(9600) ;
  Serial.println(1);//("System up!") ;

  pinMode(irPin1, INPUT) ; //setting pin irPin1 for limit switch 1
  pinMode(irPin2, INPUT) ;
  pinMode(irPin3, INPUT) ;

  AFMS.begin() ;

  Serial.println(2);//("Press any key to continue...") ;
  Serial.flush() ;
  while ( Serial.available() < 0 ) ;
  eepmem_setup() ;

  Serial.flush() ;
  while ( Serial.available() > 0 )
    Serial.read() ;
  Serial.println(3);//("Setup complete.");
}

void loop()
{
  check_irq1() ; check_irq2() ; check_irq3() ;
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
    stepMot = 0 ; motStop = 1 ; 
    Serial.print(4);Serial.print("_");//("Arrived at ") ; 
    Serial.println(dispCount) ;
  }
  if ( ifDelay )
    delay ( delayVal ) ;
  check_serial() ;
}

