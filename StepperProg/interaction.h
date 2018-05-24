void check_serial() //check serial for input
{
  if ( motStop ) //only when motor is stopped
  {
    if ( hitOnce ){
      Serial.println("Options: (h)ome, (d)irection, (s)tart, (p)reset, (c)alibration, (q)uit");
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

        if ( dispCount > 201920 )
        {
          state = BACKWARD ;
        }
        if ( dispCount < 201920 )
        {
          state = FORWARD ;
        }
        stepMot = 1 ;
        motStop = 0 ;
        myMotor->setSpeed(100) ; //move at 100 rpm and fall back to loop
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
      case 's': //set destination
      {
        Serial.println(F("Provide Destination (6 Digits): "));
        while (Serial.available() < 6 );
        dest = Serial.parseInt() ;
        Serial.print(F("You have chosen: "));
        Serial.println(dest) ;
        stepMot = STEPMOT ;
        motStop = 0 ;
        limitsw1 = 1 ;
        limitsw2 = 1 ;
        Serial.flush();
        hitOnce = 1 ;
      }
      break ;
      case 'p': //set coordinates
      {
        Serial.flush();
        Serial.println(F("Set RPM (3 digits): "));
        while (Serial.available() < 3 );
        sRPM = Serial.parseInt() ;
        if ( sRPM > 100 )
          sRPM = 1 ;
        Serial.print(F("RPM: "));
        Serial.println(sRPM);
        myMotor->setSpeed(sRPM);
        Serial.flush();
        Serial.println(F("Set Destination (6 digits): "));
        while (Serial.available() < 6 );
        dest = Serial.parseInt() ;
        Serial.print(F("You have chosen: "));
        Serial.println(dest) ;
        stepMot = 1 ;
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