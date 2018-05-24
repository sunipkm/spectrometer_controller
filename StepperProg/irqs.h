void check_irq1() //limitswitch 1
{
  if ( digitalRead(irPin1) == HIGH && limitsw1 ) //if the pin is set AND it has been hit the first time (to get rid of the multiple change direction problem)
  { 
    Serial.println("Hit limit switch 1 on irPin 1"); //let us know
    limitsw1 = 0 ; //so that doesn't enter the function even if the switch is high for quite a few loops
    chdir() ; //reverse, but in the real case should just change direction of motion
    stepMot = 0 ; //stop motor
    motStop = 1 ; //indicate that motor has stopped
  }
}


void check_irq2() //limitswitch 2
{
  if ( digitalRead(irPin2) == HIGH && limitsw2 ) //if the pin is set AND it has been hit the first time (to get rid of the multiple change direction problem)
  { 
    Serial.println("Hit limit switch 2 on irPin 2"); //let us know
    limitsw2 = 0 ; //so that doesn't enter the function even if the switch is high for quite a few loops
    chdir() ; //reverse, but in the real case should just change direction of motion
    stepMot = 0 ; //stop motor
    motStop = 1 ; //indicate that motor has stopped
  }
}

void check_irq3() //limitswitch 2
{
  if ( digitalRead(irPin3) == HIGH && limitsw3 ) //if the pin is set AND it has been hit the first time (to get rid of the multiple change direction problem)
  { 
    Serial.println("Hit stop switch on irPin 3"); //let us know
    limitsw3 = 0 ; //so that doesn't enter the function even if the switch is high for quite a few loops
    stepMot = 0 ; //stop motor
    motStop = 1 ; //indicate that motor has stopped
  }
}