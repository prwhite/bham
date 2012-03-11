#ifndef bham_h
#define bham_h


#include <stdint.h>

template< uint8_t dx = 64 > // max brightness level
class bham
{
	public:

			/// Initialize 	bresenham counters for PWM.
			// val [ 0 , vmax )
		void init ( uint8_t mval )
			{
				mask = mval; 		// value to 'or' in to output byte for SPI
			}
	
			// dy = value
		void set ( uint8_t val )
			{
				dy = val;
				sw = 2 * dy - dx;
				incx = 2 * dy;
				incxy = 2 * ( dy - dx );
			}
		
		void overrun ()
			{
				sw = 2 * dy - dx;
			}
		
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

template< uint8_t Num = 8, uint8_t Samples = 64 >
class bhams
{
		bham< Samples > bh[ Num ];
		int counter;
		
	public:
		
		void init ()
			{
				for ( uint8_t num = 0; num < Num; ++num )
					bh[ num ].init ( 0x01 << num );
			}
		
		void set ( uint8_t const* vals )
			{
				counter = 0;
				for ( uint8_t num = 0; num < Num; ++num )
					bh[ num ].set ( vals[ num ] );
			}
		
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
