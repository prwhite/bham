// Copyright (c) 2012, Payton R. White
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
// 
// Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
// Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef bham_h
#define bham_h


#include <stdint.h>

/**
  * The bham class is used for generating a single high frequency PWM output
  * signal from a single low frequency input signal.
  * @param dx The number of distinct signal levels for the input and output.
  *   Data should be conditioned to be in this range before being used with the set() method.
  */

template< uint8_t dx = 64 > // max brightness level
class bham
{
	public:

			/** Initialize Bresenham counters for PWM.
			  * @param mval Specifies which bit of a signal set this corresponds to.
			  * Mainly to support the \ref bhams class.  When using this class directly for
			  * a single PWM output, the value 1 can be used.  The range is [0,8).
			  */
		void init ( uint8_t mval )
			{
				mask = mval; 		// value to 'or' in to output byte for SPI
			}
	
			/**
			  * Set the current input value for this instance.
			  * @param val A value in the range [0,dx).
			  */
		void set ( uint8_t val )
			{
				dy = val;
				sw = 2 * dy - dx;
				incx = 2 * dy;
				incxy = 2 * ( dy - dx );
			}
		
		  /**
		    * Called to reset this instance to start 'back at the beginning' of
		    * the PWM cycle.  Calling this is cheaper than calling set() if
		    * the other Bresenham values aren't changing.
		    */
		void overrun ()
			{
				sw = 2 * dy - dx;
			}
		
		  /**
		    * Called every update to get the new PWM output value.  Generally called
		    * from an interrupt routine at a relatively high rate (240hz or greater).
		    * @return The current HIGH/LOW state of the output signal.  The HIGH state
		    * is arithmetically or'd with the value set in the init() call.
		    */
		uint8_t loop ()
			{
				if ( sw < 0 ) { sw += incx;  return 0x00; }
				else 		  { sw += incxy; return mask; }
			}
	
	private:
	
		uint8_t mask;
		uint8_t dy;
		int incx;
		int incxy;
		int sw;
};

/**
  * The bhams class is used for generating a set of high frequency PWM output
  * signals from a set of low frequency input signals.
  * @param Num The number of discrete signals to generate.  The current maximum is 8.
  * @param Samples The number of distinct signal levels for the input and output.
  *   Data should be conditioned to be in this range before being used with the set() method.
  */

template< uint8_t Num = 8, uint8_t Samples = 64 >
class bhams
{
		bham< Samples > bh[ Num ];
		int counter;
		
	public:

			/** 
			  * Initialize bresenham counters for PWM.
			  */		
		void init ()
			{
				for ( uint8_t num = 0; num < Num; ++num )
					bh[ num ].init ( 0x01 << num );
			}

			/**
			  * Set the current input value set for this instance.
			  * @param vals An array containing values in the range [0,dx).
			  */
		void set ( uint8_t const* vals )
			{
				counter = 0;
				for ( uint8_t num = 0; num < Num; ++num )
					bh[ num ].set ( vals[ num ] );
			}

		  /**
		    * Called every update to get the new PWM output value.  Generally called
		    * from an interrupt routine at a relatively high rate (240hz or greater).
		    * @return Each bit in the output corresponds to the HIGH/LOW state of each 
		    * output PWM line.
		    */		
		uint8_t loop ()
			{
#define DOOVERRUN
#ifdef DOOVERRUN
				if ( counter < Samples )
					++counter;
				else
				{
					counter = 0;
					for ( uint8_t num = 0; num < Num; ++num )
						bh[ num ].overrun ();
				}
#endif

				uint8_t out = 0x00;
				for ( uint8_t num = 0; num < Num; ++num )
					out |= bh[ num ].loop ();
				return out;
			}
};


#endif // bham_h
