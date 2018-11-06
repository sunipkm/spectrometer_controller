void check_serial() //check serial for input
{
  while ( Serial.available() > 0 ) 
    Serial.read() ;
  if ( motStop ) //only when motor is stopped
  {
    if ( hitOnce ){
      Serial.println();
      Serial.print(11);Serial.print("_");//("Current location: ") ;
      Serial.println(dispCount);
      Serial.println() ;
      Serial.println(12);//(F("Options: (h)ome, (s)et destination, (p)reset motor speed, (c)alibration, (d)elay, (q)uit\n"));
//    if ( state == FORWARD )
//        Serial.println(F("Currently moving towards LS 2"));
//    if (state == BACKWARD )
//        Serial.println(F("Currently moving towards LS 1"));
      hitOnce = 0 ;
    }
    
    //Serial.flush();
    
    while (Serial.available() < 1 ) ; //if only serial input is available
    char inCh = Serial.read() ; //read char from serial
    Serial.print(13);Serial.print("_");//(F("You selected: ")) ;
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
      case 'h' : //go home
      {
        stepMot = STEPMOT ;
        Serial.println(14);//(F("Going home...")) ;
        Serial.print(15);Serial.print("_");//(F("Current location: ")) ;
        Serial.println(dispCount);

        if ( dispCount > 201920 )
        {
          state = BACKWARD ;
          Serial.println(16);//(F("Moving towards Limit Switch 1")) ;
        }
        if ( dispCount < 201920 )
        {
          state = FORWARD ;
          Serial.println(17);//(F("Moving towards Limit Switch 2")) ;
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
      case 'd': //insert delay 
      {
//        //Serial.println("Changing state...");
//        Serial.print(F("Initial state: ")) ;
//        Serial.println(state);
//        chdir() ;
//        Serial.print(F("Final state: ")) ;
//        Serial.println(state);
//        Serial.flush();
        Serial.println(18);//(F("Enter y to set delay, n to turn off delay.")) ;
        while ( Serial.available() < 1 ) ;
        char inCh1 = Serial.read() ;
        switch ( inCh1 ) {
          default:
            break ;
          case 'y' :
          {
            ifDelay = 1 ;
            do {
            Serial.println(19);//(F("The delay is in integer (ms) < 2000 ms, in 4 digits.")) ;
            while(Serial.available()<1);
            delayVal = Serial.parseInt();//readInt(4) ;
            } while ( delayVal > 2000 ) ; 
          }

            break ;
          case 'n' :
          {
            ifDelay = 0 ;
          }

            break ;
        }
        hitOnce = 1 ;
      }
      break ;
      case 's': //set destination
      {
        //while (Serial.available()>0);
        Serial.println(20);//(F("Provide Destination (6 Digits): "));
        while(Serial.available()<1);
        dest = Serial.parseInt();//readInt(6) ;
        Serial.print(21);Serial.print("_");//(F("You have chosen: "));
        Serial.println(dest) ;
        if ( dest < 2000 || dest > MAX_DISP_BOUND-6000 )
        {
          Serial.println(22);//(F("Too close to either limits. Please enter new destinations. You can not go more than 2000 steps closer to the limits."));
          stepMot = 0 ;
          motStop = 1 ;
        }
        else
        {
          stepMot = 1 ;
          motStop = 0 ;
        }
        state = dest < dispCount ? BACKWARD : FORWARD ;
        if ( state == FORWARD )
          Serial.println(23);//(F("Moving towards Limit Switch 2")) ;
        if ( state == BACKWARD )
          Serial.println(24);//(F("Moving towards Limit Switch 1")) ;
        limitsw1 = 1 ;
        limitsw2 = 1 ;
        limitsw3 = 1 ;
        Serial.flush();
        hitOnce = 1 ;
      }
      break ;
      case 'p': //set speed (almost ineffective :3 )
      {
        Serial.flush();
        while (Serial.available()>0);
        Serial.println(25);//(F("Set RPM (3 digits): "));
        ///while (Serial.available() < 3 );
        while(Serial.available()<1);
        sRPM = Serial.parseInt();
        if ( sRPM > 100 || sRPM < 1 )
          sRPM = 1 ;
        Serial.print(26);Serial.print("_");//(F("RPM: "));
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
        Serial.println(27);//(F("Quitting...")) ;
        saved_loc = dispCount ;
        eepmem_store() ;
        Serial.println(28);//(F("Updated current location in EEPROM.")) ;
        myMotor->release() ;
        Serial.println(29);//(F("Released motor.")) ;
        Serial.println(30);//(F("It is now safe to unplug the Arduino.")) ;
      }
      break ;
      }
    //}
  }
}

void chdir()
{
  Serial.println(31);//("Changing dir...");
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
    input = "" ;
    test = 0 ;
    byte k = n ;
    while ( k-- )
    {
      while ( Serial.available() < 1 ) ;
      char t = Serial.read() ;
      Serial.flush() ;
      if ( t < 48 || t > 57 )
      {
        Serial.println(32);//("\nInvalid number, try again.") ;
        test = 1 ;
        break ;
      }
      else{
        input += t ;
        //Serial.print(t) ;
      }
    }
  } while ( test ) ;
  //Serial.println() ;
  return input.toInt() ;
}

