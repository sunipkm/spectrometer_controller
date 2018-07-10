#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 


// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);



/* System Globals */

//from eeprom
unsigned int __mem_m1 = 100 ; //default start address, you can think of rotating this periodically

unsigned long saved_loc = 0 ; //retrieved location

#define POWEROFF_SUCCESS 23

#define MAX_DISP_BOUND 403840

byte bootstat = 0 ;

//from main system
int STEPMOT = 1 ;
int irPin1 = 2 ;
int irPin2 = 3 ;
int irPin3 = 4 ;

volatile byte state = FORWARD ;
volatile unsigned int stepMot = 0 ;
volatile int motStop = 1 ;
volatile unsigned long dispCount = 0L ; //init to 0
volatile byte limitsw1 = 1 ;
volatile byte limitsw2 = 1 ;
volatile byte limitsw3 = 1 ;
volatile byte sRPM = 1 ;
volatile int hitOnce = 1 ;
volatile long dest = 0L ; //destionation
volatile byte ifDelay = 0 ;
volatile unsigned int delayVal = 0 ;

/***************************************/

/* Function Headers */
void ask_calibration(bool) ;
unsigned long eeprom_readlong(unsigned int addr ) ;
void eeprom_writelong(unsigned int addr , unsigned long n ) ;
void eepmem_clear() ;
bool eepmem_begin() ;
void eepmem_setup() ;
void eepmem_store() ;
void check_serial() ;
void check_irq1  () ;
void check_irq2  () ;
void check_irq3  () ;
void check_irq4  () ;
void chdir       () ;
unsigned long readInt(byte n) ; //number of digits to read

/***************************************/


/* Function Declarations */
#include "sk_eeprom.h"
#include "calibration.h"
#include "irqs.h"
#include "interaction.h"

/***************************************/
