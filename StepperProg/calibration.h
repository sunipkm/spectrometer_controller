void ask_calibration(bool needed)
{
  if ( needed ) {
  calib_begin: Serial.flush() ;
  Serial.print(5);//(F("Entering calibration mode. Enter 2 to move towards switch 1, enter 1 to move towards switch 2 to obtain location calibration:")) ;
  Serial.print("_"); Serial.println(saved_loc);
  while(Serial.available()<1) ;
  byte dir = Serial.parseInt() ;
  switch(dir)
  {
    case FORWARD :
    {
      Serial.println(6);//(F("Towards Limit Switch 2: ")) ;
    }
    break ;

    case BACKWARD :
    {
      Serial.println(7);//(F("Towards Limit Switch 1: ")) ;
    }
    break ;

    case 0 :
        goto calib_end ;
        break ;
    case 3 :
        Serial.println(39);
        while(Serial.available()<1);
        saved_loc = Serial.parseInt();
        goto calib_end;

    default:
      goto calib_begin ;
    break ;
  }
  myMotor -> setSpeed(100) ; //100 rpm
  while ( true )
  {
    if ( digitalRead(irPin1) == HIGH )
      break ;
    if ( digitalRead(irPin2) == HIGH )
      break ;
    if ( digitalRead(irPin3) == HIGH )
      break ;
    myMotor->step(1,dir,DOUBLE) ;
  }
  if (digitalRead(irPin1) == HIGH )
  {
    Serial.println(8);//("Hit switch 1") ;
    saved_loc = 0 ;
    myMotor->step(4000,FORWARD,DOUBLE) ;
    saved_loc = 4000 ;
  }
  else if ( digitalRead(irPin2) == HIGH ){
    Serial.println(9);//("Hit switch 2") ;
    saved_loc = 403840 ;
    myMotor->step(4000,BACKWARD,DOUBLE) ;
    saved_loc -= 4000 ;
  }
  else
    goto calib_begin ;
  } //end if needed
  calib_end: dispCount = saved_loc ; //either after calibration or from memory, dispCout = saved_loc
  Serial.print(10);Serial.print("_");//("In calibration: Current location: ");
  Serial.println(dispCount);
  return ;  
}
