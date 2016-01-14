/*

Target MCU = ATTiny85(V)
Frequency = 1 Mhz

Pins config:
PB0..PB3 = LEDs
PB4 = button (using AVR's internal pull-up)

*/

#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

//=[ Globals ]=================================================================

uint8_t g_current_pin = PB0;
uint8_t g_time_run = 0;

//=[ ISR ]=====================================================================

//-[ Button click ]------------------------------------------------------------
ISR( PCINT0_vect )
{
	// No need even to debounce or check if it's high or low
	
	// Power up timer (everything but timer powered down)
	// PRR = _BV( PRTIM1 ) | _BV( PRUSI ) | _BV( PRADC );
	TCCR0B = _BV( CS00 ) | _BV( CS02 );	
	
	// Disable button while timer is working
	GIMSK = 0;
	
	// Idle mode
	// MCUCR = 0;
}

//-[ Timer 0 overflow ]--------------------------------------------------------
ISR( TIM0_OVF_vect )
{
		// Switch LED (and don't disable pull-up) :)
		PORTB = _BV( g_current_pin ) | _BV( PB4 );
		
		/* 
		The increment logic below is safe, here's code from iotnx5.h:
		===[ cut start ]===
		#define PB5     5
		#define PB4     4
		#define PB3     3
		#define PB2     2
		#define PB1     1
		#define PB0     0
		===[ cut end ]===
		*/
		
		// would prefer to shift this instead of assignments, but not good for code maintenance
		if( ++g_current_pin > PB3 ) g_current_pin = PB0;
		
		// switch off after some period of time
		if( ++g_time_run > 30 ) 
		{
			// Power down timer (everything)
			// PRR = _BV( PRTIM1 ) | _BV( PRTIM0 ) | _BV( PRUSI ) | _BV( PRADC );
			TCCR0B = 0;
			
			// reset globals
			g_time_run = 0;	
			g_current_pin = PB0;		
			
			// Enable button
			GIMSK = _BV( PCIE );
			
			// Disable LEDs
			PORTB = _BV( PB4 );
			
			// Power-down mode
			// MCUCR = _BV( SM1 );
		}
}

int main(void)
{
	cli();
	
	//-[ Initialization ]------------------------------------------------------
	DDRB = ~( _BV( PB4 ) );		// everything but PB4 is output
	PORTB = _BV( PB4 );			// GROUND everything, PB4 pull-up enable (Button)
	
	// Power-down mode
	// MCUCR = _BV( SM1 ); 
	
	// Disable comparator
	// ACSR = _BV( ACD );
	
	// Disable timers, USI and comparator
	// PRR = _BV( PRTIM1 ) | _BV( PRTIM0 ) | _BV( PRUSI ) | _BV( PRADC );
		
	// Enable PCINT0 at PB4 (button)
	GIMSK = _BV( PCIE );
	PCMSK = _BV( PCINT4 );
	
	// Enable Timer 0 overflow interrupt
	TIMSK = _BV( TOIE0 );
	// TCCR0B = _BV( CS00 ) | _BV( CS02 );	
	
	sei();
	
	//-[ Main Loop ]-----------------------------------------------------------
	
	while( 1 ) 
	{
	    sleep_enable();
	    sleep_cpu();
	    sleep_disable();
	}
}

