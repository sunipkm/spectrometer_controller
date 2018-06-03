void check_serial() //check serial for input
{
  Serial.flush() ;
  if ( motStop ) //only when motor is stopped
  {
    if ( hitOnce ){
      Serial.println();
      Serial.print("Current location: ") ;
      Serial.println(dispCount);
      Serial.println() ;
    if ( (!limitsw1)||(!limitsw2))
      Serial.println(F("Options: (h)ome, (s)et destination"));
      
      Serial.println(F("Options: (h)ome, (d)irection, (s)et destination, (p)reset motor speed, (c)alibration, (q)uit\n"));
    if ( state == FORWARD )
        Serial.println(F("Currently moving towards LS 2"));
    if (state == BACKWARD )
        Serial.println(F("Currently moving towards LS 1"));
      hitOnce = 0 ;
    }
    
    //Serial.flush();
    
    while (Serial.available() < 1 ) ; //if only serial input is available
    char inCh = Serial.read() ; //read char from serial
    Serial.print(F("You selected: ")) ;
    Serial.println(inCh) ;
    Serial.flush() ;
    
    switch ( inCh ) {
      
      default:
      {
        stepMot = 0 ;
        motStop = 1 ;
        hitOnce = 1 ;
      }
      break ;
      case 'c' :
      {
        ask_calibration(true);
        stepMot = 0 ;
        motStop = 1 ;
        hitOnce = 1 ;
      }
      break ;
      case 'h' : //go home //needs to be modified severely
      {
        stepMot = STEPMOT ;
        Serial.println(F("Going home...")) ;
        Serial.println(dispCount);

        if ( dispCount > 201920 )
        {
          state = BACKWARD ;
        }
        if ( dispCount < 201920 )
        {
          state = FORWARD ;
        }
        dest = 201920 ; //middle
        stepMot = 1 ;
        motStop = 0 ;
        myMotor->setSpeed(100) ; //move at 100 rpm and fall back to loop
        Serial.flush();
        hitOnce = 1 ;
        limitsw1 = 1 ;
        limitsw2 = 1 ;
        limitsw3 = 1 ;
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
      case 's': //set destination
      {
        Serial.println(F("Provide Destination (6 Digits): "));
        dest = readInt(6) ;
        Serial.print(F("You have chosen: "));
        Serial.println(dest) ;
        if ( dest < 100 || dest > MAX_DISP_BOUND-100 )
        {
          Serial.println(F("Too close to either limits. Please enter new destinations. You can not go more than 100 steps closer to the limits."));
          stepMot = 0 ;
          motStop = 1 ;
        }
        else
        {
          stepMot = 1 ;
          motStop = 0 ;
        }
        state = dest < dispCount ? BACKWARD : FORWARD ;
        limitsw1 = 1 ;
        limitsw2 = 1 ;
        limitsw3 = 1 ;
        Serial.flush();
        hitOnce = 1 ;
      }
      break ;
      case 'p': //set coordinates
      {
        Serial.flush();
        Serial.println(F("Set RPM (3 digits): "));
        ///while (Serial.available() < 3 );
        sRPM = readInt(3);
        if ( sRPM > 100 || sRPM < 1 )
          sRPM = 1 ;
        Serial.print(F("RPM: "));
        Serial.println(sRPM);
        myMotor->setSpeed(sRPM);
        Serial.flush();
        hitOnce = 1 ;
        limitsw1 = 1 ;
        limitsw2 = 1 ;
        limitsw3 = 1 ;
      }
      break ;
      case 'q': //quitting
      {
        Serial.flush() ;
        Serial.println(F("Quitting...")) ;
        saved_loc = dispCount ;
        eepmem_store() ;
        myMotor->release() ;
        Serial.println(F("It is now safe to unplug the Arduino.")) ;
      }
      break ;
      }
    //}
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

unsigned long readInt(byte n)
{
  String input = "" ;
  Serial.flush() ;
  byte test = 0 ;
  do {
    test = 0 ;
    byte k = n ;
    while ( k-- )
    {
      while ( Serial.available() < 1 ) ;
      char t = Serial.read() ;
      Serial.flush() ;
      if ( t < 48 || t > 57 )
      {
        Serial.println("\nInvalid number, try again.") ;
        test = 1 ;
        break ;
      }
      else{
        input += t ;
        Serial.print(t) ;
      }
    }
  } while ( test ) ;
  Serial.println() ;
  return input.toInt() ;
}

