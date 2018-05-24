void ask_calibration()
{
	calib_begin: Serial.flush() ;
	Serial.println(F("Entering calibration mode. Enter 1 to move towards switch 1, enter 2 to move towards switch 2 to obtain location calibration:")) ;
	while(Serial.available()<1) ;
	byte dir = Serial.parseInt() ;
	switch(dir)
	{
		case FORWARD :
		{
			Serial.println(F("Towards Limit Switch 2: ")) ;
		}
		break ;

		case BACKWARD :
		{
			Serial.println(F("Towards Limit Switch 1: ")) ;
		}
		break ;

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
		Serial.println("Hit switch 1") ;
		saved_loc = 0 ;
		myMotor->step(4000,FORWARD,DOUBLE) ;
		saved_loc = 4000 ;
	}
	else if ( digitalRead(irPin1) == HIGH ){
		Serial.println("Hit switch 2") ;
		saved_loc = 403840 ;
		myMotor->step(4000,BACKWARD,DOUBLE) ;
		saved_loc -= 4000 ;
	}
	else
		goto calib_begin ;
	dispCount = saved_loc ; //current displacement
	return ;	
}