#include <Wire.h>

#include "function_headers.h"

void setup()
{
  Serial.begin(115200) ;
  Serial.println(1);//("System up!") ;

  pinMode(irPin1, INPUT) ; //setting pin irPin1 for limit switch 1
  pinMode(irPin2, INPUT) ;
  pinMode(irPin3, INPUT) ;

  pinMode(A0, INPUT) ; //photoresistor sense pin

  AFMS.begin() ;

  Serial.println(2);//("Press any key to continue...") ;
  Serial.flush() ;
  while ( Serial.available() < 1 ) ;
  while ( Serial.available() > 0 )
    Serial.read() ;
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
    //read 50 data points at step
    byte tcount = 50 ;
    long res = 0 , res2 = 0;
    if(spitData)
    {
      while(tcount--){
      delayMicroseconds(50);
      unsigned int val = analogRead(A0);
      res += val ;
      //res2 = res2*res2 ;
      res2 += val*val ;}
      //res2 = sqrt(res2) ;
    
    Serial.print(40);Serial.print("_");Serial.print(dispCount);Serial.print("_");Serial.print(res/50.);Serial.print("_");Serial.println(res2/50.);
    }
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
