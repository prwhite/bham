



function bham ( x0, y0, x1, y1 )
{
	var dx = Math.abs ( x1 - x0 );
	var dy = Math.abs ( y1 - y0 );

console.log ( dx, dy );
	
	var sw = 2 * dy - dx;

console.log ( sw );
	
	var incx  = 2 * dy;
	var incxy = 2 * ( dy - dx );

console.log ( incx, incxy );

	while ( x0 < x1 )
	{
		console.log ( x0, y0 );

		++x0;
		
		if ( sw < 0 )
			sw += incx;
		else
		{
			++y0; sw += incxy;
		}
	}
}


bham ( 0, 0, 5, 3 );
bham ( 0, 0, 5, 7 );


