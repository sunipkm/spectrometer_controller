#include <EEPROM.h>

unsigned long eeprom_readlong(unsigned int addr )
{
	byte a[4] ; //4 bytes
	for ( byte i = 0 ; i < 4 ; i++ ){
		a[i] = EEPROM.read(addr+i) ; //read four consecutive bytes, the calling function must take care of EEPROM boundaries
		//Serial.println(a[i]) ;
	}
	unsigned long result = a[3] ;
	result = result * 256 + a[2] ;
	result = result * 256 + a[1] ;
	result = result * 256 + a[0] ;
	//Serial.println(result) ;
	return result ;
}

void eeprom_writelong(unsigned int addr , unsigned long n )
{
	for (byte i = 0 ; i < 4 ; i++)
		EEPROM.write(addr+i,(byte)(n>>(8*i))&0xff);
	return ;
}

void eepmem_clear()
{
	for ( byte i = __mem_m1 ; i < __mem_m1 + 7 ; i++ )
		EEPROM.write(i,0) ;
	return ;
}

bool eepmem_begin()
{
	if ( EEPROM.read(__mem_m1) == 0x55 && EEPROM.read(__mem_m1+6) == 0xaa ) //magic found
		return true ;
	__mem_m1 = 0x00 ;
	unsigned int barrier = EEPROM.length() - 4 ;
	while(true)
	{
		if ( EEPROM.read(__mem_m1) == 0x55 && EEPROM.read(__mem_m1+6) == 0xaa ) //searching for magic
			return true ; //found elsewhere
		else if ( __mem_m1 < barrier )
			__mem_m1++ ;
		else
			break ; //not found
	}
	//revert to default addr
	__mem_m1 = 100 ;
	return false ;
}

void eepmem_setup()
{
	bool status = eepmem_begin() ;
	//Serial.println(status) ;

	if ( status ) {
		bootstat = EEPROM.read( __mem_m1 + 1 ) ; //read powerdown status
		//Serial.println(EEPROM.read( __mem_m1 + 1 ));
		if ( bootstat == POWEROFF_SUCCESS ){ //successfully stored current location, so we need to retrieve it.
			saved_loc = eeprom_readlong(__mem_m1 + 2) ; //read 4 bytes in little endian byte order
		}
		else
			saved_loc = MAX_DISP_BOUND + 10 ; //location is further than maximum boundary, not allowed to store that in current location 
		EEPROM.write(__mem_m1+2 , 0 ) ; //clear the status
	}
	else {
		ask_calibration() ;
	}
}

void eepmem_store()
{
	if ( __mem_m1 != 100 ) //if not default location, clear it
		EEPROM.write(__mem_m1,0) ; //clearing the lower magic byte will do
	__mem_m1 = 100 ; //it doesn't matter even if we set this to 100 AGAIN
	EEPROM.write(__mem_m1,0x55) ; //write the low magic
	eeprom_writelong(__mem_m1+2,saved_loc) ; //store the magic
	EEPROM.write(__mem_m1+6,0xaa) ; //upper magic byte
	EEPROM.write(__mem_m1+1,POWEROFF_SUCCESS);
	return ;
}

