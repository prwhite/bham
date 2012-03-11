
#include "bham.h"

#ifndef F_CPU
#define HOSTEDTESTING
#endif

#ifdef HOSTEDTESTING
#include <iostream>

int main ( int argc, char* argv )
{
	static const uint8_t Pins = 4;
	static const uint8_t Max = 15;
	uint8_t vals[ Pins ] = { 0, 4, 9, Max };
	
	bhams< Pins, Max > bhs;
	bhs.init ();
	bhs.set ( vals );
	
	for ( uint8_t num = 0; num <= Max + 25; ++num )
	{
		uint8_t out = bhs.loop ();
		
		for ( uint8_t pin = 0; pin < Pins; ++pin )
			std::cout << ( ( out >> pin ) & 0x01 );
		std::cout << std::endl;
	}
	
	return 0;
}

#endif // HOSTEDTESTING
